#include "Actor.h"
#include "Component/ActorComponent.h"
#include "World.h"
#include "Component/InputComponent.h"
#include "Component/TransformComponent.h"

AActor::AActor()
	:TransformationComponent(nullptr)
	, InputComponent(nullptr)
	, bIsEnableTick(true)
	, bMarkDelete(false)
{
	InputComponent = ConstructComponent<UInputComponent>(this);
	TransformationComponent = ConstructComponent<UTransformComponent>(this);
}

AActor::~AActor()
{

}

void AActor::SetActorLocation(const XMFLOAT3& InLoc)
{

}

void AActor::SetActorRotation(const XMFLOAT3& InRot)
{

}

void AActor::SetActorScale3D(const XMFLOAT3& InScale)
{

}

void AActor::Init(UWorld* InWorld)
{
	OuterWorld=InWorld;
}

void AActor::BeginPlay()
{
	
}

void AActor::Tick(float DeltaTime)
{
	for(auto It=ResigteredCompMap.begin(); It !=ResigteredCompMap.end();++It)
	{
		if((*It).second->GetIsTick())
			(*It).second->TickComponent(DeltaTime);
	}
}

void AActor::EndPlay()
{

}

void AActor::DestroyPlay()
{
	for (auto It = ResigteredCompMap.begin(); It != ResigteredCompMap.end(); ++It)
	{
		It->second->Destroy();
		delete It->second;
	}
	ResigteredCompMap.clear();
	std::unordered_map<FString, UActorComponent*> tmpmap;
	ResigteredCompMap.swap(tmpmap);
}

void AActor::DestroyNow()
{
	GetWorld()->DestroyActor(this);
}

void AActor::MarkPendingKill()
{
	bMarkDelete = true;
}

void AActor::SetTick(bool NewEnableTick)
{
	bIsEnableTick=NewEnableTick;
}

bool AActor::IsAtWorld(UWorld* InWorld)
{
	return InWorld==OuterWorld;
}

UWorld* AActor::GetWorld() const
{
	return  OuterWorld;
}

void AActor::ResigterComponent(UActorComponent* InComp)
{
	if(!InComp)
		return;
	auto It= ResigteredCompMap.find(InComp->GetGuidToString());
	if(It!=ResigteredCompMap.end())
	{
		GE_LOG(Warning,"this component is resgitered!");
		return;
	}
	ResigteredCompMap[InComp->GetGuidToString()]=InComp;
	InComp->InitComponent();
}
