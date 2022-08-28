#pragma once
#include "EngineMinimal.h"
#include "Core/RenderingInterface.h"
#include "Core/Actor.h"
#include "MeshBufferData.h"


class AStaticMesh: public AActor,public IRenderingIntface
{
public:
	typedef AStaticMesh Super;
	AStaticMesh();
	virtual ~AStaticMesh();
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void EndPlay();
	virtual void DestroyPlay();
	virtual void BuildMesh(const FMeshData* InData);


};

