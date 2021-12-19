#include "AShaderCache.h"
#include "ARenderDevice.h"

AShaderCache::AShaderCache(ARenderDevice* renderDevice, AShaderType shaderType) :
	m_Device( renderDevice->GetDevice()),
	m_DeviceContext( renderDevice->GetContext()),
	m_Type ( shaderType )
{


}

AShaderCache::~AShaderCache()
{

}

ComPtr<ID3D11InputLayout> AShaderCache::GetInputLayout()
{
	return m_InputLayout;
}

ComPtr<ID3D11VertexShader> AShaderCache::GetVertexShader()
{ 
	return m_VShader; 
}

ComPtr<ID3D11HullShader> AShaderCache::GetHullShader()
{ 
	return m_HShader;
}

ComPtr<ID3D11DomainShader> AShaderCache::GetDomainShader() 
{ 
	return m_DShader;
}

ComPtr<ID3D11GeometryShader> AShaderCache::GetGeometryShader()
{ 
	return m_GShader;
}

ComPtr<ID3D11PixelShader> AShaderCache::GetPixelShader() 
{
	return m_PShader;
}

ComPtr<ID3D11ComputeShader> AShaderCache::GetComputeShader() 
{ 
	return m_CShader;
}

UINT AShaderCache::GetThreadX()
{
	return m_ThreadX;
}

UINT AShaderCache::GetThreadY()
{
	return m_ThreadY; 
}
 
UINT AShaderCache::GetThreadZ()
{
	return m_ThreadZ;
}

void AShaderCache::SetCSThreads(UINT x, UINT y, UINT z)
{
	m_ThreadX = x;
	m_ThreadY = y;
	m_ThreadZ = z; 
}

