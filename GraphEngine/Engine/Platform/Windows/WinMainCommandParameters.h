#pragma once
#include "EngineMinimal.h"
#if defined(_WIN32)||defined(_WIN64)

class FWinMainCommandParameters
{
 friend class FWindowsEngine;
public:
	FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InpreInstance, PSTR IncmdLine, int InshowCmd);
private:
	HINSTANCE HInstance;
	HINSTANCE PreInstance;
	PSTR CmdLine;
	int ShowCmd;
};
#elif

#endif