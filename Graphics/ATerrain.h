#pragma once

#include "ACommonIncludes.h"

class ARenderDevice;

class ATerrain
{
public:
	ATerrain();
	~ATerrain();

	bool Initialize(ARenderDevice* renderDevice, const wchar_t* heightMapFilePath, const wchar_t* pixelShaderFilePath, const wchar_t* vertexShaderFilePath);
	void Render(ID3D11DeviceContext* context, Matrix worlMatrix, Matrix viewMatrix, Matrix projMatrix);

	int GetIndexCount();

private:

	struct VertexType
	{
		Vector3 position;
		Vector4 color;
	};

	bool LoadHeightMap(const wchar_t* heightMapFilePath);
	bool LoadHeightMapFromRAW(const wchar_t* heightMapFilePath, uint32_t width, uint32_t height);
	void NormalizeHeightMap();
	void ShutdownHeightMap();

	uint32_t m_TerrainWidth;
	uint32_t m_TerrainHeight;
	uint32_t m_VertexCount;
	uint32_t m_IndexCount;

	ComPtr<ID3D11Buffer>	m_VertexBuffer;
	ComPtr<ID3D11Buffer>	m_IndexBuffer;

	Vector3*				m_HeightMap;

	unique_ptr<class AShaderCache>	m_PixelShader;
	unique_ptr<class AShaderCache>	m_VertexShader;

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	ComPtr<ID3D11Buffer>	m_MatrixBuffer; 
};

