#include "EngineRenderConfig.h"


FEngineRenderConfig::FEngineRenderConfig()
	:ScreenWidth(1280)
	, ScreenHight(720)
	, RefreshRate(60)
	, SwapChainCount(2)
{

}

FEngineRenderConfig& FEngineRenderConfig::Get()
{
	static FEngineRenderConfig Config;
	return Config;
}


