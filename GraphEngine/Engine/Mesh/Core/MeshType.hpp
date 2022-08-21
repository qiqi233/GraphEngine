#pragma once
#include "EngineMinimal.h"

//顶点数据
struct FVertexData
{
	XMFLOAT3 Postion;//顶点坐标
	XMFLOAT4 Color;//顶点颜色
public:
	FVertexData()=default;
	FVertexData(XMFLOAT3 InPostion, XMFLOAT4 InColor= XMFLOAT4(Colors::White))
		:Postion(InPostion)
		, Color(InColor)
	{
		
	}
};

struct FMeshRenderingData
{
	std::vector<FVertexData>VertexData;
	std::vector<uint16_t> IndexData;

};
