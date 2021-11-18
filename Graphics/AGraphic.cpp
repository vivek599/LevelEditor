
#include "AGraphic.h"
#include "ARenderDevice.h"


AGraphic::AGraphic(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen)
{
	m_RenderDevice = ARenderDevice::CreateDevice(hwnd, screenWidth, screenHeight, vsyncEnabled, full_screen);

	m_RenderDevice->InitCommonPipeLineStates();
 
	m_Device		= m_RenderDevice->GetDevice();
	m_DeviceContext	= m_RenderDevice->GetContext();

	m_Viewport.Width = (float)screenWidth;
	m_Viewport.Height = (float)screenHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	// Setup the projection matrix.
	m_Near	= 0.1f;
	m_Far	= 1000.0f;
	m_FieldOfView = XM_PIDIV4;
	m_AspectRatio = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_ProjectionMatrix	= XMMatrixPerspectiveFovLH( m_FieldOfView, m_AspectRatio, m_Near, m_Far );
	m_WorldMatrix		= XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoNear	=	0.1f;
	m_OrthoFar	=	100.0f;
	m_OrthoMatrix	 = XMMatrixOrthographicLH( (float)screenWidth, (float)screenHeight, m_OrthoNear, m_OrthoFar );

	m_ResizeSuccess = true;

}

AGraphic::~AGraphic()
{
	ARenderDevice::DestroyDevice(m_RenderDevice);
}

void AGraphic::BeginScene(float r, float g, float b, float a)
{
	if (m_ResizeSuccess)
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;

		// Clear the back buffer.
		m_DeviceContext->ClearRenderTargetView(m_RenderDevice->GetRenderTargetView().Get(), color);

		// Clear the depth buffer.
		//m_DeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		m_RenderDevice->BeginFrame();
	}
}

bool AGraphic::Update(float deltaTime)
{

	return true;
}

bool AGraphic::Render()
{
	if (m_ResizeSuccess)
	{
		m_DeviceContext->RSSetState(m_RenderDevice->GetRSCullBackFace());
		m_DeviceContext->OMSetRenderTargets(1, m_RenderDevice->GetRenderTargetView().GetAddressOf(), NULL);
		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	return true;
}

void AGraphic::EndScene()
{
	if (m_ResizeSuccess)
	{
		m_RenderDevice->EndFrame();
	}
}

void AGraphic::Resize(int width, int height)
{
	m_ResizeSuccess = m_RenderDevice->ResizeSwapChainBuffers(width, height);

	// Set up the viewport.
	m_Viewport.Width = width;
	m_Viewport.Height = height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
}

Matrix AGraphic::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

Matrix AGraphic::GetWorldMatrix()
{
	return m_WorldMatrix;
}

Matrix AGraphic::GetOrthoMatrix()
{
	return m_OrthoMatrix;
}

