
#include "AGraphic.h"
#include "ARenderDevice.h"
#include "ATerrain.h"


AGraphic::AGraphic(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen) :
	m_ResizeSuccess(true),
	m_TerrainInitilized(false)
{
	m_RenderDevice = ARenderDevice::CreateDevice(hwnd, screenWidth, screenHeight, vsyncEnabled, full_screen);

	m_RenderDevice->InitCommonPipeLineStates();
	m_RenderDevice->InitDepthBuffers();

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

	m_OrthoMatrix	= XMMatrixOrthographicLH( (float)screenWidth, (float)screenHeight, m_OrthoNear, m_OrthoFar );
	m_ViewMatrix	= XMMatrixLookAtLH( Vector3(0.0f, 50.0f, -10.0f), Vector3(512, 0.0f, 512.0f), Vector3(0.0f, 1.0f, 0.0f));


}

AGraphic::~AGraphic()
{
	ARenderDevice::DestroyDevice(m_RenderDevice);
}

bool AGraphic::InitializeTerrain( const wchar_t* heightMapFilePath, const wchar_t* pixelShaderFilePath, const wchar_t* vertexShaderFilePath)
{
	m_Terrain.reset(new ATerrain());
	m_TerrainInitilized = m_Terrain->Initialize( m_RenderDevice, heightMapFilePath, pixelShaderFilePath, vertexShaderFilePath);

	return true;
}

void AGraphic::BeginScene(Color color)
{
	if (m_ResizeSuccess)
	{
		float c[4];

		// Setup the color to clear the buffer to.
		c[0] = color.R();
		c[1] = color.G();
		c[2] = color.B();
		c[3] = color.A();
		 
		// Clear the back buffer.
		m_DeviceContext->ClearRenderTargetView(m_RenderDevice->GetRenderTargetView().Get(), c);

		// Clear the depth buffer.
		m_DeviceContext->ClearDepthStencilView(m_RenderDevice->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

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
		m_DeviceContext->OMSetRenderTargets(1, m_RenderDevice->GetRenderTargetView().GetAddressOf(), m_RenderDevice->GetDepthStencilView().Get());
		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}
	else
	{
		return false;
	}

	if (m_TerrainInitilized)
	{
		m_Terrain->Render(m_RenderDevice->GetContext().Get(), m_WorldMatrix, m_ViewMatrix, m_ProjectionMatrix);
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

