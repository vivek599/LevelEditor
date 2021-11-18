#include "ARenderDevice.h"

ARenderDevice::ARenderDevice(const ARenderDevice&)
{

}

ARenderDevice::ARenderDevice(const InitialisationParams& initParams) :
	m_ScreenWidth(initParams.screenWidth),
	m_ScreenHeight(initParams.screenHeight),
	m_VsyncEnabled(initParams.vsyncEnabled),
	m_FullScreen(initParams.fullScreen),
	m_SwapChain(initParams.dxgiSwapChain),
	m_Device(initParams.d3dDevice),
	m_DeviceContext(initParams.d3dDeviceContext),
	m_RenderTargetView(initParams.d3dRenderTargetView)
{
	InitCommonPipeLineStates();

}

void ARenderDevice::operator=(const ARenderDevice&)
{

}

ARenderDevice::~ARenderDevice()
{

}

ARenderDevice* ARenderDevice::CreateDevice(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = MAX_BUFFER_COUNT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	const D3D_FEATURE_LEVEL requiredFeatureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	IDXGISwapChain*			dxgiSwapChain;
	ID3D11Device*			d3dDevice;
	ID3D11DeviceContext*	d3dDeviceContext;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, // Default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		requiredFeatureLevel,
		_countof(requiredFeatureLevel),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&dxgiSwapChain,
		&d3dDevice,
		nullptr,
		&d3dDeviceContext);

	AHRASSERT(hr);

	ComPtr<ID3D11Resource> backBuffer;
	HRESULT getBackBuffer = dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(getBackBuffer))
	{
		d3dDeviceContext->Release();
		d3dDevice->Release();
		dxgiSwapChain->Release();
		return 0;
	}

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {}; 
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	ID3D11RenderTargetView* d3dRenderTargetView;
	HRESULT createRenderTargetView = d3dDevice->CreateRenderTargetView( backBuffer.Get(), &viewDesc, &d3dRenderTargetView);

	//  Create our graphics device
	InitialisationParams gfxParams;
	gfxParams.window					= hwnd;
	gfxParams.screenWidth				= screenWidth;
	gfxParams.screenHeight				= screenHeight;
	gfxParams.vsyncEnabled				= vsyncEnabled;
	gfxParams.dxgiSwapChain				= dxgiSwapChain;
	gfxParams.d3dDevice					= d3dDevice;
	gfxParams.d3dDeviceContext			= d3dDeviceContext;
	gfxParams.d3dRenderTargetView		= d3dRenderTargetView; 
	 
	ARenderDevice* newDevice = new ARenderDevice(gfxParams); 


	return newDevice;
}

void ARenderDevice::DestroyDevice(ARenderDevice* device)
{
	delete device;
}

void ARenderDevice::BeginFrame()
{

}

void ARenderDevice::EndFrame()
{
	if (m_VsyncEnabled)
	{
		m_SwapChain->Present( 1, 0);
	}
	else
	{
		m_SwapChain->Present( 0, DXGI_PRESENT_ALLOW_TEARING);
	}
}

ComPtr<ID3D11Device> ARenderDevice::GetDevice()
{
	return m_Device;
}

ComPtr<ID3D11DeviceContext> ARenderDevice::GetContext()
{
	return m_DeviceContext;
} 

ComPtr<ID3D11RenderTargetView> ARenderDevice::GetRenderTargetView()
{
	return m_RenderTargetView;
}

ID3D11RasterizerState* ARenderDevice::GetRSWireFrame()
{
	return m_RasterizerStateWireFrame.Get();
}

ID3D11RasterizerState* ARenderDevice::GetRSCullBackFace()
{
	return m_RasterizerStateCullBackFace.Get();
}

ID3D11RasterizerState* ARenderDevice::GetRSCullFrontFace()
{
	return m_RasterizerStateCullFrontFace.Get();
}

ID3D11RasterizerState* ARenderDevice::GetRSNoCullFace()
{
	return m_RasterizerStateNoCullFace.Get();
}

ID3D11DepthStencilState* ARenderDevice::GetDepthEnableState()
{
	return m_DepthEnableStencilState.Get();
}

ID3D11DepthStencilState* ARenderDevice::GetDepthDisableState()
{
	return m_DepthDisableStencilState.Get();
}

ID3D11SamplerState* ARenderDevice::GetSampleRepeat()
{
	return m_SampleStateRepeat.Get();
}

ID3D11SamplerState* ARenderDevice::GetSampleMirror()
{
	return m_SampleStateMirror.Get();
}

ID3D11SamplerState* ARenderDevice::GetSampleClamp()
{
	return m_SampleStateClamp.Get();
}

ID3D11SamplerState* ARenderDevice::GetSampleMirrorOnce()
{
	return m_SampleStateMirrorOnce.Get();
}

void ARenderDevice::OnResize()
{

}

void ARenderDevice::ResizeSwapChainBuffers(uint32_t width, uint32_t height)
{

}

void ARenderDevice::InitCommonPipeLineStates()
{
	CreateBlendStates();
	CreateDepthStates();
	CreateRasterizerStates();
	CreateSamplerStates();
}

bool ARenderDevice::CreateBlendStates()
{
	D3D11_BLEND_DESC blendStateDescription = {};

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	for (int i = 1; i < 8; i++)
	{
		blendStateDescription.RenderTarget[i] = blendStateDescription.RenderTarget[0];
	}

	// Create the blend state using the description.
	HRESULT hr = m_Device->CreateBlendState(&blendStateDescription, m_AlphaEnableBlendState.ReleaseAndGetAddressOf());
	AHRASSERT(SUCCEEDED(result));

	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	hr = m_Device->CreateBlendState(&blendStateDescription, m_AlphaDisableBlendState.ReleaseAndGetAddressOf());
	AHRASSERT(SUCCEEDED(result));

	return true;
}

bool ARenderDevice::CreateDepthStates()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//stencil operation if pixel is front facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//if pixel is back facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create depth stencil state
	HRESULT hr = m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthDisableStencilState.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.StencilEnable = true;

	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthEnableStencilState.ReleaseAndGetAddressOf());
	AHRASSERT(hr);
	return true;
}

bool ARenderDevice::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.f;

	HRESULT hr = m_Device->CreateRasterizerState(&rasterDesc, m_RasterizerStateNoCullFace.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	rasterDesc.CullMode = D3D11_CULL_BACK;
	hr = m_Device->CreateRasterizerState(&rasterDesc, m_RasterizerStateCullBackFace.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	hr = m_Device->CreateRasterizerState(&rasterDesc, m_RasterizerStateCullFrontFace.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = m_Device->CreateRasterizerState(&rasterDesc, m_RasterizerStateWireFrame.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	return true;
}

bool ARenderDevice::CreateSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = m_Device->CreateSamplerState(&samplerDesc, m_SampleStateClamp.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = m_Device->CreateSamplerState(&samplerDesc, m_SampleStateRepeat.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

	hr = m_Device->CreateSamplerState(&samplerDesc, m_SampleStateMirror.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

	hr = m_Device->CreateSamplerState(&samplerDesc, m_SampleStateMirrorOnce.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	return true;
}







