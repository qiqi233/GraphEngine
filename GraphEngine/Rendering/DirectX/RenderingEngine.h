#pragma once
#include <Platform/Windows/GameTimer.h>

class IRenderEngine
{
public:
	virtual void Rendering(float InDeltaTime)=0;
	virtual void Update(const GameTimer& gt)=0;
	IRenderEngine();
	virtual ~IRenderEngine();
};