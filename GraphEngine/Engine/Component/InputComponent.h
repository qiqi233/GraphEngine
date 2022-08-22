#pragma once
#include "Component.h"



class UInputComponent :public UComponent
{
	DECLARE_DELEGATE_MOREPARAM(FInputEventDelegate,void,const FInputKey&);
public:
	UInputComponent();
	virtual ~UInputComponent();
public:
	virtual void InitComponent()override;
	virtual void TickComponent(float DeltaTime)override;
	virtual void Destroy()override;
	FORCEINLINE FInputEventDelegate& GetInputEventDelegate() {return InputEventDelegate;};
private:
	FInputEventDelegate InputEventDelegate;
};
