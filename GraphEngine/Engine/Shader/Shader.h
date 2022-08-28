#pragma once
#include "EngineMinimal.h"

class FShader
{
public:
	LPVOID GetBufferPointer();
	SIZE_T GetBufferSize();

	static FShader BuildShaders(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion);
private:
	ComPtr<ID3DBlob> ShaderCode;
};