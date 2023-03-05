#include "DirectXCommon.h"

#include<cassert>
#include<vector>
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

using	namespace Microsoft::WRL;


//デバイスの初期化
void DirectXCommon::InitializeDevice() {
#ifdef _DEBUG
	//デバックレイヤーをオンにする
	ComPtr < ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif

	
	//ComPtr < ID3D12Device> device = nullptr;
	//ComPtr<	IDXGIFactory7> dxgiFactory = nullptr;


	//DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//アダプターの列検挙
	std::vector< ComPtr<IDXGIAdapter4>>adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr < IDXGIAdapter4> tmpAdapter = nullptr;
	//パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference
		(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}
	//妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3	adapterDesc;
		//アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
		{
			//デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}
	//対応レベルの配列
	D3D_FEATURE_LEVEL	levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL	featureLevel;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}

	}


}
//コマンドの初期化
void DirectXCommon::InitializeCommand() {
	//コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	assert(SUCCEEDED(result));

	//コマンドリストを生成
	result = device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&comList));
	assert(SUCCEEDED(result));


	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}
//スワップチェーンの初期化
void DirectXCommon::InitializeSwapchain() {
	
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain1のComPtrを用意
	//スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(),
		winApp->GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1);
	//生成したIDXGISwapChan1のオブジェクトをIDXGISwapChain4に変換する
	swapChain1.As(&swapChain);

	assert(SUCCEEDED(result));
}
//レンダーターゲットビューの初期化
void DirectXCommon::InitializeRenderTargetView(){
	//デスクリプタヒープの設定
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;

	//デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	//バックバッファ
	backBuffers.resize(swapChainDesc.BufferCount);

	//スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		//スワップチェーンからバッファを取得 
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒープのハンドルを取得 
		D3D12_CPU_DESCRIPTOR_HANDLE	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表でアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC	rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}
//深度バッファの初期化
void DirectXCommon::InitializeDepthBuffer(){
	//リソース設定
	D3D12_RESOURCE_DESC	depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::window_width;//レンダーターゲットに合わせる
	depthResourceDesc.Height = WinApp::window_height;//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//デプスステンシル
	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE	depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//深度値1.0ｆ（最大値）でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	//リソース設定
	
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//深度ビュー用デスクリプタヒープ作成
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC	dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}
//フェンスの初期化
void DirectXCommon::InitializeFence() {
	//フェンスの生成
	fence = nullptr;
	fenceVal = 0;

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

//描画前処理
void DirectXCommon::PreDraw() {
	// バックバッファの番号を取得(2つなので0番か1番)
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// 1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	comList->ResourceBarrier(1, &barrierDesc);

	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	D3D12_CPU_DESCRIPTOR_HANDLE	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	comList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 3.画面クリア R G B A
	//値を書き込むと自動的に転送される
	FLOAT clearColor[4] = { 0.1f,0.25f,0.5f,1.0f }; // 青っぽい色
	comList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	comList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//スペースキーが押されていたら
	//if (key[DIK_SPACE])
	//{
	//	FLOAT clearColor[] = { 0.5f,0.5f, 0.5f,0.0f };
	//	comList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//}
	// 4.描画コマンドここから

	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	viewport.Width = WinApp::window_width;
	viewport.Height = WinApp::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	// ビューポート設定コマンドを、コマンドリストに積む
	comList->RSSetViewports(1, &viewport);

	//シザー矩形
	D3D12_RECT	scissorRect{};
	scissorRect.left = 0;                                       // 切り抜き座標左
	scissorRect.right = scissorRect.left + WinApp::window_width;        // 切り抜き座標右
	scissorRect.top = 0;                                        // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + WinApp::window_height;       // 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	comList->RSSetScissorRects(1, &scissorRect);
}
//描画後処理
void DirectXCommon::PostDraw(){
	// バックバッファの番号を取得(2つなので0番か1番)
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	comList->ResourceBarrier(1, &barrierDesc);

	// 命令のクローズ
	result = comList->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	ID3D12CommandList* commandLists[] = { comList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);
	// 画面に表示するバッファをフリップ(裏表の入替え)
	
	result = swapChain->Present(1, 0);
	//assert(SUCCEEDED(result));

	// コマンドの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	// キューをクリア
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = comList->Reset(cmdAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void DirectXCommon::Initialize(WinApp* winApp_) {
	//NULL検出
	assert(winApp_);
	//メンバ変数に代入
	this->winApp = winApp_;
	//デバイスの初期化
	InitializeDevice();
	//コマンドの初期化
	InitializeCommand();
	//スワップチェーンの初期化
	InitializeSwapchain();
	//レンダーターゲットビューの初期化
	InitializeRenderTargetView();
	//深度バッファの初期化
	InitializeDepthBuffer();
	//フェンスの初期化
	InitializeFence();

#ifdef _DEBUG


	ComPtr < ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		//抑制するエラー
		D3D12_MESSAGE_ID	denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制する表示レベル
		D3D12_MESSAGE_SEVERITY	severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER	filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->Release();
	}
#endif
}