#pragma once


#include "ACommonIncludes.h"



const int MAX_BUFFER_COUNT = 2;

class ARenderDevice
{
public:
	static ARenderDevice* CreateDevice(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen);
	static void DestroyDevice(ARenderDevice* device);

	void BeginFrame();
	void EndFrame();

	ComPtr<ID3D11Device> GetDevice();
	ComPtr<ID3D11DeviceContext> GetContext();

	UINT							GetNumRTVs();
	ComPtr<ID3D11RenderTargetView>	GetRenderTargetView();
	ComPtr<ID3D11DepthStencilView>	GetDepthStencilView();

	ID3D11RasterizerState*		GetRSWireFrame();
	ID3D11RasterizerState*		GetRSCullBackFace();
	ID3D11RasterizerState*		GetRSCullFrontFace();
	ID3D11RasterizerState*		GetRSNoCullFace();
	ID3D11DepthStencilState*	GetDepthEnableState();
	ID3D11DepthStencilState*	GetDepthDisableState();
	ID3D11SamplerState**		GetSampleRepeat();
	ID3D11SamplerState**		GetSampleMirror();
	ID3D11SamplerState**		GetSampleClamp();
	ID3D11SamplerState**		GetSampleMirrorOnce();

	void InitDepthBuffers();
	void InitCommonPipeLineStates();
	bool ResizeSwapChainBuffers(uint32_t width, uint32_t height);

private:

	struct InitialisationParams
	{
		HWND							window;
		bool							vsyncEnabled;
		bool							fullScreen;
		uint32_t						screenWidth;
		uint32_t						screenHeight;
		ID3D11Device*					d3dDevice;
		IDXGISwapChain*					dxgiSwapChain;
		ID3D11DeviceContext*			d3dDeviceContext;
		ID3D11RenderTargetView*			d3dRenderTargetView;
		DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	};

	ARenderDevice(const InitialisationParams& initParams);

	~ARenderDevice();
	ARenderDevice(const ARenderDevice&);
	void operator=(const ARenderDevice&);

	ComPtr<ID3D11Device>				m_Device;
	ComPtr<ID3D11DeviceContext>			m_DeviceContext;
	ComPtr<IDXGISwapChain>				m_SwapChain;
	ID3D11RenderTargetView*				m_RenderTargetView;
	ComPtr<ID3D11DepthStencilView>		m_DepthStencilView;
	ComPtr<ID3D11Texture2D>				m_DepthStencilBuffer;

	HWND							m_Window;
	bool							m_VsyncEnabled;
	bool							m_FullScreen;
	uint32_t						m_ScreenWidth;
	uint32_t						m_ScreenHeight;
	DXGI_SWAP_CHAIN_DESC			m_SwapChainDesc;

	bool CreateBlendStates();
	bool CreateDepthStates();
	bool CreateRasterizerStates();
	bool CreateSamplerStates();

	static void GetHardwareAdapter(IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter);

	ComPtr<ID3D11BlendState>				m_AlphaDisableBlendState;
	ComPtr<ID3D11BlendState>				m_AlphaEnableBlendState;

	ComPtr<ID3D11RasterizerState>			m_RasterizerStateWireFrame;
	ComPtr<ID3D11RasterizerState>			m_RasterizerStateCullBackFace;
	ComPtr<ID3D11RasterizerState>			m_RasterizerStateCullFrontFace;
	ComPtr<ID3D11RasterizerState>			m_RasterizerStateNoCullFace;

	ComPtr<ID3D11DepthStencilState>			m_DepthEnableStencilState;
	ComPtr<ID3D11DepthStencilState>			m_DepthDisableStencilState;

	ComPtr<ID3D11SamplerState >				m_SampleStateRepeat;
	ComPtr<ID3D11SamplerState >				m_SampleStateMirror;
	ComPtr<ID3D11SamplerState >				m_SampleStateClamp;
	ComPtr<ID3D11SamplerState >				m_SampleStateMirrorOnce;

};

