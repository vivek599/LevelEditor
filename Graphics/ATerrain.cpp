#include "ATerrain.h"
#include "ARenderDevice.h"
#include "AShaderCache.h"
#include "AQuadTree.h"

ATerrain::ATerrain()
{
	m_HeightMap = nullptr;
	m_radiusMax = 1;
	m_strength = 1;


}

ATerrain::~ATerrain()
{
	// Release the height map data.
	ShutdownHeightMap();
}

bool ATerrain::Initialize(ARenderDevice* renderDevice, TerrainInitializationParams& params)
{
	HRESULT hr = S_OK;
	bool result = true;;
	ID3D11Device* device = renderDevice->GetDevice().Get();

	// Load in the height map for the terrain.
	result = LoadHeightMapFromBMP(params.heightMap);
	if (!result)
	{
		return false;
	}

	// Normalize the height of the height map.
	NormalizeHeightMap();

	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Calculate the texture coordinates.
	CalculateTextureCoordinates();

	// Load the texture.
	result = LoadTexture(device, params.textureLayers);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	SAFEDELETE(m_Vertices);
	SAFEDELETE(m_Indices);

	result = InitGeometry(m_Vertices, m_Indices);
	if (!result)
	{
		return false;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_Vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = m_Indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	hr = device->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	//SAFEDELETE(Vertices); not deleting for erosion
	//SAFEDELETE(Indices);

	result = InitConstantBuffers(device);
	if (!result)
	{
		return false;
	}

	//init shader
	m_PixelShader.reset(new AShaderCache(renderDevice, AShaderType::PIXELSHADER));
	m_PixelShader->CompileShaderFromFile(params.pixelSHader);
	m_PixelShader->CreateReflector();
	m_PixelShader->CreateInputLayout();

	m_VertexShader.reset(new AShaderCache(renderDevice, AShaderType::VERTEXSHADER));
	m_VertexShader->CompileShaderFromFile(params.vertexSHader);
	m_VertexShader->CreateReflector();
	m_VertexShader->CreateInputLayout();

	return true;
}

void ATerrain::Update(ARenderDevice* renderDevice, float deltaTime, Matrix worlMatrix, Matrix viewMatrix, Matrix projMatrix)
{
	//thread erodeThread([=]() {Erode(20, deltaTime); return 1; });

	ID3D11DeviceContext* context = renderDevice->GetContext().Get();

	HRESULT hr = S_OK; 
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Transpose the matrices to prepare them for the shader.
	worlMatrix = XMMatrixTranspose(worlMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projMatrix = XMMatrixTranspose(projMatrix);

	// Lock the constant buffer so it can be written to.
	hr = context->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return;
	}

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worlMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projMatrix;

	// Unlock the constant buffer.
	context->Unmap(m_MatrixBuffer.Get(), 0);

	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	// Lock the light constant buffer so it can be written to.
	hr = context->Map(m_LightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return;
	}

	// Get a pointer to the data in the constant buffer.
	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = m_AmbientColor;
	dataPtr2->diffuseColor = m_DiffuseColor;
	dataPtr2->lightDirection = m_LightDirection;
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	context->Unmap(m_LightBuffer.Get(), 0);

	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	hr = context->Map(m_ShaderParametersBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return;
	}

	ShaderParametersBuffer* dataPtr3 = (ShaderParametersBuffer*)mappedResource.pData;

	dataPtr3->TextureUVScale = m_TerrainTextureUVScale;

	// Unlock the constant buffer.
	context->Unmap(m_ShaderParametersBuffer.Get(), 0);


	SAFEDELETE(m_Vertices);
	SAFEDELETE(m_Indices);
	//erodeThread.join();

	bool result = InitGeometry(m_Vertices, m_Indices);
	if (!result)
	{
		return;
	}

	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	hr = context->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return;
	}

	memcpy(mappedResource.pData, m_Vertices, m_VertexCount * sizeof(VertexType));

	context->Unmap(m_VertexBuffer.Get(), 0);

	SAFEDELETE(m_Vertices);
}

