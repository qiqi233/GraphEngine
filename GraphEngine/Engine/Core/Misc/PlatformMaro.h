#pragma once
#include <string>
#if defined (_WIN32)||defined (_WIN64)
using int32 =int;
using uint32= unsigned int;
using uint16 =unsigned short;
using FString=std::string;
#endif