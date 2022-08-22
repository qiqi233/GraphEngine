﻿#include "BoxMesh.h"
UBoxMesh::UBoxMesh()
{

}

UBoxMesh::~UBoxMesh()
{

}

void UBoxMesh::Init()
{
	Super::Init();
}

void UBoxMesh::Draw(float InDeltaTime)
{
	Super::Draw(InDeltaTime);
}

void UBoxMesh::BuildMesh(const FMeshRenderingData* InData)
{
	Super::BuildMesh(InData);
}

void UBoxMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth, XMFLOAT4 InColor/* = XMFLOAT4(Colors::White)*/)
{
	/**
	 *	    7___________6
	 *	  / |		 / |
	 *  /	|	   /   |
	 * 4-----------5-  |
	 * |	|3____|___2/
	 * |   /	  |  /
	 * |  /		  | /
	 * 0-----------1
	 */

	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;

	//构建我们的顶点
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(-CWidth, -CHeight, -CDepth), InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(-CWidth, CHeight, -CDepth),	InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(CWidth, CHeight, -CDepth),	InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(CWidth, -CHeight, -CDepth),	InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(-CWidth, -CHeight, CDepth),	InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(-CWidth, CHeight, CDepth),	InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(CWidth, CHeight, CDepth),	InColor));
	MeshData.VertexData.push_back(FVertexData(XMFLOAT3(CWidth, -CHeight, CDepth),	InColor));

	//构建我们的索引
	//前
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

	//后
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

	//左
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

	//右
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

	//上
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

	//下
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);
}

