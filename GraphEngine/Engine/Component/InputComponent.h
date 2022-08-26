#pragma once
#include "ActorComponent.h"
#include "Input/InputType.h"



class UInputComponent :public UActorComponent
{
	DECLARE_DELEGATE_MOREPARAM(FInputEventDelegate,void,FInputKey);
	DECLARE_DELEGATE_MOREPARAM(FMouseEventDelegate, void, EMouseState, int,int);
	DECLARE_DELEGATE_MOREPARAM(FMouseWheelEventDelegate, void, int,int,float);
	DECLARE_DELEGATE_MOREPARAM(FMouseMoveEventDelegate, void, int, int);
public:
	UInputComponent();
	virtual ~UInputComponent();
public:
	virtual void InitComponent()override;
	virtual void TickComponent(float DeltaTime)override;
	virtual void Destroy()override;
	FORCEINLINE FInputEventDelegate& GetInputDownEvent() {return InputDownEventDelegate;}
	FORCEINLINE FInputEventDelegate& GetInputUpEvent() { return InputUpEventDelegate; }
	FORCEINLINE FMouseEventDelegate& GetMouseDownEvent() { return MouseDownEvent; }
	FORCEINLINE FMouseEventDelegate& GetMouseUpEvent() { return MouseUpEvent; }
	FORCEINLINE FMouseWheelEventDelegate& GetMouseWheelEvent() { return MouseWheelEvent; }
	FORCEINLINE FMouseMoveEventDelegate& GetMouseMoveEvent() { return MouseMoveEvent; }
protected:
	virtual void OnInputKeyDown(FInputKey InputKey);
	virtual void OnInputKeyUp(FInputKey InputKey);
	virtual void OnMouseButtonDown(EMouseState InMouseState,int X, int Y);
	virtual void OnMouseButtonUp(EMouseState InMouseState, int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);
private:
	FInputEventDelegate				InputDownEventDelegate;
	FInputEventDelegate				InputUpEventDelegate;
	FMouseEventDelegate				MouseDownEvent;
	FMouseEventDelegate				MouseUpEvent;
	FMouseMoveEventDelegate			MouseMoveEvent;
	FMouseWheelEventDelegate		MouseWheelEvent;

	FDelegateHandle MouseWheelHandle;
	FDelegateHandle MouseDownHandle;
	FDelegateHandle MouseUpHandle;
	FDelegateHandle MouseMoveHandle;
	FDelegateHandle KeyDownHandle;
	FDelegateHandle KeyUpHandle;

	FInputKey CapInputKey;
};
