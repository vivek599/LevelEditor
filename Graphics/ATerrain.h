#pragma once

#include "ACommonIncludes.h"

class ARenderDevice;

class ATerrain
{
public:
	ATerrain();
	~ATerrain();

	bool Initialize(ARenderDevice* renderDevice, const wchar_t* heightMapFilePath, 
		const wchar_t* pixelShaderFilePath, const wchar_t* vertexShaderFilePath, const wchar_t* textureFilename);


	
	void Render(ARenderDevice* renderDevice, Matrix worlMatrix, Matrix viewMatrix, Matrix projMatrix);

	int GetIndexCount();

	void SetAmbientColor( Vector4 val );
	void SetDiffuseColor( Vector4 val );
	void SetLightDirection( Vector3 val );

	void SetTextureUVScale(float val);

private:

	struct VertexType
	{
		Vector4 position;
		Vector2 texture;
		Vector3 normal;
	};

	bool InitGeometry(VertexType*& Vertices, uint32_t*& Indices);
	bool InitConstantBuffers(ID3D11Device* device);
	bool LoadHeightMapFromBMP(const wchar_t* heightMapFilePath);
	bool LoadHeightMapFromPNG(const wchar_t* heightMapFilePath);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void CalculateTextureCoordinates();
	bool LoadTexture(ID3D11Device* device, const wchar_t* textureFilename);
	void ShutdownHeightMap();

	uint32_t m_TerrainWidth;
	uint32_t m_TerrainHeight;
	uint32_t m_VertexCount;
	uint32_t m_IndexCount;

	ComPtr<ID3D11Buffer>	m_VertexBuffer;
	ComPtr<ID3D11Buffer>	m_IndexBuffer;

	VertexType*				m_HeightMap;

	unique_ptr<class AShaderCache>	m_PixelShader;
	unique_ptr<class AShaderCache>	m_VertexShader;

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	struct LightBufferType
	{
		Vector4 ambientColor;
		Vector4 diffuseColor;
		Vector3 lightDirection;
		float padding;
	};

	struct ShaderParametersBuffer
	{
		Vector4 TextureUVScale;
	};

	ComPtr<ID3D11ShaderResourceView>	m_TerrainTextureSrvLayer0;
	ComPtr<ID3D11Resource>				m_TerrainTextureLayer0;
	int									m_TextureRepeatConstant = 8;

	ComPtr<ID3D11Buffer>	m_MatrixBuffer;
	ComPtr<ID3D11Buffer>	m_LightBuffer;
	ComPtr<ID3D11Buffer>	m_ShaderParametersBuffer;

	Vector4 m_AmbientColor;
	Vector4 m_DiffuseColor;
	Vector3 m_LightDirection;

	Vector4 m_TerrainTextureUVScale;
};

