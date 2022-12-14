#pragma once
#include "simple_library.h"
#include "DebugInfo/EngineLog.h"
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"dxgi.lib")

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <vector>
#include <windef.h>
#include "DirectX/d3dx12.h"
#include <DirectXMath.h>
#include "Core/Misc/PlatformMaro.h"
#include "DebugInfo/EngineDebug.h"
#include "DebugInfo/EngineLog.h"
#include <Rotator.h>
#include <Matrix.h>
#include <Vector.h>
using namespace UEMath;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class IEngine* Engine;
#ifndef  DirectX12Engine
#define  DirectX12Engine
#endif // ! DirectX12Engine

#if defined(_WIN64)||defined(_WIN32)
class FWindowsEngine;
using FPlatformEngine=FWindowsEngine;
extern FPlatformEngine* GetEngine();
#endif