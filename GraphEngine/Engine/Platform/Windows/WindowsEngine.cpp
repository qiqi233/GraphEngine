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
	//日志系统初始化
	const char LogPath[] = "../Log";
	init_log_system(LogPath);
	GE_LOG(Log, "Engine pre init success!");
	//处理命令

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
		//构建Mesh
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
			//PM_NOREMOVE 消息不从队列里除掉。
			//PM_REMOVE   消息从队列里除掉。
			//PM_NOYIELD  此标志使系统不释放等待调用程序空闲的线程
			//PM_QS_INPUT 处理鼠标和键盘消息。
			//PM_QS_PAINT 处理画图消息。
			//PM_QS_POSTMESSAGE 处理所有被寄送的消息，包括计时器和热键。
			//PM_QS_SENDMESSAGE 处理所有发送消息。
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
				break;
			TranslateMessage(&Msg);//翻译消息的
			DispatchMessage(&Msg);//分发消息的
		}
		else
		{
			//Sleep(static_cast<unsigned long>(DeltaTime * 1000));
			//清除上一帧
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
	//注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;//是否需要额外空间
	WindowsClass.cbWndExtra = 0;//是否需要额外内存
	WindowsClass.hbrBackground = nullptr;//如果有设置哪就是GDI擦除
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//设置一个箭头光标
	WindowsClass.hIcon = nullptr; //应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = NULL;//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance; //窗口实例
	WindowsClass.lpszClassName = L"GraphEngine";//窗口名字
	WindowsClass.lpszMenuName = nullptr;//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//怎么绘制窗口 垂直和水平重绘
	WindowsClass.lpfnWndProc = EngineWindowProc;//消息处理函数

	//注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		GE_LOG(Error, "Register windows class fail.");
		MessageBox(NULL, L"Register windows class fail.", L"Error", MB_OK);
	}

	//@rect 适口
	//WS_OVERLAPPEDWINDOW 适口风格
	//NULL 没有菜单
	RECT Rect{ 0,0,FEngineRenderConfig::Get().ScreenWidth,FEngineRenderConfig::Get().ScreenHight };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);


	const int x = GetSystemMetrics(SM_CXSCREEN);
	const int y = GetSystemMetrics(SM_CYSCREEN);

	MainHwnd = CreateWindowEx(
		NULL,//窗口额外的风格
		L"GraphEngine", // 窗口名称
		L"Graph Engine",//会显示在窗口的标题栏上去
		WS_OVERLAPPEDWINDOW, //窗口风格
		(x - FEngineRenderConfig::Get().ScreenWidth) / 2, (y - FEngineRenderConfig::Get().ScreenHight) / 2,//窗口的坐标
		FEngineRenderConfig::Get().ScreenWidth, FEngineRenderConfig::Get().ScreenHight,//
		NULL, //副窗口句柄
		nullptr, //菜单句柄
		InParameters.HInstance,//窗口实例
		NULL);//
	if (!MainHwnd)
	{
		GE_LOG(Error, "CreateWindow Failed..");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//显示窗口
	ShowWindow(MainHwnd, SW_SHOW);

	//窗口是脏的，刷新一下
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
	//S_OK				0x000000	成功创建
	//E_UNEXPECTED		0x8000FFFF	意外的失败
	//E_NOTIMPL			0x80004001	未实现
	//E_OUTOFMEMORY		0x800700E	未能分配所需的内存
	//E_INVALIDARG		0x8007005	一个或多个参数无效
	//etc...  #include <winerror.h>
	//创建DXGI工厂组件，用于创建DirectX图形基础结构(DXGI)对象
	CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory));
	//NULL::表示使用默认的显示器
	Result = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(Result))
	{
		//warp 高级光栅化平台（软件模拟）
		ComPtr<IDXGIAdapter> warpAdpter;
		DX12_DEBUG_MESSAGE(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdpter)));
		DX12_DEBUG_MESSAGE(D3D12CreateDevice(warpAdpter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&D3dDevice)));
	}
	//围栏是为了平衡CPU与GPU
	Result = D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(D3dFence.GetAddressOf()));
	//创建命令队列
	D3D12_COMMAND_QUEUE_DESC QueueDesc{};//队列的描述
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;


	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL = 0,正常优先级。
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH = 100,高优先级。
	//D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME = 10000//全局实时优先级。
	//QueueDesc.Priority= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	Result = D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue));
	//创建命令分配器
	//D3D12_COMMAND_LIST_TYPE_DIRECT = 0,GPU可以直接执行的命令
	//D3D12_COMMAND_LIST_TYPE_BUNDLE = 1,
	//D3D12_COMMAND_LIST_TYPE_COMPUTE = 2,指定用于计算的命令缓冲区。
	//D3D12_COMMAND_LIST_TYPE_COPY = 3,指定用于复制的命令缓冲区。
	//D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE = 4,指定用于视频解码的命令缓冲区。
	//D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS = 5,指定用于视频处理的命令缓冲区。
	//D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE = 4,指定用于视频编码的命令缓冲区。
	Result = D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()));

	//创建命令列表
	Result = D3dDevice->CreateCommandList(
		0, //默认单个GPU
		D3D12_COMMAND_LIST_TYPE_DIRECT,//直接类型
		CommandAllocator.Get(),//将CommandList关联到Allocator
		nullptr,//ID3D12PipelineState
		IID_PPV_ARGS(&GraphicsCommandList));
	//重置命令列表，先关闭它
	GraphicsCommandList->Close();

	if (FAILED(Result))
	{
		//命令队列创建失败
		GE_LOG(Error, "GraphicsCommandList Create Failed[%d]", (int)Result);
	}
	//////////////////////检测和开启多重采样///////////////////////
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS  QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;//采样次数
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;
	Result = D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,//采样的质量级别
		&QualityLevels,
		sizeof(QualityLevels)
	);
	//if(SUCCEEDED(Result))
	//{
	//	bMSAAEnable=true;
	//}
	M4QualityLevels = QualityLevels.NumQualityLevels;//采样级别
	///////////////////////////////////创建交换连///////////////////////////////////
	DXGISwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::Get().ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::Get().ScreenHight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::Get().RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::Get().SwapChainCount;
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//纹理格式
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或资源作为输出渲染目标。
	SwapChainDesc.OutputWindow = MainHwnd;//指定windows句柄
	SwapChainDesc.Windowed = true;//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//IDXGISwapChain::ResizeTarget

	//多重采样设置
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();

	Result = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, DXGISwapChain.GetAddressOf());
	if (FAILED(Result))  //    如果发生了错误 
	{

	}
	//////////////////初始化资源描述符///////////////////////////////
	//渲染目标的视图资源
	D3D12_DESCRIPTOR_HEAP_DESC RTVDesc;
	RTVDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDesc.NodeMask = 0;
	RTVDesc.NumDescriptors = FEngineRenderConfig::Get().SwapChainCount;
	RTVDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	DX12_DEBUG_MESSAGE(D3dDevice->CreateDescriptorHeap(&RTVDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//深度/模板视图资源
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
	//CPU等GPU渲染完
	WaitGPUCommandQueueComplete();
	//重置命令列表
	DX12_DEBUG_MESSAGE(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));


	for (UINT i = 0; i < FEngineRenderConfig::Get().SwapChainCount; ++i)
	{
		SwapChainResource[i].Reset();
	}
	DepthStencilResource.Reset();

	//更改交换链的后台缓冲区大小、格式和缓冲区数量。这应该在应用程序窗口大小调整时调用。
	DXGISwapChain->ResizeBuffers(
		FEngineRenderConfig::Get().SwapChainCount,
		FEngineRenderConfig::Get().ScreenWidth,
		FEngineRenderConfig::Get().ScreenHight, BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	//获取表示堆开始的 CPU 描述符句柄。
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (UINT i = 0; i < FEngineRenderConfig::Get().SwapChainCount; ++i)
	{
		DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainResource[i]));
		//nullptr:填写nullptr表示使用我们初始化设置的默认值
		D3dDevice->CreateRenderTargetView(SwapChainResource[i].Get(), nullptr, RTVHandle);
		//移动指针
		RTVHandle.Offset(1, RTVDescriptorSize);
	}
	GE_LOG(Log, "WindowsEngine PostInit() initialization complete.");

	//创建需要提交的资源
	//D3D12_HEAP_TYPE_DEFAULT 默认堆 只有GPU会读写这个深度缓冲区
	//D3D12_HEAP_TYPE_UPLOAD 上传堆 
	//D3D12_HEAP_TYPE_READBACK 需要CPU读取的，称之为回读堆
	//资源描述
	D3D12_RESOURCE_DESC Resource_desc;
	Resource_desc.Width = FEngineRenderConfig::Get().ScreenWidth;
	Resource_desc.Height = FEngineRenderConfig::Get().ScreenHight;
	Resource_desc.Alignment = 0;//对齐方式
	Resource_desc.MipLevels = 1;
	Resource_desc.DepthOrArraySize = 1;
	Resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//资源的维度
	Resource_desc.SampleDesc.Count = GetDXGISampleCount();
	Resource_desc.SampleDesc.Quality = GetDXGISampleQuality();
	Resource_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	Resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//允许你去使用一个深度模板
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

	//同步资源的状态，资源的表达，跟同步方式
	CD3DX12_RESOURCE_BARRIER RESOURCE_BARRIER = 
	CD3DX12_RESOURCE_BARRIER::Transition(
	DepthStencilResource.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GraphicsCommandList->ResourceBarrier(1,
		&RESOURCE_BARRIER);
	GraphicsCommandList->Close();
	//提交命令
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//这些会覆盖原先windows画布
	//描述视口尺寸
	ViewportInfo.TopLeftX = 0;
	ViewportInfo.TopLeftY = 0;
	ViewportInfo.Width = FEngineRenderConfig::Get().ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::Get().ScreenHight;
	ViewportInfo.MinDepth = 0.f;
	ViewportInfo.MaxDepth = 1.f;

	//矩形
	ViewportRect.left = 0;
	ViewportRect.top = 0;
	ViewportRect.right = FEngineRenderConfig::Get().ScreenWidth;
	ViewportRect.bottom = FEngineRenderConfig::Get().ScreenHight;

	//CPU等GPU渲染完
	WaitGPUCommandQueueComplete();

}

