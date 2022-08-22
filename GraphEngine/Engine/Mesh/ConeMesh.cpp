#include "ConeMesh.h"

UConeMesh::UConeMesh()
{

}

UConeMesh::~UConeMesh()
{

}

void UConeMesh::Init()
{
	Super::Init();
}

void UConeMesh::Draw(float InDeltaTime)
{
	Super::Draw(InDeltaTime);
}

void UConeMesh::BuildMesh(const FMeshRenderingData* InData)
{
	Super::BuildMesh(InData);
}

void UConeMesh::CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, XMFLOAT4 InColor /*= XMFLOAT4(InColor)*/)
{
	//半径间隔
	float RadiusInterval = -InRadius / (float)InHeightSubdivision;
	//高度间隔
	float HeightInterval = InHeight / (float)InHeightSubdivision;

	float BetaValue = XM_2PI / (float)InAxialSubdivision;

	//构建顶部
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(0.f, 0.5f * InHeight, 0.f), XMFLOAT4(InColor)));

	uint32_t Index = MeshData.VertexData.size();

	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i;
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			MeshData.VertexData.push_back(FVertexData(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),//x
					Y,//y
					Radius * sinf(j * BetaValue)), //z
				XMFLOAT4(InColor)));
		}
	}

	//添加中点
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(0.f, -0.5f * InHeight, 0.f), XMFLOAT4(InColor)));

	//绘制index模型
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	float BaseIndex = 1;
	float VertexCircleNum = InAxialSubdivision + 1;
	//绘制腰围
	for (uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//我们绘制的是四边形
			//三角形1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//三角形2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	//绘制南极
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;
	BaseIndex = SouthBaseIndex - VertexCircleNum;
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)
	{
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + Index);
		MeshData.IndexData.push_back(BaseIndex + Index + 1);
	}
}

