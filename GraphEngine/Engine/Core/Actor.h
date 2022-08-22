#pragma once
#include "EngineMinimal.h"
#include "Object.h"


class AActor:public UObject
{
public:
	AActor();
	virtual ~AActor();
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void EndPlay();
	virtual void DestroyPlay();

};

