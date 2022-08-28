#pragma once
#include "EngineMinimal.h"
#include "RenderingEngine.h"
#include "Core/Engine.h"


class FRenderingPipeline;
class FDX12RenderEngine:public IRenderEngine
{
	friend class FWindowsEngine;
	friend class FRenderingPipeline;
public:
	FDX12RenderEngine();
	virtual ~FDX12RenderEngine();
public:
	UINT GetDXGISampleCount()const;
	UINT GetDXGISampleQuality()const;
	DXGI_FORMAT	GetBackBufferFormat()const;
	DXGI_FORMAT	GetDepthStencilFormat()const;
private:
	void SetMianWindowsHandle(HWND InNewMianWindowsHandle);
	bool InitDirect3D();
	void PostInitDirect3D();
	virtual void Update(const GameTimer& gt)override;
	//Rendering
	virtual void Rendering(float InDeltaTime)override;
public:
	ID3D12Resource* GetCurrentSwapBuffResource() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView()  const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;
	void WaitGPUCommandQueueComplete();
	ComPtr<IDXGIFactory4>	GetDXGIFactory()const{return DXGIFactory;};
	ComPtr<ID3D12Device>	GetD3dDevice()const {return D3dDevice;};
	ComPtr<ID3D12Fence>		GetD3dFence()const {return D3dFence;};

	ComPtr<ID3D12CommandQueue>			GetCommandQueue()const {return CommandQueue;};
	ComPtr<ID3D12CommandAllocator>		GetCommandAllocator()const {return CommandAllocator;}
	ComPtr<ID3D12GraphicsCommandList>	GetGraphicsCommandList()const {return GraphicsCommandList;}

	ComPtr<IDXGISwapChain> GetDXGISwapChain;
protected:
	//���ڴ���DirectXͼ�λ����ṹ(DXGI)����
	ComPtr<IDXGIFactory4>	DXGIFactory;
	//��������������������б�������С�Fence����Դ���ܵ�״̬���󡢶ѡ�ջ����ǩ�����������������Դ��ͼ
	ComPtr<ID3D12Device>	D3dDevice;
	//һ������ͬ��CPU��һ������GPU�Ķ���
	ComPtr<ID3D12Fence>		D3dFence;

	ComPtr<ID3D12CommandQueue>			CommandQueue;//������� ÿ��GPU������һ���������
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;//��������Ĵ���������������������
	//ExecuteCommandLists() ��GraphicsCommandList--�ύ-->��CommandQueue
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;//�����б� ÿ��CPU������һ�������б��������ύ��������У�CommandQueue��

	ComPtr<IDXGISwapChain> DXGISwapChain;//������������


	ComPtr<ID3D12DescriptorHeap>		RTVHeap;	//RenderTargerViewHeap
	std::vector<ComPtr<ID3D12Resource>>	SwapChainResource;

	ComPtr<ID3D12DescriptorHeap>		DSVHeap;	//DepthStencilViewHeap ���/ģ�建����
	ComPtr<ID3D12Resource>				DepthStencilResource;
protected:
	D3D12_VIEWPORT ViewportInfo;
	D3D12_RECT	ViewportRect;
private:
	int CurrentSwapBuffIndex;
	int CurrentFenceIndex;
	HWND			MainHwnd;
	int				M4QualityLevels;
	bool			bMSAAEnable;//MSAA�����ز��� SSAA����������
	DXGI_FORMAT		BackBufferFormat;
	DXGI_FORMAT		DepthStencilFormat;
	UINT RTVDescriptorSize;
	UINT DSVDescriptorSize;
	UINT CBVDescriptorSize;
private:
	FRenderingPipeline* RenderingPipeline;
	UINT64 mCurrentFence = 0;
};