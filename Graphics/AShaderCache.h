#pragma once

#include "ACommonIncludes.h"

enum AShaderType
{
	VERTEXSHADER,
	HULLSHADER,
	DOMAINSHADER,
	GEOMETRYSHADER,
	PIXELSHADER,
	COMPUTESHADER
};

enum ShaderParameterType
{
	Invalid,    // Invalid parameter. Doesn't store a type.
	Texture,    // Texture.
	Sampler,    // Texture sampler.
	Buffer,     // Buffers, ConstantBuffers, StructuredBuffers.
	RWTexture,  // Texture that can be written to in a shader (using Store operations).
	RWBuffer,   // Read/write structured buffers.
};

class ARenderDevice;


class AShaderCache
{
private:

	bool CompileVSFromFile(const wchar_t* filepath);
	bool CompileHSFromFile(const wchar_t* filepath);
	bool CompileDSFromFile(const wchar_t* filepath);
	bool CompileGSFromFile(const wchar_t* filepath);
	bool CompilePSFromFile(const wchar_t* filepath);
	bool CompileCSFromFile(const wchar_t* filepath);
	 
	DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc); 
  
	ComPtr<ID3D11VertexShader>				m_VShader;
	ComPtr<ID3D11HullShader>				m_HShader;
	ComPtr<ID3D11DomainShader>				m_DShader; 
	ComPtr<ID3D11GeometryShader>			m_GShader;
	ComPtr<ID3D11PixelShader>				m_PShader;
	ComPtr<ID3D11ComputeShader>				m_CShader;

	ComPtr<ID3D11InputLayout>				m_InputLayout; 

	ComPtr<ID3D10Blob>						m_ByteCode; 

	AShaderType								m_Type;
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;
	ComPtr<ID3D10Blob>						m_ErrorMsg;
	ComPtr<ID3D11ShaderReflection>			m_Reflector;

	UINT									m_ThreadX;
	UINT									m_ThreadY;
	UINT									m_ThreadZ;

	vector<ShaderParameterType>				m_ParameterType;
public:
	bool CompileShaderFromFile(const wchar_t* filepath);
	bool LoadPreCompiled(const wchar_t* filepath);
	bool CreateInputLayout();
	bool CreateReflector();

	AShaderCache(ARenderDevice* renderDevice, AShaderType shaderType);
	~AShaderCache();

	ComPtr<ID3D11InputLayout>		GetInputLayout();

	ComPtr<ID3D11VertexShader>		GetVertexShader();
	ComPtr<ID3D11HullShader>		GetHullShader();
	ComPtr<ID3D11DomainShader>		GetDomainShader();
	ComPtr<ID3D11GeometryShader>	GetGeometryShader();
	ComPtr<ID3D11PixelShader>		GetPixelShader();
	ComPtr<ID3D11ComputeShader>		GetComputeShader();

	UINT GetThreadX();
	UINT GetThreadY();
	UINT GetThreadZ();

	void SetCSThreads(UINT x, UINT y, UINT z); 


};
 
