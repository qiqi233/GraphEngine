#include "Guid.h"
#include <stdarg.h>
#include <combaseapi.h>

FString get_printf_s(const char* format, ...)
{
	FString out_buf;
	char buf[33] = { 0 };
	memset(buf, 0, sizeof(char) * 33);
	va_list args;
	va_start(args, format);
	_vsnprintf_s(buf, 33,33, format, args);
	va_end(args);
	buf[32] = 0;

	out_buf=buf;
	return out_buf;
}
FString FGuid::ToString()
{
	return get_printf_s("%08X%08X%08X%08X", A, B, C, D);
}

FGuid FGuid::NewGuid()
{
	FGuid Result(0, 0, 0, 0);
	assert(CoCreateGuid((GUID*)&Result) == S_OK);
	return Result;
}