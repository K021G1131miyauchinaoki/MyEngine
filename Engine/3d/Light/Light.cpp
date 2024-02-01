/**
 * @file Light.h
 * @brief　モデルのライト
 */
#include "Light.h"
#include<cassert>
using namespace MyEngin;

//静的メンバ変数の実体
ID3D12Device* Light::device = nullptr;

void Light::StaticInitialize(ID3D12Device* device_) {
	//再初期化チェック
	assert(!Light::device);
	//デバイスチェック
	assert(device_);
	//静的メンバ変数のセット
	Light::device = device_;
}

Light* Light::Create() {
	//ライトのインスタンスを生成
	Light* instance = new Light();
	//初期化
	instance->Initialize();
	//生成したインスタンスを返す
	return instance;
}


void Light::Initialize() {
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = ( sizeof(ConstBufferData) + 0xff ) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	//定数バッファへデータ転送
	TransferConstBuffer();
	
}

void Light::TransferConstBuffer(){
	//定数バッファのマッピング
	ConstBufferData* constMapData = nullptr;
	result = constBuff->Map(0,nullptr,( void** ) &constMapData);//マッピング
	if ( SUCCEEDED(result) )
	{
		constMapData->lightv = DirectX::XMVectorNegate(lightdir);
		constMapData->lightcolor = lightcolor;
		constBuff->Unmap(0,nullptr);
	}
}

void Light::SetLightDir(const DirectX::XMVECTOR& lightdir_){
	//正規化してセット
	lightdir = DirectX::XMVector3Normalize(lightdir_);
	dirty = true;
}

void Light::SetLightColor(const DirectX::XMFLOAT3& lightcolor_) {
	//正規化してセット
	lightcolor = lightcolor_;
	dirty = true;
}

void Light::Update(){
	if ( dirty )
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterindex) {
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterindex,constBuff->GetGPUVirtualAddress());
}

