#include "TransformComponent.h"
#include <DirectXMathVector.inl>
#include "Math/MathHelper.h"

UTransformComponent::UTransformComponent()
	: Position(0.f, 0.f, 0.f)
	, Rotation(FRotator::ZeroRotator)
	, Scale(1.f, 1.f, 1.f)
	, ForwardVector(0.f, 0.f, 1.f)
	, RightVector(1.f, 0.f, 0.f)
	, UPVector(0.f, 1.f, 0.f)
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


}

void UTransformComponent::Destroy()
{
	Super::Destroy();
}

void UTransformComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	Position = InNewPosition;
}

void UTransformComponent::SetRotation(const UEMath::FRotator& InNewRotation)
{

	Rotation=InNewRotation;
	//XMConvertToRadians：度数转弧度值
	//XMConvertToDegrees：弧度值转度数
	float RollRadians = XMConvertToRadians(InNewRotation.Roll);
	float PithRadians = XMConvertToRadians(InNewRotation.Pitch);
	float YawRadians = XMConvertToRadians(InNewRotation.Yaw);

	//旋转矩阵
	XMMATRIX RotMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);
	//XMVector3TransformNormal:使用输入矩阵的第 0、1 和 2 行进行转换以进行旋转和缩放，并忽略第 3 行。
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotMatrix));
}

void UTransformComponent::SetScale(const fvector_3d& InNewScale)
{
	Scale.x = InNewScale.x;
	Scale.y = InNewScale.y;
	Scale.z = InNewScale.z;
}

void UTransformComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	ForwardVector = InForwardVector;
}

void UTransformComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	RightVector=InRightVector;
}

void UTransformComponent::SetUPVector(const XMFLOAT3& InUPVector)
{
	UPVector=InUPVector;
}

void UTransformComponent::MarkRotationMofity()
{
	FVector RollAxis(ForwardVector.x, ForwardVector.y, ForwardVector.z);
	FVector PitchAxis(RightVector.x, RightVector.y, RightVector.z);;
	FVector YawAxis(UPVector.x, UPVector.y, UPVector.z);;
	FMatrix RotMatrix(RollAxis, PitchAxis, YawAxis, FVector::ZeroVector);
	Rotation=RotMatrix.Rotator();
}

void UTransformComponent::CorrectionVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));

	Right = XMVector3Cross(Up, Forward);

	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UPVector, Up);
	XMStoreFloat3(&ForwardVector, Forward);
}

fvector_3d UTransformComponent::GetCorrectionPosition()
{
	fvector_3d F3D;
	GetCorrectionPosition(F3D);

	return F3D;
}

void UTransformComponent::GetCorrectionPosition(fvector_3d& InPos3)
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMVECTOR NewPosition = XMLoadFloat3(&Position);

	InPos3.x = -XMVectorGetX(XMVector3Dot(NewPosition, Right));
	InPos3.y = -XMVectorGetX(XMVector3Dot(NewPosition, Up));
	InPos3.z = -XMVectorGetX(XMVector3Dot(NewPosition, Forward));
}

