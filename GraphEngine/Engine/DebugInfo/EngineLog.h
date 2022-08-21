#pragma once

#if defined(_WIN32)||defined(_WIN64)
#include "simple_library.h"
#define Log #Log
#define Success #Success
#define Warning #Warning
#define Error #Error

#define  GE_LOG(Category,format,...) log_##Category(format,__VA_ARGS__)
#elif
 
#endif