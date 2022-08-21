#pragma once
#include <winerror.h>
#include "EngineLog.h"

#ifndef DX12_DEBUG_MESSAGE
#define DX12_DEBUG_MESSAGE(InValue)\
{\
	HRESULT Result=InValue;\
	if(FAILED(Result))\
	{\
		GE_LOG(Error,"Error =%i",(int)Result);\
		assert(0);\
	}\
	else if (SUCCEEDED(Result))\
	{\
		GE_LOG(Success, "Success ! ");\
	}\
}
#endif
