#pragma once
#include "EngineMinimal.h"
#include "Core/Engine.h"
#include <vector>
#include "simple_core_minimal/simple_c_guid/simple_guid.h"

#if defined(_WIN32) ||defined(_WIN64) 
#include "Platform/Windows/WindowsEngine.h"
#endif
using FGuid= simple_c_guid;
//äÖÈ¾½Ó¿Ú

#if defined(_WIN32) ||defined(_WIN64) 
using FPlatformEngine = FWindowsEngine;
#else
using FPlatformEngine = FEngine;
#endif

class IRenderingIntface
{
	friend class FWindowsEngine;
public:
	IRenderingIntface();
	virtual ~IRenderingIntface();
protected:
	static void Rendering(float InDeltaTime);
	virtual void Init(){};
	virtual void PreDraw(float InDeltaTime) = 0;
	virtual void Update(float InDeltaTime) = 0;
	virtual void Draw(float InDeltaTime)=0;
protected:
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer,const void* Data,UINT64 Size);
	bool operator==(const IRenderingIntface& InOther);
	ComPtr<ID3D12Device> GetD3dDevice();
	FPlatformEngine* GetEngine();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

	ComPtr<ID3D12GraphicsCommandList>	GetGraphicsCommandList();
	FGuid GetGuid();
public:
	static std::vector<IRenderingIntface*> RenderingIntface;
	FGuid RenderGuid;
};

