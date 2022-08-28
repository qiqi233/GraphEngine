#include "Actor.h"
#include "Component/ActorComponent.h"
#include "World.h"
#include "Component/InputComponent.h"
#include "Component/TransformComponent.h"

AActor::AActor()
	:TransformationComponent(nullptr)
	, bIsEnableTick(true)
	, bMarkDelete(false)
{
	
	TransformationComponent = ConstructComponent<UTransformComponent>(this);
}

AActor::~AActor()
{

}

void AActor::SetPosition(const XMFLOAT3& InNewPosition)
{
	if(TransformationComponent)
	{
		TransformationComponent->SetPosition(InNewPosition);
	}
}

void AActor::SetRotation(const FRotator& InNewRotation)
{
	if (TransformationComponent)
	{
		TransformationComponent->SetRotation(InNewRotation);
	}
}

void AActor::SetScale(const fvector_3d& InNewScale)
{
	if (TransformationComponent)
	{
		TransformationComponent->SetScale(InNewScale);
	}
}

void AActor::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	if (TransformationComponent)
	{
		TransformationComponent->SetForwardVector(InForwardVector);
	}
}

void AActor::SetRightVector(const XMFLOAT3& InRightVector)
{
	if (TransformationComponent)
	{
		TransformationComponent->SetRightVector(InRightVector);
	}
}

void AActor::SetUPVector(const XMFLOAT3& InUPVector)
{
	if (TransformationComponent)
	{
		TransformationComponent->SetUPVector(InUPVector);
	}
}

DirectX::XMFLOAT3 AActor::GetPosition()
{
	if (TransformationComponent)
	{
		return TransformationComponent->GetPosition();
	}
	return DirectX::XMFLOAT3();
}

FRotator AActor::GetRotation()
{
	if (TransformationComponent)
	{
		return TransformationComponent->GetRotation();
	}
	return FRotator::ZeroRotator;
}

fvector_3d AActor::GetScale() const
{
	if (TransformationComponent)
	{
		return TransformationComponent->GetScale();
	}
	return fvector_3d();
}

DirectX::XMFLOAT3 AActor::GetForwardVector()
{
	if (TransformationComponent)
	{
		return TransformationComponent->GetForwardVector();
	}
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 AActor::GetRightVector()
{
	if (TransformationComponent)
	{
		return TransformationComponent->GetRightVector();
	}
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 AActor::GetUPVector()
{
	if (TransformationComponent)
	{
		return TransformationComponent->GetUPVector();
	}
	return DirectX::XMFLOAT3();
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
