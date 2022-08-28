#pragma once
#include "EngineMinimal.h"
#include "Object.h"

class UWorld;
class UActorComponent;
class UTransformComponent;
class UInputComponent;
class AActor:public UObject
{
	friend class UWorld;
public:
	using Super = AActor;
	AActor();
	virtual ~AActor();
public:
	void SetPosition(const XMFLOAT3& InNewPosition);
	void SetRotation(const FRotator& InNewRotation);
	void SetScale(const fvector_3d& InNewScale);

	void SetForwardVector(const XMFLOAT3& InForwardVector);
	void SetRightVector(const XMFLOAT3& InRightVector);
	void SetUPVector(const XMFLOAT3& InUPVector);
public:
	XMFLOAT3 GetPosition();
	FRotator GetRotation();
	fvector_3d GetScale() const;

	XMFLOAT3 GetForwardVector();
	XMFLOAT3 GetRightVector();
	XMFLOAT3 GetUPVector();
public:
	virtual void Init(UWorld* InWorld);
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void EndPlay();
	virtual void DestroyPlay();

	void DestroyNow();//立即删除
	void MarkPendingKill();//下一帧删除
	void SetTick(bool NewEnableTick);

	bool IsAtWorld(UWorld* InWorld);
	FORCEINLINE bool GetIsTick()const { return bIsEnableTick; }
	UWorld* GetWorld()const;

	void ResigterComponent(UActorComponent* InComp);
protected:
	UPROPROTY()
	UTransformComponent* TransformationComponent;
private:
	std::unordered_map<FString,UActorComponent*> ResigteredCompMap;
	bool bIsEnableTick;
	UWorld*   OuterWorld;
	bool bMarkDelete;//标记为删除
};

