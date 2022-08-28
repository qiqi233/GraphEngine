#pragma once
#include "MathHelper.h"

#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Math/Quat.h"

namespace UEMath
{

struct FTransform
{
public:
	FVector Translation = FVector::ZeroVector;
	FVector Scale3D = FVector::OneVector;	// not in use
	FQuat Rotation = FQuat::Identity;

public:
	FTransform() = default;

	FTransform(const FVector& translation) 
		: Translation(translation)
		, Rotation(0.f, 0.f, 0.f, 1.f) 
	{}

	FTransform(const FQuat& rotation)
		: Translation(0.f, 0.f, 0.f)
		, Rotation(rotation)
	{}

	FTransform(const FVector& translation, const FQuat& rotation)
		: Translation(translation)
		, Rotation(rotation)
	{}

	FTransform(const FTransform& transform)
		: Translation(transform.Translation)
		, Rotation(transform.Rotation)
	{}

	bool operator==(const FTransform& t) const
	{
		return (Translation == t.Translation) && (Rotation == t.Rotation);
	}

	bool operator!=(const FTransform& t) const
	{
		return (Translation != t.Translation) || (Rotation != t.Rotation);
	}

	FORCEINLINE explicit FTransform(const FMatrix& InMatrix)
	{
		FMatrix M = InMatrix;

		// Get the 3D scale from the matrix
		Scale3D = M.ExtractScaling();

		// If there is negative scaling going on, we handle that here
		if (InMatrix.Determinant() < 0.f)
		{
			// Assume it is along X and modify transform accordingly. 
			// It doesn't actually matter which axis we choose, the 'appearance' will be the same
			Scale3D.X *= -1.f;
			M.SetAxis(0, -M.GetScaledAxis(EAxis::X));
		}

		Rotation = FQuat(M);
		Translation = InMatrix.GetOrigin();

		// Normalize rotation
		Rotation.Normalize();
	}

	FORCEINLINE const FVector& GetTranslation() const
	{
		return Translation;
	}

	FORCEINLINE void SetTranslation(const FVector& pos)
	{
		Translation = pos;
	}

	FORCEINLINE const FQuat& GetRotation() const
	{
		return Rotation;
	}
	FORCEINLINE void SetRotation(const FQuat& quat)
	{
		Rotation = quat;
	}

	FORCEINLINE void Set(const FVector& V, const FQuat& Q)
	{
		Translation = V;
		Rotation = Q;
	}

	FORCEINLINE void NormalizeRotation()
	{
		Rotation.Normalize();
	}

	FORCEINLINE FTransform Multi(const FTransform& other) const
	{
		return Transform(other);
	}

	FORCEINLINE bool ContainsNan() const
	{
		// TODO: log nan info
		return Translation.ContainsNaN() && Rotation.ContainsNaN();
	}

	FORCEINLINE void SetIdentity()
	{
		Translation = FVector::ZeroVector;
		Rotation = FQuat::Identity;
	}

	static FORCEINLINE FTransform Identity()
	{
		FTransform Trans;
		Trans.Translation = FVector::ZeroVector;
		Trans.Rotation = FQuat::Identity;
		return Trans;
	}

	FORCEINLINE void SetByPosAndQuat(const FVector& position, const FQuat& rotation)
	{
		Translation = position;
		Rotation = rotation;
	}

	FORCEINLINE bool IsIdentity() const
	{
		return Translation.IsNearlyZero() && Rotation.IsIdentity();
	}

	FORCEINLINE FTransform GetNormalized() const
	{
		return FTransform(Translation, Rotation.GetNormalized());
	}

	// tranInv.Translation = transformInv(Zero)
	FORCEINLINE FTransform GetInverse() const
	{
		return FTransform(Rotation.UnrotateVector(-Translation), Rotation.Inverse());
	}

	FORCEINLINE FVector Rotate(const FVector& input) const
	{
		return Rotation.RotateVector(input);
	}

	FORCEINLINE FVector RotateInv(const FVector& input) const
	{
		return Rotation.UnrotateVector(input);
	}

	// Transform position
	FORCEINLINE FVector TransformPosition(const FVector& V) const
	{
		//ContainsNan();
		return Rotation.RotateVector(V) + Translation;
	}

	FORCEINLINE FVector TransformPositionNoScale(const FVector& V) const
	{
		//ContainsNan();
		return Rotation.RotateVector(V) + Translation;
	}

	FORCEINLINE FVector InverseTransformPosition(const FVector& V) const
	{
		//ContainsNan();
		return Rotation.UnrotateVector(V - Translation);
	}

	// Transform vector
	FORCEINLINE FVector TransformVector(const FVector& V) const
	{
		//ContainsNan();
		return Rotation.RotateVector(V);
	}

	FORCEINLINE FVector TransformVectorNoScale(const FVector& V) const
	{
		//ContainsNan();
		return Rotation.RotateVector(V);
	}

	FORCEINLINE FVector InverseTransformVector(const FVector& V) const
	{
		return Rotation.UnrotateVector(V);
	}

	FORCEINLINE FVector InverseTransformVectorNoScale(const FVector& V) const
	{
		return Rotation.UnrotateVector(V);
	}

	FORCEINLINE FTransform Transform(const FTransform& src) const
	{
		return FTransform(
			Rotation.RotateVector(src.Translation) + Translation,
			(Rotation * src.Rotation).GetNormalized());
	}

	//! Transform transform from parent (returns compound transform: first src, then this->inverse)  =>  inv(*this) * src
	FORCEINLINE FTransform InverseTransform(const FTransform& src) const
	{
		FQuat invRot = (Rotation.GetNormalized()).Inverse();
		return FTransform(invRot.RotateVector(src.Translation - Translation), (invRot * src.Rotation).GetNormalized());
	}

	FORCEINLINE static FTransform Slerp(const FTransform& left, const FTransform& right, const float t)
	{
		return FTransform(
			TLerp(left.Translation, right.Translation, t),
			FQuat::Slerp(left.Rotation, right.Rotation, t)
		);
	}
};
}// namespace UEMath