bool AShaderCache::CompileVSFromFile(const wchar_t* filepath)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	hr = D3DCompileFromFile(filepath, nullptr, nullptr, "main", "vs_5_0", shaderFlags, 0, &m_ByteCode, &m_ErrorMsg);
	if (FAILED(hr))
	{
		if (m_ErrorMsg)
		{
			MessageBoxA(nullptr, (LPCSTR)m_ErrorMsg->GetBufferPointer(), "Error", MB_OK);
		}
		else
		{
			MessageBoxW(nullptr, filepath, L"Missing vs shader file...", MB_OK);
		}
		return false;
	}

	hr = m_Device->CreateVertexShader(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), nullptr, m_VShader.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(string("CreateVertexShader Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}


	return true;
}

bool AShaderCache::CompileHSFromFile(const wchar_t* filepath)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	hr = D3DCompileFromFile(filepath, nullptr, nullptr, "main", "hs_5_0", shaderFlags, 0, &m_ByteCode, &m_ErrorMsg);
	if (FAILED(hr))
	{
		if (m_ErrorMsg)
		{
			MessageBoxA(nullptr, (LPCSTR)m_ErrorMsg->GetBufferPointer(), "Error", MB_OK);
		}
		else
		{
			MessageBoxW(nullptr, filepath, L"Missing hs shader file...", MB_OK);
		}
		return false;
	}

	hr = m_Device->CreateHullShader(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), nullptr, m_HShader.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(string("CreateHullShader Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}


	return true;
}

bool AShaderCache::CompileDSFromFile(const wchar_t* filepath)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	hr = D3DCompileFromFile(filepath, nullptr, nullptr, "main", "ds_5_0", shaderFlags, 0, &m_ByteCode, &m_ErrorMsg);
	if (FAILED(hr))
	{
		if (m_ErrorMsg)
		{
			MessageBoxA(nullptr, (LPCSTR)m_ErrorMsg->GetBufferPointer(), "Error", MB_OK);
		}
		else
		{
			MessageBoxW(nullptr, filepath, L"Missing ds shader file...", MB_OK);
		}
		return false;
	}

	hr = m_Device->CreateDomainShader(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), nullptr, m_DShader.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(string("CreateDomainShader Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}


	return true;
}

bool AShaderCache::CompileGSFromFile(const wchar_t* filepath)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	hr = D3DCompileFromFile(filepath, nullptr, nullptr, "main", "gs_5_0", shaderFlags, 0, &m_ByteCode, &m_ErrorMsg);
	if (FAILED(hr))
	{
		if (m_ErrorMsg)
		{
			MessageBoxA(nullptr, (LPCSTR)m_ErrorMsg->GetBufferPointer(), "Error", MB_OK);
		}
		else
		{
			MessageBoxW(nullptr, filepath, L"Missing gs shader file...", MB_OK);
		}
		return false;
	}

	hr = m_Device->CreateGeometryShader(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), nullptr, m_GShader.ReleaseAndGetAddressOf()); 
	if (FAILED(hr))
	{
		ALOG(string("CreateGeometryShader Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}


	return true;
}

bool AShaderCache::CompilePSFromFile(const wchar_t* filepath)
{
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	hr = D3DCompileFromFile(filepath, nullptr, nullptr, "main", "ps_5_0", shaderFlags, 0, &m_ByteCode, &m_ErrorMsg);
	if (FAILED(hr))
	{
		if (m_ErrorMsg)
		{
			MessageBoxA(nullptr, (LPCSTR)m_ErrorMsg->GetBufferPointer(), "Error", MB_OK);
		}
		else
		{
			MessageBoxW(nullptr, filepath, L"Missing ps shader file...", MB_OK);
		}
		return false;
	}

	hr = m_Device->CreatePixelShader(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), nullptr, m_PShader.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(string("CreatePixelShader Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}

	return true;
}

bool AShaderCache::CompileCSFromFile(const wchar_t* filepath)
{
	HRESULT hr = S_OK;

	hr = D3DCompileFromFile(filepath, nullptr, nullptr, "main", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_ByteCode, &m_ErrorMsg);
	if (FAILED(hr)) 
	{
		if (m_ErrorMsg)
		{
			MessageBoxA(nullptr, (LPCSTR)m_ErrorMsg->GetBufferPointer(), "Error", MB_OK);
		}
		else
		{
			MessageBoxW(nullptr, filepath, L"Missing cs shader file...", MB_OK);
		}
		return false;
	}

	hr = m_Device->CreateComputeShader(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), nullptr, m_CShader.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		ALOG(string("CreateComputeShader Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}

	return true;
}

bool AShaderCache::CompileShaderFromFile(const wchar_t* filepath)
{
	switch (m_Type)
	{
	case VERTEXSHADER:		CompileVSFromFile(filepath);	break;
	case HULLSHADER:		CompileHSFromFile(filepath);	break;
	case DOMAINSHADER:		CompileDSFromFile(filepath);	break;
	case GEOMETRYSHADER:	CompileGSFromFile(filepath);	break;
	case PIXELSHADER:		CompilePSFromFile(filepath);	break;
	case COMPUTESHADER:		CompileCSFromFile(filepath);	break;; 
	}

	return true;
}

bool AShaderCache::LoadPreCompiled(const wchar_t* filepath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob( filepath, m_ByteCode.ReleaseAndGetAddressOf() );
	if (FAILED(hr))
	{
		ALOG(string("D3DReadFileToBlob Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}

	return true;
}

bool AShaderCache::CreateInputLayout()
{
	HRESULT hr = S_OK;

	// Query input parameters and build the input layout
	D3D11_SHADER_DESC shaderDescription;
	hr = m_Reflector->GetDesc(&shaderDescription);
	if (FAILED(hr))
	{
		ALOG(string("GetDesc Failed!") + string(__FUNCTION__) + to_string(__LINE__));
		return false;
	}

	vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

	UINT numInputParameters = shaderDescription.InputParameters;

	for (UINT i = 0; i < numInputParameters; i++)
	{
		D3D11_INPUT_ELEMENT_DESC inputElement;
		D3D11_SIGNATURE_PARAMETER_DESC parameterSignature;

		m_Reflector->GetInputParameterDesc(i, &parameterSignature);

		inputElement.SemanticName = parameterSignature.SemanticName;
		inputElement.SemanticIndex = parameterSignature.SemanticIndex;
		inputElement.InputSlot = 0;//i; // TODO: If using interleaved arrays, then the input slot should be 0.  If using packed arrays, the input slot will vary.
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // TODO: Figure out how to deal with per-instance data? .. Don't. Just use structured buffers to store per-instance data and use the SV_InstanceID as an index in the structured buffer.
		inputElement.InstanceDataStepRate = 0;
		inputElement.Format = GetDXGIFormat(parameterSignature);

		assert(inputElement.Format != DXGI_FORMAT_UNKNOWN);

		inputElements.push_back(inputElement);

	}

	if (inputElements.size() > 0)
	{
		hr = m_Device->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), m_InputLayout.GetAddressOf());
		if (FAILED(hr))
		{
			ALOG(string("CreateInputLayout Failed!") + string(__FUNCTION__) + to_string(__LINE__));
			return false;
		}
	}

	// Query Resources that are bound to the shader.
	for (UINT i = 0; i < shaderDescription.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		m_Reflector->GetResourceBindingDesc(i, &bindDesc);
		std::string resourceName = bindDesc.Name;

		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			m_ParameterType.push_back(ShaderParameterType::Texture);
			break;
		case D3D_SIT_SAMPLER:
			m_ParameterType.push_back(ShaderParameterType::Sampler);
			break;
		case D3D_SIT_CBUFFER:
		case D3D_SIT_STRUCTURED:
			m_ParameterType.push_back(ShaderParameterType::Buffer);
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			m_ParameterType.push_back(ShaderParameterType::RWBuffer);
			break;
		case D3D_SIT_UAV_RWTYPED:
			m_ParameterType.push_back(ShaderParameterType::RWTexture);
			break;
		}
	}

	return true;
}

bool AShaderCache::CreateReflector()
{
	HRESULT hr = S_OK;

	hr = D3DReflect(m_ByteCode->GetBufferPointer(), m_ByteCode->GetBufferSize(), IID_ID3D11ShaderReflection, &m_Reflector);
	AHRASSERT(hr);

	return true;
}

// Determine DXGI format
// Inspired by: http://takinginitiative.net/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
DXGI_FORMAT AShaderCache::GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
{
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	if (paramDesc.Mask == 1) // 1 component
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 3) // 2 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 7) // 3 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32B32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32B32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 15) // 4 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		break;
		}
	}

	return format;
}

