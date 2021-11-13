#pragma once


#include "ACommonIncludes.h"


class ARenderDevice;
class AShaderCache;
class AConstantBuffer;
class AMaterial;
class ARenderTarget;
class ATexture;

enum RasterizerState
{
	RSStateSolid,
	RSStateWireFrame,
	RSStateCullBackFace,
	RSStateCullFrontFace,
	RSStateNoCullFace
};

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
};

class ARenderSystem
{
private:
	bool									m_bIsComputeShader;
	ComPtr<ID3D11Device>					m_Device;
	ComPtr<ID3D11DeviceContext>				m_DeviceContext;

	vector< shared_ptr<ARenderTarget> >		m_RenderTargets;
	vector< shared_ptr<AConstantBuffer> >	m_ConstantBuffers;

	shared_ptr<AMaterial>					m_Material;
	shared_ptr<AShaderCache>				m_VShader;
	shared_ptr<AShaderCache>				m_HShader;
	shared_ptr<AShaderCache>				m_DShader;
	shared_ptr<AShaderCache>				m_GShader;
	shared_ptr<AShaderCache>				m_PShader;
	shared_ptr<AShaderCache>				m_CShader; 

	ID3D11RasterizerState*					m_CurrentRasterizerState;

	D3D11_PRIMITIVE_TOPOLOGY				m_Topology;

	UINT									m_IndexCountPerInstance;
	UINT									m_InstanceCount = 1;
	UINT									m_StartIndexLocation = 0;
	INT										m_BaseVertexLocation = 0;
	UINT									m_StartInstanceLocation = 0;

	ComPtr < ID3D11Buffer>					m_vertexBuffer;
	ComPtr < ID3D11Buffer>					m_indexBuffer;
	ComPtr < ID3D11Buffer>					m_instanceBuffer;
	vector<ID3D11SamplerState*>				m_Samplers;

public:
	ARenderSystem(ARenderDevice* renderDevice);
	~ARenderSystem();

	void SetMaterial(shared_ptr < AMaterial> mat ); 
	void SetRenderTargets(vector <shared_ptr<ARenderTarget> >& renderTargets);
	void SetShaderResources(vector <shared_ptr<ATexture> >& shaderResources); 
	void SetConstantBuffers(vector<shared_ptr<AConstantBuffer>>& constantBuffers);
	void SetShaders( AShaderCache* cs = nullptr, AShaderCache* ps = nullptr, AShaderCache* vs = nullptr, AShaderCache* hs = nullptr, AShaderCache* ds = nullptr, AShaderCache* gs = nullptr);
	void SetTopology( D3D11_PRIMITIVE_TOPOLOGY topology );
	void SetRasterizerState( ID3D11RasterizerState* pRasterizerState );
	void SetSamplers(vector<ID3D11SamplerState*>& samplers);
	void Render();

};

