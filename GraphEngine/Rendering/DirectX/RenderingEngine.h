#pragma once

class IRenderEngine
{
public:
	virtual void Rendering(float InDeltaTime)=0;
	IRenderEngine();
	virtual ~IRenderEngine();
};