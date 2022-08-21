#pragma once 
#include "ICommandParameters.h"

class IEngine
{
public:
 virtual int PreInit(ICommandParameters Parameters)=0;
 virtual int Init(ICommandParameters Parameters) = 0;
 virtual int PostInit() = 0;
 virtual void Tick(float DeltaTime) = 0;

 virtual int PreExit() = 0;
 virtual int Exit() = 0;
 virtual int PostExit() = 0;


};