void FWindowsEngine::Rendering(float InDeltaTime)
{
	//重置录制相关的内存，为下一帧做准备
	//DX12_DEBUG_MESSAGE(CommandAllocator->Reset());
	if(IRenderingIntface::RenderingIntface.size()>0)
	{	
		//重置命令分配器，PSO每个Mesh里面都有一个，这个有问题
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
	//指向哪个资源 转换其状态
	//通知驱动程序它需要同步对资源的多次访问。
	//-------------↓↓↓↓↓↓------------------//
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = 
	CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffResource(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//1:提交的资源描述的数量
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);
	//--------------↑↑↑↑↑↑--------------------//


	//需要每帧执行
	//绑定矩形框
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);	

	//清除画布
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(), DirectX::Colors::Black, 0,nullptr);
	//清除模板/深度缓存
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0,nullptr);

	//输出的合并阶段
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(
		1, //指定绑定的数据
		&SwapBufferView,//指定渲染的目标
		true,//连续的内存布局
		&DepthStencilView//指定深度
	);
	
	//渲染其他内容
	IRenderingIntface::Rendering(InDeltaTime);
	///////////////////////////////////////////////
	//-------------↓↓↓↓↓↓------------------//
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = 
	CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//1:提交的资源描述的数量
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
	//--------------↑↑↑↑↑↑--------------------//

	//录入完成 一共8个命令
	DX12_DEBUG_MESSAGE(GraphicsCommandList->Close());

	//提交命令
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//交换两个buff缓冲区
	DX12_DEBUG_MESSAGE(DXGISwapChain->Present(0,/*垂直同步*/  0));
	//0或者1
	CurrentSwapBuffIndex = (++CurrentSwapBuffIndex) % FEngineRenderConfig::Get().SwapChainCount;
	//CPU等GPU渲染完
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
	//向GUP设置新的隔离点 等待GPU处理玩信号
	DX12_DEBUG_MESSAGE(CommandQueue->Signal(D3dFence.Get(), CurrentFenceIndex));

	if (D3dFence->GetCompletedValue() < CurrentFenceIndex)
	{
		//创建或打开一个事件内核对象,并返回该内核对象的句柄.
		//SECURITY_ATTRIBUTES
		//CREATE_EVENT_INITIAL_SET  0x00000002
		//CREATE_EVENT_MANUAL_RESET 0x00000001
		//ResetEvents 如果设置了第一个参数（？）需要用ResetEvents这个函数释放事件
		HANDLE EventEX = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

		//GPU完成后会通知我们的Handle
		DX12_DEBUG_MESSAGE(D3dFence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		//等待GPU,注意：这种方式会阻塞主线程，非常低效。后面会用挂起的方式去做。
		WaitForSingleObject(EventEX, INFINITE);
		CloseHandle(EventEX);
		CurrentFenceIndex = 0;
	}
}

#endif

