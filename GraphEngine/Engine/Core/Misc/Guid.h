#pragma once
#include <assert.h>
#include "PlatformMaro.h"

/**
 * Implements a globally unique identifier.
 */
struct FGuid
{
public:

	/** Default constructor. */
	FGuid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{ }

	/**
	 * Creates and initializes a new GUID from the specified components.
	 *
	 * @param InA The first component.
	 * @param InB The second component.
	 * @param InC The third component.
	 * @param InD The fourth component.
	 */
	FGuid(uint32 InA, uint32 InB, uint32 InC, uint32 InD)
		: A(InA), B(InB), C(InC), D(InD)
	{ }
public:

	/**
	 * Compares two GUIDs for equality.
	 *
	 * @param X The first GUID to compare.
	 * @param Y The second GUID to compare.
	 * @return true if the GUIDs are equal, false otherwise.
	 */
	friend bool operator==(const FGuid& X, const FGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}

	/**
	 * Compares two GUIDs for inequality.
	 *
	 * @param X The first GUID to compare.
	 * @param Y The second GUID to compare.
	 * @return true if the GUIDs are not equal, false otherwise.
	 */
	friend bool operator!=(const FGuid& X, const FGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}

	/**
	 * Compares two GUIDs.
	 *
	 * @param X The first GUID to compare.
	 * @param Y The second GUID to compare.
	 * @return true if the first GUID is less than the second one.
	 */
	friend bool operator<(const FGuid& X, const FGuid& Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
				((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
					((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
	}

	/**
	 * Provides access to the GUIDs components.
	 *
	 * @param Index The index of the component to return (0...3).
	 * @return The component.
	 */
	uint32& operator[](uint32 Index)
	{
		assert(Index >= 0);
		assert(Index < 4);

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	/**
	 * Provides read-only access to the GUIDs components.
	 *
	 * @param Index The index of the component to return (0...3).
	 * @return The component.
	 */
	const uint32& operator[](uint32 Index) const
	{
		assert(Index >= 0);
		assert(Index < 4);

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}
	/**
	* Guid default string conversion.
	*/
	friend FString LexToString(FGuid Value)
	{
		return Value.ToString();
	}

public:


	/**
	*使GUID无效。
	*/
	void Invalidate()
	{
		A = B = C = D = 0;
	}

	/**
	*检查此GUID是否有效。
	*
	*所有组件都设置为零的GUID被视为无效。
	*
	*@return true如果有效，否则为false。
	*@见无效
	*/
	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}

	/**
	*将此GUID转换为其字符串表示形式。
	*
	*@返回字符串表示。
	*/
	FString ToString();

public:

public:

	/**
	 * Returns a new GUID.
	 *
	 * @return A new GUID.
	 */
	static FGuid NewGuid();
public:

	/** Holds the first component. */
	uint32 A;

	/** Holds the second component. */
	uint32 B;

	/** Holds the third component. */
	uint32 C;

	/** Holds the fourth component. */
	uint32 D;
};