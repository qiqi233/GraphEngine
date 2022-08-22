#pragma once
#include "Component.h"

class UTransformComponent :public UComponent
{
public:
	UTransformComponent();
	virtual ~UTransformComponent();
public:
	virtual void InitComponent()override;
	virtual void TickComponent(float DeltaTime)override;
	virtual void Destroy()override;
};
