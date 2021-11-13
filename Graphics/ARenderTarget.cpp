#include "ARenderTarget.h"
#include "ARenderDevice.h"

ARenderTarget::ARenderTarget(ARenderDevice* renderDevice, ARenderTargetType type, UINT width, UINT height, UINT ArraySize, DXGI_FORMAT Format ) :
	m_Type(type),
	m_Width(width),
	m_Height(height),
	m_ArraySize(ArraySize),
	m_Format(Format),
	m_Device(renderDevice->GetDevice()),
	m_DeviceContext(renderDevice->GetContext())
{
	switch (m_Type)
	{
	case ATEX_1D_RT:			CreateTexture1D();	break;
	case ATEX_2D_RT:			CreateTexture2D();	break;
	case ATEX_3D_RT:	 		CreateTexture3D();	break;
	}

	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.Width = (float)m_Width;
	m_Viewport.Height = (float)m_Height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
} 

ARenderTarget::~ARenderTarget()
{

}

bool ARenderTarget::CreateTexture1D()
{
	// Create texture
	D3D11_TEXTURE1D_DESC desc;
	desc.Width = m_Width;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = m_Format;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	float onePixel[] = { 0.0f,0.0f,0.0f,0.0f };
	initData.pSysMem = onePixel;
	initData.SysMemPitch = 1;
	initData.SysMemSlicePitch = 1;

	HRESULT hr = m_Device->CreateTexture1D(&desc, nullptr, &m_RenderTargetTexture1D);
	if (SUCCEEDED(hr) && m_RenderTargetTexture1D != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = m_Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		SRVDesc.Texture1D.MipLevels = 1;

		hr = m_Device->CreateShaderResourceView(m_RenderTargetTexture1D.Get(), &SRVDesc, m_pSRV.ReleaseAndGetAddressOf());

		AHRASSERT(hr);
	}

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format = desc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
	renderTargetViewDesc.Texture1D.MipSlice = 0;

	// Create the render target view.
	hr = m_Device->CreateRenderTargetView(m_RenderTargetTexture1D.Get(), &renderTargetViewDesc, m_pRTV.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	return true;
}

bool ARenderTarget::CreateTexture2D()
{
	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = m_Format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = m_Device->CreateTexture2D(&desc, nullptr, &m_RenderTargetTexture2D);

	if (SUCCEEDED(hr) && m_RenderTargetTexture2D != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = m_Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = m_Device->CreateShaderResourceView(m_RenderTargetTexture2D.Get(), &SRVDesc, m_pSRV.ReleaseAndGetAddressOf());

		AHRASSERT(hr);
	}

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format = desc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = m_Device->CreateRenderTargetView(m_RenderTargetTexture2D.Get(), &renderTargetViewDesc, m_pRTV.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	return true;
}

bool ARenderTarget::CreateTexture3D()
{
	// Create texture
	D3D11_TEXTURE3D_DESC desc;
	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.Depth = m_ArraySize;
	desc.Format = m_Format;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = m_Device->CreateTexture3D(&desc, nullptr, &m_RenderTargetTexture3D);
	if (SUCCEEDED(hr) && m_RenderTargetTexture3D != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		SRVDesc.Texture3D.MipLevels = desc.MipLevels;
		SRVDesc.Texture3D.MostDetailedMip = 0;

		hr = m_Device->CreateShaderResourceView(m_RenderTargetTexture3D.Get(), &SRVDesc, m_pSRV.ReleaseAndGetAddressOf());

		AHRASSERT(hr); 
	} 
	  
	// Setup the description of the render target view. 
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc; 
	 
	renderTargetViewDesc.Format = desc.Format; 
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D; 
	renderTargetViewDesc.Texture3D.MipSlice = 0; 
	 
	// Create the render target view. 
	hr = m_Device->CreateRenderTargetView(m_RenderTargetTexture3D.Get(), &renderTargetViewDesc, m_pRTV.ReleaseAndGetAddressOf()); 
	AHRASSERT(hr);

	return true;
}
 
void ARenderTarget::CreateDSV()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_Width;
	depthBufferDesc.Height = m_Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = m_ArraySize;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = m_Device->CreateTexture2D(&depthBufferDesc, nullptr, m_DepthStencilBuffer.ReleaseAndGetAddressOf());
	AHRASSERT(hr);

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depthStencilViewDesc, m_pDSV.ReleaseAndGetAddressOf() );
	AHRASSERT(hr);
}

ComPtr<ID3D11ShaderResourceView> ARenderTarget::GetSRV() const
{
	return m_pSRV;
}

ComPtr<ID3D11DepthStencilView> ARenderTarget::GetDSV() const
{
	return m_pDSV;
}

ComPtr<ID3D11RenderTargetView> ARenderTarget::GetRTV() const
{
	return m_pRTV;
}

D3D11_VIEWPORT ARenderTarget::GetViewport() const
{
	return m_Viewport;
}

 