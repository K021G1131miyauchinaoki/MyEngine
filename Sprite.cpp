#include "Sprite.h"



//インデックスデータ
unsigned	short	indices[] = {
	0,1,2,
	1,2,3,
};

void	Sprite::Initialize(SpriteCommon* spriteCommon_) {
	//変数へコピー
	spriteCommon = spriteCommon_;
	directXCom = spriteCommon_->GetdxCom();

	//頂点データ
	Vertex	vertices[] = {
	{{ -0.5f, -0.5f, 0.0f },{0.0f,1.0f}}, // 左下
	{{ -0.5f, +0.5f, 0.0f },{0.0f,0.0f}}, // 左上
	{{ +0.5f, -0.5f, 0.0f },{1.0f,1.0f}}, // 右下
	{{ +0.5f, +0.5f, 0.0f },{1.0f,0.0f}}, // 右上
	};


	UINT	sizeVB=static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
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
	Vertex* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(vertices[0]);
	// 定数バッファの設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};   // ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial)); // 頂点データ全体のサイズ
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// 定数バッファの生成
	//ID3D12Resource* constBffMarerial = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBffMarerial));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBffMarerial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング

	//インディックスデータ全体のサイズ
	UINT	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	// リソース設定

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インディックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インディックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp,//ヒープの設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インディックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インディックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];//インディックスをコピー
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インディックスバッファビューの作成
	//D3D12_INDEX_BUFFER_VIEW	ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//横方向ピクセル数
	const size_t	textureWidth = 256;
	//縦方向ピクセル数
	const size_t	textureHeight = 256;
	//配列の要素数
	const size_t	imgeDataCount = textureWidth * textureHeight;
	//画像イメージデータ配列
	XMFLOAT4*	imageData = new	XMFLOAT4[imgeDataCount];

	//全ピクセルの色を初期化
	for (size_t i = 0; i < imgeDataCount; i++) {
		imageData[i].x = 1.0f;//R
		imageData[i].y = 0.0f;//G
		imageData[i].z = 0.0f;//B
		imageData[i].w = 1.0f;//A
	}
	//テクスチャバッファ設定
	//ヒープ設定
	D3D12_HEAP_PROPERTIES	textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth;//幅
	textureResourceDesc.Height = textureHeight;//高さ
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
}

void Sprite::Draw() {
	comList = directXCom->GetCommandList();

	constMapMaterial->color = XMFLOAT4(1, 1, 1, 0.5f);

	// パイプラインステートとルートシグネチャの設定コマンド
	comList->SetPipelineState(spriteCommon->GetPipelineState());
	comList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());

	// プリミティブ形状の設定コマンド
	comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	//三角形リスト		TRIANGLELIST
	//三角形ストリップ	TRIANGLESTRIP
	//線リスト			LINELIST
	//線ストリップ		LINESTRIP
	//点リスト			POINTLIST
	// 頂点バッファビューの設定コマンド
	comList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビュー
	comList->SetGraphicsRootConstantBufferView(0,constBffMarerial->GetGPUVirtualAddress());

	//インディックスバッファビューの設定コマンド
	comList->IASetIndexBuffer(&ibView);

	// 描画コマンド
	comList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);//全ての頂点を使って描画
}