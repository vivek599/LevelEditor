#include "ARenderSystem.h"
#include "ARenderDevice.h"
#include "ATexture.h"
#include "ARenderTarget.h"
#include "AConstantBuffer.h"
#include "AShaderCache.h"



ARenderSystem::ARenderSystem(ARenderDevice* renderDevice) : 
				m_Device(renderDevice->GetDevice()),
				m_DeviceContext(renderDevice->GetContext())
{ 

}

ARenderSystem::~ARenderSystem()
{

}

void ARenderSystem::SetRenderTargets(vector <shared_ptr<ARenderTarget> >& renderTargets)
{
	vector < ID3D11RenderTargetView* > rtvs;

	for (int i = 0; i < renderTargets.size(); i++ )
	{
		rtvs.push_back( renderTargets[i]->GetRTV() );
	}

	m_DeviceContext->OMSetRenderTargets(rtvs.size(), rtvs.data(), renderTargets[0]->GetDSV());
}

void ARenderSystem::SetShaderResources(vector <shared_ptr<ATexture> >& shaderResources)
{
	int srvres = 0;

	for (int i = 0; i < shaderResources.size(); i++ )
	{
		if (shaderResources[i]->IsUAV() == false )
		{
			auto srv = shaderResources[i]->GetSRV();
			if (m_bIsComputeShader)
			{
				m_DeviceContext->CSSetShaderResources(srvres++, 1, &srv);
			}
			else
			{
				m_DeviceContext->PSSetShaderResources(srvres++, 1, &srv);
			}
		}
	}

	const UINT uavInitCount = -1;
	int uavres = 0;
	for (int i = 0; i < shaderResources.size(); i++)
	{
		if (shaderResources[i]->IsUAV())
		{
			auto uav = shaderResources[i]->GetUAV();
			m_DeviceContext->CSSetUnorderedAccessViews(uavres++, 1, &uav, &uavInitCount);
		}
	} 
}

void ARenderSystem::SetConstantBuffers(vector <shared_ptr<AConstantBuffer> >& constantBuffers)
{
	int cbuf[6] = {0};

	for (int i = 0; i < constantBuffers.size(); i++)
	{
		BITMASK_SWITCH(constantBuffers[i]->GetVisibility())
		{
		case VISIBLE_VERTEX:
			m_DeviceContext->VSSetConstantBuffers(cbuf[0]++, 1, constantBuffers[i]->GetBuffer().GetAddressOf());
			break;
		case VISIBLE_HULL:
			m_DeviceContext->HSSetConstantBuffers(cbuf[1]++, 1, constantBuffers[i]->GetBuffer().GetAddressOf());
			break;
		case VISIBLE_DOMAIN:
			m_DeviceContext->DSSetConstantBuffers(cbuf[2]++, 1, constantBuffers[i]->GetBuffer().GetAddressOf());
			break;
		case VISIBLE_GEOMETRY:
			m_DeviceContext->GSSetConstantBuffers(cbuf[3]++, 1, constantBuffers[i]->GetBuffer().GetAddressOf());
			break;
		case VISIBLE_PIXEL:
			m_DeviceContext->PSSetConstantBuffers(cbuf[4]++, 1, constantBuffers[i]->GetBuffer().GetAddressOf());
			break;
		case VISIBLE_COMPUTE:
			m_DeviceContext->CSSetConstantBuffers(cbuf[5]++, 1, constantBuffers[i]->GetBuffer().GetAddressOf());
			break;
		}
	}
} 

void ARenderSystem::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	if (m_bIsComputeShader)
	{
		m_DeviceContext->CSSetShader(m_CShader->GetComputeShader().Get(), nullptr, 0);
		m_DeviceContext->Dispatch(m_CShader->GetThreadX(), m_CShader->GetThreadY(), m_CShader->GetThreadZ() );
	}
	else 
	{
		m_DeviceContext->RSSetState(m_CurrentRasterizerState);
		m_DeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_DeviceContext->IASetIndexBuffer( m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
		m_DeviceContext->IASetPrimitiveTopology(m_Topology);
		m_DeviceContext->IASetInputLayout(m_VShader->GetInputLayout().Get());
		m_DeviceContext->VSSetShader(m_VShader->GetVertexShader().Get(), nullptr, 0);
		m_DeviceContext->HSSetShader(m_HShader->GetHullShader().Get(), nullptr, 0);
		m_DeviceContext->DSSetShader(m_DShader->GetDomainShader().Get(), nullptr, 0);
		m_DeviceContext->GSSetShader(m_GShader->GetGeometryShader().Get(), nullptr, 0);
		m_DeviceContext->PSSetShader(m_PShader->GetPixelShader().Get(), nullptr, 0);

		m_DeviceContext->PSSetSamplers(0, m_Samplers.size(), m_Samplers.data());

		m_DeviceContext->DrawIndexedInstanced(m_IndexCountPerInstance, m_InstanceCount, m_StartIndexLocation,m_BaseVertexLocation, m_StartInstanceLocation );
	} 
} 
 
void ARenderSystem::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_Topology = topology;
}

void ARenderSystem::SetRasterizerState( ID3D11RasterizerState* pRasterizerState )
{
	m_CurrentRasterizerState = pRasterizerState;
}

void ARenderSystem::SetSamplers(vector<ID3D11SamplerState*>& samplers)
{
	m_Samplers = samplers;
}

void ARenderSystem::SetMaterial(shared_ptr < AMaterial> mat)
{
	m_Material = mat;
}

void ARenderSystem::SetShaders(AShaderCache* cs, AShaderCache* ps, AShaderCache* vs, AShaderCache* hs, AShaderCache* ds, AShaderCache* gs )
{ 
	m_VShader.reset( vs ); 
	m_HShader.reset( hs ); 
	m_DShader.reset( ds ); 
	m_GShader.reset( gs ); 
	m_PShader.reset( ps ); 
	m_CShader.reset( cs ); 

} 
 









