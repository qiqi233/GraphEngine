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

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class IEngine* Engine;

#ifndef  DirectX12Engine
#define  DirectX12Engine
#endif // ! DirectX12Engine
