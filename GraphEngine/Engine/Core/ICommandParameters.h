#pragma once
#if defined(_WIN32)||defined(_WIN64)
#include "Platform/Windows/WinMainCommandParameters.h"

using ICommandParameters=FWinMainCommandParameters;
#elif

 using ICommandParameters = FWinMainCommandParameters;
#endif