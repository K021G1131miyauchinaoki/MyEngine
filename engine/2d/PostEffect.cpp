#include "PostEffect.h"
#include"DirectXTex.h"
#include<WinApp.h>
#include<d3dx12.h>
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

DirectXCommon* PostEffect::dxCommon = nullptr;

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
	HRESULT result;

	//基底クラスとしての初期化
	Sprite::Initialize(spriteCommon_);

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::width,
		(int32_t)WinApp::height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	CD3DX12_HEAP_PROPERTIES texHeap = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	result = dxCommon->GetDevice()->CreateCommittedResource(
		&texHeap,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

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
		result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[]img;
	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//ＳＲＶ用デスクリプタヒープを生成
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));

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

	// 頂点バッファビューの設定コマンド
	cmdList_->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//インディックスバッファビューの設定コマンド
	cmdList_->IASetIndexBuffer(&ibView);
	//定数バッファビュー(SRV)の設定コマンド
	//cmdList_->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	// 描画コマンド
	cmdList_->DrawIndexedInstanced(_countof(indice), texIndex, 0, 0, 0);//全ての頂点を使って描画
}