#include "Object.h"

UObject::UObject()
{
	ObjGuid=FGuid::NewGuid();
}

UObject::~UObject()
{

}

void UObject::BeginPlay()
{

}
void UObject::DestroyPlay()
{

}
