#pragma once

struct FEngineRenderConfig
{
	int ScreenWidth;
	int ScreenHight;
	int RefreshRate;
	int SwapChainCount;
	static FEngineRenderConfig& Get();
private:
	FEngineRenderConfig();
};