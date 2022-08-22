#pragma once
#include "EngineMinimal.h"
#include "Core/Engine.h"
#if defined(_WIN32)||defined(_WIN64)
#include "DirectX/DX12RenderingEngine.h"

class FWindowsEngine :public IEngine
{
public:
	friend class IRenderingIntface;
	FWindowsEngine();
	~FWindowsEngine();
	virtual int PreInit(ICommandParameters Parameters) override;
	virtual int Init(ICommandParameters Parameters)override;
	virtual int PostInit()override;
	virtual void Tick(float DeltaTime)override;

	virtual int PreExit()override;
	virtual int Exit()override;
	virtual int PostExit()override;
private:
	bool InitWindows(ICommandParameters InParameters);
private:
	HWND			MainHwnd;
	FDX12RenderEngine* DX12RenderEngine;
};

#endif
