#include "TransformComponent.h"

UTransformComponent::UTransformComponent()
{

}

UTransformComponent::~UTransformComponent()
{

}

void UTransformComponent::InitComponent()
{
	Super::InitComponent();

}

void UTransformComponent::TickComponent(float DeltaTime)
{
	Super::TickComponent(DeltaTime);
	if(GetAsyncKeyState('W')&0x8000)
	{
		
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{

	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{

	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{

	}

}

void UTransformComponent::Destroy()
{
	Super::Destroy();
}

