#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "Core/UploadBuffer.h"
#include "Math/MathHelper.h"
#include "Shader/Shader.h"
#include "MeshBufferData.h"


class UMesh: public UObject,public IRenderingIntface
{

public:
	typedef UMesh Super;
	UMesh();
	virtual ~UMesh();
public:
	virtual void Init() override;
	//用于更新Mesh的相关矩阵数据
	virtual void PreDraw(float InDeltaTime)override;
	virtual void Update(float InDeltaTime)override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData);
};

