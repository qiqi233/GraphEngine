#pragma once
#include "EngineMinimal.h"

enum EPressState
{
	Press,//����
	Release,//�ɿ�
};

struct FInputKey
{
	FInputKey()=default;

	string KeyName;
	EPressState PressState;
};