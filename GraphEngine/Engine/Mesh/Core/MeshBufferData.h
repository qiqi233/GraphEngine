#pragma once
#include "EngineMinimal.h"

//��������
struct FVertexData
{
	XMFLOAT3 Position;//��������
	XMFLOAT3 Normal;//����
	XMFLOAT4 Color;//������ɫ

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
