#pragma once
#include "Core/Object.h"
#include "Component.h"

class AActor;
class UActorComponent :public UComponent
{

public:
	using Super = UActorComponent;
	UActorComponent();
	virtual ~UActorComponent();
public:
	virtual void InitComponent() override;
	virtual void TickComponent(float DeltaTime)override;
	virtual void Destroy()override;

	void ResigterComponent(AActor* InA);
	void SetTick(bool NewEnableTick);
	FORCEINLINE bool GetIsTick()const { return bIsEnableTick; }
private:
	bool bIsEnableTick;
};

template<class T>
T* ConstructComponent(AActor* InA)
{
	T* Comp = NewObject<T>();
	Comp->ResigterComponent(InA);
	return Comp;
}