bool ATerrain::InitConstantBuffers(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&matrixBufferDesc, nullptr, m_MatrixBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&lightBufferDesc, nullptr, m_LightBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_BUFFER_DESC shaderParamBufferDesc;
	shaderParamBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderParamBufferDesc.ByteWidth = sizeof(ShaderParametersBuffer);
	shaderParamBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderParamBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderParamBufferDesc.MiscFlags = 0;
	shaderParamBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&shaderParamBufferDesc, nullptr, m_ShaderParametersBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool ATerrain::InitGeometry(VertexType*& Vertices, uint32_t*& Indices)
{
	uint32_t index, i, j;
	float tu, tv;

	int index1, index2, index3, index4;

	// Calculate the number of vertices in the terrain mesh.
	m_VertexCount = (m_TerrainWidth - 1) * (m_TerrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_IndexCount = m_VertexCount;

	// Create the vertex array.
	Vertices = new VertexType[m_VertexCount];
	if (!Vertices)
	{
		return false;
	}

	// Create the index array.
	Indices = new uint32_t[m_IndexCount];
	if (!Indices)
	{
		return false;
	}

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j < (m_TerrainHeight - 1); j++)
	{
		for (i = 0; i < (m_TerrainWidth - 1); i++)
		{
			index1 = (m_TerrainHeight * j) + i;          // Bottom left.
			index2 = (m_TerrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (m_TerrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (m_TerrainHeight * (j + 1)) + (i + 1);  // Upper right.

			// Upper left.
			tv = m_HeightMap[index3].texture.y;
			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) 
			{ 
				tv = 0.0f; 
			}

			Vertices[index].position = m_HeightMap[index3].position;
			Vertices[index].texture = Vector2(m_HeightMap[index3].texture.x, tv);
			Vertices[index].normal = m_HeightMap[index3].normal;
			Indices[index] = index++;

			// Upper right.
			tu = m_HeightMap[index4].texture.x;
			tv = m_HeightMap[index4].texture.y;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) 
			{ 
				tu = 1.0f; 
			}

			if (tv == 1.0f) 
			{ 
				tv = 0.0f; 
			}

			Vertices[index].position = m_HeightMap[index4].position;
			Vertices[index].texture = Vector2(tu, tv);
			Vertices[index].normal = m_HeightMap[index4].normal;
			Indices[index] = index++;

			// Bottom left.
			Vertices[index].position = m_HeightMap[index1].position;
			Vertices[index].texture = m_HeightMap[index1].texture;
			Vertices[index].normal = m_HeightMap[index1].normal;
			Indices[index] = index++;

			// Bottom left.
			Vertices[index].position = m_HeightMap[index1].position;
			Vertices[index].texture = m_HeightMap[index1].texture;
			Vertices[index].normal = m_HeightMap[index1].normal;
			Indices[index] = index++;

			// Upper right.
			tu = m_HeightMap[index4].texture.x;
			tv = m_HeightMap[index4].texture.y;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) 
			{ 
				tu = 1.0f; 
			}

			if (tv == 1.0f) 
			{ 
				tv = 0.0f; 
			}

			Vertices[index].position = m_HeightMap[index4].position;
			Vertices[index].texture = Vector2(tu, tv);
			Vertices[index].normal = m_HeightMap[index4].normal;
			Indices[index] = index++;

			// Bottom right.
			tu = m_HeightMap[index2].texture.x;

			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) 
			{ 
				tu = 1.0f; 
			}

			Vertices[index].position = m_HeightMap[index2].position;
			Vertices[index].texture = Vector2(tu, m_HeightMap[index2].texture.y);
			Vertices[index].normal = m_HeightMap[index2].normal;
			Indices[index] = index++;
		}
	}

	return true;
}

void ATerrain::Render(ARenderDevice* renderDevice)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	ID3D11DeviceContext* context = renderDevice->GetContext().Get();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Finanly set the constant buffer in the shader with the updated values.
	context->VSSetConstantBuffers(0, 1, m_MatrixBuffer.GetAddressOf());

	ID3D11Buffer* buffers[] = { m_LightBuffer.Get(), m_ShaderParametersBuffer.Get() };
	context->PSSetConstantBuffers(0, _countof(buffers), buffers );

	// Set shader texture resource in the pixel shader.
	context->PSSetShaderResources(0, m_TerrainTextureSrvLayers.size(), m_TerrainTextureSrvLayers.data());

	// Set the vertex input layout.
	context->IASetInputLayout(m_VertexShader->GetInputLayout().Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	context->VSSetShader(m_VertexShader->GetVertexShader().Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader->GetPixelShader().Get(), nullptr, 0);

	// Set the sampler state in the pixel shader.
	context->PSSetSamplers(0, 1, renderDevice->GetSampleRepeat());

	// Render the triangle.
	context->DrawIndexed(m_IndexCount, 0, 0);
}

