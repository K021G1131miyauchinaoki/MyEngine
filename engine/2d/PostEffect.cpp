#include "PostEffect.h"
#include"DirectXTex.h"
#include<WinApp.h>
#include<d3dx12.h>
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

DirectXCommon* PostEffect::dxCommon = nullptr;
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

//インデックスデータ
unsigned	short	indice[] = {
	0,1,2,
	1,2,3,
};

PostEffect::PostEffect() 
	:Sprite(
		100,
		{0,0},
		{500.0f,500.0f},
		{1,1,1,1},
		{0.0f,0.0f},
		false,
		false
)
{

}

void PostEffect::StaticInitialize(DirectXCommon* dxCommon_) {
	dxCommon = dxCommon_;
}

void PostEffect::Initialize(SpriteCommon* spriteCommon_) {
	

	//基底クラスとしての初期化
	Sprite::Initialize(spriteCommon_);

	//InitializeTex();
	//InitializeSRV();
	//InitializeRTV();
	//InitializeDepth();
	//InitializeDSV();
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

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList_) {

	matWorld = XMMatrixIdentity();
	//matWorld.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	//matWorld.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	//matWorld *= XMMatrixScaling(1.0f, 1.0f, 0.0f);
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	constMap->color = color;
	constMap->mat = matWorld * matProjection;

	//非表示
	if (isInvisible)
	{
		return;
	}

	spriteCommon->SetTextureCommands(texIndex);
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// パイプラインステートとルートシグネチャの設定コマンド
	//cmdList_->SetPipelineState(spriteCommon->GetPiprlineState());
	//cmdList_->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
	// プリミティブ形状の設定コマンド
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	// 頂点バッファビューの設定コマンド
	cmdList_->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//インディックスバッファビューの設定コマンド
	cmdList_->IASetIndexBuffer(&ibView);
	//定数バッファビュー(SRV)の設定コマンド
	D3D12_GPU_DESCRIPTOR_HANDLE descHeapSRVHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
	cmdList_->SetGraphicsRootDescriptorTable(1,descHeapSRVHandle);
	// 描画コマンド
	cmdList_->DrawIndexedInstanced(_countof(indice), texIndex, 0, 0, 0);//全ての頂点を使って描画
}

void PostEffect::InitializeTex(){

}

void PostEffect::InitializeSRV(){
	
}

void PostEffect::InitializeRTV(){
	
}

void PostEffect::InitializeDepth(){
	
}

void PostEffect::InitializeDSV(){
	
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