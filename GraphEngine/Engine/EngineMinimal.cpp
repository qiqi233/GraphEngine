#include "EngineMinimal.h"
#include "Platform/Windows/WindowsEngine.h"

#if defined(_WIN64)||defined(_WIN32)
FPlatformEngine* GetEngine()
{
	return dynamic_cast<FWindowsEngine*>(Engine);
}
#endif