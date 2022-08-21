#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "MeshType.hpp"
#include "Mesh.h"


class UBoxMesh: public UMesh
{
public:
	UBoxMesh();
	virtual ~UBoxMesh();
protected:
	virtual void Init() override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData)override ;
public:
	static UBoxMesh* CreateMesh();
};

