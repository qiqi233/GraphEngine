#pragma once
#include "CoreMinimal.h"
#include "Math/MathHelper.h"


struct ViewportConstantsBuffer
{
public:
	ViewportConstantsBuffer()
		: ViewMatrix(MathHelper::Identity4x4())
		, ProjectMatrix(MathHelper::Identity4x4())
		, ViewProjectionMatrix(MathHelper::Identity4x4())
	{

	}
	//���������λ��,UP��Right,Look
	XMFLOAT4X4 ViewMatrix;

	//���ӷ�Χ ���ӽǶ�
	XMFLOAT4X4 ProjectMatrix;

	//View*Project
	XMFLOAT4X4 ViewProjectionMatrix;

};



class IViewport
{
public:
	IViewport();
	virtual ~IViewport();

	void ViewportInit();
	//���������λ��,UP��Right,Look
	XMFLOAT4X4 ViewMatrix;

	//���ӷ�Χ ���ӽǶ�
	XMFLOAT4X4 ProjectMatrix;
};