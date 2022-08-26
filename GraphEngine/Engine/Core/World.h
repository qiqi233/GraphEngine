#pragma once
#include "CoreMinimal.h"
#include "Object.h"
#include "vector"

class ACamera;
class AActor;
class UWorld :public UObject
{
public:
	UWorld();
	virtual ~UWorld();

	void InitWorld();
	void TickWorld(float DeltaTime);
	void DestroyWorld();
	void DestroyActor(AActor* InA);
	template<class T>
	T* SpawnActor()
	{
		T* TempActor= NewObject<T>();
		TempActor->Init(this);
		TempActor->BeginPlay();
		return TempActor;
	}
	
private:
	std::unordered_map<FString,AActor*> Actors;
	ACamera* MainCamera;
};