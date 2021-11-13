#pragma once


#include "ACommonIncludes.h"


enum ARenderTargetType
{
	ATEX_1D_RT,
	ATEX_2D_RT,
	ATEX_3D_RT
};

class ARenderDevice;

class ARenderTarget
{

private:

	bool CreateTexture1D();
	bool CreateTexture2D();
	bool CreateTexture3D();

	void CreateDSV();
	ARenderTargetType m_Type;

	UINT m_Width;
	UINT m_Height;
	UINT m_ArraySize;
	DXGI_FORMAT m_Format;

	D3D11_VIEWPORT m_Viewport;

	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;

	ComPtr<ID3D11ShaderResourceView>		m_pSRV;
	ComPtr<ID3D11DepthStencilView>			m_pDSV;
	ComPtr<ID3D11RenderTargetView>			m_pRTV;

	ComPtr<ID3D11Texture1D>					m_RenderTargetTexture1D;
	ComPtr<ID3D11Texture2D>					m_RenderTargetTexture2D;
	ComPtr<ID3D11Texture3D>					m_RenderTargetTexture3D;
	ComPtr<ID3D11Texture2D>					m_DepthStencilBuffer;

public:
	ARenderTarget(ARenderDevice* renderDevice, ARenderTargetType type, UINT width, UINT height, UINT ArraySize, DXGI_FORMAT Format);
	~ARenderTarget();

	ComPtr<ID3D11ShaderResourceView>		GetSRV() const;
	ComPtr<ID3D11DepthStencilView>			GetDSV() const;
	ComPtr<ID3D11RenderTargetView>			GetRTV() const;
	D3D11_VIEWPORT							GetViewport() const;

};