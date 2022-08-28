#pragma once
#include "EngineMinimal.h"
#include "Core/RenderingInterface.h"
#include "Core/MeshBufferData.h"


class UCustomMesh : public IRenderingIntface
{
public:
	UCustomMesh();
	virtual ~UCustomMesh();
public:
	void CreateMesh(FMeshData& MeshData, string& InPath);
private:
	static bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshData& MeshData);
};

