#include "Camera.h"
#include "Component/InputComponent.h"
#include "Component/TransformComponent.h"

ACamera::ACamera()
{
	InputComponent=ConstructComponent<UInputComponent>();
	TransformationComponent = ConstructComponent<UTransformComponent>();
}

ACamera::~ACamera()
{

}

void ACamera::BeginPlay()
{
	InputComponent->GetInputEventDelegate().Bind(this,&ACamera::UpdateInputEvent);
}

void ACamera::Tick(float DeltaTime)
{

}

void ACamera::EndPlay()
{

}

void ACamera::DestroyPlay()
{

}

void ACamera::UpdateInputEvent(const FInputKey& InputKey)
{

}

