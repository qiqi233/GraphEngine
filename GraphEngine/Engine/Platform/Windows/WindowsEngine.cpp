#include "WindowsEngine.h"
#include "WindowsMessageProcessing.h"
#include "Config/EngineRenderConfig.h"
#include "Core/World.h"
#if defined(_WIN32)||defined(_WIN64)
FWindowsEngine::FWindowsEngine()
	:DX12RenderEngine(new FDX12RenderEngine())
{
	
}
FWindowsEngine::~FWindowsEngine()
{

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
	DX12RenderEngine->SetMianWindowsHandle(MainHwnd);
	if (!DX12RenderEngine->InitDirect3D())
	{
		GE_LOG(Error, "Init Direct3D Failed!");
		return 0;
	}
	DX12RenderEngine->PostInitDirect3D();
	GE_LOG(Log, "WindowsEngine Init() initialization complete.");

	MainWorld=NewObject<UWorld>();
	MainWorld->InitWorld();
	return 0;
}

int FWindowsEngine::PostInit()
{
	
	return 0;
}

void FWindowsEngine::Tick(float DeltaTime)
{
	MSG Msg{};
	mTimer.Reset();
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
			mTimer.Tick();
			CalculateFrameStats();
			if (!mAppPaused)
			{
				DX12RenderEngine->Update(mTimer);
				DX12RenderEngine->Rendering(mTimer.DeltaTime());
				MainWorld->TickWorld(mTimer.DeltaTime());
			}
			else
			{
				Sleep(100);
			}
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
	MainWorld->DestroyWorld();
	delete MainWorld;
	GE_LOG(Log, "WindowsEngine Exit() initialization complete.");
	return 0;
}

int FWindowsEngine::PostExit()
{
	GE_LOG(Log, "WindowsEngine PostExit() initialization complete.");
	return 0;
}


UWorld* FWindowsEngine::GetMainWorld() const
{
	return MainWorld;
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

void FWindowsEngine::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		wstring fpsStr = to_wstring(fps);
		wstring mspfStr = to_wstring(mspf);

		wstring windowText = mMainWndCaption +
			L"    fps: " + fpsStr +
			L"   mspf: " + mspfStr;

		SetWindowText(MainHwnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

#endif

