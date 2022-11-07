#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<cassert>
#include<vector>
#include"WinApp.h"

#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")


using	namespace Microsoft::WRL;
#pragma once
class DirectXCommon
{
public://メンバ関数
	//初期化
	void Initialize(WinApp* winApp_);
	//デバイスの初期化
	void InitializeDevice();
	//コマンドの初期化
	void InitializeCommand();
	//スワップチェーンの初期化
	void InitializeSwapchain();
	//レンダーターゲットビュー
	void IntializeRenderTargetView();
	//深度バッファの初期化
	void InitializeDepthBuffer();
	//フェンスの初期化
	void InitializeFence();
private:
	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device>device;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7>dxgiFactory;
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC	commandQueueDesc{};
	ComPtr < IDXGISwapChain4> swapChain = nullptr;
	ComPtr < ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> comList = nullptr;
	ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1	swapChainDesc{};
private:
	WinApp* winApp = nullptr;
	HRESULT	result;
};

