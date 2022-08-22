#include "MeshManage.h"
#include "Config/EngineRenderConfig.h"
#include <Core/UploadBuffer.h>
#include "../SphereMesh.h"
#include "../CustomMesh.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CylinderMesh.h"
#include "../PlaneMesh.h"

UMeshManage::UMeshManage()
	: VertexSizeInBytes(0)
	, VertexStrideInBytes(0)
	, IndexSizeInBytes(0)
	, IndexFormat(DXGI_FORMAT_R16_UINT)
	, IndexSize(0)
	, WorldMatrix(MathHelper::Identity4x4())
	, ViewMatrix(MathHelper::Identity4x4())
	, ProjectMatrix(MathHelper::Identity4x4())
{

}

void UMeshManage::Init()
{
	float AspectRatio = (float)FEngineRenderConfig::Get().ScreenWidth / (float)FEngineRenderConfig::Get().ScreenHight;
	//(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)
	//������Ұ��������͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * MathHelper::Pi, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�
		AspectRatio,//��ͼ�ռ� X:Y ���ݺ�ȡ�
		1.0f,//��������ƽ��ľ��롣��������㡣
		1000.f//��Զ����ƽ��ľ��롣��������㡣
	);
	//XMStoreFloat4x4(&ProjectMatrix,XMMatrixTranspose(Project));
	XMStoreFloat4x4(&ProjectMatrix, Project);
}

void UMeshManage::BuildMesh(const FMeshRenderingData* InData)
{
	HRESULT bSuccess;
	//---------------------ģ�͹���-------------------------------------------//
	VertexStrideInBytes = sizeof(FVertexData);
	VertexSizeInBytes = InData->VertexData.size() * VertexStrideInBytes;
	IndexSize = InData->IndexData.size();
	IndexSizeInBytes = InData->IndexData.size() * sizeof(uint16_t);

	//����һ������Ķ����ƵĻ�������С
	DX12_DEBUG_MESSAGE(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InData->VertexData.data(), VertexSizeInBytes);
	//����һ�������Ķ����ƵĻ�������С
	DX12_DEBUG_MESSAGE(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InData->IndexData.data(), IndexSizeInBytes);
	//�������㻺�����Լ�����������
	GPUVertexBufferPtr = ConstructDefaultBuffer(GPUVertexBufferUploadPtr, InData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(GPUIndexBufferUploadPtr, InData->IndexData.data(), IndexSizeInBytes);


	//---------------------��������������-------------------------------------------//
	//���������������ϴ��ѣ�Ϊ�˽�������CPU�˿������ϴ���������׼����
	objectConstants = std::make_unique<UploadBuffer<ObjectConstants>>(GetD3dDevice().Get(), 1, true);

	//D3D12_CPU_DESCRIPTOR_HANDLE CBVDescHandle;
	//���������������ѣ����ڴ�ų�����������ͼ����������һ�鳣�������������ڸö���
	D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc;
	CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVHeapDesc.NodeMask = 0;
	CBVHeapDesc.NumDescriptors = 1;//������һ��
	CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(CBVHeap.GetAddressOf()));
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = objectConstants->Resource()->GetGPUVirtualAddress();
	//����������������ͼ�����԰������ɴ��һ�������������Ķ���
	D3D12_CONSTANT_BUFFER_VIEW_DESC  CBVDesc;
	CBVDesc.BufferLocation = cbAddress;
	CBVDesc.SizeInBytes = objectConstants->CalcConstantBufferByteSize();
	GetD3dDevice()->CreateConstantBufferView(&CBVDesc, CBVHeap->GetCPUDescriptorHandleForHeapStart());

	//�����������������������Ǹ��������������������߸������������������Ǹ���������
	//��������ֻ��һ��������
	CD3DX12_ROOT_PARAMETER RootParameter[1];
	//�������ķ�Χ������˵��������ʲô
	CD3DX12_DESCRIPTOR_RANGE DescriptorRange;
	DescriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,//��������Χ��CBV
		1,//����������
		0,//�Ĵ�������0-15
		0//�Ĵ����ռ� space0 space1 ������0
	);
	//��ʼ������������
	RootParameter[0].InitAsDescriptorTable(1, &DescriptorRange);

	//��ǩ�������������ڰ����еĸ�ǩ������������������棬Ȼ�󽻸�D3D12SerializeRootSignature����Ӳ�������Ǵ���һ�������ĸ�ǩ���ڴ�
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		1,
		RootParameter,
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//�洢��һϵ�г����������Ķ������ڴ�飬���Ǳ����ڳ��������������棬�������һ������������������ͼ
	//����������Ǵӡ������������ѡ������õ�һϵ�г�����������ͼ
	ComPtr<ID3DBlob> CBVBlob;
	ComPtr<ID3DBlob> ErrorBlob;
	//���л�һ���������ڴ棬���ڴ��һϵ�и�ǩ��
	bSuccess = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, CBVBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
	if (ErrorBlob)
	{
		GE_LOG(Error, "CBVBlob Serialize is failed,[%s]", (char*)ErrorBlob->GetBufferPointer());
		assert(bSuccess);
	}
	//������ǩ��
	bSuccess = GetD3dDevice()->CreateRootSignature(0, CBVBlob->GetBufferPointer(), CBVBlob->GetBufferSize(), IID_PPV_ARGS(RootSignature.GetAddressOf()));
	DX12_DEBUG_MESSAGE(bSuccess);

	//-------------------------------����ˮ��״̬PSO----------------------------------------//

	//������ɫ��
	VertexShader.BuildShaders(L"../GraphEngine/Shader/testShader.hlsl", "VSMain", "vs_5_0");
	PixelShader.BuildShaders(L"../GraphEngine/Shader/testShader.hlsl", "PSMain", "ps_5_0");

	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	//������ˮ��״̬���Ѷ������󶨵���ˮ���� ���������������ڣ�VS��PS��ɫ������ǩ������������MSAA��ͼԪģʽ��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	memset(&psoDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	//�����벼������
	psoDesc.InputLayout.NumElements = (UINT)InputElementDesc.size();
	psoDesc.InputLayout.pInputElementDescs = InputElementDesc.data();

	//��vs��ɫ��
	psoDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	psoDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());
	//��ps��ɫ��
	psoDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
	psoDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());

	//�󶨸�ǩ��
	psoDesc.pRootSignature = RootSignature.Get();

	//���ù�դ��״̬
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// psoDesc.RasterizerState.FrontCounterClockwise=false;
	//0000..0000
	psoDesc.SampleMask = UINT_MAX;//�����ȼ�
	//����ͼԪ�����ͣ�������
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//rendertarget����
	psoDesc.NumRenderTargets = 1;
	//���û��ģʽ
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//�������/ģ��״̬
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//���ò�������
	psoDesc.SampleDesc.Count = GetRenderEngine()->GetDXGISampleCount();
	psoDesc.SampleDesc.Quality = GetRenderEngine()->GetDXGISampleQuality();

	//����RTV��ʽ
	psoDesc.RTVFormats[0] = GetRenderEngine()->GetBackBufferFormat();
	//�������/ģ���ʽ
	psoDesc.DSVFormat = GetRenderEngine()->GetDepthStencilFormat();

	bSuccess = GetD3dDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(PSO.GetAddressOf()));
	//ThrowIfFailed(bSuccess);
	//-----------------------------------------------------------------------//


}

