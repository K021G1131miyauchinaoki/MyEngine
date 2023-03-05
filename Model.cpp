#include "Model.h"
#include"DirectXTex/DirectXTex.h"

//静的メンバ変数の実体
ID3D12Device* Model::device = nullptr;

Model* Model::LoadFromOBJ(const	std::string& modelname) {
	//インスタンスの生成
	Model* model = new Model();
	//デスクリプタヒープの生成
	model->InitializeDescriptorHeap();
	//obj読み込み
	model->LoadFromOBJInternal(modelname);
	//バッファの生成
	model->CreateBuffers();

	return model;
}

void Model::LoadFromOBJInternal(const	std::string& modelname) {
	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	//file.open("Resources/triangle_tex/triangle_tex.obj");
	//const std::string modelname = "triangle_mat";
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	assert(!file.fail());

	std::vector<XMFLOAT3>positions;		//頂点座標
	std::vector<XMFLOAT3>normals;		//法線ベクトル
	std::vector<XMFLOAT2>texcoords;		//テクスチャUV

	//1行ずつ読み込む
	std::string line;

	while (getline(file, line)) {
		//1行分の文字列をストリームに変更して解析しやすくする
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');
		//先頭文字列がｖなら頂点座標
		if (key == "v")
		{
			//XYZ座標読み込み
			XMFLOAT3	position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			//VertexPosNormalUv	vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}
		//先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			std::string	index_string;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス一個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexNormal;
				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				//頂点インデックスに追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}
		//先頭文字列がvtならポリゴン
		if (key == "vt")
		{
			//UV成分の読み込み
			XMFLOAT2	texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//v方向反転　//モデリングツールによって方向が逆になるため
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列がvnならポリゴン
		if (key == "vn")
		{
			//xyz成分の読み込み
			XMFLOAT3	normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		//先頭文字列がならポリゴン
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			std::string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}
	}
	file.close();
}

void Model::LoadTexture(const	std::string& directoryPath, const std::string& filename) {
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//ファイルパスを結合
	std::string	filepath = directoryPath + filename;

	//ユニコード文字列に変更する
	wchar_t	wfilepath[128];
	int	iBufferSize = MultiByteToWideChar(
		CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	// WICテクスチャのロード
	//result = LoadFromWICFile( L"Resources/tex1.png", WIC_FLAGS_NONE, &metadata, scratchImg);

	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);

	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr, IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename) {
	//ファイルストリーム
	std::ifstream	file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	//1行ずつ読み込む
	std::string line;

	while (std::getline(file, line)) {
		//一行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		std::string	key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());//先頭の文字を削除
		}
		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> material.name;
		}
		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		//先頭文字列がKaならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		//先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			line_stream >> material.textureFilename;
			//テクスチャ読み込み
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	file.close();
}

void Model::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::CreateBuffers() {
	HRESULT result = S_FALSE;

	//UINT sizeVB = static_cast<UINT>(sizeof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeof(vertices);
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//UINT sizeIB = static_cast<UINT>(sizeof(indices));
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// リソース設定
	resourceDesc.Width = sizeIB;

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeof(indices);
	ibView.SizeInBytes = sizeIB;

	resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
	//定数バッファデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	// nullptrチェック
	assert(device);
	assert(cmdList);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial,
		constBuffB1->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//// 定数バッファビューをセット
	//cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//// シェーダリソースビューをセット
	//cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	if (material.textureFilename.size() > 0)
	{
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	}
	// 描画コマンド
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}