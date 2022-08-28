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
	//������Ұ��������͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * MathHelper::Pi, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�
		AspectRatio,//��ͼ�ռ� X:Y ���ݺ�ȡ�
		1.0f,//��������ƽ��ľ��롣��������㡣
		1000.f//��Զ����ƽ��ľ��롣��������㡣
	);
	//XMStoreFloat4x4(&ProjectMatrix,XMMatrixTranspose(Project));
	XMStoreFloat4x4(&ProjectMatrix, Project);
}

