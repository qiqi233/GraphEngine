#pragma once
#include "Mesh.h"
#include "Core/RenderingInterface.h"
#include "Core/Object.h"
#include "DirectX/DirectXDeviceInterfece.h"

class FRenderingResourcesUpdate;

class UMeshManage 
	: public UObject
	, public IRenderingIntface
	, public IDirectXDeviceInterfece
{
public:
	UMeshManage();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void Update(float DeltaTime);
public:
	UMesh* CreateBoxMesh(
		float InHeight,
		float InWidth,
		float InDepth);
	
	UMesh* CreateConeMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	UMesh* CreateCylinderMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	UMesh* CreatePlaneMesh(
		float InHeight, 
		float InWidth,
		uint32_t InHeightSubdivide, 
		uint32_t InWidthSubdivide);

	UMesh* CreateSphereMesh(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	UMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);

private:
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;	//CPU顶点缓冲区指针
	ComPtr<ID3DBlob> CPUIndexBufferPtr;		//CPU顶点索引缓冲区指针

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;	//GPU顶点缓冲区指针
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;	//GPU顶点索引缓冲区指针

	ComPtr<ID3D12Resource> GPUVertexBufferUploadPtr;	//临时的GPU顶点缓冲区指针 Upload
	ComPtr<ID3D12Resource> GPUIndexBufferUploadPtr;	//临时的GPU顶点索引缓冲区指针 Upload

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
