#include "ATexture.h"
#include "ARenderDevice.h"
 
bool ATexture::CreateBuffer()
{
	HRESULT hr = S_OK;

	// Create a GPU buffer to store the data.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = m_Width * m_PixelSizeOrStride;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	if (IsUAV())
	{
		bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = m_PixelSizeOrStride;

	hr = m_Device->CreateBuffer(&bufferDesc, nullptr, &m_Buffer);
	if (FAILED(hr))
	{
		ALOG(wstring(TEXT("CreateBuffer Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
		return false;
	}

	if (IsUAV())
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = m_Width * m_Height;
		uavDesc.Buffer.Flags = 0;

		hr = m_Device->CreateUnorderedAccessView(m_Buffer.Get(), &uavDesc, &m_pUAV);
		if (FAILED(hr))
		{
			ALOG(wstring(TEXT("CreateUnorderedAccessView Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
			return false;
		}
	}
	else
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = m_Width* m_Height;

		hr = m_Device->CreateShaderResourceView(m_Buffer.Get(), &srvDesc, &m_pSRV);
		if (FAILED(hr))
		{
			ALOG(wstring(TEXT("CreateShaderResourceView Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
			return false;
		}
	}



	return true;
}
 
ATexture::ATexture(ARenderDevice* renderDevice, ATextureType type, UINT width, UINT height, UINT ArraySize, DXGI_FORMAT Format, bool bUav, UINT pixelSize ) :
	m_Type(type),
	m_Width(width),
	m_Height(height),
	m_ArraySize(ArraySize),
	m_Format(Format),
	m_bUAV(bUav),
	m_PixelSizeOrStride(pixelSize),
	m_Device(renderDevice->GetDevice()),
	m_DeviceContext(renderDevice->GetContext())
{
	switch (m_Type)
	{ 
		case ATEX_STRUCT_BUFFER:	CreateBuffer();		break;
	}
}

ATexture::~ATexture()
{

}

bool ATexture::LoadFromFile(const wchar_t* textureFileName)
{
	HRESULT hr = S_OK;

	switch (m_Type)
	{
	case ATEX_1D:
		//hr = CreateWICTextureFromFile( m_Device.Get(), textureFileName, m_textureResource.ReleaseAndGetAddressOf(), m_pSRV.ReleaseAndGetAddressOf());
		break;
	case ATEX_2D:
		//hr = CreateWICTextureFromFile(m_Device.Get(), textureFileName, m_textureResource.ReleaseAndGetAddressOf(), m_pSRV.ReleaseAndGetAddressOf());
		break;
	case ATEX_3D:
		//hr = CreateWICTextureFromFile(m_Device.Get(), textureFileName, m_textureResource.ReleaseAndGetAddressOf(), m_pSRV.ReleaseAndGetAddressOf());
		break;
	}

	return true;
}

bool ATexture::LoadData(const void* pData, size_t size, UINT pixelSize )
{
	D3D11_BOX destRegion;

	destRegion.left		= 0;	destRegion.right	= m_Width;
	destRegion.top		= 0;	destRegion.bottom	= m_Height;
	destRegion.front	= 0;	destRegion.back		= m_ArraySize;

	switch (m_Type)
	{
	case ATEX_1D:
		m_DeviceContext->UpdateSubresource(m_textureResource1D.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	case ATEX_2D:
		m_DeviceContext->UpdateSubresource( m_textureResource2D.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	case ATEX_3D:
		m_DeviceContext->UpdateSubresource(m_textureResource3D.Get(), 0, &destRegion, pData, m_Width * pixelSize, m_Width * pixelSize * m_Height );
		break;
	case ATEX_STRUCT_BUFFER:
		m_DeviceContext->UpdateSubresource(m_Buffer.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	}

	return true;
}

ComPtr<ID3D11ShaderResourceView> ATexture::GetSRV() const
{
	return m_pSRV;
}

ComPtr<ID3D11UnorderedAccessView> ATexture::GetUAV() const
{
	return m_pUAV;
}

ComPtr<ID3D11RenderTargetView> ATexture::GetRTV() const
{
	return m_pRTV;
}

ATextureType ATexture::GetType()
{
	return m_Type;
}

bool ATexture::IsUAV()
{
	return m_bUAV;
}