int ATerrain::GetIndexCount()
{
	return m_IndexCount;
}

void ATerrain::SetAmbientColor(Vector4 val)
{
	m_AmbientColor = val;
}

void ATerrain::SetDiffuseColor(Vector4 val)
{
	m_DiffuseColor = val;
}

void ATerrain::SetLightDirection(Vector3 val)
{
	m_LightDirection = val;
}

void ATerrain::SetTextureUVScale(float val)
{
	m_TerrainTextureUVScale = Vector4(val);
}

uint32_t ATerrain::GetWidth() const
{
	return m_TerrainWidth;
}

uint32_t ATerrain::GetHeight() const
{
	return m_TerrainHeight;
}

bool ATerrain::LoadHeightMapFromBMP(const wchar_t* heightMapFilePath)
{
	ifstream filePtr;
	uint32_t count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	uint64_t imageSize, i, j, k, index;
	uint8_t* bitmapImage;
	uint8_t height;

	// Open the height map file in binary.
	filePtr.open(heightMapFilePath, ios::binary);
	if (!filePtr.good())
	{
		return false;
	}

	// Read in the file header.
	filePtr.read(reinterpret_cast<char*>(&bitmapFileHeader), sizeof(BITMAPFILEHEADER));

	// Read in the bitmap info header.
	filePtr.read(reinterpret_cast<char*>(&bitmapInfoHeader), sizeof(BITMAPINFOHEADER));

	// Save the dimensions of the terrain.
	m_TerrainWidth = bitmapInfoHeader.biWidth;
	m_TerrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_TerrainWidth * m_TerrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new uint8_t[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	filePtr.seekg(bitmapFileHeader.bfOffBits, ios::beg);

	// Read in the bitmap image data.
	filePtr.read(reinterpret_cast<char*>(bitmapImage), imageSize);

	// Close the file.
	filePtr.close();

	// Create the structure to hold the height map data.
	m_HeightMap = new VertexType[m_TerrainWidth * m_TerrainHeight];
	if (!m_HeightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j < m_TerrainHeight; j++)
	{
		for (i = 0; i < m_TerrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_TerrainHeight * j) + i;

			m_HeightMap[index].position.x = (float)i;
			m_HeightMap[index].position.y = (float)height;
			m_HeightMap[index].position.z = (float)j;
			m_HeightMap[index].position.w = 1.0f;

			k += 3;
		}
	}

	SAFEDELETE(bitmapImage);

	return true;
}

bool ATerrain::LoadHeightMapFromPNG(const wchar_t* heightMapFilePath)
{
	USES_CONVERSION;
	const char* filename = W2A(heightMapFilePath);
 
	vector<uint8_t> png; //the raw pixels
	vector<uint8_t> image; //the raw pixels
	uint32_t _width, _height;

	uint32_t error = lodepng::load_file(png, filename);
	if (error)
	{
		throw;
	}

	lodepng::State state;
	error = lodepng_inspect( &_width, &_height, &state, png.data(), png.size());
	if (error)
	{
		throw;
	}

	//decode
	error = lodepng::decode(image, _width, _height, state, png);
	if (error)
	{
		throw string("png load error!");
	}

	m_TerrainWidth	= _width;
	m_TerrainHeight = _height;

	// Create the structure to hold the height map data.
	m_HeightMap = new VertexType[m_TerrainWidth * m_TerrainHeight];
	if (!m_HeightMap)
	{
		return false;
	}

	uint64_t pos = 0;
	// Read the image data into the height map.
	for ( uint32_t j = 0; j < m_TerrainHeight; j++)
	{
		for (uint32_t i = 0; i < m_TerrainWidth; i++)
		{
			uint64_t index	= m_TerrainWidth * j + i;
			uint16_t height = (image[pos] << 8) | image[pos + 1];
			pos += 2;

			m_HeightMap[index].position.x = (float)i;
			m_HeightMap[index].position.y = (float)height;
			m_HeightMap[index].position.z = (float)j;
		}
	}

	return true;
}

void ATerrain::NormalizeHeightMap()
{
	return;
	for (uint32_t j = 0; j < m_TerrainHeight; j++)
	{
		for (uint32_t i = 0; i < m_TerrainWidth; i++)
		{
			m_HeightMap[(m_TerrainHeight * j) + i].position.y /= 768.0f;
		}
	} 
}

