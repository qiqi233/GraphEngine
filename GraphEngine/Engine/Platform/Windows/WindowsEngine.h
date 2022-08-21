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
};

#endif
