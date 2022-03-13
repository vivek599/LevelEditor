#pragma once



#include "ACommonIncludes.h"

class ARenderDevice;

enum EShaderVisibility
{
	VISIBLE_VERTEX		= 1 << 0,
	VISIBLE_HULL		= 1 << 1,
	VISIBLE_DOMAIN		= 1 << 2,
	VISIBLE_GEOMETRY	= 1 << 3,
	VISIBLE_PIXEL		= 1 << 4,
	VISIBLE_COMPUTE		= 1 << 5,
};

class AConstantBuffer
{
private:

	ComPtr<ID3D11Buffer>					m_Buffer;
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;


	UINT64									m_BufferSize;
	EShaderVisibility						m_Visibility;

public:
	AConstantBuffer(ARenderDevice* renderDevice);
	~AConstantBuffer();

	void CreateBuffer(size_t Size, EShaderVisibility visibility);
	bool UpdateBuffer(const void* data, size_t size);


	ComPtr<ID3D11Buffer> GetBuffer();

	EShaderVisibility GetVisibility();

};






