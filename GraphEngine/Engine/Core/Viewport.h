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
	//定义摄像机位置,UP，Right,Look
	XMFLOAT4X4 ViewMatrix;

	//可视范围 可视角度
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
	//定义摄像机位置,UP，Right,Look
	XMFLOAT4X4 ViewMatrix;

	//可视范围 可视角度
	XMFLOAT4X4 ProjectMatrix;
};