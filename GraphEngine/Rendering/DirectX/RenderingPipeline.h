#pragma once
#include "EngineMinimal.h"
#include "DirectXDeviceInterfece.h"
#include "FrameResource.h"
#include "Shader/Shader.h"

class FDX12RenderEngine;
class FRenderingPipeline:public IDirectXDeviceInterfece
{
	friend class FDX12RenderEngine;
public:
	FRenderingPipeline(FDX12RenderEngine* RenderEngine);
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildGeometry();
	void BuildRenderItems();
	void BuildFrameResources();
	void BuildDescriptorHeaps();
	void BuildConstantBufferViews();
	void BuildPSOs();


	void OnKeyboardInput(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

	virtual void Update(const GameTimer& gt);

	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList);
private:
	UINT RTVDescriptorSize;
	UINT DSVDescriptorSize;
	UINT CBVDescriptorSize;
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;
	//存放用于渲染的模型数据
	std::unordered_map<std::string, std::unique_ptr<IRenderingIntface>> mGeometries;
	std::unordered_map<std::string, FShader> mShaders;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;

	PassConstants mMainPassCB;

	UINT mPassCbvOffset = 0;

	bool mIsWireframe = false;
private:
	FDX12RenderEngine* DX12RenderEngine;

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.2f * XM_PI;
	float mRadius = 15.0f;

	POINT mLastMousePos;
};