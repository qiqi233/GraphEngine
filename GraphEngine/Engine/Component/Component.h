#pragma once
#include "Core/Object.h"


class UComponent :public UObject
{
public:
	UComponent();
	virtual ~UComponent();
public:
	virtual void InitComponent();
	virtual void TickComponent(float DeltaTime);
	virtual void Destroy();
};


