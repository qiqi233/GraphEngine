#include "World.h"
#include "Camera.h"
#include "Actor.h"

UWorld::UWorld()
{
	
}

UWorld::~UWorld()
{

}

void UWorld::InitWorld()
{
	MainCamera = SpawnActor<ACamera>();
	Actors[MainCamera->GetGuidToString()]= MainCamera;
}

void UWorld::TickWorld(float DeltaTime)
{
	for (auto It = Actors.begin(); It != Actors.end();)
	{
		if ((*It).second->bMarkDelete)
		{
			It->second->DestroyNow();
		}
		else
		{
			if (It->second->GetIsTick())
				It->second->Tick(DeltaTime);
			++It;
		}

	}
}

void UWorld::DestroyWorld()
{
	for(std::unordered_map<FString, AActor*>::iterator It=Actors.begin();It!=Actors.end();++It)
	{
		It->second->EndPlay();
		It->second->DestroyPlay();
		delete It->second;
	}
	Actors.clear();
	std::unordered_map<FString, AActor*> tmpAs;
	Actors.swap(tmpAs);
}

void UWorld::DestroyActor(AActor* InA)
{
	if(!InA)
		return;
	if(!InA->IsAtWorld(this))
		return;
	auto It= Actors.find(InA->GetGuidToString());

	Actors.erase(InA->GetGuidToString());

	InA->EndPlay();
	InA->DestroyPlay();
	delete InA;
	InA=nullptr;
}

