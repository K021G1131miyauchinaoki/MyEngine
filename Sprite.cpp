#include "Sprite.h"
#include"DirectXTex/DirectXTex.h"


//インデックスデータ
unsigned	short	indices[] = {
	0,1,2,
	1,2,3,
};

XMMATRIX Sprite::matProjection;
//void	Sprite::Loadtexture(uint32_t index) {
//
//	TexMetadata	metadata{};
//	ScratchImage	scratchImg{};
//
//	result = LoadFromWICFile(
//		L"Resources/tex.png",
//		WIC_FLAGS_NONE,
//		&metadata, scratchImg);
//
//
//	ScratchImage	mipChain{};
//	result = GenerateMipMaps(
//		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT, 0, mipChain);
//	if (SUCCEEDED(result))
//	{
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//	metadata.format = MakeSRGB(metadata.format);
//	//テクスチャバッファ設定
//	//ヒープ設定
//	D3D12_HEAP_PROPERTIES	textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//リソース設定
//	D3D12_RESOURCE_DESC	textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = metadata.format;
//	textureResourceDesc.Width = metadata.width;
//	textureResourceDesc.Height = (UINT16)metadata.height;
//	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	//テクスチャバッファの生成
//	ID3D12Resource* texBuff = nullptr;
//	result = directXCom->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuffers[index]));
//
//	//全ミップマップについて
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		//ミップマップレベルを指定してイメージを取得
//		const	Image* img = scratchImg.GetImage(i, 0, 0);
//		//テクスチャバッファにデータ転送
//		result = texBuffers[index]->WriteToSubresource(
//			(UINT)i,
//			nullptr,			 //全領域へコピー
//			img->pixels,		 //元データアドレス
//			(UINT)img->rowPitch, //１ラインサイズ
//			(UINT)img->slicePitch//全サイズ
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	
//
//	//SRVヒープの先頭ハンドルを取得
//	D3D12_CPU_DESCRIPTOR_HANDLE	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
//
//	//シェーダリソースビュー設定
//	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc{};		//設定構造体
//	D3D12_RESOURCE_DESC	resDesc = texBuffers[index]->GetDesc();
//	srvDesc.Format = resDesc.Format;//RGBA	float
//	srvDesc.Shader4ComponentMapping =
//		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;
//
//	//ハンドルの指す位置にシェーダーリソースビュー作成
//	srvHandle.ptr += index;
//	directXCom->GetDevice()->CreateShaderResourceView(texBuffers[index].Get(), 
//		&srvDesc, 
//		srvHandle);
//
//}
void	Sprite::Initialize(SpriteCommon* spriteCommon_) {
	//変数へコピー
	spriteCommon = spriteCommon_;
	directXCom = spriteCommon_->GetdxCom();

	


	//UINT	sizeVB=static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//頂点バッファ
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(Vertex)*4;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点バッファの生成
	//ID3D12Resource* vertBuff = nullptr;
	result = directXCom->GetResult();
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	Update();

	//頂点バッファビューの作成
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeof(Vertex)*4;
	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);
	// 定数バッファの設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};   // ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData)+0xff)&~0xff; // 頂点データ全体のサイズ
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = directXCom->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
	assert(SUCCEEDED(result));
	constMap->color = color;
	constMap->mat = matProjection;
	
	constMap->mat = XMMatrixIdentity();
	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)directXCom->GetSwapChainDesc().Width,
		(float)directXCom->GetSwapChainDesc().Height, 0.0f,
		0.0f, 1.0f);
	//並行投影行列の計算
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, 1280,
	//	720, 0,
	//	0.0f, 1.0f);
	
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


	

	//元データ解放
	//delete[]	imageData;
	//srvの最大個数
	const	size_t	kMaxSRVCount = 2056;
	
		
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

}
void	Sprite::SetRotation(const float& rotation_) {
	rotation = rotation_;
	Update();

}

void	Sprite::SetPosition(const XMFLOAT2& position_) { 
	position = position_;
	Update();
}

void	Sprite::SetColor(const XMFLOAT4& color_) {
	color =	color_;
	Update();
}

void	Sprite::SetSize(const XMFLOAT2& size_) {
	size = size_;
	Update();
}

void	Sprite::SetAnchorPoint(const XMFLOAT2& anchorPoint_) {
	anchorPoint = anchorPoint_;
	Update();
}

// 左右反転の設定
void Sprite::SetIsFlipX(bool isFlipX_){
	isFlipX = isFlipX_;
	Update();
}

// 上下反転の設定
void Sprite::SetIsFlipY(bool isFlipY_){
	isFlipY = isFlipY_;
	Update();
}

// 非表示
void Sprite::SetIsInvisible(bool isInvisible_) {
	isInvisible = isInvisible_;
	Update();
}

void Sprite::Draw() {
	//srvHeap = spriteCommon->GetSrvHeap();
	comList = directXCom->GetCommandList();
	matWorld = XMMatrixIdentity();
	//matWorld.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	//matWorld.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	//matWorld *= XMMatrixScaling(1.0f, 1.0f, 0.0f);
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	constMap->color = color;
	constMap->mat = matWorld*matProjection;

	//非表示
	if (isInvisible)
	{
		return;
	}

	spriteCommon->SetTextureCommands(textureIndex);
	
	// 頂点バッファビューの設定コマンド
	comList->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	comList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	

	//インディックスバッファビューの設定コマンド
	comList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	//comList->SetGraphicsRootConstantBufferView(2, constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	comList->DrawIndexedInstanced(_countof(indices), textureIndex, 0, 0, 0);//全ての頂点を使って描画
}

void Sprite::Update()
{
	enum vertexNum
	{
		LB,//左下
		LT,//左上
		RB,//右下
		RT,//右上
	};
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.x) * size.y;
	float bottom = (1.0f - anchorPoint.x) * size.y;

	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	//頂点データ
	Vertex vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	vertices[LB].uv = {0.0f,1.0f }; // 左下
	vertices[LT].uv = {0.0f,0.0f }; // 左上
	vertices[RB].uv = {1.0f,1.0f }; // 右下
	vertices[RT].uv = {1.0f,0.0f }; // 右上

	// 頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}