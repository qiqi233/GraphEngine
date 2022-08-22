#include "InputComponent.h"

UInputComponent::UInputComponent()
{

}

UInputComponent::~UInputComponent()
{

}

void UInputComponent::InitComponent()
{
	
}

void UInputComponent::TickComponent(float DeltaTime)
{
	FInputKey InputKey;
	if(InputEventDelegate.IsBound())
	{
		InputEventDelegate.Execute(InputKey);
	}
}

void UInputComponent::Destroy()
{
	
}

