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

	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	
	//デバイスの取得
	ID3D12Device*GetDevice()const { return device.Get(); }
	//コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList()const { return comList.Get(); }

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
	//バックバッファ
	std::vector< ComPtr<ID3D12Resource>>	backBuffers;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC	rtvHeapDesc{};
	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC	dsvHeapDesc{};
	//深度ビュー用デスクリプタヒープ作成
	ComPtr < ID3D12DescriptorHeap> dsvHeap = nullptr;
	//フェンスの生成
	ComPtr < ID3D12Fence> fence = nullptr;
	UINT64	fenceVal = 0;
	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc{};
private:
	WinApp* winApp = nullptr;
	HRESULT	result;
};

