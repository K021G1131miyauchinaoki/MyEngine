#include "PostEffect.h"
#include"DirectXTex.h"
#include<WinApp.h>
#include<d3dx12.h>
#include<d3dcompiler.h>
#include<memory>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

DirectXCommon* PostEffect::dxCommon = nullptr;
const float PostEffect::clearColor[4] = { 0.5f,0.5f,0.0f,1.0f };

PostEffect::PostEffect(){}

void PostEffect::StaticInitialize(DirectXCommon* dxCommon_) {
	dxCommon = dxCommon_;
}

void PostEffect::Initialize() {
	

	//基底クラスとしての初期化
	//Sprite::Initialize(spriteCommon_);
	CreateVertexBuffer();
	CreateTex();
	CreateSRV();
	CreateRTV();
	CreateDepth();
	CreateDSV();
	CreateGraphicsPipelineState();

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList_) {
	
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList_->SetPipelineState(pipelineState.Get());
	cmdList_->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状の設定コマンド
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	// 頂点バッファビューの設定コマンド
	cmdList_->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//定数バッファビュー(SRV)の設定コマンド
	D3D12_GPU_DESCRIPTOR_HANDLE descHeapSRVHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
	cmdList_->SetGraphicsRootDescriptorTable(1,descHeapSRVHandle);
	// 描画コマンド
	cmdList_->DrawInstanced(_countof(vertices_), 1, 0, 0);//全ての頂点を使って描画
}

void PostEffect::CreateTex(){
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::width,
		(int32_t)WinApp::height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	CD3DX12_HEAP_PROPERTIES texHeap = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);
	result_ = dxCommon->GetDevice()->CreateCommittedResource(
		&texHeap,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result_));

	{//テクスチャを赤クリア
		//画素数(1280*720=921600ピクセル)
		const uint32_t pixelCount = WinApp::width * WinApp::height;
		//画像1行分のデータサイズ
		const uint32_t rowPitch = sizeof(uint32_t) * WinApp::width;
		//画像1行分のデータサイズ
		const uint32_t depthPitch = sizeof(uint32_t) * WinApp::height;
		//画像イメージ
		uint32_t* img = new uint32_t[pixelCount];
		for (size_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//テクスチャバッファデータにデータ転送
		result_ = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result_));
		delete[]img;
	}
}

void PostEffect::CreateSRV(){
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//ＳＲＶ用デスクリプタヒープを生成
	result_ = dxCommon->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	dxCommon->GetDevice()->CreateShaderResourceView(
		texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateRTV(){
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープを生成
	result_ = dxCommon->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result_));

	//レンダーゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープにRTV作成
	dxCommon->GetDevice()->CreateRenderTargetView(
		texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateDepth(){
	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, WinApp::width, WinApp::height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	CD3DX12_HEAP_PROPERTIES depthProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE depthVale = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result_ = dxCommon->GetDevice()->CreateCommittedResource(
		&depthProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthVale,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result_));
}

void PostEffect::CreateDSV(){
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result_ = dxCommon->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result_));
	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxCommon->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateVertexBuffer() {
	//頂点バッファの生成
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * 4);
	result_ = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result_));

	//頂点データ
	Vertex vertices[4] = {
		{{-0.5f,-0.5f,0.0f},{0.0f,1.0f}},//左下
		{{-0.5f,+0.5f,0.0f},{0.0f,0.0f}},//左上
		{{+0.5f,-0.5f,0.0f},{1.0f,1.0f}},//右下
		{{+0.5f,+0.5f,0.0f},{1.0f,0.0f}},//右上
	};

	//頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result_ = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result_))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(Vertex) * 4;
	vbView.StrideInBytes = sizeof(Vertex);

	//定数バッファの生成
	resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
	result_ = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result_));

	//定数バッファにデータ転送
	result_ = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result_))
	{
		constMap->color = this->color;
		constMap->mat = XMMatrixIdentity();
		constBuff->Unmap(0, nullptr);
	}
}

void PostEffect::CreateGraphicsPipelineState() {
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//頂点シェーダーの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
	//エラーなら
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//ピクセルシェーダーの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
	//エラーなら
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	pipelineDesc{};
	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//ラスタライザの設定
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	//デプスステンシルステート
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書きルール

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBAすべてのチャンネルを描画

	blenddesc.BlendEnable = false;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//ブレンドステート設定
	//pipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定(三角形)
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1;//描画対象１つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;//1ピクセルにつき1回サンプリング

	//デスクリプタレンジの設定
	CD3DX12_DESCRIPTOR_RANGE	descriptorRange;
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0レジスタ

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER	rootParams[2] = {};
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_ALL);

	//スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result_ = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	
	//ルートシグネチャの生成
	result_ = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	//グラフィックスパイプラインの生成
	result_ = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList*	 cmdList_) {
	CD3DX12_RESOURCE_BARRIER resourceBuff = 
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	//リソースバリアを変更（シェーダーリソース→描画可能）
	cmdList_->ResourceBarrier(
		1,&resourceBuff);
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//ビューポートの設定
	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(WinApp::width), static_cast<float>(WinApp::height));
	cmdList_->RSSetViewports(1, &viewPort);
	//シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<float>(WinApp::width), static_cast<float>(WinApp::height));
	cmdList_->RSSetScissorRects(1, &rect);

	//全画面クリア
	cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList_) {
	CD3DX12_RESOURCE_BARRIER resourceBuff =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//リソースバリアを変更(描画可能→シェーダーリソース)
	cmdList_->ResourceBarrier(1, &resourceBuff);
}