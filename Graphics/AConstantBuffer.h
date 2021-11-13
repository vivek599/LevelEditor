#pragma once



#include "ACommonIncludes.h"

class ARenderDevice;

enum AShaderVisibility
{
	VSVISIBLE = 1 << 0,
	HSVISIBLE = 1 << 1,
	DSVISIBLE = 1 << 2,
	GSVISIBLE = 1 << 3,
	PSVISIBLE = 1 << 4,
	CSVISIBLE = 1 << 5,
};

class AConstantBuffer
{
private:

	ComPtr<ID3D11Buffer>					m_Buffer;
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;


	size_t									m_BufferSize;
	AShaderVisibility						m_Visibility;
	void CreateBuffer();

public:
	AConstantBuffer(ARenderDevice* renderDevice, AShaderVisibility visibility );
	~AConstantBuffer();

	bool UpdateBuffer(const void* data, size_t size);


	ComPtr<ID3D11Buffer> GetBuffer();

	AShaderVisibility GetVisibility();

};






