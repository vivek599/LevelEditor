#pragma once

#include "../Graphics/ACommonIncludes.h"



class AGraphic
{
public:
	AGraphic(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen);
	~AGraphic();

	bool InitializeTerrain( const wchar_t* heightMapFilePath, const wchar_t* pixelShaderFilePath, const wchar_t* vertexShaderFilePath, const wchar_t* textureFilename);
	void BeginScene(Color color);
	bool Update(float deltaTime);
	bool Render();
	void EndScene();
	void Resize(int width, int height);

	Matrix GetProjectionMatrix();
	Matrix GetWorldMatrix();
	Matrix GetOrthoMatrix();

	void SetTextureUVScale(float val);
private:

	Matrix m_ProjectionMatrix;
	Matrix m_WorldMatrix;
	Matrix m_OrthoMatrix;
	Matrix m_ViewMatrix;

	class ARenderDevice*	m_RenderDevice;

	ComPtr<ID3D11Device>				m_Device;
	ComPtr<ID3D11DeviceContext>			m_DeviceContext;
	D3D11_VIEWPORT			m_Viewport;
	float					m_FieldOfView;
	float					m_AspectRatio;
	float					m_Near;
	float					m_Far;
	float					m_OrthoNear;
	float					m_OrthoFar;

	bool					m_ResizeSuccess;
	bool					m_TerrainInitilized;

	unique_ptr<class ATerrain> m_Terrain;
	float					m_TextureUVScale;
};

