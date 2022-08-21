#include "WindowsEngine.h"
#include "WindowsMessageProcessing.h"
#include "DebugInfo/EngineDebug.h"
#include "Config/EngineRenderConfig.h"
#include <oaidl.h>
#include <atlcomcli.h>
#include "Core/RenderingInterface.h"
#include "Mesh/BoxMesh.h"
#if defined(_WIN32)||defined(_WIN64)
FWindowsEngine::FWindowsEngine()
	:CurrentSwapBuffIndex(0)
	, M4QualityLevels(0)
	, bMSAAEnable(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
	, DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	for (int i = 0; i < FEngineRenderConfig::Get().SwapChainCount; ++i)
	{
		SwapChainResource.push_back(ComPtr<ID3D12Resource>());
	}
}

int FWindowsEngine::PreInit(ICommandParameters Parameters)
{
	//��־ϵͳ��ʼ��
	const char LogPath[] = "../Log";
	init_log_system(LogPath);
	GE_LOG(Log, "Engine pre init success!");
	//��������

	GE_LOG(Log, "Engine pre init success!");
	return 0;

}

int FWindowsEngine::Init(ICommandParameters Parameters)
{
	if (!InitWindows(Parameters))
	{
		GE_LOG(Error, "Engine pre init Failed!");
		return -1;
	}
	if (!InitDirect3D())
	{
		GE_LOG(Error, "Init Direct3D Failed!");
		return 0;
	}
	PostInitDirect3D();


	GE_LOG(Log, "WindowsEngine Init() initialization complete.");
	return 0;
}

int FWindowsEngine::PostInit()
{
	DX12_DEBUG_MESSAGE(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	{
		//����Mesh
		UBoxMesh* Box = UBoxMesh::CreateMesh();

	}
	DX12_DEBUG_MESSAGE(GraphicsCommandList->Close());
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);
	WaitGPUCommandQueueComplete();



	return 0;
}

void FWindowsEngine::Tick(float DeltaTime)
{
	MSG Msg{};
	while (true)
	{	
			//PM_NOREMOVE ��Ϣ���Ӷ����������
			//PM_REMOVE   ��Ϣ�Ӷ����������
			//PM_NOYIELD  �˱�־ʹϵͳ���ͷŵȴ����ó�����е��߳�
			//PM_QS_INPUT �������ͼ�����Ϣ��
			//PM_QS_PAINT ����ͼ��Ϣ��
			//PM_QS_POSTMESSAGE �������б����͵���Ϣ��������ʱ�����ȼ���
			//PM_QS_SENDMESSAGE �������з�����Ϣ��
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
				break;
			TranslateMessage(&Msg);//������Ϣ��
			DispatchMessage(&Msg);//�ַ���Ϣ��
		}
		else
		{
			//Sleep(static_cast<unsigned long>(DeltaTime * 1000));
			//�����һ֡
			Rendering(DeltaTime);
		}
	}

}

int FWindowsEngine::PreExit()
{
	GE_LOG(Log, "WindowsEngine PreExit() initialization complete.");
	return 0;
}

int FWindowsEngine::Exit()
{
	GE_LOG(Log, "WindowsEngine Exit() initialization complete.");
	return 0;
}

int FWindowsEngine::PostExit()
{
	GE_LOG(Log, "WindowsEngine PostExit() initialization complete.");
	return 0;
}


bool FWindowsEngine::InitWindows(ICommandParameters InParameters)
{
	//ע�ᴰ��
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = nullptr;//����������ľ���GDI����
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//����һ����ͷ���
	WindowsClass.hIcon = nullptr; //Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = NULL;//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance; //����ʵ��
	WindowsClass.lpszClassName = L"GraphEngine";//��������
	WindowsClass.lpszMenuName = nullptr;//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//��ô���ƴ��� ��ֱ��ˮƽ�ػ�
	WindowsClass.lpfnWndProc = EngineWindowProc;//��Ϣ������

	//ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		GE_LOG(Error, "Register windows class fail.");
		MessageBox(NULL, L"Register windows class fail.", L"Error", MB_OK);
	}

	//@rect �ʿ�
	//WS_OVERLAPPEDWINDOW �ʿڷ��
	//NULL û�в˵�
	RECT Rect{ 0,0,FEngineRenderConfig::Get().ScreenWidth,FEngineRenderConfig::Get().ScreenHight };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);


	const int x = GetSystemMetrics(SM_CXSCREEN);
	const int y = GetSystemMetrics(SM_CYSCREEN);

	MainHwnd = CreateWindowEx(
		NULL,//���ڶ���ķ��
		L"GraphEngine", // ��������
		L"Graph Engine",//����ʾ�ڴ��ڵı�������ȥ
		WS_OVERLAPPEDWINDOW, //���ڷ��
		(x - FEngineRenderConfig::Get().ScreenWidth) / 2, (y - FEngineRenderConfig::Get().ScreenHight) / 2,//���ڵ�����
		FEngineRenderConfig::Get().ScreenWidth, FEngineRenderConfig::Get().ScreenHight,//
		NULL, //�����ھ��
		nullptr, //�˵����
		InParameters.HInstance,//����ʵ��
		NULL);//
	if (!MainHwnd)
	{
		GE_LOG(Error, "CreateWindow Failed..");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//��ʾ����
	ShowWindow(MainHwnd, SW_SHOW);

	//��������ģ�ˢ��һ��
	UpdateWindow(MainHwnd);

	GE_LOG(Log, "InitWindows complete.");
	return true;
}

