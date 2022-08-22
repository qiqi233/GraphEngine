#pragma once
#include "EngineMinimal.h"

//顶点数据
struct FVertexData
{
	XMFLOAT3 Position;//顶点坐标
	XMFLOAT3 Normal;//法线
	XMFLOAT4 Color;//顶点颜色

public:
	FVertexData()=default;
	FVertexData(XMFLOAT3 InPostion, XMFLOAT4 InColor= XMFLOAT4(Colors::White))
		: Position(InPostion)
		, Normal(0, 1, 0)
		, Color(InColor)
	{
		
	}
	FVertexData(XMFLOAT3 InPostion, XMFLOAT3 InNormal,XMFLOAT4 InColor = XMFLOAT4(Colors::White))
		: Position(InPostion)
		, Normal(InNormal)
		, Color(InColor)

	{

	}
};

struct FMeshRenderingData
{
	std::vector<FVertexData>VertexData;
	std::vector<uint16_t> IndexData;

};
