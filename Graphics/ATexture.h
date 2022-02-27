#pragma once

#include "ACommonIncludes.h"


class ARenderDevice;

class ATexture
{
private:

	ComPtr<ID3D11Resource>					m_Resource;
	ComPtr<ID3D11Texture1D>					m_Resource1D;
	ComPtr<ID3D11Texture2D>					m_Resource2D;
	ComPtr<ID3D11Texture3D>					m_Resource3D;
	ComPtr<ID3D11Buffer>					m_Buffer;

	ComPtr<ID3D11ShaderResourceView>		m_pSRV;
	ComPtr<ID3D11UnorderedAccessView>		m_pUAV;
	ComPtr<ID3D11RenderTargetView>			m_pRTV;

	enum ATextureType
	{
		ATEX_1D,
		ATEX_2D,
		ATEX_3D,
		ATEX_RW2D,
		ATEX_STRUCT_BUFFER,
		ATEX_RWBUFFER,
		ATEX_RT_2D
	}m_Type;

	bool m_bUAV; 
	UINT m_Width;
	UINT m_Height;
	UINT m_Depth;
	//UINT m_MipLevels;
	//UINT m_ArraySize;
	//UINT m_PixelSizeOrStride;
	DXGI_FORMAT m_Format;
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;
	D3D11_SUBRESOURCE_DATA					m_InitData;

	UINT									GetPixelSize(DXGI_FORMAT Format);

public:
	ATexture(ARenderDevice* renderDevice);
	~ATexture();

	bool LoadFromFile(const wchar_t* textureFileName); 
	bool LoadData(const void* pData, size_t size, UINT pixelSize); 

	ID3D11ShaderResourceView*	GetSRV() const; 
	ID3D11UnorderedAccessView*	GetUAV() const; 
	ID3D11RenderTargetView*		GetRTV() const; 
	 
	ATextureType GetType();
	bool IsUAV();
	void CreateRWTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format);
	bool CreateRWBuffer(UINT Width, UINT Height, DXGI_FORMAT Format, UINT Stride);
	bool CreateBuffer(UINT Width, UINT Height, DXGI_FORMAT Format, UINT Stride);
	void CreateTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format);
	void CreateTexture1D(UINT Width, DXGI_FORMAT Format);
	void CreateTexture3D(UINT Width, UINT Height, UINT Depth, DXGI_FORMAT Format);
	void CreateRenderTarget2D(UINT Width, UINT Height, DXGI_FORMAT Format);
	ID3D11Texture2D* GetResource2D() const;
};
 




