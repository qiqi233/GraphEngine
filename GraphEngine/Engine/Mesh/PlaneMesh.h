#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "Core/Mesh.h"


class UPlaneMesh : public UMesh
{
public:
	UPlaneMesh();
	virtual ~UPlaneMesh();
public:
	virtual void Init() override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData)override;
public:
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide, XMFLOAT4 InColor = XMFLOAT4(Colors::White));
};

