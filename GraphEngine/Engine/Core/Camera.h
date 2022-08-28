#pragma once
#include "CoreMinimal.h"
#include "Actor.h"
#include "Viewport.h"



enum class ECameraType:uint8_t
{
	Edit=0,//�༭����
	Observer,//�۲�������

};

class UTransformComponent;
class UInputComponent;
class ACamera 
		: public AActor
		, public IViewport
{
public:
	ACamera();
	virtual ~ACamera();

	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
	virtual void EndPlay()override;
	virtual void DestroyPlay()override;

	void RotatorAxis(XMFLOAT3 InAxis,float InV);
	void MoveFromAxis(XMFLOAT3 InAxis,float InValue);
protected:
	//Axis
	virtual void MoveForward(float InValue);
	virtual void MoveRight(float InValue);

	//Action
	virtual void OnInputKeyDown(FInputKey InputKey);
	virtual void OnInputKeyUp(FInputKey InputKey);
	virtual void OnMouseButtonDown(EMouseState InMouseState, int X, int Y);
	virtual void OnMouseButtonUp(EMouseState InMouseState, int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);
	virtual void BuildViewMatrix(float DeltaTime);
private:
	void CaptureInputDownEvent(FInputKey InputKey);
	void CaptureUpdateInputUpEvent(FInputKey InputKey);
private:
	UPROPROTY()
	UInputComponent* InputComponent;
	UPROPROTY()
	ECameraType CameraType;
	//��꼤��
	bool bLeftMouseDownAction;
	bool bRightMouseDownAction;
	bool bMiddenMouseDownAction;
	//���������
	float MoveSensitivity = 1;//�ƶ�
	float RotSensitivity = 1;//��ת
	float ScaleSensitivity = 1;//����
	POINT LastMousePosition;

	//��������
	float Radius;
	float A;//Theta
	float B;//

};