#include "Sprite.h"

//頂点データ
XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.0f},
	{-0.5f,+0.5f,0.0f},
	{+0.5f,-0.5f,0.0f},
};

void	Sprite::Initialize(SpriteCommon* spriteCommon_) {
	//変数へコピー
	spriteCommon = spriteCommon_;
	directXCom = spriteCommon_->GetdxCom();

	UINT	sizeVB=static_cast<UINT>(sizeof(XMFLOAT3)*_countof(vertices));
	//頂点バッファ
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = directXCom->GetResult();
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//繋がりの解除
	vertBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(XMFLOAT3);
}

void Sprite::Draw() {
	comList = directXCom->GetCommandList();
	// パイプラインステートとルートシグネチャの設定コマンド
	comList->SetPipelineState(spriteCommon->GetPipelineState());
	comList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());

	// プリミティブ形状の設定コマンド
	comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	comList->IASetVertexBuffers(0, 1, &vbView);

	// 描画コマンド
	comList->DrawInstanced(_countof(vertices), 1, 0, 0);//全ての頂点を使って描画
}