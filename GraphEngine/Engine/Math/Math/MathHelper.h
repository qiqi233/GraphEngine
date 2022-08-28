#pragma once
#include "Math/UnrealMathUtility.h"

namespace UEMath
{
	template <class T>
	FORCEINLINE T TLerp(const T& rValA, const T& rValB, const float fraction)
	{
		return rValA * (1.0f - fraction) + rValB * fraction;
	}
}