#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "Core/Mesh.h"


class UConeMesh : public UMesh
{
public:
	UConeMesh();
	virtual ~UConeMesh();
public:
	virtual void Init() override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData)override;

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision , XMFLOAT4 Color = XMFLOAT4(Colors::White));
};

