#pragma once
#include "EngineMinimal.h"
#include "Core/Engine.h"
#include <vector>
#include "simple_core_minimal/simple_c_guid/simple_guid.h"

#if defined(_WIN32) ||defined(_WIN64) 
#include "Platform/Windows/WindowsEngine.h"
#endif
#include "Core/Misc/Guid.h"
//äÖÈ¾½Ó¿Ú
class IRenderingIntface
{
	friend class FWindowsEngine;
public:
	IRenderingIntface();
	virtual ~IRenderingIntface();

	bool operator==(const IRenderingIntface& InOther);
protected:
	virtual void Init(){};
	virtual void PreDraw(float InDeltaTime) = 0;
	virtual void Update(float InDeltaTime) = 0;
	virtual void Draw(float InDeltaTime)=0;
protected:
	FGuid GetGuid();
public:
	FGuid RenderGuid;
};