void UMeshManage::Update(float DeltaTime)
{
	XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);

	XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z, 1.0f);
	XMVECTOR ViewTarget = XMVectorZero();
	XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

	XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
	XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

	XMMATRIX ATRIXWorld = XMLoadFloat4x4(&WorldMatrix);
	XMMATRIX ATRIXProject = XMLoadFloat4x4(&ProjectMatrix);
	XMMATRIX WVP = ATRIXWorld * ViewLookAt * ATRIXProject;

	ObjectConstants ContantObj;
	XMStoreFloat4x4(&ContantObj.WorldViewProj, XMMatrixTranspose(WVP));
	objectConstants->CopyData(0, ContantObj);
}

void UMeshManage::Draw(float DeltaTime)
{
	//���ó�����������
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	//���ø�ǩ��
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());

	D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();
	// 	VBV.BufferLocation= GPUVertexBufferPtr->GetGPUVirtualAddress();
	// 	VBV.SizeInBytes= VertexSizeInBytes;
	// 	VBV.StrideInBytes=VertexStrideInBytes;
		//����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
	GetGraphicsCommandList()->IASetVertexBuffers(
		0,//��ʼ����� 0-15 
		1,//k k+1 ... k+n-1 
		&VBV);

	D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
	// 	IBV.BufferLocation = GPUIndexBufferUploadPtr->GetGPUVirtualAddress();
	// 	IBV.SizeInBytes = IndexSizeInBytes;
	// 	IBV.Format = IndexFormat;
	GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

	//��������Ҫ���Ƶ�����ͼԪ �� �� ��
	GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//������������
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	//�����Ļ���
	GetGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,//��������
		1,//����ʵ������
		0,//���㻺������һ�������Ƶ�����
		0,//GPU ��������������ȡ�ĵ�һ��������λ�á�
		0);//�ڴӶ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��
}

void UMeshManage::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

D3D12_VERTEX_BUFFER_VIEW UMeshManage::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
    VBV.SizeInBytes = VertexSizeInBytes;
    VBV.StrideInBytes = VertexStrideInBytes;

    return VBV;
}

D3D12_INDEX_BUFFER_VIEW UMeshManage::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = IndexSizeInBytes;
    IBV.Format = IndexFormat;

    return IBV;
}

UMesh* UMeshManage::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<USphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

UMesh* UMeshManage::CreateMesh(string& InPath)
{
    return CreateMesh<UCustomMesh>(InPath);
}

UMesh* UMeshManage::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
    return CreateMesh<UBoxMesh>(InHeight, InWidth, InDepth);
}

UMesh* UMeshManage::CreateConeMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<UConeMesh>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

UMesh* UMeshManage::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<UCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

UMesh* UMeshManage::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    return CreateMesh<UPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes>
T* UMeshManage::CreateMesh(ParamTypes && ...Params)
{
    T* MyMesh = new T();

    //��ȡģ����Դ
    FMeshRenderingData MeshData;
    MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

    MyMesh->BeginPlay();

    //����mesh
    BuildMesh(&MeshData);

    MyMesh->Init();

    return MyMesh;
}