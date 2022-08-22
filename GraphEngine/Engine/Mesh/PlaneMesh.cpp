#include "PlaneMesh.h"

UPlaneMesh::UPlaneMesh()
{

}

UPlaneMesh::~UPlaneMesh()
{

}

void UPlaneMesh::Init()
{
	Super::Init();
}

void UPlaneMesh::Draw(float InDeltaTime)
{
	Super::Draw(InDeltaTime);
}

void UPlaneMesh::BuildMesh(const FMeshRenderingData* InData)
{
	Super::BuildMesh(InData);
}

void UPlaneMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide, XMFLOAT4 InColor /*= XMFLOAT4(Colors::White)*/)
{
	auto SubdivideValue = [&](float InValue, uint32_t InSubdivideValue)->float
	{
		if (InSubdivideValue <= 1)
		{
			return InValue;
		}

		return InValue / ((float)InSubdivideValue - 1);
	};

	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;

	float HeightSubdivideValue = SubdivideValue(InHeight, InHeightSubdivide);
	float WidthSubdivideValue = SubdivideValue(InWidth, InWidthSubdivide);

	//���Ƶ��λ��
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CHeight - i * HeightSubdivideValue;
		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CWidth - j * WidthSubdivideValue;
			MeshData.VertexData.push_back(FVertexData(
				XMFLOAT3(
					X,//x
					0.f,//y
					Z), //z
				XMFLOAT4(InColor)));
		}
	}

	//����index
	for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
		{
			////���ǻ��Ƶ����ı���
			//���ǻ��Ƶ����ı���
			//������1
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j);

			//������2
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
		}
	}
}

