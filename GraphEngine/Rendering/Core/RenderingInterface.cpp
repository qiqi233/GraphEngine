#include "RenderingInterface.h"
#include "Platform/Windows/WindowsEngine.h"

Microsoft::WRL::ComPtr<ID3D12Device> IRenderingIntface::GetD3dDevice()
{
	if(FPlatformEngine* WindowsEngine= GetEngine())
	{
		return WindowsEngine->D3dDevice;
	}
	return ComPtr<ID3D12Device>();
}

#if defined(_WIN32) ||defined(_WIN64) 
FPlatformEngine* IRenderingIntface::GetEngine()
{
	return dynamic_cast<FWindowsEngine*>(Engine);
}
#else
FPlatformEngine* IRenderingIntface::GetEngine()
{
	return Engine;
}
#endif


Microsoft::WRL::ComPtr<ID3D12CommandAllocator> IRenderingIntface::GetCommandAllocator()
{
	if (FWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->CommandAllocator;
	}

	return NULL;
}
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> IRenderingIntface::GetGraphicsCommandList()
{
	if (FPlatformEngine* WindowsEngine = GetEngine())
	{
		return WindowsEngine->GraphicsCommandList;
	}
	return ComPtr<ID3D12GraphicsCommandList>();
}

FGuid IRenderingIntface::GetGuid()
{
	return RenderGuid;
}

std::vector<IRenderingIntface*> IRenderingIntface::RenderingIntface;
IRenderingIntface::IRenderingIntface()
{
	create_guid(&RenderGuid);

	RenderingIntface.push_back(this);
}

IRenderingIntface::~IRenderingIntface()
{
	for(std::vector<IRenderingIntface*>::iterator Tmp= RenderingIntface.begin();Tmp!=RenderingIntface.end();++Tmp)
	{
		if(*Tmp==this)
		{
			RenderingIntface.erase(Tmp);
			break;
		}
	}
}

void IRenderingIntface::Rendering(float InDeltaTime)
{
	for (std::vector<IRenderingIntface*>::iterator Tmp = RenderingIntface.begin(); Tmp != RenderingIntface.end(); ++Tmp)
	{
		(*Tmp)->Update(InDeltaTime);
		(*Tmp)->Draw(InDeltaTime);
	}

}

ComPtr<ID3D12Resource> IRenderingIntface::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& uploadBuffer, const void* initData, UINT64 byteSize)
{
	ComPtr<ID3D12Resource> defaultBuffer;
	ID3D12Device* device=GetD3dDevice().Get();
	//����ʵ�ʵ�Ĭ�ϻ�������Դ��
	CD3DX12_HEAP_PROPERTIES DefalutHeapProperties= CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC DefalutResDesc=CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	DX12_DEBUG_MESSAGE(device->CreateCommittedResource(
		&DefalutHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&DefalutResDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

	//Ϊ�˽�CPU�ڴ����ݸ��Ƶ�Ĭ�ϻ�������������Ҫ�����м��ϴ��ѡ�
	CD3DX12_HEAP_PROPERTIES UploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC UploadResDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	DX12_DEBUG_MESSAGE(device->CreateCommittedResource(
		&UploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&UploadResDesc,
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
	CD3DX12_RESOURCE_BARRIER ResouceBarrierCopy=CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResouceBarrierCopy);

	UpdateSubresources<1>(GetGraphicsCommandList().Get(), defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

	CD3DX12_RESOURCE_BARRIER ResouceBarrierCommon = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResouceBarrierCommon);

	//ע�⣺��������������֮��uploadBuffer���뱣�ֻ״̬����Ϊ
	//��δִ��ִ��ʵ�ʸ��Ƶ������б�
	//�����߿�����֪��������ִ�к��ͷ�uploadBuffer��

	return defaultBuffer;
}

bool IRenderingIntface::operator==(const IRenderingIntface& InOther)
{
	return guid_equal(&RenderGuid,&InOther.RenderGuid);
}

