#pragma once
#include "EngineMinimal.h"
#include "Core/Engine.h"
#if defined(_WIN32)||defined(_WIN64)

#include <exception> 

class FWindowsEngine :public IEngine
{
public:
	friend class IRenderingIntface;
	FWindowsEngine();
	virtual int PreInit(ICommandParameters Parameters) override;
	virtual int Init(ICommandParameters Parameters)override;
	virtual int PostInit()override;
	virtual void Tick(float DeltaTime)override;

	virtual int PreExit()override;
	virtual int Exit()override;
	virtual int PostExit()override;
public:
		UINT GetDXGISampleCount()const;
		UINT GetDXGISampleQuality()const;
		DXGI_FORMAT	GetBackBufferFormat()const;
		DXGI_FORMAT	GetDepthStencilFormat()const;
private:
	bool InitWindows(ICommandParameters InParameters);
	bool InitDirect3D();
	void PostInitDirect3D();
	//Rendering
	void Rendering(float InDeltaTime);
	ID3D12Resource* GetCurrentSwapBuffResource() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView()  const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;
	void WaitGPUCommandQueueComplete();
protected:
	//用于创建DirectX图形基础结构(DXGI)对象
	ComPtr<IDXGIFactory4>	DXGIFactory;
	//创建命令分配器、命令列表、命令队列、Fence、资源、管道状态对象、堆、栈、根签名、采样器和许多资源视图
	ComPtr<ID3D12Device>	D3dDevice;
	//一个用于同步CPU和一个或多个GPU的对象
	ComPtr<ID3D12Fence>		D3dFence;

	ComPtr<ID3D12CommandQueue>			CommandQueue;//命令队列 每个GPU都会有一个命令队列
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;//负责命令的储存分配向↓↓↓↓↓↓↓
	//ExecuteCommandLists() 把GraphicsCommandList--提交-->给CommandQueue
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;//命令列表 每个CPU都会有一个命令列表，把命令提交给命令队列（CommandQueue）
	
	ComPtr<IDXGISwapChain> DXGISwapChain;//缓冲区交换连
	

	ComPtr<ID3D12DescriptorHeap>		RTVHeap;	//RenderTargerViewHeap
	std::vector<ComPtr<ID3D12Resource>>	SwapChainResource;
	
	ComPtr<ID3D12DescriptorHeap>		DSVHeap;	//DepthStencilViewHeap 深度/模板缓冲区
	ComPtr<ID3D12Resource>				DepthStencilResource;
protected:
	D3D12_VIEWPORT ViewportInfo;
	D3D12_RECT	ViewportRect;
private:
	int CurrentSwapBuffIndex;
	int CurrentFenceIndex;
	HWND			MainHwnd;
	int				M4QualityLevels;
	bool			bMSAAEnable;//MSAA：多重采样 SSAA：超级采样
	DXGI_FORMAT		BackBufferFormat;
	DXGI_FORMAT		DepthStencilFormat;
	UINT RTVDescriptorSize;
};

#endif
