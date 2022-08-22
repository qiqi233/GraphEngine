#pragma once
#include "CoreMinimal.h"


class UObject
{
public:
	UObject();
	virtual ~UObject();
public:
	virtual void BeginPlay();
	virtual void DestroyPlay();
};

