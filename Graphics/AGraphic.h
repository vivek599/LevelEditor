#pragma once

#include "../Graphics/ACommonIncludes.h"



class AGraphic
{
public:
	AGraphic(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen);
	~AGraphic();


	void BeginScene(float r, float g, float b, float a);
	bool Update(float deltaTime);
	bool Render();
	void EndScene();

	Matrix GetProjectionMatrix();
	Matrix GetWorldMatrix();
	Matrix GetOrthoMatrix();

private:

	Matrix m_ProjectionMatrix;
	Matrix m_WorldMatrix;
	Matrix m_OrthoMatrix;

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
};

