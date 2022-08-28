#pragma once
#include "EngineMinimal.h"
#include "ActorComponent.h"

class UTransformComponent :public UActorComponent
{
public:
	UTransformComponent();
	virtual ~UTransformComponent();
public:
	virtual void InitComponent()override;
	virtual void TickComponent(float DeltaTime)override;
	virtual void Destroy()override;
public:
	void SetPosition(const XMFLOAT3& InNewPosition);
	void SetRotation(const UEMath::FRotator& InNewRotation);
	void SetScale(const fvector_3d& InNewScale);

	void SetForwardVector(const XMFLOAT3& InForwardVector);
	void SetRightVector(const XMFLOAT3& InRightVector);
	void SetUPVector(const XMFLOAT3& InUPVector);

	void MarkRotationMofity();
public:
	XMFLOAT3& GetPosition() { return Position; }
	UEMath::FRotator GetRotation() const { return Rotation; }
	fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }

	XMFLOAT3& GetForwardVector() { return ForwardVector; }
	XMFLOAT3& GetRightVector() { return RightVector; }
	XMFLOAT3& GetUPVector() { return UPVector; }
public:
	//½ÃÕýUp right look(fd) ·µ»Ø pos
	void CorrectionVector();

	fvector_3d GetCorrectionPosition();
	void GetCorrectionPosition(fvector_3d& InPos3);

private:
	XMFLOAT3 Position;
	UEMath::FRotator Rotation;
	XMFLOAT3 Scale;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UPVector;
};
