
#include "AGraphic.h"
#include "ARenderDevice.h"
#include "ATerrain.h"
#include "ACemara.h"


AGraphic::AGraphic(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen) :
	m_ResizeSuccess(true),
	m_TerrainInitilized(false)
{
	m_RenderDevice = ARenderDevice::CreateDevice(hwnd, screenWidth, screenHeight, vsyncEnabled, full_screen);

	m_RenderDevice->InitCommonPipeLineStates();
	m_RenderDevice->InitDepthBuffers();

	m_Device		= m_RenderDevice->GetDevice();
	m_DeviceContext	= m_RenderDevice->GetContext();

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_Viewport.width = (float)screenWidth;
	m_Viewport.height = (float)screenHeight;
	m_Viewport.minDepth = 0.0f;
	m_Viewport.maxDepth = 1.0f;
	m_Viewport.x = 0.0f;
	m_Viewport.y = 0.0f;

	// Setup the projection matrix.
	m_Near	= 0.1f;
	m_Far	= 10000.0f;
	m_FieldOfView = XM_PIDIV4;
	m_AspectRatio = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_ProjectionMatrix	= XMMatrixPerspectiveFovLH( m_FieldOfView, m_AspectRatio, m_Near, m_Far );
	m_WorldMatrix		= XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoNear	=	0.1f;
	m_OrthoFar	=	100.0f;

	m_OrthoMatrix	= XMMatrixOrthographicLH( (float)screenWidth, (float)screenHeight, m_OrthoNear, m_OrthoFar );
	//m_ViewMatrix	= XMMatrixLookAtLH( Vector3(0.0f, 256.0f, -256.0f), Vector3(512, 0.0f, 512.0f), Vector3(0.0f, 1.0f, 0.0f));

	m_Camera.reset(new ACemara());
	m_Camera->SetPosition(0.0f, 256.0f, -256.0f);
	m_Camera->SetFocus(512, 0.0f, 512.0f);
	m_Camera->CreateFrustum(m_ProjectionMatrix);
}

AGraphic::~AGraphic()
{
	ARenderDevice::DestroyDevice(m_RenderDevice);
}

bool AGraphic::InitializeTerrain(TerrainInitializationParams* params)
{
	m_Terrain.reset(new ATerrain());
	m_TerrainInitilized = m_Terrain->Initialize( m_RenderDevice, *params);

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
	m_Camera->Update(deltaTime);
	if (m_TerrainInitilized)
	{
		m_Terrain->Update(m_RenderDevice, deltaTime, m_WorldMatrix, m_Camera->GetViewMatrix(), m_ProjectionMatrix);
		UnprojectMouseCoord();

		if (m_LeftMouseDown)
		{
			if (!m_Terrain->SculptingInProgress() || m_MouseMoving)
			{
				if (m_Terrain->RayTerrainIntersect(m_RayOrigin, m_RayDirection))
				{
					SculptTerrain(deltaTime);
				}
			}
			else
			{
				SculptTerrain(deltaTime);
			}
		}
		else
		{
			m_Terrain->ResetSculptingProgress();
		}
	}

	return true;
}

void AGraphic::SculptTerrain(float deltaTime)
{
	switch (m_TerrainSculptmode)
	{
	case RAISE:
		m_Terrain->Raise(deltaTime);
		break;
	case LOWER:
		m_Terrain->Lower(deltaTime);
		break;
	case FLATTEN:
		m_Terrain->Flatten(deltaTime);
		break;
	case SMOOTH:
		m_Terrain->Smooth(deltaTime);
		break;
	default:
		break;
	}
}

bool AGraphic::Render()
{
	m_Camera->Render();

	if (m_ResizeSuccess)
	{
		m_DeviceContext->RSSetState(m_RenderDevice->GetRSCullBackFace());
		m_DeviceContext->OMSetRenderTargets(1, m_RenderDevice->GetRenderTargetView().GetAddressOf(), m_RenderDevice->GetDepthStencilView().Get());
		m_DeviceContext->RSSetViewports(1, m_Viewport.Get11());
	}
	else
	{
		return false;
	}

	if (m_TerrainInitilized)
	{
		m_Terrain->SetAmbientColor(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		m_Terrain->SetDiffuseColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_Terrain->SetLightDirection(Vector3(-0.5f, -1.0f, 0.0f));
		m_Terrain->SetTextureUVScale(m_TextureUVScale);
		m_Terrain->Render(m_RenderDevice);
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
	m_Viewport.width = width;
	m_Viewport.height = height;
	m_Viewport.minDepth = 0.0f;
	m_Viewport.maxDepth = 1.0f;
	m_Viewport.x = 0;
	m_Viewport.y = 0;
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

void AGraphic::SetTextureUVScale(float val)
{
	m_TextureUVScale = val;
}

bool AGraphic::UnprojectMouseCoord()
{ 
	Vector3 mouseNear	= Vector3(m_MouseX, m_MouseY, 0.0f);
	Vector3 mouseFar	= Vector3(m_MouseX, m_MouseY, 1.0f);

	mouseNear	= m_Viewport.Unproject(mouseNear, m_ProjectionMatrix, m_Camera->GetViewMatrix(), m_WorldMatrix);
	mouseFar	= m_Viewport.Unproject(mouseFar, m_ProjectionMatrix, m_Camera->GetViewMatrix(), m_WorldMatrix);

	m_RayDirection = mouseFar - mouseNear;
	m_RayDirection.Normalize();

	m_RayOrigin = mouseNear;
	 
	return true;
}

void AGraphic::SetMouseState(int mouseX, int mouseY, bool mouseDown, bool mouseMoving)
{
	m_MouseX = mouseX;
	m_MouseY = mouseY;
	m_LeftMouseDown = mouseDown;
	m_MouseMoving = mouseMoving;
}

void AGraphic::SetSculptRadius(int radius)
{
	if (m_Terrain)
	{
		m_Terrain->SetBrushRadius(radius);
	}
}

void AGraphic::SetSculptStrenght(int strength)
{
	if (m_Terrain)
	{
		m_Terrain->SetBrushStrength(strength);
	}
}

void AGraphic::SetTerrainSculptMode(ESculptMode mode)
{
	m_TerrainSculptmode = mode;
}





