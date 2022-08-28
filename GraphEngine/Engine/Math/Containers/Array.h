#pragma once

#include "CoreTypes.h"
#include <vector>

namespace UEMath
{
	template<class T>
	class TArray : public std::vector<T>
	{
	public:
		TArray()
			: std::vector<T>() 
		{}

		TArray(TArray<T>&& Other)
			: std::vector<T>(std::move(Other)) {}
		TArray(const TArray<T>& Other)
			: std::vector<T>(Other) {}
		TArray(std::initializer_list<T> l)
			: std::vector<T>(l) {}

		FORCEINLINE void operator=(TArray<T>&& Other)
		{
			std::vector<T>::operator=(std::move(Other));
		}

		FORCEINLINE void SetNum(const int32 Size)
		{
			std::vector<T>::resize(Size);
		}

		FORCEINLINE int32 Num() const
		{
			return static_cast<int32>(std::vector<T>::size());
		}

		FORCEINLINE void Add(const T& Elem)
		{
			std::vector<T>::push_back(Elem);
		}

		FORCEINLINE void PushBack(const T& Elem)
		{
			std::vector<T>::push_back(Elem);
		}

		FORCEINLINE void Empty(int32 Slack = 0)
		{
			std::vector<T>::resize(0);
			std::vector<T>::reserve(Slack);
		}

		FORCEINLINE void Reserve(int32 NewSize = 0)
		{
			std::vector<T>::reserve(NewSize);
		}

		FORCEINLINE void Reset(int32 NewSize = 0)
		{
			Empty(NewSize);
		}

		FORCEINLINE int Find(const T& Item) const
		{
			const int DataNum = Num();
			for (int Index = 0; Index < DataNum; ++Index)
			{
				if (std::vector<T>::at(Index) == Item)
					return Index;
			}
			return -1;
		}

		FORCEINLINE bool Find(const T& Item, int& Index) const
		{
			const int DataNum = Num();
			for (Index = 0; Index < DataNum; ++Index)
			{
				if (std::vector<T>::at(Index) == Item)
					return true;
			}
			Index = -1;
			return false;
		}

		void Append(const TArray<T>& Source)
		{
			UE_ASSERT((void*)this != (void*)&Source);

			int SourceCount = Source.Num();

			// Do nothing if the source is empty.
			if (!SourceCount)
			{
				return;
			}

			// Allocate memory for the new elements.
			const int OldNum = Num();
			std::vector<T>::reserve(OldNum + SourceCount);
			for (int i = 0; i < SourceCount; ++i)
			{
				Add(Source[i]);
			}
		}

		FORCEINLINE int AddRange(const TArray<T>& ItemArray)
		{
			const int OldNum = Num();
			for (int i = 0; i < ItemArray.Num(); ++i)
				std::vector<T>::push_back(ItemArray[i]);
			return OldNum;
		}

		FORCEINLINE int AddUninitialized(int Count = 1)
		{
			const int OldNum = Num();
			std::vector<T>::resize(OldNum + Count);
			return OldNum;
		}

		FORCEINLINE int AddZeroed(int Count = 1)
		{
			const int OldNum = Num();
			std::vector<T>::resize(OldNum + Count);
			for (int Index = OldNum; Index < OldNum + Count; ++Index)
			{
				this->at(Index) = 0;
			}
			return Index;
		}

		FORCEINLINE void RemoveAt(int Index)
		{
			const int DataNum = Num();
			UE_ASSERT(Index < DataNum, Index >= 0);
			std::vector<T>::erase(std::vector<T>::begin() + Index);	
		}

		FORCEINLINE void Remove(const T& Item)
		{
			const int DataNum = Num();
			int count = 0;
			T* Data = std::vector<T>::data();
			for (int Index = 0; Index < DataNum; ++Index)
			{
				if (Data[Index] == Item)
					continue;
				if (count < Index)
					Data[count] = Data[Index];
				++count;
			}
			std::vector<T>::resize(count);
		}

		FORCEINLINE T& Last(int IndexFromTheEnd = 0)
		{
			const int DataNum = Num();
			return std::vector<T>::data()[DataNum - IndexFromTheEnd - 1];
		}

		FORCEINLINE bool IsValidIndex(int Index) const
		{
			return Index >= 0 && Index < Num();
		}

		FORCEINLINE void Sort()
		{
			std::sort(begin(), end());
		}

		FORCEINLINE T* GetData()
		{
			return (T*)(std::vector<T>::data());
		}
		FORCEINLINE const T* GetData() const
		{
			return (const T*)(std::vector<T>::data());
		}
	};
}// namespace UEMath