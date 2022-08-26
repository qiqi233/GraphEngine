
#include "d3dUtil.h"
#include <fstream>

bool d3dUtil::IsKeyDown(int vkeyCode)
{
    return (GetAsyncKeyState(vkeyCode) & 0x8000) != 0;
}

ComPtr<ID3DBlob> d3dUtil::LoadBinary(const std::wstring& filename)
{
    std::ifstream fin(filename, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    std::ifstream::pos_type size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);

    ComPtr<ID3DBlob> blob;
    DX12_DEBUG_MESSAGE(D3DCreateBlob(size, blob.GetAddressOf()));

    fin.read((char*)blob->GetBufferPointer(), size);
    fin.close();

    return blob;
}

Microsoft::WRL::ComPtr<ID3D12Resource> d3dUtil::CreateDefaultBuffer(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
    ComPtr<ID3D12Resource> defaultBuffer;
    CD3DX12_HEAP_PROPERTIES DefHeapProperty=CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC DefResDesc=CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    //����ʵ�ʵ�Ĭ�ϻ�������Դ��
    DX12_DEBUG_MESSAGE(device->CreateCommittedResource(
        &DefHeapProperty,
        D3D12_HEAP_FLAG_NONE,
        &DefResDesc,
		D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

    //Ϊ�˽�CPU�ڴ����ݸ��Ƶ�Ĭ�ϻ�������������Ҫ�����м��ϴ��ѡ�
	CD3DX12_HEAP_PROPERTIES UpHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC UpResDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    DX12_DEBUG_MESSAGE(device->CreateCommittedResource(
        &UpHeapProperty,
		D3D12_HEAP_FLAG_NONE,
        &UpResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


	//����Ҫ���Ƶ�Ĭ�ϻ����������ݡ�
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

	//�ƻ������ݸ��Ƶ�Ĭ�ϻ�������Դ���ڸ߼����ϣ�helper������������Դ
	//��CPU�ڴ渴�Ƶ��м��ϴ����С�Ȼ��ʹ��ID3D12CommandList:��CopySubresourceRegion��
	//�м��ϴ������ݽ����Ƶ�mBuffer��
    CD3DX12_RESOURCE_BARRIER ResouceBarrierCopy = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    cmdList->ResourceBarrier(1, &ResouceBarrierCopy);
    UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
    CD3DX12_RESOURCE_BARRIER ResouceBarrierCommon = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	cmdList->ResourceBarrier(1, &ResouceBarrierCommon);

	//ע�⣺��������������֮��uploadBuffer���뱣�ֻ״̬����Ϊ
	//��δִ��ִ��ʵ�ʸ��Ƶ������б�
	//�����߿�����֪��������ִ�к��ͷ�uploadBuffer��



    return defaultBuffer;
}

ComPtr<ID3DBlob> d3dUtil::CompileShader(
	const std::wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const std::string& entrypoint,
	const std::string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> byteCode = nullptr;
	ComPtr<ID3DBlob> errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if(errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

    DX12_DEBUG_MESSAGE(hr);

	return byteCode;
}


