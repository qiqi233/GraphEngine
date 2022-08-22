#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "Core/Mesh.h"


class UCustomMesh : public UMesh
{
public:
	UCustomMesh();
	virtual ~UCustomMesh();
public:
	virtual void Init() override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData)override;

	void CreateMesh(FMeshRenderingData& MeshData, string& InPath);
private:
	static bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData);
};

