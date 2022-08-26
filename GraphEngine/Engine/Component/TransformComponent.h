#pragma once
#include "ActorComponent.h"

class UTransformComponent :public UActorComponent
{
public:
	UTransformComponent();
	virtual ~UTransformComponent();
public:
	virtual void InitComponent()override;
	virtual void TickComponent(float DeltaTime)override;
	virtual void Destroy()override;
private:
	
};
