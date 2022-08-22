#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "Core/Mesh.h"


class UBoxMesh: public UMesh
{
public:
	UBoxMesh();
	virtual ~UBoxMesh();
public:
	virtual void Init() override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData)override ;
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth, XMFLOAT4 Color=XMFLOAT4(Colors::White));
};

