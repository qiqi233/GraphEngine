#include "ActorComponent.h"
#include "Core/Actor.h"

UActorComponent::UActorComponent()
	:bIsEnableTick(false)
{

}

UActorComponent::~UActorComponent()
{

}

void UActorComponent::InitComponent()
{
	bIsEnableTick = true;
}

void UActorComponent::TickComponent(float DeltaTime)
{

}

void UActorComponent::Destroy()
{

}

void UActorComponent::ResigterComponent(AActor* InA)
{
	assert(InA);
	
	InA->ResigterComponent(this);

}

void UActorComponent::SetTick(bool NewEnableTick)
{
	bIsEnableTick=NewEnableTick;
}

