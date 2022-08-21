#include "EngineFactory.h"
#include "Core/Engine.h"
#if defined(_WIN32)||defined(_WIN64)
#include "Platform/Windows/WindowsEngine.h"
#endif

IEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)||defined(_WIN64)
	return new FWindowsEngine();
#elif defined(__linux__)
	//创建linux平台引擎
#endif

}

