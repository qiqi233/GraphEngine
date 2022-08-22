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
	//基于视野构建左手透视投影矩阵
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * MathHelper::Pi, //以弧度为单位的自上而下的视场角。
		AspectRatio,//视图空间 X:Y 的纵横比。
		1.0f,//到近剪裁平面的距离。必须大于零。
		1000.f//到远剪裁平面的距离。必须大于零。
	);
	//XMStoreFloat4x4(&ProjectMatrix,XMMatrixTranspose(Project));
	XMStoreFloat4x4(&ProjectMatrix, Project);
}

void UMeshManage::BuildMesh(const FMeshRenderingData* InData)
{
	HRESULT bSuccess;
	//---------------------模型构建-------------------------------------------//
	VertexStrideInBytes = sizeof(FVertexData);
	VertexSizeInBytes = InData->VertexData.size() * VertexStrideInBytes;
	IndexSize = InData->IndexData.size();
	IndexSizeInBytes = InData->IndexData.size() * sizeof(uint16_t);

	//创建一个顶点的二进制的缓冲区大小
	DX12_DEBUG_MESSAGE(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InData->VertexData.data(), VertexSizeInBytes);
	//创建一个索引的二进制的缓冲区大小
	DX12_DEBUG_MESSAGE(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InData->IndexData.data(), IndexSizeInBytes);
	//创建顶点缓冲区以及索引缓冲区
	GPUVertexBufferPtr = ConstructDefaultBuffer(GPUVertexBufferUploadPtr, InData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(GPUIndexBufferUploadPtr, InData->IndexData.data(), IndexSizeInBytes);


	//---------------------常量缓冲区构建-------------------------------------------//
	//创建常量缓冲区上传堆，为了将常量从CPU端拷贝到上传缓冲区做准备。
	objectConstants = std::make_unique<UploadBuffer<ObjectConstants>>(GetD3dDevice().Get(), 1, true);

	//D3D12_CPU_DESCRIPTOR_HANDLE CBVDescHandle;
	//创建常量描述符堆，用于存放常量缓冲区视图，可以理解成一组常量缓冲区，存于该堆中
	D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc;
	CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVHeapDesc.NodeMask = 0;
	CBVHeapDesc.NumDescriptors = 1;//数量是一个
	CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(CBVHeap.GetAddressOf()));
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = objectConstants->Resource()->GetGPUVirtualAddress();
	//创建常量缓冲区视图，可以把它理解成存放一个常量缓冲区的对象
	D3D12_CONSTANT_BUFFER_VIEW_DESC  CBVDesc;
	CBVDesc.BufferLocation = cbAddress;
	CBVDesc.SizeInBytes = objectConstants->CalcConstantBufferByteSize();
	GetD3dDevice()->CreateConstantBufferView(&CBVDesc, CBVHeap->GetCPUDescriptorHandleForHeapStart());

	//创建根参数，根参数可以是根常量、根描述符表、或者根描述符，我们这里是跟描述符表
	//这里我们只有一个根参数
	CD3DX12_ROOT_PARAMETER RootParameter[1];
	//根参数的范围，或者说是类型是什么
	CD3DX12_DESCRIPTOR_RANGE DescriptorRange;
	DescriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,//描述符范围是CBV
		1,//描述符数量
		0,//寄存器索引0-15
		0//寄存器空间 space0 space1 这里是0
	);
	//初始化根描述符表
	RootParameter[0].InitAsDescriptorTable(1, &DescriptorRange);

	//根签名的描述，用于把所有的根签名都存在这个描述里面，然后交给D3D12SerializeRootSignature，让硬件给我们创建一块连续的根签名内存
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		1,
		RootParameter,
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//存储着一系列常量缓冲区的二进制内存块，它们被放在常量缓冲区堆里面，堆里面的一个叫做常量缓冲区视图
	//描述符表就是从【常量缓冲区堆】里面拿到一系列常量缓冲区视图
	ComPtr<ID3DBlob> CBVBlob;
	ComPtr<ID3DBlob> ErrorBlob;
	//序列化一块连续的内存，用于存放一系列根签名
	bSuccess = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, CBVBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
	if (ErrorBlob)
	{
		GE_LOG(Error, "CBVBlob Serialize is failed,[%s]", (char*)ErrorBlob->GetBufferPointer());
		assert(bSuccess);
	}
	//创建根签名
	bSuccess = GetD3dDevice()->CreateRootSignature(0, CBVBlob->GetBufferPointer(), CBVBlob->GetBufferSize(), IID_PPV_ARGS(RootSignature.GetAddressOf()));
	DX12_DEBUG_MESSAGE(bSuccess);

	//-------------------------------绑定流水线状态PSO----------------------------------------//

	//编译着色器
	VertexShader.BuildShaders(L"../GraphEngine/Shader/testShader.hlsl", "VSMain", "vs_5_0");
	PixelShader.BuildShaders(L"../GraphEngine/Shader/testShader.hlsl", "PSMain", "ps_5_0");

	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	//创建流水线状态，把东西都绑定到流水线上 东西包括但不限于：VS、PS着色器、根签名、采样器、MSAA、图元模式等
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	memset(&psoDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	//绑定输入布局描述
	psoDesc.InputLayout.NumElements = (UINT)InputElementDesc.size();
	psoDesc.InputLayout.pInputElementDescs = InputElementDesc.data();

	//绑定vs着色器
	psoDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	psoDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());
	//绑定ps着色器
	psoDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
	psoDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());

	//绑定根签名
	psoDesc.pRootSignature = RootSignature.Get();

	//配置光栅化状态
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// psoDesc.RasterizerState.FrontCounterClockwise=false;
	//0000..0000
	psoDesc.SampleMask = UINT_MAX;//采样等级
	//绘制图元的类型：三角形
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//rendertarget数量
	psoDesc.NumRenderTargets = 1;
	//设置混合模式
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//设置深度/模板状态
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//设置采样级别
	psoDesc.SampleDesc.Count = GetRenderEngine()->GetDXGISampleCount();
	psoDesc.SampleDesc.Quality = GetRenderEngine()->GetDXGISampleQuality();

	//设置RTV格式
	psoDesc.RTVFormats[0] = GetRenderEngine()->GetBackBufferFormat();
	//设置深度/模板格式
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
	//设置常量描述符堆
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	//设置根签名
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());

	D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();
	// 	VBV.BufferLocation= GPUVertexBufferPtr->GetGPUVirtualAddress();
	// 	VBV.SizeInBytes= VertexSizeInBytes;
	// 	VBV.StrideInBytes=VertexStrideInBytes;
		//绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
	GetGraphicsCommandList()->IASetVertexBuffers(
		0,//起始输入槽 0-15 
		1,//k k+1 ... k+n-1 
		&VBV);

	D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
	// 	IBV.BufferLocation = GPUIndexBufferUploadPtr->GetGPUVirtualAddress();
	// 	IBV.SizeInBytes = IndexSizeInBytes;
	// 	IBV.Format = IndexFormat;
	GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

	//定义我们要绘制的哪种图元 点 线 面
	GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//设置描述符表
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	//真正的绘制
	GetGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,//顶点数量
		1,//绘制实例数量
		0,//顶点缓冲区第一个被绘制的索引
		0,//GPU 从索引缓冲区读取的第一个索引的位置。
		0);//在从顶点缓冲区读取每个实例数据之前添加到每个索引的值。
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

    //提取模型资源
    FMeshRenderingData MeshData;
    MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

    MyMesh->BeginPlay();

    //构建mesh
    BuildMesh(&MeshData);

    MyMesh->Init();

    return MyMesh;
}