bool ATerrain::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float length;
	Vector3* normals;
	Vector3 vector1;
	Vector3 vector2;
	Vector3 vertex1;
	Vector3 vertex2;
	Vector3 vertex3;
	Vector3 sum;

	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new Vector3[(m_TerrainHeight - 1) * (m_TerrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (m_TerrainHeight - 1); j++)
	{
		for (i = 0; i < (m_TerrainWidth - 1); i++)
		{
			index1 = (j * m_TerrainHeight) + i;
			index2 = (j * m_TerrainHeight) + (i + 1);
			index3 = ((j + 1) * m_TerrainHeight) + i;

			// Get three vertices from the face.
			vertex1 = (Vector3)m_HeightMap[index1].position; 
			vertex2 = (Vector3)m_HeightMap[index2].position; 
			vertex3 = (Vector3)m_HeightMap[index3].position; 

			// Calculate the two vectors for this face.
			vector1 = vertex1 - vertex3;
			vector2 = vertex3 - vertex2;

			index = (j * (m_TerrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index] = vector1.Cross(vector2);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j < m_TerrainHeight; j++)
	{
		for (i = 0; i < m_TerrainWidth; i++)
		{
			// Initialize the sum.
			sum = Vector3(0.0f); 

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_TerrainHeight - 1)) + (i - 1);

				sum += normals[index]; 
				count++;
			}

			// Bottom right face.
			if ((i < (m_TerrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_TerrainHeight - 1)) + i;

				sum += normals[index];
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_TerrainHeight - 1)))
			{
				index = (j * (m_TerrainHeight - 1)) + (i - 1);
				 
				sum += normals[index];
				count++;
			}

			// Upper right face.
			if ((i < (m_TerrainWidth - 1)) && (j < (m_TerrainHeight - 1)))
			{
				index = (j * (m_TerrainHeight - 1)) + i;
				 
				sum += normals[index];
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum /= (float)count; 

			// Calculate the length of this normal.
			sum.Normalize();

			// Get an index to the vertex location in the height map array.
			index = (j * m_TerrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_HeightMap[index].normal = sum;
		}
	}

	// Release the temporary normals.
	SAFEDELETE(normals);

	return true;
}

void ATerrain::CalculateTextureCoordinates()
{
	for (int j = 0; j < m_TerrainHeight; j++)
	{
		for (int i = 0; i < m_TerrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_HeightMap[(m_TerrainHeight * j) + i].texture.x = float(i)/(m_TerrainWidth-1);
			m_HeightMap[(m_TerrainHeight * j) + i].texture.y = float(j)/(m_TerrainHeight-1);
		}
	}
}

