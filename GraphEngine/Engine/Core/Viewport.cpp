#include "Viewport.h"
#include "Config/EngineRenderConfig.h"

IViewport::IViewport()
{

}

IViewport::~IViewport()
{

}

void IViewport::ViewportInit()
{
	float AspectRatio = (float)FEngineRenderConfig::Get().ScreenWidth / (float)FEngineRenderConfig::Get().ScreenHight;
	//(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)
	//基于视野构建左手透视投影矩阵
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * MathHelper::Pi, //以弧度为单位的自上而下的视场角。
		AspectRatio,//视图空间 X:Y 的纵横比。
		1.0f,//到近剪裁平面的距离。必须大于零。
		1000.f//到远剪裁平面的距离。必须大于零。
	);
	//XMStoreFloat4x4(&ProjectMatrix,XMMatrixTranspose(Project));
	XMStoreFloat4x4(&ProjectMatrix, Project);
}

