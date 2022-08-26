#include "Camera.h"
#include "Component/InputComponent.h"
#include "Component/TransformComponent.h"

ACamera::ACamera()
{
	
}

ACamera::~ACamera()
{

}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	InputComponent->GetInputDownEvent().Bind(this,&ACamera::CaptureInputDownEvent);
	InputComponent->GetInputUpEvent().Bind(this, &ACamera::CaptureUpdateInputUpEvent);
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACamera::EndPlay()
{
	Super::EndPlay();
}

void ACamera::DestroyPlay()
{
	Super::DestroyPlay();
}

void ACamera::CaptureInputDownEvent(FInputKey InputKey)
{
	if(InputKey == EKeys::W)
	{
		OutputDebugString(L"down W\n");
	}
	if (InputKey == EKeys::S)
	{
		OutputDebugString(L"down S\n");
	}
	if (InputKey == EKeys::A)
	{
		OutputDebugString(L"down A\n");
	}
	if (InputKey == EKeys::D)
	{
		OutputDebugString(L"down D\n");
	}

}

void ACamera::CaptureUpdateInputUpEvent(FInputKey InputKey)
{
	if (InputKey == EKeys::W)
	{
		OutputDebugString(L"up W\n");
	}
	if (InputKey == EKeys::S)
	{
		OutputDebugString(L"up S\n");
	}
	if (InputKey == EKeys::A)
	{
		OutputDebugString(L"up A\n");
	}
	if (InputKey == EKeys::D)
	{
		OutputDebugString(L"up D\n");
	}
}

