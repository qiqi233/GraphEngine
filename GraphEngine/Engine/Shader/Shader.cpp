#include "Shader.h"
#include <assert.h>
LPVOID FShader::GetBufferPointer()
{
	return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize()
{
	return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion)
{
	ComPtr<ID3DBlob> ErrorShaderMsg;
	HRESULT R = D3DCompileFromFile(InFileName.c_str(),
		NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		InEntryFunName.c_str(), InShadersVersion.c_str(),
#if _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
0
#endif
		,0,&ShaderCode,&ErrorShaderMsg);

	if (ErrorShaderMsg)
	{
		GE_LOG(Error,"%s", (char*)ErrorShaderMsg->GetBufferPointer());
	}

	//ʧ�ܾͱ�����
	DX12_DEBUG_MESSAGE(R);
}