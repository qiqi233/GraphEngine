#pragma once

#include "EngineMinimal.h"
#include "GC/CodeRefMacro.h"
#include "Object.h"
#include "Core/Misc/PlatformMaro.h"
#include "Component/Input/InputType.h"
#include "Core/Misc/Delegate.h"
template<class T>
T* NewObject()
{
	return new T();
}