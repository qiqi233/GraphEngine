#pragma once
#include "CoreMinimal.h"
#include "Misc/Guid.h"


class UObject
{
public:
	UObject();
	virtual ~UObject();
public:
	virtual void BeginPlay();
	virtual void DestroyPlay();
	FORCEINLINE FGuid GetObjGuid()const {return ObjGuid;}
	FString GetGuidToString(){return ObjGuid.ToString();}
private:
	FGuid  ObjGuid;
};

