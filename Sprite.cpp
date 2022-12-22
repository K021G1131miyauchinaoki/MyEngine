#include "Sprite.h"
#include"DirectXTex/DirectXTex.h"


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
	Vertex vertices[] = {
		{{	0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{	0.0f,  0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}},
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
	
	
	constMap->mat = XMMatrixIdentity();
	
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

	TexMetadata	metadata{};
	ScratchImage	scratchImg{};

	result = LoadFromWICFile(
		L"Resources/tex.png",
		WIC_FLAGS_NONE,
		&metadata, scratchImg);


	ScratchImage	mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	metadata.format = MakeSRGB(metadata.format);

	//テクスチャバッファ設定
	//ヒープ設定
	D3D12_HEAP_PROPERTIES	textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT16)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	result =directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img->pixels,		 //元データアドレス
			(UINT)img->rowPitch, //１ラインサイズ
			(UINT)img->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}

	//元データ解放
	//delete[]	imageData;
	//srvの最大個数
	const	size_t	kMaxSRVCount = 2056;
	//デスクリプタヒープの設定
	//D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	//ID3D12DescriptorHeap* srvHeap = nullptr;
	result = directXCom->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc{};		//設定構造体
	srvDesc.Format = resDesc.Format;//RGBA	float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

}

void Sprite::Draw() {
	comList = directXCom->GetCommandList();
	//constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	matWorld = XMMatrixIdentity();
	matWorld.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	matWorld.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	matWorld *= XMMatrixScaling(1.0f, 1.0f, 0.0f);
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	constMap->color = XMFLOAT4(1, 0, 0, 0.5f);
	constMap->mat = matWorld;
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
	//定数バッファビュー（CBV）の設定コマンド
	comList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//SRVヒープの設定コマンド
	comList->SetDescriptorHeaps(1, &srvHeap);
	//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
	D3D12_GPU_DESCRIPTOR_HANDLE	srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメータ１番に設定
	comList->SetGraphicsRootDescriptorTable(1, srvGpuHandle); 

	//インディックスバッファビューの設定コマンド
	comList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	comList->SetGraphicsRootConstantBufferView(2, constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	comList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);//全ての頂点を使って描画
}