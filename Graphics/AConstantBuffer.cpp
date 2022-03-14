#include "AConstantBuffer.h"
#include "ARenderDevice.h"

void AConstantBuffer::CreateBuffer(size_t Size, EShaderVisibility visibility)
{
	D3D11_BUFFER_DESC bufferDesc;

	m_Visibility = visibility;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = m_BufferSize = (UINT)Size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = m_Device->CreateBuffer(&bufferDesc, nullptr, m_Buffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(wstring(TEXT("CreateBuffer Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
	}
}

AConstantBuffer::AConstantBuffer(ARenderDevice* renderDevice) : 
	m_Device(renderDevice->GetDevice()),
	m_DeviceContext(renderDevice->GetContext())
{ 

}

AConstantBuffer::~AConstantBuffer()
{

}

bool AConstantBuffer::UpdateBuffer(const void* data, size_t size)
{
	HRESULT hr = S_OK;

	assert(size == m_BufferSize); 

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = m_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		ALOG(wstring(TEXT("Map Failed!")) + wstring(TEXT(__FUNCTION__)) + to_wstring(__LINE__));
	}

	memcpy(mappedResource.pData, data, m_BufferSize);

	m_DeviceContext->Unmap(m_Buffer.Get(), 0);

	return true;
}

ComPtr<ID3D11Buffer> AConstantBuffer::GetBuffer()
{
	return m_Buffer;
}

EShaderVisibility AConstantBuffer::GetVisibility()
{
	return m_Visibility;
} 








