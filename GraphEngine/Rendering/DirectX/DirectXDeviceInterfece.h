#pragma once
#include "EngineMinimal.h"

#if defined(_WIN32)|| defined(_WIN64)
class FWindowsEngine;
class FDX12RenderEngine;
#else
class FEngine;
#endif

#if defined(DirectX12Engine)
using FRenderEngine = FDX12RenderEngine;
#else
using FRenderEngine = FDX12RenderEngine;
#endif
//提供渲染内容的接口
class IDirectXDeviceInterfece
{
public:
	FRenderEngine* GetRenderEngine();
	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

	ComPtr<ID3D12GraphicsCommandList>	GetGraphicsCommandList();
};

//提供渲染内容的接口
struct IDirectXDeviceInterfece_Struct
{
public:
	FRenderEngine* GetRenderEngine();
	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

	ComPtr<ID3D12GraphicsCommandList>	GetGraphicsCommandList();
private:
	IDirectXDeviceInterfece Interfece;
};