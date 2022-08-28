#include "Camera.h"
#include "Component/InputComponent.h"
#include "Component/TransformComponent.h"
#include "EngineMinimal.h"
#include "Platform/Windows/WindowsEngine.h"
#include <DirectXMathConvert.inl>
#include <DirectXMathVector.inl>

ACamera::ACamera()
	: InputComponent(nullptr)
	, CameraType(ECameraType::Edit)
{
	InputComponent = ConstructComponent<UInputComponent>(this);
}

ACamera::~ACamera()
{

}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	ViewportInit();
	//Actions
	InputComponent->GetInputDownEvent().Bind(this,&ACamera::CaptureInputDownEvent);
	InputComponent->GetInputUpEvent().Bind(this, &ACamera::CaptureUpdateInputUpEvent);
	InputComponent->GetMouseDownEvent().Bind(this, &ACamera::OnMouseButtonDown);
	InputComponent->GetMouseUpEvent().Bind(this, &ACamera::OnMouseButtonUp);
	InputComponent->GetMouseWheelEvent().Bind(this, &ACamera::OnMouseWheel);
	InputComponent->GetMouseMoveEvent().Bind(this, &ACamera::OnMouseMove);

	//Axis
	InputComponent->BindAxis(EKeys::W,1.f,this, &ACamera::MoveForward);
	InputComponent->BindAxis(EKeys::S, -1.f, this, &ACamera::MoveForward);
	InputComponent->BindAxis(EKeys::A, -1.f, this, &ACamera::MoveRight);
	InputComponent->BindAxis(EKeys::D, 1.f, this, &ACamera::MoveRight);
}

void ACamera::Tick(float DeltaTime)
{
	TickDeltaTime=DeltaTime;
	Super::Tick(DeltaTime);
	BuildViewMatrix(DeltaTime);
}

void ACamera::EndPlay()
{
	Super::EndPlay();
}

void ACamera::DestroyPlay()
{
	Super::DestroyPlay();
}

void ACamera::RotatorAxis(XMFLOAT3 InAxis, float InV)
{
	XMVECTOR RotAxis=XMLoadFloat3(&InAxis);
	//拿到相机的方向
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UPVector = TransformationComponent->GetUPVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	//拿到关于RotAxis的旋转矩阵
	XMMATRIX RotationMatrix = XMMatrixRotationAxis(RotAxis,InV);

	//计算各个方向和按照Z轴旋转后的最终效果
	XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationMatrix));
	XMStoreFloat3(&TransformationComponent->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationMatrix));
	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationMatrix));

	if(TransformationComponent)
	{
		TransformationComponent->MarkRotationMofity();
	}
}

void ACamera::MoveFromAxis(XMFLOAT3 InAxis, float InValue)
{
	if (CameraType == ECameraType::Edit)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		XMVECTOR MovemAxis = XMLoadFloat3(&InAxis);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, MovemAxis, Position));
		TransformationComponent->SetPosition(AT3Position);
	}
}

void ACamera::MoveForward(float InValue)
{
	if (CameraType == ECameraType::Edit)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		XMFLOAT3 AT3ForwardVector = TransformationComponent->GetForwardVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue *MoveSensitivity* TickDeltaTime);
		XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));
		TransformationComponent->SetPosition(AT3Position);
	}
}

void ACamera::MoveRight(float InValue)
{
	if (CameraType == ECameraType::Edit)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		XMFLOAT3 AT3RightVector = TransformationComponent->GetRightVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue*MoveSensitivity * TickDeltaTime);
		XMVECTOR Right = XMLoadFloat3(&AT3RightVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		TransformationComponent->SetPosition(AT3Position);
	}
}

void ACamera::OnInputKeyDown(FInputKey InputKey)
{
	if (InputKey == EKeys::Q)
	{
		CameraType = ECameraType::Observer;
	}
	if (InputKey == EKeys::E)
	{
		CameraType = ECameraType::Edit;
	}
}

void ACamera::OnInputKeyUp(FInputKey InputKey)
{
	
}

