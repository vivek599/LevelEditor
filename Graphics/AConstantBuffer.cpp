#include "AConstantBuffer.h"
#include "ARenderDevice.h"

void AConstantBuffer::CreateBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)m_BufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = m_Device->CreateBuffer(&bufferDesc, nullptr, m_Buffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(string("CreateBuffer Failed!") + string(__FUNCTION__) + to_string(__LINE__));
	}
}

AConstantBuffer::AConstantBuffer(ARenderDevice* renderDevice, AShaderVisibility visibility) :
	m_Visibility(visibility),
	m_Device(renderDevice->GetDevice()),
	m_DeviceContext(renderDevice->GetContext())
{ 
	CreateBuffer();
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
		ALOG(string("Map Failed!") + string(__FUNCTION__) + to_string(__LINE__));
	}

	memcpy(mappedResource.pData, data, m_BufferSize);

	m_DeviceContext->Unmap(m_Buffer.Get(), 0);

	return true;
}

ComPtr<ID3D11Buffer> AConstantBuffer::GetBuffer()
{
	return m_Buffer;
}

AShaderVisibility AConstantBuffer::GetVisibility()
{
	return m_Visibility;
} 








