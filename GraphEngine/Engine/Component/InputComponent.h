#pragma once
#include "ActorComponent.h"
#include "Input/InputType.h"




class UInputComponent :public UActorComponent
{
	DECLARE_DELEGATE_MOREPARAM(FInputEventDelegate,void,FInputKey);
	DECLARE_DELEGATE_MOREPARAM(FAxisBindingDelegate, void, float);
	DECLARE_DELEGATE_MOREPARAM(FMouseEventDelegate, void, EMouseState, int,int);
	DECLARE_DELEGATE_MOREPARAM(FMouseWheelEventDelegate, void, int,int,float);
	DECLARE_DELEGATE_MOREPARAM(FMouseMoveEventDelegate, void, int, int);
public:
	struct FInputAxisBinding
	{
		/** The axis mapping being bound to. */
		FInputKey InputKey;
		float AxisValue;
		FAxisBindingDelegate InputAxisBinding;
			FInputAxisBinding()
			: InputKey(FInputKey())
			, AxisValue(0.f)
		{ }

		FInputAxisBinding(FInputKey InKey,float Axis=1)
			: InputKey(InKey)
			, AxisValue(Axis)
		{ }
		void Exetuce()
		{
			if(GetAsyncKeyState(InputKey.Code)&0x8000)
			{
				if (InputAxisBinding.IsBound())
				{
					InputAxisBinding.Execute(AxisValue);
				}
			}
		}
	};
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


	template<class TObjectType,class TReturn, typename ...ParamTypes>
	void BindAxis(FInputKey InputKey,float Value, TObjectType* InObject, TReturn(TObjectType::* InFuncation)(ParamTypes ...))
	{
		FInputAxisBinding AxisBinding(InputKey, Value);
		AxisBinding.InputAxisBinding.Bind(InObject, InFuncation);

		InputAxisBindingMap[InputKey.Code]= AxisBinding;
		
	}
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
	std::unordered_map<int,FInputAxisBinding>  InputAxisBindingMap;
};
