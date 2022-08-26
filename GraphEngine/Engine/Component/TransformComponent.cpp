#include "TransformComponent.h"
#include <DirectXMathVector.inl>

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

void UTransformComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	Position = InNewPosition;
}

void UTransformComponent::SetRotation(const fvector_3d& InNewRotation)
{
	//XMConvertToRadians：度数转弧度值
	//XMConvertToDegrees：弧度值转度数
	float RollRadians = XMConvertToRadians(InNewRotation.z);
	float PithRadians = XMConvertToRadians(InNewRotation.x);
	float YawRadians = XMConvertToRadians(InNewRotation.y);

	//旋转矩阵
	XMMATRIX RotMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotMatrix));
}

void UTransformComponent::SetScale(const fvector_3d& InNewScale)
{

}

void UTransformComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{

}

void UTransformComponent::SetRightVector(const XMFLOAT3& InRightVector)
{

}

void UTransformComponent::SetUPVector(const XMFLOAT3& InUPVector)
{

}