void ACamera::OnMouseButtonDown(EMouseState InMouseState, int X, int Y)
{
	if(InMouseState==EMouseState::LButton)
	{
		bLeftMouseDownAction = true;
	}
	if (InMouseState == EMouseState::RButton)
	{
		bRightMouseDownAction = true;
	}
	if (InMouseState == EMouseState::MButton)
	{
		bMiddenMouseDownAction = true;
	}
	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetEngine()->GetMainHwnd());
}

void ACamera::OnMouseButtonUp(EMouseState InMouseState, int X, int Y)
{
	if (InMouseState == EMouseState::LButton)
	{
		bLeftMouseDownAction = false;
	}
	if (InMouseState == EMouseState::RButton)
	{
		bRightMouseDownAction = false;
	}
	if (InMouseState == EMouseState::MButton)
	{
		bMiddenMouseDownAction = false;
	}
	ReleaseCapture();
}

void ACamera::OnMouseMove(int X, int Y)
{
	if (bRightMouseDownAction)
	{
		float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * RotSensitivity* TickDeltaTime);
		float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * RotSensitivity * TickDeltaTime);

		switch (CameraType)
		{
		case ECameraType::Edit:
		{
			RotatorAxis(XMFLOAT3(0, 1, 0), XRadians);//Rot Yaw
			RotatorAxis(TransformationComponent->GetRightVector(), YRadians);//Rot Pitch
			//RotateAroundZAxis(XRadians);
			break;
		}
		case ECameraType::Observer:
		{
			A += (-XRadians);
			B += YRadians;

			A = math_libray::Clamp(A, 0.f, XM_2PI * 2.f);
			break;
		}
		}
	}
	if (bLeftMouseDownAction)
	{
		float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * MoveSensitivity *TickDeltaTime*5);
		float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * MoveSensitivity * TickDeltaTime * 5);

		MoveFromAxis(TransformationComponent->GetRightVector(),-XRadians);
		XMVECTOR TmpRightVector=XMLoadFloat3(&TransformationComponent->GetRightVector());
		XMFLOAT3 TmpUpFloat3(0,1,0);
		XMVECTOR TmpUp = XMLoadFloat3(&TmpUpFloat3);
		XMVECTOR TempForwad=XMVector3Cross(TmpRightVector, TmpUp);
		XMStoreFloat3(&TmpUpFloat3,TempForwad);
		MoveFromAxis(TmpUpFloat3, YRadians);
	}
	LastMousePosition.x = X;
	LastMousePosition.y = Y;
}

void ACamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CameraType == ECameraType::Observer)
	{
		Radius += (InDelta / 100.f);

		//限制在一定的范围内
		Radius = math_libray::Clamp(Radius, 7.f, 40.f);
	}
}

void ACamera::BuildViewMatrix(float DeltaTime)
{
	switch (CameraType)
	{
	case ECameraType::Edit:
	{
		//计算和矫正轴
		TransformationComponent->CorrectionVector();

		//算出按自身方向移动意图
		fvector_3d V3;
		TransformationComponent->GetCorrectionPosition(V3);

		//构建Viewmatrix
		XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
		XMFLOAT3 UPVector = TransformationComponent->GetUPVector();
		XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

		ViewMatrix = {
			RightVector.x,	UPVector.x,	ForwardVector.x,	0.f,
			RightVector.y,	UPVector.y,	ForwardVector.y,	0.f,
			RightVector.z,	UPVector.z,	ForwardVector.z,	0.f,
			V3.x,			V3.y,		V3.z,				1.f };

		break;
	}
	case ECameraType::Observer:
	{
		XMFLOAT3& CameraPos = TransformationComponent->GetPosition();

		CameraPos.x = Radius * sinf(B) * cosf(A);
		CameraPos.z = Radius * sinf(B) * sinf(A);
		CameraPos.y = Radius * cosf(B);

		XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f);
		XMVECTOR ViewTarget = XMVectorZero();
		XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

		XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
		XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

		break;
	}
	}
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

