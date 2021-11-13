#pragma once

#include "ACommonIncludes.h"


class ARenderDevice;

enum ATextureType
{
	ATEX_1D,
	ATEX_2D,
	ATEX_3D, 
	ATEX_STRUCT_BUFFER,
};

class ATexture
{
private:

	ATextureType m_Type;

	ComPtr<ID3D11Resource>					m_textureResource;
	ComPtr<ID3D11Texture1D>					m_textureResource1D;
	ComPtr<ID3D11Texture2D>					m_textureResource2D;
	ComPtr<ID3D11Texture3D>					m_textureResource3D;
	ComPtr<ID3D11Buffer>					m_Buffer;

	ComPtr<ID3D11ShaderResourceView>		m_pSRV;
	ComPtr<ID3D11UnorderedAccessView>		m_pUAV;
	ComPtr<ID3D11RenderTargetView>			m_pRTV;

	bool CreateBuffer(); 

	bool m_bUAV; 
	UINT m_Width;
	UINT m_Height;
	UINT m_MipLevels;
	UINT m_ArraySize;
	UINT m_PixelSizeOrStride;
	DXGI_FORMAT m_Format;
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;
	D3D11_SUBRESOURCE_DATA					m_InitData;

public:
	ATexture(ARenderDevice* renderDevice, ATextureType type, UINT width = 256, UINT height = 256, UINT ArraySize = 1, DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM, bool bUav = false, UINT pixelSize = 4 );
	~ATexture();

	bool LoadFromFile(const wchar_t* textureFileName); 
	bool LoadData(const void* pData, size_t size, UINT pixelSize); 

	ComPtr<ID3D11ShaderResourceView>	GetSRV() const; 
	ComPtr<ID3D11UnorderedAccessView>	GetUAV() const; 
	ComPtr<ID3D11RenderTargetView>		GetRTV() const; 
	 
	ATextureType GetType();
	bool IsUAV();
};
 




