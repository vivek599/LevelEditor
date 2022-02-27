#include "ATexture.h"
#include "ARenderDevice.h"
 
bool ATexture::CreateBuffer(UINT Width, UINT Height, DXGI_FORMAT Format, UINT Stride)
{
	HRESULT hr = S_OK;
	if (m_Buffer)
	{
		return false;
	}

	m_Type = ATEX_RWBUFFER;
	m_Width = Width;
	m_Height = Height;

	m_bUAV = true;

	// Create a GPU buffer to store the data.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = Width * Height * Stride;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	if (IsUAV())
	{
		bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = Stride;

	hr = m_Device->CreateBuffer(&bufferDesc, nullptr, m_Buffer.ReleaseAndGetAddressOf());
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
		uavDesc.Buffer.NumElements = Width * Height;
		uavDesc.Buffer.Flags = 0;

		hr = m_Device->CreateUnorderedAccessView(m_Buffer.Get(), &uavDesc, m_pUAV.ReleaseAndGetAddressOf());
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
		srvDesc.Buffer.NumElements = Width* Height;

		hr = m_Device->CreateShaderResourceView(m_Buffer.Get(), &srvDesc, m_pSRV.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ALOG(wstring(TEXT("CreateShaderResourceView Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
			return false;
		}
	}

	return true;
}
 
void ATexture::CreateRWTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format)
{
	if (m_Resource2D)
	{
		return;
	}

	m_Type = ATEX_RW2D;
	m_Width = Width;
	m_Height = Height;

	D3D11_TEXTURE2D_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_TEXTURE2D_DESC));
	Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = m_Format;
	Desc.Height = Height;
	Desc.Width = Width;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	//D3D11_SUBRESOURCE_DATA subData = {};
	//vector<float> initialData(m_Width * m_Height, 0.000001f);
	//subData.pSysMem = initialData.data();
	//subData.SysMemPitch = m_Width * m_PixelSizeOrStride;
	//subData.SysMemSlicePitch = m_Width * m_Height * m_PixelSizeOrStride;

	HRESULT hr = m_Device->CreateTexture2D(&Desc, nullptr, m_Resource2D.ReleaseAndGetAddressOf());
	if (FAILED(hr)) 
	{
		throw "RWTexture2D Create Failed!";
	}

	m_bUAV = true;
	//Create UAV 
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = m_Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	hr = m_Device->CreateUnorderedAccessView(m_Resource2D.Get(), &uavDesc, m_pUAV.ReleaseAndGetAddressOf());
	if (FAILED(hr)) 
	{
		throw "Error! UAV Creation failed!";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = m_Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 0;
	hr = m_Device->CreateShaderResourceView(m_Resource2D.Get(), &srvDesc, m_pSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		throw "Error! SRV Creation failed!";
	}
}

bool ATexture::CreateRWBuffer(UINT Width, UINT Height, DXGI_FORMAT Format, UINT Stride)
{
	if (m_Buffer)
	{
		return false;
	}

	HRESULT hr = S_OK;
	m_Type = ATEX_RWBUFFER;
	m_Width = Width;
	m_Height = Height;

	// Create a GPU buffer to store the data.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = Width * Height * Stride;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	if (IsUAV())
	{
		bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	bufferDesc.StructureByteStride = 0;

	hr = m_Device->CreateBuffer(&bufferDesc, nullptr, m_Buffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(wstring(TEXT("CreateBuffer Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
		return false;
	}

	if (IsUAV())
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = Width * Height;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

		hr = m_Device->CreateUnorderedAccessView(m_Buffer.Get(), &uavDesc, m_pUAV.ReleaseAndGetAddressOf());
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
		srvDesc.Buffer.NumElements = Width * Height;

		hr = m_Device->CreateShaderResourceView(m_Buffer.Get(), &srvDesc, m_pSRV.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ALOG(wstring(TEXT("CreateShaderResourceView Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
			return false;
		}
	}


}

UINT ATexture::GetPixelSize(DXGI_FORMAT Format)
{
	switch (Format)
	{
	case DXGI_FORMAT_UNKNOWN:
		break;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 12;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
		return 8;

	case DXGI_FORMAT_R32G8X24_TYPELESS:
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		break;
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		break;
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		break;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
		return 4;

	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return 4;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
		return 2;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		return 1;

	case DXGI_FORMAT_R1_UNORM:
		break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
		break;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		break;
	case DXGI_FORMAT_BC1_TYPELESS:
		break;
	case DXGI_FORMAT_BC1_UNORM:
		break;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC2_TYPELESS:
		break;
	case DXGI_FORMAT_BC2_UNORM:
		break;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC3_TYPELESS:
		break;
	case DXGI_FORMAT_BC3_UNORM:
		break;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC4_TYPELESS:
		break;
	case DXGI_FORMAT_BC4_UNORM:
		break;
	case DXGI_FORMAT_BC4_SNORM:
		break;
	case DXGI_FORMAT_BC5_TYPELESS:
		break;
	case DXGI_FORMAT_BC5_UNORM:
		break;
	case DXGI_FORMAT_BC5_SNORM:
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return 4;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		break;
	case DXGI_FORMAT_BC6H_TYPELESS:
		break;
	case DXGI_FORMAT_BC6H_UF16:
		break;
	case DXGI_FORMAT_BC6H_SF16:
		break;
	case DXGI_FORMAT_BC7_TYPELESS:
		break;
	case DXGI_FORMAT_BC7_UNORM:
		break;
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		break;
	case DXGI_FORMAT_AYUV:
		break;
	case DXGI_FORMAT_Y410:
		break;
	case DXGI_FORMAT_Y416:
		break;
	case DXGI_FORMAT_NV12:
		break;
	case DXGI_FORMAT_P010:
		break;
	case DXGI_FORMAT_P016:
		break;
	case DXGI_FORMAT_420_OPAQUE:
		break;
	case DXGI_FORMAT_YUY2:
		break;
	case DXGI_FORMAT_Y210:
		break;
	case DXGI_FORMAT_Y216:
		break;
	case DXGI_FORMAT_NV11:
		break;
	case DXGI_FORMAT_AI44:
		break;
	case DXGI_FORMAT_IA44:
		break;
	case DXGI_FORMAT_P8:
		break;
	case DXGI_FORMAT_A8P8:
		break;
	case DXGI_FORMAT_B4G4R4A4_UNORM:
		break;
	case DXGI_FORMAT_P208:
		break;
	case DXGI_FORMAT_V208:
		break;
	case DXGI_FORMAT_V408:
		break;
	case DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
		break;
	case DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
		break;
	case DXGI_FORMAT_FORCE_UINT:
		break;
	default:
		break;
	}

	return 4;
}

ATexture::ATexture(ARenderDevice* renderDevice) :
	m_Device(renderDevice->GetDevice()),
	m_DeviceContext(renderDevice->GetContext())
{
	
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
	destRegion.front	= 0;	destRegion.back		= 1;

	switch (m_Type)
	{
	case ATEX_1D:
		m_DeviceContext->UpdateSubresource(m_Resource1D.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	case ATEX_2D:
		m_DeviceContext->UpdateSubresource( m_Resource2D.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	case ATEX_3D:
		m_DeviceContext->UpdateSubresource(m_Resource3D.Get(), 0, &destRegion, pData, m_Width * pixelSize, m_Width * pixelSize * m_Height );
		break;
	case ATEX_STRUCT_BUFFER:
		m_DeviceContext->UpdateSubresource(m_Buffer.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	case ATEX_RW2D:
		m_DeviceContext->UpdateSubresource(m_Resource2D.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	case ATEX_RWBUFFER:
		m_DeviceContext->UpdateSubresource(m_Buffer.Get(), 0, &destRegion, pData, m_Width * pixelSize, 0);
		break;
	}

	return true;
}

ID3D11ShaderResourceView* ATexture::GetSRV() const
{
	return m_pSRV.Get();
}

ID3D11UnorderedAccessView* ATexture::GetUAV() const
{
	return m_pUAV.Get();
}

ID3D11RenderTargetView* ATexture::GetRTV() const
{
	return m_pRTV.Get();
}

bool ATexture::IsUAV()
{
	return m_bUAV;
}

void ATexture::CreateTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format)
{
	m_Type = ATEX_2D;
	m_Width = Width;
	m_Height = Height;
	m_Format = Format;
	if (m_Resource2D)
	{
		return;
	}

	D3D11_TEXTURE2D_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_TEXTURE2D_DESC));
	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.Format = Format;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	//D3D11_SUBRESOURCE_DATA subData = {};
	//vector<float> initialData(m_Width * m_Height, 0.000001f);
	//subData.pSysMem = initialData.data();
	//subData.SysMemPitch = m_Width * m_PixelSizeOrStride;
	//subData.SysMemSlicePitch = m_Width * m_Height * m_PixelSizeOrStride;

	HRESULT hr = m_Device->CreateTexture2D(&Desc, nullptr, m_Resource2D.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		throw "RWTexture2D Create Failed!";
	}

	m_bUAV = false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = m_Device->CreateShaderResourceView(m_Resource2D.Get(), &srvDesc, m_pSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		throw "Error! SRV Creation failed!";
	}
}

void ATexture::CreateTexture1D(UINT Width, DXGI_FORMAT Format)
{
	m_Type = ATEX_1D;
	m_Width = Width; 
	if (m_Resource1D)
	{
		return;
	}
	D3D11_TEXTURE1D_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_TEXTURE1D_DESC));
	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = Format;
	Desc.Width = Width;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	//D3D11_SUBRESOURCE_DATA subData = {};
	//vector<float> initialData(m_Width * m_Height, 0.000001f);
	//subData.pSysMem = initialData.data();
	//subData.SysMemPitch = m_Width * m_PixelSizeOrStride;
	//subData.SysMemSlicePitch = m_Width * m_Height * m_PixelSizeOrStride;

	HRESULT hr = m_Device->CreateTexture1D(&Desc, nullptr, m_Resource1D.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		throw "RWTexture2D Create Failed!";
	}

	m_bUAV = false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = m_Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Texture1D.MipLevels = 1;
	srvDesc.Texture1D.MostDetailedMip = 0;
	hr = m_Device->CreateShaderResourceView(m_Resource1D.Get(), &srvDesc, m_pSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		throw "Error! SRV Creation failed!";
	}
}

void ATexture::CreateTexture3D(UINT Width, UINT Height, UINT Depth, DXGI_FORMAT Format)
{
	if (m_Resource3D)
	{
		return;
	}
	throw std::logic_error("The method or operation is not implemented.");
}

void ATexture::CreateRenderTarget2D(UINT Width, UINT Height, DXGI_FORMAT Format)
{
	if (m_Resource2D)
	{
		return;
	}

	m_Type = ATEX_RT_2D;
	m_Width = Width;
	m_Height = Height;
	m_Format = Format;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = Format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = m_Device->CreateTexture2D(&desc, nullptr, m_Resource2D.ReleaseAndGetAddressOf());

	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		SRVDesc.Texture2D.MostDetailedMip = 0;

		hr = m_Device->CreateShaderResourceView(m_Resource2D.Get(), &SRVDesc, m_pSRV.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ALOG(wstring(TEXT("CreateShaderResourceView Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
			throw "Error! SRV Creation failed!";
		}
	}

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = desc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = m_Device->CreateRenderTargetView(m_Resource2D.Get(), &renderTargetViewDesc, m_pRTV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(wstring(TEXT("CreateRenderTargetView Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
		throw "Error! RTV Creation failed!";
	}
}

ID3D11Texture2D* ATexture::GetResource2D() const
{
	return m_Resource2D.Get();
}