bool ATerrain::LoadTexture(ID3D11Device* device, vector<const wchar_t*>& textureFilenames)
{
	m_TerrainTextureLayers.resize(textureFilenames.size());
	m_TerrainTextureSrvLayers.resize(textureFilenames.size());


	for (int i = 0; i < textureFilenames.size(); i++)
	{
		HRESULT hr = CreateWICTextureFromFile(device, textureFilenames[i], &m_TerrainTextureLayers[i], &m_TerrainTextureSrvLayers[i]);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ATerrain::ShutdownHeightMap()
{
	SAFEDELETE(m_HeightMap);





}

void ATerrain::Erode(int cycles, float dt) 
{
	/*
	  Note: Everything is properly scaled by a time step-size "dt"
	*/
	srand(time(0));
	//Do a series of iterations! (5 Particles)
	for (int i = 0; i < cycles; i++) 
	{
		//Spawn New Particle
		Vector2 newpos = Vector2(rand() % m_TerrainWidth, rand() % m_TerrainHeight);
		Particle drop(newpos);

		//As long as the droplet exists...
		while (drop.Volume > m_MinVol) 
		{
			int index = (int)drop.Pos.y * m_TerrainWidth + (int)drop.Pos.x;
			//int index1 = (int)(drop.Pos.y * m_TerrainWidth + drop.Pos.x);

			if (index >= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
			{
				index = (m_TerrainWidth - 1) * (m_TerrainHeight - 1);
			}


			Vector3 n = m_HeightMap[index].normal;  //Surface Normal at Position

			//Accelerate particle using Newtonian mechanics using the surface normal.
			drop.Speed += dt * Vector2(n.x, n.z) / (drop.Volume * m_Density);//F = ma, so a = F/m
			drop.Pos += dt * drop.Speed;
			drop.Speed *= (1.0 - dt * m_Friction);       //Friction Factor

			int index1 = (int)drop.Pos.y * m_TerrainWidth + (int)drop.Pos.x;
			if (index1 >= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
			{
				index1 = (m_TerrainWidth - 1) * (m_TerrainHeight - 1);
			}

			/*
			  Note: For multiplied factors (e.g. friction, evaporation)
			  time-scaling is correctly implemented like above.
			*/

			//Check if Particle is still in-bounds
			BoundingBox b;
			b.Center = Vector3(m_TerrainWidth, 0.0f, m_TerrainHeight) * 0.5f;
			b.Extents = Vector3(m_TerrainWidth, 65536.0f, m_TerrainHeight) * 0.5f;
			Vector3 dropPos = Vector3(drop.Pos.x, 0.0f, drop.Pos.y);

			if (b.Contains(dropPos) == ContainmentType::DISJOINT)
			{
				break;
			}

			//Compute sediment capacity difference
			float maxsediment = drop.Volume * drop.Speed.Length() * (m_HeightMap[index].position.y - m_HeightMap[index1].position.y);
			if (maxsediment < 0.0)
			{
				maxsediment = 0.0;
			}

			float sdiff = maxsediment - drop.Sediment;

			//Act on the Height map and Droplet!
			drop.Sediment += dt * m_DepositionRate * sdiff;
			m_HeightMap[index].position.y -= dt * drop.Volume * m_DepositionRate * sdiff;

			//Evaporate the Droplet (Note: Proportional to Volume! Better: Use shape factor to make proportional to the area instead.)
			drop.Volume *= (1.0 - dt * m_EvapRate);
		}
	}
}

Vector3 ATerrain::GetBestIntersectionPoint(Ray ray, BoundingBox& outBox)
{ 
	for (size_t i = 0; i < AQuadTree::HitQueue.size(); i++)
	{
		int x = AQuadTree::HitQueue[i].x;
		int z = AQuadTree::HitQueue[i].z;


		if (x < 0 || z < 0 || x > m_TerrainWidth - 1 || z > m_TerrainHeight - 1)
		{
			return Vector3(-1.0f);
		}

		uint64_t index = m_TerrainHeight * z + x;
		int y = m_HeightMap[index].position.y;

		Vector3 v0 = Vector3(x, y, z);
		Vector3 v1 = Vector3(x+1, y, z);
		Vector3 v2 = Vector3(x+1, y, z+1);
		Vector3 v3 = Vector3(x, y, z+1);

		Ray downwardRay = Ray(Vector3(0.0f, 10000.0f, 0.0f),Vector3(0.0f, -1.0f, 0.0f));

		float dist = 0.0f;
		Vector3 ExactIntersect = v0;
		if(downwardRay.Intersects(v0, v1, v3, dist))
		{
			ExactIntersect = downwardRay.position + downwardRay.direction * dist;
		} 
		else if(downwardRay.Intersects(v1, v2, v3, dist))
		{
			ExactIntersect = downwardRay.position + downwardRay.direction * dist;
		}

		Vector3 nearestPointDir = AQuadTree::HitQueue[i] - ray.position;
		float dotCurrent = nearestPointDir.Dot(ray.direction);
		if (abs(ExactIntersect.y - AQuadTree::HitQueue[i].y) < 0.5f && dotCurrent > 0.95f)
		{
			m_ClosestPoint = AQuadTree::HitQueue[i];
			AQuadTree::HitQueue.clear();
			break;
		}
	}

	return m_ClosestPoint;
}

bool ATerrain::RayTerrainIntersect(Vector3 rayOrigin, Vector3 rayDirection)
{
	Ray ray(rayOrigin, rayDirection);
	BoundingBox MaxBound = BoundingBox( Vector3(m_TerrainWidth * 0.5f, 0.0f, m_TerrainHeight * 0.5f), Vector3(m_TerrainWidth * 0.5f, 32768.0f, m_TerrainHeight * 0.5f) );
	unique_ptr<class AQuadTree>		QTree;
	QTree.reset(new AQuadTree(ray, MaxBound.Center, MaxBound.Extents));
	 
	BoundingBox b = QTree->SubDevide(ray.position, ray.position + 10000.f * ray.direction);
	 
	BoundingBox outBox;
	m_PickedPoint = GetBestIntersectionPoint(ray, outBox);//v0;
	return true;
}

void ATerrain::Raise(float deltaTime)
{
	int x = m_PickedPoint.x;
	int z = m_PickedPoint.z;
	if (x < 0 || z < 0 || x > m_TerrainWidth - 1 || z > m_TerrainHeight - 1)
	{
		return;
	}
	 
	for (int i = -m_radiusMax; i < m_radiusMax; i++)
	{
		for (int j = -m_radiusMax; j < m_radiusMax; j++)
		{
			uint64_t index = (m_TerrainHeight * (z + j)) + (x + i);
			if (index >= 0 && index <= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
			{
				float radius = Vector2(i, j).Length();
				//m_HeightMap[index].position.y += radius > radiusMax ? 0.0f : strength * SmotherStep(radius, radiusMax, radiusMax - radius);
				m_HeightMap[index].position.y += (radius > m_radiusMax ? 0.0f : m_strength * (cosf(XM_PI * radius / float(m_radiusMax)) + 1.0f) * 0.5f) * deltaTime * 10.0f;
			}
		}
	}
}

void ATerrain::Lower(float deltaTime)
{
	int x = m_PickedPoint.x;
	int z = m_PickedPoint.z;
	if (x < 0 || z < 0 || x > m_TerrainWidth - 1 || z > m_TerrainHeight - 1)
	{
		return;
	}
	 
	for (int i = -m_radiusMax; i < m_radiusMax; i++)
	{
		for (int j = -m_radiusMax; j < m_radiusMax; j++)
		{
			uint64_t index = (m_TerrainHeight * (z + j)) + (x + i);
			if (index >= 0 && index <= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
			{
				float radius = Vector2(i, j).Length();
				//m_HeightMap[index].position.y -= radius > radiusMax ? 0.0f : strength * SmotherStep(radius, radiusMax, radiusMax - radius);
				m_HeightMap[index].position.y -= (radius > m_radiusMax ? 0.0f : m_strength * (cosf(XM_PI * radius / float(m_radiusMax)) + 1.0f) * 0.5f)* deltaTime * 10.0f;
			}
		}
	}
}

void ATerrain::Flatten(float deltaTime)
{
	int x = m_PickedPoint.x;
	int z = m_PickedPoint.z;
	if (x < 0 || z < 0 || x > m_TerrainWidth - 1 || z > m_TerrainHeight - 1)
	{
		return;
	}
	 
	m_PickedPoint.y = m_HeightMap[m_TerrainHeight * z + x].position.y;

	for (int i = -m_radiusMax; i < m_radiusMax; i++)
	{
		for (int j = -m_radiusMax; j < m_radiusMax; j++)
		{
			uint64_t index = (m_TerrainHeight * (z + j)) + (x + i);
			if (index >= 0 && index <= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
			{
				float radius = Vector2(i, j).Length();
				if (radius < m_radiusMax)
				{
					m_HeightMap[index].position.y = m_PickedPoint.y;
				}
			}
		}
	}
}

void ATerrain::Smooth(float deltaTime)
{
	/*int x = m_PickedPoint.x;
	int z = m_PickedPoint.z;
	if (x < 0 || z < 0 || x > m_TerrainWidth - 1 || z > m_TerrainHeight - 1)
	{
		return;
	}

	int smoothRadius = 5; 
	int radiusMax = 50;
	for (int i = -radiusMax; i < radiusMax; i++)
	{
		for (int j = -radiusMax; j < radiusMax; j++)
		{
			uint64_t index = (m_TerrainHeight * (z + j)) + (x + i);
			if (index >= 0 && index <= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
			{
				float avgY = 0.0f;
				float radius = Vector2(i, j).Length();
				for (int k = -smoothRadius; k < smoothRadius; k++)
				{
					for (int l = -smoothRadius; l < smoothRadius; l++)
					{
						uint64_t index2 = (m_TerrainHeight * (z + j + l)) + (x + i + k);
						if (index2 >= 0 && index2 <= (m_TerrainWidth - 1) * (m_TerrainHeight - 1))
						{
							float radius2 = Vector2(k, l).Length();
							avgY += radius2 > smoothRadius ? 0.0f : m_HeightMap[index2].position.y;
						}
					}
				}
				avgY /= (smoothRadius * smoothRadius);
				m_HeightMap[index].position.y = radius > radiusMax ? 0.0f : avgY;
			}
		}
	}*/

}

void ATerrain::SetBrushRadius(int val)
{
	m_radiusMax = val;
}

void ATerrain::SetBrushStrength(float val)
{
	m_strength = val;
}

void ATerrain::ResetClosestPoint()
{
	m_ClosestPoint = Vector3(-1.0f);
}