bool FWindowsEngine::InitDirect3D()
{
	HRESULT Result = S_OK;
	//Debug
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(D3D12Debug.GetAddressOf()))))
	{
		D3D12Debug->EnableDebugLayer();
	}
	//HRESULT;
	//S_OK				0x000000	�ɹ�����
	//E_UNEXPECTED		0x8000FFFF	�����ʧ��
	//E_NOTIMPL			0x80004001	δʵ��
	//E_OUTOFMEMORY		0x800700E	δ�ܷ���������ڴ�
	//E_INVALIDARG		0x8007005	һ������������Ч
	//etc...  #include <winerror.h>
	//����DXGI������������ڴ���DirectXͼ�λ����ṹ(DXGI)����
	CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory));
	//NULL::��ʾʹ��Ĭ�ϵ���ʾ��
	Result = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(Result))
	{
		//warp �߼���դ��ƽ̨�����ģ�⣩
		ComPtr<IDXGIAdapter> warpAdpter;
		DX12_DEBUG_MESSAGE(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdpter)));
		DX12_DEBUG_MESSAGE(D3D12CreateDevice(warpAdpter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&D3dDevice)));
	}
	//Χ����Ϊ��ƽ��CPU��GPU
	Result = D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(D3dFence.GetAddressOf()));
	//�����������
	D3D12_COMMAND_QUEUE_DESC QueueDesc{};//���е�����
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;


	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL = 0,�������ȼ���
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH = 100,�����ȼ���
	//D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME = 10000//ȫ��ʵʱ���ȼ���
	//QueueDesc.Priority= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	Result = D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue));
	//�������������
	//D3D12_COMMAND_LIST_TYPE_DIRECT = 0,GPU����ֱ��ִ�е�����
	//D3D12_COMMAND_LIST_TYPE_BUNDLE = 1,
	//D3D12_COMMAND_LIST_TYPE_COMPUTE = 2,ָ�����ڼ�������������
	//D3D12_COMMAND_LIST_TYPE_COPY = 3,ָ�����ڸ��Ƶ����������
	//D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE = 4,ָ��������Ƶ��������������
	//D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS = 5,ָ��������Ƶ��������������
	//D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE = 4,ָ��������Ƶ��������������
	Result = D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()));

	//���������б�
	Result = D3dDevice->CreateCommandList(
		0, //Ĭ�ϵ���GPU
		D3D12_COMMAND_LIST_TYPE_DIRECT,//ֱ������
		CommandAllocator.Get(),//��CommandList������Allocator
		nullptr,//ID3D12PipelineState
		IID_PPV_ARGS(&GraphicsCommandList));
	//���������б��ȹر���
	GraphicsCommandList->Close();

	if (FAILED(Result))
	{
		//������д���ʧ��
		GE_LOG(Error, "GraphicsCommandList Create Failed[%d]", (int)Result);
	}
	//////////////////////���Ϳ������ز���///////////////////////
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS  QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;//��������
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;
	Result = D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,//��������������
		&QualityLevels,
		sizeof(QualityLevels)
	);
	//if(SUCCEEDED(Result))
	//{
	//	bMSAAEnable=true;
	//}
	M4QualityLevels = QualityLevels.NumQualityLevels;//��������
	///////////////////////////////////����������///////////////////////////////////
	DXGISwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::Get().ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::Get().ScreenHight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::Get().RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::Get().SwapChainCount;
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//�����ʽ
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//ʹ�ñ������Դ��Ϊ�����ȾĿ�ꡣ
	SwapChainDesc.OutputWindow = MainHwnd;//ָ��windows���
	SwapChainDesc.Windowed = true;//�Դ�������
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//IDXGISwapChain::ResizeTarget

	//���ز�������
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();

	Result = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, DXGISwapChain.GetAddressOf());
	if (FAILED(Result))  //    ��������˴��� 
	{

	}
	//////////////////��ʼ����Դ������///////////////////////////////
	//��ȾĿ�����ͼ��Դ
	D3D12_DESCRIPTOR_HEAP_DESC RTVDesc;
	RTVDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDesc.NodeMask = 0;
	RTVDesc.NumDescriptors = FEngineRenderConfig::Get().SwapChainCount;
	RTVDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	DX12_DEBUG_MESSAGE(D3dDevice->CreateDescriptorHeap(&RTVDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//���/ģ����ͼ��Դ
	D3D12_DESCRIPTOR_HEAP_DESC DSVDesc;
	DSVDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDesc.NodeMask = 0;
	DSVDesc.NumDescriptors = 1;
	DSVDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DX12_DEBUG_MESSAGE(D3dDevice->CreateDescriptorHeap(&DSVDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));


	return true;
}

void FWindowsEngine::PostInitDirect3D()
{
	//CPU��GPU��Ⱦ��
	WaitGPUCommandQueueComplete();
	//���������б�
	DX12_DEBUG_MESSAGE(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));


	for (UINT i = 0; i < FEngineRenderConfig::Get().SwapChainCount; ++i)
	{
		SwapChainResource[i].Reset();
	}
	DepthStencilResource.Reset();

	//���Ľ������ĺ�̨��������С����ʽ�ͻ�������������Ӧ����Ӧ�ó��򴰿ڴ�С����ʱ���á�
	DXGISwapChain->ResizeBuffers(
		FEngineRenderConfig::Get().SwapChainCount,
		FEngineRenderConfig::Get().ScreenWidth,
		FEngineRenderConfig::Get().ScreenHight, BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	//��ȡ��ʾ�ѿ�ʼ�� CPU �����������
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (UINT i = 0; i < FEngineRenderConfig::Get().SwapChainCount; ++i)
	{
		DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainResource[i]));
		//nullptr:��дnullptr��ʾʹ�����ǳ�ʼ�����õ�Ĭ��ֵ
		D3dDevice->CreateRenderTargetView(SwapChainResource[i].Get(), nullptr, RTVHandle);
		//�ƶ�ָ��
		RTVHandle.Offset(1, RTVDescriptorSize);
	}
	GE_LOG(Log, "WindowsEngine PostInit() initialization complete.");

	//������Ҫ�ύ����Դ
	//D3D12_HEAP_TYPE_DEFAULT Ĭ�϶� ֻ��GPU���д�����Ȼ�����
	//D3D12_HEAP_TYPE_UPLOAD �ϴ��� 
	//D3D12_HEAP_TYPE_READBACK ��ҪCPU��ȡ�ģ���֮Ϊ�ض���
	//��Դ����
	D3D12_RESOURCE_DESC Resource_desc;
	Resource_desc.Width = FEngineRenderConfig::Get().ScreenWidth;
	Resource_desc.Height = FEngineRenderConfig::Get().ScreenHight;
	Resource_desc.Alignment = 0;//���뷽ʽ
	Resource_desc.MipLevels = 1;
	Resource_desc.DepthOrArraySize = 1;
	Resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//��Դ��ά��
	Resource_desc.SampleDesc.Count = GetDXGISampleCount();
	Resource_desc.SampleDesc.Quality = GetDXGISampleQuality();
	Resource_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	Resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//������ȥʹ��һ�����ģ��
	Resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	D3D12_CLEAR_VALUE Clear_value;
	Clear_value.DepthStencil.Depth = 1.f;
	Clear_value.DepthStencil.Stencil = 0;
	Clear_value.Format = DepthStencilFormat;
	CD3DX12_HEAP_PROPERTIES Heap_properties(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(
		&Heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&Resource_desc,
		D3D12_RESOURCE_STATE_COMMON,
		&Clear_value,
		IID_PPV_ARGS(DepthStencilResource.GetAddressOf()));
	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3dDevice->CreateDepthStencilView(DepthStencilResource.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	//ͬ����Դ��״̬����Դ�ı���ͬ����ʽ
	CD3DX12_RESOURCE_BARRIER RESOURCE_BARRIER = 
	CD3DX12_RESOURCE_BARRIER::Transition(
	DepthStencilResource.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GraphicsCommandList->ResourceBarrier(1,
		&RESOURCE_BARRIER);
	GraphicsCommandList->Close();
	//�ύ����
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��Щ�Ḳ��ԭ��windows����
	//�����ӿڳߴ�
	ViewportInfo.TopLeftX = 0;
	ViewportInfo.TopLeftY = 0;
	ViewportInfo.Width = FEngineRenderConfig::Get().ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::Get().ScreenHight;
	ViewportInfo.MinDepth = 0.f;
	ViewportInfo.MaxDepth = 1.f;

	//����
	ViewportRect.left = 0;
	ViewportRect.top = 0;
	ViewportRect.right = FEngineRenderConfig::Get().ScreenWidth;
	ViewportRect.bottom = FEngineRenderConfig::Get().ScreenHight;

	//CPU��GPU��Ⱦ��
	WaitGPUCommandQueueComplete();

}

void FWindowsEngine::Rendering(float InDeltaTime)
{
	//����¼����ص��ڴ棬Ϊ��һ֡��׼��
	//DX12_DEBUG_MESSAGE(CommandAllocator->Reset());
	if(IRenderingIntface::RenderingIntface.size()>0)
	{	
		//���������������PSOÿ��Mesh���涼��һ�������������
		for (auto& Tmp : IRenderingIntface::RenderingIntface)
		{
			Tmp->PreDraw(InDeltaTime);
		}
	}
	else
	{
		DX12_DEBUG_MESSAGE(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));
	}
	/////////////////////////////////////////////////////////
	//ָ���ĸ���Դ ת����״̬
	//֪ͨ������������Ҫͬ������Դ�Ķ�η��ʡ�
	//-------------������������------------------//
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = 
	CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffResource(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//1:�ύ����Դ����������
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);
	//--------------������������--------------------//


	//��Ҫÿִ֡��
	//�󶨾��ο�
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);	

	//�������
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(), DirectX::Colors::Black, 0,nullptr);
	//���ģ��/��Ȼ���
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0,nullptr);

	//����ĺϲ��׶�
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(
		1, //ָ���󶨵�����
		&SwapBufferView,//ָ����Ⱦ��Ŀ��
		true,//�������ڴ沼��
		&DepthStencilView//ָ�����
	);
	
	//��Ⱦ��������
	IRenderingIntface::Rendering(InDeltaTime);
	///////////////////////////////////////////////
	//-------------������������------------------//
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = 
	CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//1:�ύ����Դ����������
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
	//--------------������������--------------------//

	//¼����� һ��8������
	DX12_DEBUG_MESSAGE(GraphicsCommandList->Close());

	//�ύ����
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��������buff������
	DX12_DEBUG_MESSAGE(DXGISwapChain->Present(0,/*��ֱͬ��*/  0));
	//0����1
	CurrentSwapBuffIndex = (++CurrentSwapBuffIndex) % FEngineRenderConfig::Get().SwapChainCount;
	//CPU��GPU��Ⱦ��
	WaitGPUCommandQueueComplete();
}

