#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<vector>
#include"WinApp.h"


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
	void InitializeRenderTargetView();
	//深度バッファの初期化
	void InitializeDepthBuffer();
	//フェンスの初期化
	void InitializeFence();

	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
public://ゲッター
	//デバイス
	ID3D12Device*GetDevice()const { return device.Get(); }
	//コマンドリスト
	ID3D12GraphicsCommandList* GetCommandList()const { return comList.Get(); }
	//result
	HRESULT GetResult()const { return result; }
	//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc()const { return swapChainDesc; }
	//バックバッファ数を取得
	size_t GetBackBufferCount()const { return backBuffers.size(); }

private:
	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device>device;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7>dxgiFactory;
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC	commandQueueDesc{};
	Microsoft::WRL::ComPtr <IDXGISwapChain1>swapChain1;
	Microsoft::WRL::ComPtr<IDXGISwapChain4>swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>commandQueue = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>rtvHeap = nullptr;
	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1	swapChainDesc{};
	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>backBuffers;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC	rtvHeapDesc{};
	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC	dsvHeapDesc{};
	//深度ビュー用デスクリプタヒープ作成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>dsvHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	//フェンスの生成
	Microsoft::WRL::ComPtr<ID3D12Fence>fence = nullptr;
	UINT64	fenceVal = 0;
	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc{};
private:
	WinApp* winApp = nullptr;
	HRESULT	result;
};

