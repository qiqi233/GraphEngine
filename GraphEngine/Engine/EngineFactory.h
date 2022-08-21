#pragma once
#include "Core/Engine.h"

class FEngineFactory
{
public:
	FEngineFactory()=default;
	static IEngine* CreateEngine();
};