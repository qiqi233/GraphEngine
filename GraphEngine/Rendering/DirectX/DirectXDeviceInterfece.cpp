#include "DirectXDeviceInterfece.h"
#include "Platform/Windows/WindowsEngine.h"

FRenderEngine* IDirectXDeviceInterfece::GetRenderEngine()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return WindowsEngine->GetDX12RenderEngine();
	}
}

ComPtr<ID3D12Device> IDirectXDeviceInterfece::GetD3dDevice()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return GetRenderEngine()->GetD3dDevice();
	}
	return ComPtr<ID3D12Device>();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece::GetCommandAllocator()
{
	if (FWindowsEngine* InEngine = GetEngine())
	{
		return GetRenderEngine()->GetCommandAllocator();
	}

	return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterfece::GetGraphicsCommandList()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return GetRenderEngine()->GetGraphicsCommandList();
	}
	return ComPtr<ID3D12GraphicsCommandList>();
}




FRenderEngine* IDirectXDeviceInterfece_Struct::GetRenderEngine()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return WindowsEngine->GetDX12RenderEngine();
	}
}

ComPtr<ID3D12Device> IDirectXDeviceInterfece_Struct::GetD3dDevice()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return GetRenderEngine()->GetD3dDevice();
	}
	return ComPtr<ID3D12Device>();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece_Struct::GetCommandAllocator()
{
	if (FWindowsEngine* InEngine = GetEngine())
	{
		return GetRenderEngine()->GetCommandAllocator();
	}

	return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterfece_Struct::GetGraphicsCommandList()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return GetRenderEngine()->GetGraphicsCommandList();
	}
	return ComPtr<ID3D12GraphicsCommandList>();
}
