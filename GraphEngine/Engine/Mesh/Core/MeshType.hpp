#pragma once
#include "EngineMinimal.h"

//��������
struct FVertexData
{
	XMFLOAT3 Postion;//��������
	XMFLOAT4 Color;//������ɫ
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
