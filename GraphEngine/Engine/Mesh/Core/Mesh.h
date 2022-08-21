#pragma once
#include "EngineMinimal.h"
#include "Core/Object.h"
#include "Core/RenderingInterface.h"
#include "MeshType.hpp"
#include "Core/UploadBuffer.h"
#include "Math/MathHelper.h"
#include "Shader/Shader.h"

struct ObjectConstants
{
public:
	ObjectConstants()
	:WorldViewProj(MathHelper::Identity4x4())
	, gTime(0)
	{
		
	}
	XMFLOAT4X4 WorldViewProj;
	float   gTime;

};
class UMesh: public UObject,public IRenderingIntface
{
public:
	typedef UMesh Super;
	UMesh();
	virtual ~UMesh();
protected:
	virtual void Init() override;
	//���ڸ���Mesh����ؾ�������
	virtual void PreDraw(float InDeltaTime)override;
	virtual void Update(float InDeltaTime)override;
	virtual void Draw(float InDeltaTime) override;
	virtual void BuildMesh(const FMeshRenderingData* InData);
private:
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;	//CPU���㻺����ָ��
	ComPtr<ID3DBlob> CPUIndexBufferPtr;		//CPU��������������ָ��

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;	//GPU���㻺����ָ��
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;	//GPU��������������ָ��

	ComPtr<ID3D12Resource> GPUVertexBufferUploadPtr;	//��ʱ��GPU���㻺����ָ�� Upload
	ComPtr<ID3D12Resource> GPUIndexBufferUploadPtr;	//��ʱ��GPU��������������ָ�� Upload

	ComPtr<ID3D12RootSignature>  RootSignature;
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
	std::unique_ptr<UploadBuffer<ObjectConstants>> objectConstants;

	ComPtr<ID3D12PipelineState> PSO;

	FShader VertexShader;
	FShader PixelShader;

	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;
protected:
	UINT VertexSizeInBytes;
	UINT VertexStrideInBytes;

	UINT IndexSizeInBytes;
	DXGI_FORMAT IndexFormat;
	UINT IndexSize;

	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;
};

