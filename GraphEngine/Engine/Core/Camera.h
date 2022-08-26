#pragma once
#include "CoreMinimal.h"
#include "Actor.h"

class UTransformComponent;
class UInputComponent;
class ACamera :public AActor
{
public:
	ACamera();
	virtual ~ACamera();

	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
	virtual void EndPlay()override;
	virtual void DestroyPlay()override;
private:
	void CaptureInputDownEvent(FInputKey InputKey);
	void CaptureUpdateInputUpEvent(FInputKey InputKey);
};