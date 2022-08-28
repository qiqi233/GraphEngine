#include "EngineMinimal.h"
#include "EngineFactory.h"

inline int EngineInit(IEngine* Engine, HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
{
	int RetVal = -1;
#if defined(_WIN32)||defined(_WIN64)
	RetVal = Engine->PreInit(ICommandParameters(hInstance, preInstance, cmdLine, showCmd));
	if (RetVal != 0)
	{
		GE_LOG(Error, "Engine PreInit Fail ID=[%d]", RetVal);
		return RetVal;
	}
#endif	
	RetVal = Engine->Init(ICommandParameters(hInstance, preInstance, cmdLine, showCmd));
	if (RetVal != 0)
	{
		GE_LOG(Error, "Engine Init Fail ID=[%d]", RetVal);
		return RetVal;
	}

	RetVal = Engine->PostInit();
	if (RetVal != 0)
	{
		GE_LOG(Error, "Engine PostInit Fail ID=[%d]", RetVal);
		return RetVal;
	}
}

inline int EngineExit(IEngine* Engine)
{
	int RetVal = -1;
	RetVal = Engine->PreExit();
	if (RetVal != 0)
	{
		GE_LOG(Error, "Engine PreExit Fail ID=[%d]", RetVal);
		return RetVal;
	}
	RetVal = Engine->Exit();
	if (RetVal != 0)
	{
		GE_LOG(Error, "Engine Exit Fail ID=[%d]", RetVal);
		return RetVal;
	}
	RetVal = Engine->PostExit();
	if (RetVal != 0)
	{
		GE_LOG(Error, "Engine PostExit Fail ID=[%d]", RetVal);
		return RetVal;
	}
}

IEngine* Engine;
//hInstance：本身自己的实例
//preInstance：上次的实例
//cmdLine：传递命令
//showCmd:有多少条cmd
#if defined(_WIN32)||defined(_WIN64)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
{
	//save_data_to_disk();
	int RetVal = -1;
	Engine = FEngineFactory::CreateEngine();
	if (Engine)
	{
		//初始化
		RetVal = EngineInit(Engine, hInstance, preInstance, cmdLine, showCmd);
		Engine->Tick(0);
		EngineExit(Engine);
	}
	else
	{
		RetVal = 1;
	}
	return RetVal;
}
#endif