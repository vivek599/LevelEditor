#include "ATerrain.h"
#include "ARenderDevice.h"
#include "AShaderCache.h"

ATerrain::ATerrain()
{
	m_HeightMap = nullptr;



}

ATerrain::~ATerrain()
{
	// Release the height map data.
	ShutdownHeightMap();
}

bool ATerrain::Initialize(ARenderDevice* renderDevice, const wchar_t* heightMapFilePath, const wchar_t* pixelShaderFilePath, const wchar_t* vertexShaderFilePath)
{
	HRESULT hr = S_OK;
	bool result = true;;
	ID3D11Device* device = renderDevice->GetDevice().Get();

	// Load in the height map for the terrain.
	result = LoadHeightMap(heightMapFilePath);
	if (!result)
	{
		return false;
	}

	// Normalize the height of the height map.
	NormalizeHeightMap();

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	VertexType*		Vertices;
	uint32_t*		Indices;

	uint32_t index, i, j;

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	int index1, index2, index3, index4;

	// Calculate the number of vertices in the terrain mesh.
	m_VertexCount = (m_TerrainWidth - 1) * (m_TerrainHeight - 1) * 12;

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
			Vertices[index].position = Vector3(m_HeightMap[index3].x, m_HeightMap[index3].y, m_HeightMap[index3].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Upper right.
			Vertices[index].position = Vector3(m_HeightMap[index4].x, m_HeightMap[index4].y, m_HeightMap[index4].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Upper right.
			Vertices[index].position = Vector3(m_HeightMap[index4].x, m_HeightMap[index4].y, m_HeightMap[index4].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Bottom left.
			Vertices[index].position = Vector3(m_HeightMap[index1].x, m_HeightMap[index1].y, m_HeightMap[index1].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Bottom left.
			Vertices[index].position = Vector3(m_HeightMap[index1].x, m_HeightMap[index1].y, m_HeightMap[index1].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Upper left.
			Vertices[index].position = Vector3(m_HeightMap[index3].x, m_HeightMap[index3].y, m_HeightMap[index3].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Bottom left.
			Vertices[index].position = Vector3(m_HeightMap[index1].x, m_HeightMap[index1].y, m_HeightMap[index1].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Upper right.
			Vertices[index].position = Vector3(m_HeightMap[index4].x, m_HeightMap[index4].y, m_HeightMap[index4].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Upper right.
			Vertices[index].position = Vector3(m_HeightMap[index4].x, m_HeightMap[index4].y, m_HeightMap[index4].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Bottom right.
			Vertices[index].position = Vector3(m_HeightMap[index2].x, m_HeightMap[index2].y, m_HeightMap[index2].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Bottom right.
			Vertices[index].position = Vector3(m_HeightMap[index2].x, m_HeightMap[index2].y, m_HeightMap[index2].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;

			// Bottom left.
			Vertices[index].position = Vector3(m_HeightMap[index1].x, m_HeightMap[index1].y, m_HeightMap[index1].z);
			Vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			Indices[index] = index++;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = Vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.GetAddressOf());
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
	indexData.pSysMem = Indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	hr = device->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] Vertices;
	Vertices = 0;

	delete[] Indices;
	Indices = 0;

	//init shader
	m_PixelShader.reset(new AShaderCache(renderDevice, AShaderType::PIXELSHADER));
	m_PixelShader->CompileShaderFromFile(pixelShaderFilePath);
	m_PixelShader->CreateReflector();
	m_PixelShader->CreateInputLayout();

	m_VertexShader.reset(new AShaderCache(renderDevice, AShaderType::VERTEXSHADER));
	m_VertexShader->CompileShaderFromFile(vertexShaderFilePath);
	m_VertexShader->CreateReflector();
	m_VertexShader->CreateInputLayout();

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

	return true;
}

void ATerrain::Render(ID3D11DeviceContext* context, Matrix worlMatrix, Matrix viewMatrix, Matrix projMatrix)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr = nullptr;

	// Transpose the matrices to prepare them for the shader.
	worlMatrix = XMMatrixTranspose(worlMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projMatrix = XMMatrixTranspose(projMatrix);

	// Lock the constant buffer so it can be written to.
	result = context->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worlMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projMatrix;

	// Unlock the constant buffer.
	context->Unmap(m_MatrixBuffer.Get(), 0);

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	// Finanly set the constant buffer in the vertex shader with the updated values.
	context->VSSetConstantBuffers(0, 1, m_MatrixBuffer.GetAddressOf());

	// Set the vertex input layout.
	context->IASetInputLayout(m_VertexShader->GetInputLayout().Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	context->VSSetShader(m_VertexShader->GetVertexShader().Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader->GetPixelShader().Get(), nullptr, 0);


	// Render the triangle.
	context->DrawIndexed(m_IndexCount, 0, 0);
}

int ATerrain::GetIndexCount()
{
	return m_IndexCount;
}

bool ATerrain::LoadHeightMap(const wchar_t* heightMapFilePath)
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
	m_HeightMap = new Vector3[m_TerrainWidth * m_TerrainHeight];
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

			m_HeightMap[index].x = (float)i;
			m_HeightMap[index].y = (float)height;
			m_HeightMap[index].z = (float)j;

			k += 3;
		}
	}

	SAFEDELETE(bitmapImage);

	return true;
}

bool ATerrain::LoadHeightMapFromRAW(const wchar_t* heightMapFilePath, uint32_t width, uint32_t height)
{
	wstring wide(heightMapFilePath);
	const char* filename = string(wide.begin(), wide.end()).c_str();
 



	//// Create the structure to hold the height map data.
	//m_HeightMap = new Vector3[m_TerrainWidth * m_TerrainHeight];
	//if (!m_HeightMap)
	//{
	//	return false;
	//}

	//// Read the image data into the height map.
	//for ( uint32_t j = 0; j < m_TerrainHeight; j++)
	//{
	//	for (uint32_t i = 0; i < m_TerrainWidth; i++)
	//	{
	//		uint32_t index = m_TerrainWidth * j + i;
	//		float height = rawImage[index];

	//		m_HeightMap[index].x = (float)i;
	//		m_HeightMap[index].y = (float)height;
	//		m_HeightMap[index].z = (float)j;
	//	}
	//}



	return true;

}

void ATerrain::NormalizeHeightMap()
{
	return;

	for (uint32_t j = 0; j < m_TerrainHeight; j++)
	{
		for (uint32_t i = 0; i < m_TerrainWidth; i++)
		{
			m_HeightMap[(m_TerrainHeight * j) + i].y /= 15.0f;
		}
	} 
}

void ATerrain::ShutdownHeightMap()
{
	SAFEDELETE(m_HeightMap);





}

