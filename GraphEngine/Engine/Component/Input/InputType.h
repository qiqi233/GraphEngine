#pragma once
#include "EngineMinimal.h"

enum EPressState
{
	Press,//°´ÏÂ
	Release,//ËÉ¿ª
};

struct FInputKey
{
	FInputKey()=default;

	string KeyName;
	EPressState PressState;
};