ID3D12Resource* FWindowsEngine::GetCurrentSwapBuffResource() const
{
	return SwapChainResource[CurrentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),
		CurrentSwapBuffIndex, RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT FWindowsEngine::GetDXGISampleCount() const
{
 return bMSAAEnable ? 4 : 1;
} 

UINT FWindowsEngine::GetDXGISampleQuality() const
{
	return bMSAAEnable ? M4QualityLevels - 1 : 0;
}

DXGI_FORMAT FWindowsEngine::GetBackBufferFormat() const
{
	return BackBufferFormat;
}

DXGI_FORMAT FWindowsEngine::GetDepthStencilFormat() const
{
	return DepthStencilFormat;
}

void FWindowsEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;
	//��GUP�����µĸ���� �ȴ�GPU�������ź�
	DX12_DEBUG_MESSAGE(CommandQueue->Signal(D3dFence.Get(), CurrentFenceIndex));

	if (D3dFence->GetCompletedValue() < CurrentFenceIndex)
	{
		//�������һ���¼��ں˶���,�����ظ��ں˶���ľ��.
		//SECURITY_ATTRIBUTES
		//CREATE_EVENT_INITIAL_SET  0x00000002
		//CREATE_EVENT_MANUAL_RESET 0x00000001
		//ResetEvents ��������˵�һ��������������Ҫ��ResetEvents��������ͷ��¼�
		HANDLE EventEX = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

		//GPU��ɺ��֪ͨ���ǵ�Handle
		DX12_DEBUG_MESSAGE(D3dFence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		//�ȴ�GPU,ע�⣺���ַ�ʽ���������̣߳��ǳ���Ч��������ù���ķ�ʽȥ����
		WaitForSingleObject(EventEX, INFINITE);
		CloseHandle(EventEX);
		CurrentFenceIndex = 0;
	}
}

#endif

