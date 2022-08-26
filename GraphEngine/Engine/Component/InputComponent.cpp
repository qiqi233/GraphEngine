#include "InputComponent.h"
#include "Input/Input.h"
#include "Platform/Windows/WindowsEngine.h"
UInputComponent::UInputComponent()
{

}

UInputComponent::~UInputComponent()
{

}

void UInputComponent::InitComponent()
{
	Super::InitComponent();
	MouseWheelHandle=MousesWheelsDelegate.AddFunction(this, &UInputComponent::OnMouseWheel);

	MouseDownHandle = MouseDownDelegate.AddFunction(this, &UInputComponent::OnMouseButtonDown);
	MouseUpHandle = MouseUpDelegate.AddFunction(this, &UInputComponent::OnMouseButtonUp);

	MouseMoveHandle = MouseMoveDelegate.AddFunction(this, &UInputComponent::OnMouseMove);

	KeyDownHandle = KeyDownDelegate.AddFunction(this, &UInputComponent::OnInputKeyDown);
	KeyUpHandle = KeyUpDelegate.AddFunction(this, &UInputComponent::OnInputKeyUp);
}

void UInputComponent::TickComponent(float DeltaTime)
{
	Super::TickComponent(DeltaTime);
}

void UInputComponent::Destroy()
{
	Super::Destroy();
	MousesWheelsDelegate.RemoveDelegate(MouseWheelHandle);

	MouseDownDelegate.RemoveDelegate(MouseDownHandle);
	MouseUpDelegate.RemoveDelegate(MouseUpHandle);

	MouseMoveDelegate.RemoveDelegate(MouseMoveHandle);

	KeyDownDelegate.RemoveDelegate(KeyDownHandle);
	KeyUpDelegate.RemoveDelegate(KeyUpHandle);
}

void UInputComponent::OnInputKeyDown(FInputKey InputKey)
{
	if (GetInputDownEvent().IsBound())
	{

		if(InputKey.PressState==EPressState::Press)
		{
			if (CapInputKey != InputKey)
			{
				CapInputKey=InputKey;
				GetInputDownEvent().Execute(InputKey);
			}
		
		}
		
	}
}	

void UInputComponent::OnInputKeyUp(FInputKey InputKey)
{
	if (GetInputUpEvent().IsBound())
	{
		if (InputKey.PressState == EPressState::Release)
		{
			//ReleaseCapture();
			CapInputKey=FInputKey();
			GetInputUpEvent().Execute(InputKey);
		}
	}
}

void UInputComponent::OnMouseButtonDown(EMouseState InMouseState, int X, int Y)
{
	if (GetMouseDownEvent().IsBound())
	{
		GetMouseDownEvent().Execute(InMouseState,X,Y);
	}
}

void UInputComponent::OnMouseButtonUp(EMouseState InMouseState, int X, int Y)
{
	if (GetMouseUpEvent().IsBound())
	{
		GetMouseUpEvent().Execute(InMouseState, X, Y);
	}
}

void UInputComponent::OnMouseMove(int X, int Y)
{
	if (GetMouseMoveEvent().IsBound())
	{
		GetMouseMoveEvent().Execute(X, Y);
	}
}

void UInputComponent::OnMouseWheel(int X, int Y, float InDelta)
{
	if (GetMouseWheelEvent().IsBound())
	{
		GetMouseWheelEvent().Execute(X, Y, InDelta);
	}
}

