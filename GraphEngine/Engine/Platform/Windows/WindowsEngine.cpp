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

