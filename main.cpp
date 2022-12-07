#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex/DirectXTex.h"
#include<cassert>
#include<vector>
#include"main.h"

#pragma	comment(lib,"d3dcompiler.lib")
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

//自作クラス
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"

using namespace DirectX;
using	namespace Microsoft::WRL;

const XMFLOAT3 CatmullRomSpline(XMFLOAT3 P0, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3 P3, float t) {
	XMFLOAT3 a;
	a.x = (P1.x * 2) + ((-P0.x + P2.x) * t);
	a.y = (P1.y * 2) + ((-P0.y + P2.y) * t);
	a.z = (P1.z * 2) + ((-P0.z + P2.z) * t);

	XMFLOAT3 b;
	b.x = ((2 * P0.x) - (5 * P1.x) + (4 * P2.x) - P3.x) * t * t;
	b.y = ((2 * P0.y) - (5 * P1.y) + (4 * P2.y) - P3.y) * t * t;
	b.z = ((2 * P0.z) - (5 * P1.z) + (4 * P2.z) - P3.z) * t * t;

	XMFLOAT3 c;
	c.x = (-P0.x + (3 * P1.x) - (3 * P2.x) + P3.x) * t * t * t;
	c.y = (-P0.y + (3 * P1.y) - (3 * P2.y) + P3.y) * t * t * t;
	c.z = (-P0.z + (3 * P1.z) - (3 * P2.z) + P3.z) * t * t * t;

	XMFLOAT3	d;
	d.x = 0.5f * (a.x + b.x + c.x);
	d.y = 0.5f * (a.y + b.y + c.y);
	d.z = 0.5f * (a.z + b.z + c.z);

	return d;
}
XMFLOAT3	splinePosition(const std::vector<XMFLOAT3>& points, size_t startIndex, float t) {
	//補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];//pnの値を返す
	if (startIndex < 1)return points[1];//p1の値を返す

	//p0～p3の制御点を取得する※p1～p2を補間する
	XMFLOAT3 p0 = points[startIndex - 1];
	XMFLOAT3 p1 = points[startIndex];
	XMFLOAT3 p2 = points[startIndex + 1];
	XMFLOAT3 p3 = points[startIndex + 2];

	//Catmull-Romの式による補間
	XMFLOAT3 position = CatmullRomSpline(p0, p1, p2, p3, t);

	return position;
}

void	PlayerInitialize(ID3D12Device* device) {
	//プレイヤー
	InitializeObject3d(&player, device);
	player.position = XMFLOAT3{ 0,0,-15 };
	hpP = 3;
	isPlayer = false;
}

void	EnemyInitialize(ID3D12Device* device) {
	//プレイヤー
	InitializeObject3d(&enemy, device);
	enemy.scale = { 15.0f,15.0f, 15.0f };
	enemy.position = XMFLOAT3{ 0,0,-15 };
	hpE = 20;
	isEnemy = false;
}

void	CheckAllCollisions(){
	//AとBの座標
	XMFLOAT3	posA, posB;

	const	float	r = 1.0f;
	const	float	enemyR = 15.0f;



}

struct Vertex
{
	XMFLOAT3	pos;//xyz座標
	XMFLOAT3	normal;//法線ベクトル
	XMFLOAT2	uv;	//uv座標
};
float	R = 1.0f;
float	G = 0.0f;
float	B = 0.0f;

//windowsアプリでのエントリーポイント(main関数)
int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPIの初期化
	WinApp* winApp = nullptr;
	winApp = new	WinApp;
	winApp->Initialize();
#pragma endregion

#pragma region DirectX初期化処理
//DirectX初期化処理　　ここから
	DirectXCommon* directXCom = nullptr;
	directXCom = new DirectXCommon;
	directXCom->Initialize(winApp);

	Input* input = nullptr;
	input = new	Input;
	input->Initialize(winApp);
	//DirectX初期化処理　　ここまで
#pragma endregion

#pragma region 描画処理の初期化
// 頂点データ
	Vertex vertices[] = {
		//前
		{{-5.0f,-5.0f,-5.0f},{}, {0.0f,1.0f}},//左下
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}},//左上
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}},//右下
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}},//右上
		//後（前面とZ座標の符号が逆）
		{{ 5.0f,-5.0f,5.0f},{},{0.0f,1.0f}},//左下
		{{ 5.0f, 5.0f,5.0f},{},{0.0f,0.0f}},//左上
		{{-5.0f,-5.0f,5.0f},{},{1.0f,1.0f}},//右下
		{{-5.0f, 5.0f,5.0f},{},{1.0f,0.0f}},//右上
		//左
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}},//左下
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}},//左上
		{{-5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}},//右下
		{{-5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}},//右上
		//右（前面とZ座標の符号が逆）
		{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},//左下
		{{ 5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}},//左上
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}},//右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},//右上
		//下
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},//左下
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},//左上
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}},//右下
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}},//右上
		//上（前面とZ座標の符号が逆）
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}},//左下
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}},//左上
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}},//右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},//右上

	};
	//インディックスデータ
	unsigned	short	indices[] =
	{
		//前
		0,1,2,//三角形1つ目
		2,1,3,//三角形2つ目
		//後(前の面に4加算)
		4,5,6,
		6,5,7,
		//左
		8,9,10,
		10,9,11,
		//右
		12,13,14,
		14,13,15,
		//下
		16,18,17,
		17,18,19,
		//上
		20,21,22,
		22,21,23,
	};

	HRESULT result;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// 頂点バッファの生成
	ComPtr < ID3D12Resource> vertBuff = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	for (int i = 0; i < _countof(indices) / 3; i++)
	{//三角形1つごとに計算
		//三角形のインディックスを取り出して、一時的な変数に入れる
		unsigned	short indices0 = indices[i * 3 + 0];
		unsigned	short indices1 = indices[i * 3 + 1];
		unsigned	short indices2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標ベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR	v1 = XMVectorSubtract(p1, p0);
		XMVECTOR	v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR	normal = XMVector3Cross(v1, v2);

		//正規化（長さを1にする）
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}


	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];   // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);


	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	ComPtr < ID3D12Resource> constBffMarerial = nullptr;

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

	result = directXCom->GetDevice()->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBffMarerial));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBffMarerial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング

	//3Dオブジェクトの数
	const	size_t	kObjectCount = 2;
	//3Dオブジェクトの配列
	Object3d	object3ds[kObjectCount];

	//配列内の全オブジェクトに対して
	for (int i = 0; i < _countof(object3ds); i++)
	{
		//初期化
		InitializeObject3d(&object3ds[i], directXCom->GetDevice());

		//ここから親子構造のサンプル
		//先頭以外なら
		if (i>0)
		{
			//1つ前のオブジェクトを親に設定
			//object3ds[i].parent=&object3ds[i-1];
			//親オブジェクトの9割の大きさ
			object3ds[i].scale = { 0.9f,0.9f, 0.9f };
			//親オブジェクトに対して30度Z軸周りに回転
			object3ds[i].rotation = { 0.0f,0.0f,XMConvertToRadians(30.0f) };
			//親オブジェクトに対してz方向に-8.0ずらす
			object3ds[i].position = { 0.0f,0.0f,-8.0f };
		}

	}

#pragma	region	
	
#pragma	endregion
	//constMapTransform->mat.r[0].m128_f32[0] = 2.0f / 1280;
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / 720;
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	//並行投影行列の計算
	object3ds->constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0, WinApp::window_width,
		WinApp::window_height, 0,
		0.0f, 1.0f);
	//射影変換行列(透視投影)
	XMMATRIX	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)WinApp::window_width / WinApp::window_height,//アスペクト比
		0.1f, 1000.0f						//前端、奥端
	);
	//ビュー変換行列
	XMMATRIX	matView;
	XMFLOAT3	eye(0, 0, -100);//視点座標
	XMFLOAT3	target(0, 0, 0);//注視点座標
	XMFLOAT3	up(0, 1, 0);	//上方向ベクトル
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

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
	ComPtr < ID3D12Resource>indexBuff = nullptr;
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
	D3D12_INDEX_BUFFER_VIEW	ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
	//タイトル
#pragma	region 一枚目

	//テクスチャ読み込み
	TexMetadata	metadata{};
	ScratchImage	scratchImg{};

	result = LoadFromWICFile(
		L"Resources/title.png",
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage	mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	metadata.format = MakeSRGB(metadata.format);

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
	ComPtr < ID3D12Resource> texBuff = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
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

	//srvの最大個数
	const	size_t	kMaxSRVCount = 2056;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
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
	directXCom->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
#pragma	endregion
	//プレイヤー
#pragma	region	二枚目
	TexMetadata	metadata2{};
	ScratchImage	scratchImg2{};

	result = LoadFromWICFile(
		L"Resources/player.png",
		WIC_FLAGS_NONE,
		&metadata2, scratchImg2);

	result = GenerateMipMaps(
		scratchImg2.GetImages(), scratchImg2.GetImageCount(), scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImg2 = std::move(mipChain);
		metadata2 = scratchImg2.GetMetadata();
	}

	metadata2.format = MakeSRGB(metadata2.format);

	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;
	textureResourceDesc2.Height = (UINT16)metadata2.height;
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

	//2枚目
	ComPtr < ID3D12Resource> texBuff2 = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));

	//2枚目
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img2 = scratchImg2.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img2->pixels,		 //元データアドレス
			(UINT)img2->rowPitch, //１ラインサイズ
			(UINT)img2->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}
	//1つハンドルを進める
	UINT	incrementSize = directXCom->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc2{};		//設定構造体
	srvDesc2.Format = textureResourceDesc2.Format;//RGBA	float
	srvDesc2.Shader4ComponentMapping =D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);
#pragma	endregion
	//プレイヤーの弾
#pragma	region	三枚目
	TexMetadata	metadata3{};
	ScratchImage	scratchImg3{};

	result = LoadFromWICFile(
		L"Resources/black.png",
		WIC_FLAGS_NONE,
		&metadata3, scratchImg3);

	result = GenerateMipMaps(
		scratchImg3.GetImages(), scratchImg3.GetImageCount(), scratchImg3.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImg3 = std::move(mipChain);
		metadata3 = scratchImg3.GetMetadata();
	}

	metadata3.format = MakeSRGB(metadata3.format);

	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc3{};
	textureResourceDesc3.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc3.Format = metadata3.format;
	textureResourceDesc3.Width = metadata3.width;
	textureResourceDesc3.Height = (UINT16)metadata3.height;
	textureResourceDesc3.DepthOrArraySize = (UINT16)metadata3.arraySize;
	textureResourceDesc3.MipLevels = (UINT16)metadata3.mipLevels;
	textureResourceDesc3.SampleDesc.Count = 1;

	//3枚目
	ComPtr < ID3D12Resource> texBuff3 = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff3));

	//3枚目
	for (size_t i = 0; i < metadata3.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img3 = scratchImg3.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff3->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img3->pixels,		 //元データアドレス
			(UINT)img3->rowPitch, //１ラインサイズ
			(UINT)img3->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}
	//1つハンドルを進める
	srvHandle.ptr += incrementSize;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc3{};		//設定構造体
	srvDesc3.Format = textureResourceDesc3.Format;//RGBA	float
	srvDesc3.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc3.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc3.Texture2D.MipLevels = textureResourceDesc3.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff3.Get(), &srvDesc3, srvHandle);
#pragma	endregion
	//敵
#pragma	region	四枚目
	TexMetadata	metadata4{};
	ScratchImage	scratchImg4{};

	result = LoadFromWICFile(
		L"Resources/enemy.png",
		WIC_FLAGS_NONE,
		&metadata4, scratchImg4);

	result = GenerateMipMaps(
		scratchImg4.GetImages(), scratchImg4.GetImageCount(), scratchImg4.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImg4 = std::move(mipChain);
		metadata4 = scratchImg3.GetMetadata();
	}

	metadata4.format = MakeSRGB(metadata4.format);

	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc4{};
	textureResourceDesc4.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc4.Format = metadata4.format;
	textureResourceDesc4.Width = metadata4.width;
	textureResourceDesc4.Height = (UINT16)metadata4.height;
	textureResourceDesc4.DepthOrArraySize = (UINT16)metadata4.arraySize;
	textureResourceDesc4.MipLevels = (UINT16)metadata4.mipLevels;
	textureResourceDesc4.SampleDesc.Count = 1;

	//枚目
	ComPtr < ID3D12Resource> texBuff4 = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc4,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff4));

	//枚目
	for (size_t i = 0; i < metadata4.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img4 = scratchImg4.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff4->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img4->pixels,		 //元データアドレス
			(UINT)img4->rowPitch, //１ラインサイズ
			(UINT)img4->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}
	//1つハンドルを進める
	srvHandle.ptr += incrementSize;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc4{};		//設定構造体
	srvDesc4.Format = textureResourceDesc4.Format;//RGBA	float
	srvDesc4.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc4.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc4.Texture2D.MipLevels = textureResourceDesc4.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff4.Get(), &srvDesc4, srvHandle);
#pragma	endregion
	//敵弾
#pragma	region	五枚目
	TexMetadata	metadata5{};
	ScratchImage	scratchImg5{};

	result = LoadFromWICFile(
		L"Resources/enemyB.png",
		WIC_FLAGS_NONE,
		&metadata5, scratchImg5);

	result = GenerateMipMaps(
		scratchImg5.GetImages(), scratchImg5.GetImageCount(), scratchImg5.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImg5 = std::move(mipChain);
		metadata5 = scratchImg5.GetMetadata();
	}

	metadata5.format = MakeSRGB(metadata5.format);

	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc5{};
	textureResourceDesc5.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc5.Format = metadata5.format;
	textureResourceDesc5.Width = metadata5.width;
	textureResourceDesc5.Height = (UINT16)metadata5.height;
	textureResourceDesc5.DepthOrArraySize = (UINT16)metadata5.arraySize;
	textureResourceDesc5.MipLevels = (UINT16)metadata5.mipLevels;
	textureResourceDesc5.SampleDesc.Count = 1;

	//枚目
	ComPtr < ID3D12Resource> texBuff5 = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc5,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff5));

	//枚目
	for (size_t i = 0; i < metadata5.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img5 = scratchImg5.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff5->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img5->pixels,		 //元データアドレス
			(UINT)img5->rowPitch, //１ラインサイズ
			(UINT)img5->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}
	//1つハンドルを進める
	srvHandle.ptr += incrementSize;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc5{};		//設定構造体
	srvDesc5.Format = textureResourceDesc5.Format;//RGBA	float
	srvDesc5.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc5.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc5.Texture2D.MipLevels = textureResourceDesc5.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff5.Get(), &srvDesc5, srvHandle);
#pragma	endregion
	//クリア
#pragma	region	六枚目
	TexMetadata	metadata6{};
	ScratchImage	scratchImg6{};

	result = LoadFromWICFile(
		L"Resources/clear.png",
		WIC_FLAGS_NONE,
		&metadata6, scratchImg6);

	result = GenerateMipMaps(
		scratchImg6.GetImages(), scratchImg6.GetImageCount(), scratchImg6.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImg6 = std::move(mipChain);
		metadata6 = scratchImg6.GetMetadata();
	}

	metadata6.format = MakeSRGB(metadata6.format);

	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc6{};
	textureResourceDesc6.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc6.Format = metadata6.format;
	textureResourceDesc6.Width = metadata6.width;
	textureResourceDesc6.Height = (UINT16)metadata6.height;
	textureResourceDesc6.DepthOrArraySize = (UINT16)metadata6.arraySize;
	textureResourceDesc6.MipLevels = (UINT16)metadata6.mipLevels;
	textureResourceDesc6.SampleDesc.Count = 1;

	//枚目
	ComPtr < ID3D12Resource> texBuff6 = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc6,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff6));

	//枚目
	for (size_t i = 0; i < metadata6.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img6 = scratchImg6.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff6->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img6->pixels,		 //元データアドレス
			(UINT)img6->rowPitch, //１ラインサイズ
			(UINT)img6->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}
	//1つハンドルを進める
	srvHandle.ptr += incrementSize;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc6{};		//設定構造体
	srvDesc6.Format = textureResourceDesc6.Format;//RGBA	float
	srvDesc6.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc6.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc6.Texture2D.MipLevels = textureResourceDesc6.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff6.Get(), &srvDesc6, srvHandle);
#pragma	endregion
	//クリア
#pragma	region	七枚目
	TexMetadata	metadata7{};
	ScratchImage	scratchImg7{};

	result = LoadFromWICFile(
		L"Resources/over.png",
		WIC_FLAGS_NONE,
		&metadata7, scratchImg7);

	result = GenerateMipMaps(
		scratchImg7.GetImages(), scratchImg7.GetImageCount(), scratchImg7.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImg7 = std::move(mipChain);
		metadata7 = scratchImg7.GetMetadata();
	}

	metadata7.format = MakeSRGB(metadata7.format);

	//リソース設定
	D3D12_RESOURCE_DESC	textureResourceDesc7{};
	textureResourceDesc7.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc7.Format = metadata7.format;
	textureResourceDesc7.Width = metadata7.width;
	textureResourceDesc7.Height = (UINT16)metadata7.height;
	textureResourceDesc7.DepthOrArraySize = (UINT16)metadata7.arraySize;
	textureResourceDesc7.MipLevels = (UINT16)metadata7.mipLevels;
	textureResourceDesc7.SampleDesc.Count = 1;

	//枚目
	ComPtr < ID3D12Resource> texBuff7 = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc7,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff7));

	//枚目
	for (size_t i = 0; i < metadata7.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const	Image* img7 = scratchImg7.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff7->WriteToSubresource(
			(UINT)i,
			nullptr,			 //全領域へコピー
			img7->pixels,		 //元データアドレス
			(UINT)img7->rowPitch, //１ラインサイズ
			(UINT)img7->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}
	//1つハンドルを進める
	srvHandle.ptr += incrementSize;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc7{};		//設定構造体
	srvDesc7.Format = textureResourceDesc7.Format;//RGBA	float
	srvDesc7.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc7.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc7.Texture2D.MipLevels = textureResourceDesc7.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXCom->GetDevice()->CreateShaderResourceView(texBuff7.Get(), &srvDesc7, srvHandle);
#pragma	endregion

#pragma region シェーダ


	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}


	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}, // (1行で書いたほうが見やすい)
		{//法線ベクトル
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}, // (1行で書いたほうが見やすい)
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に


	// ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];

#pragma	region	ブレンド
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	//減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	//色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	#pragma	endregion

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE	descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;                                              //ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER	rootParams[3] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor.ShaderRegister = 0;
	rootParams[0].Descriptor.RegisterSpace = 0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].Descriptor.ShaderRegister = 1;
	rootParams[2].Descriptor.RegisterSpace = 0;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	// ルートシグネチャ
	ComPtr < ID3D12RootSignature> rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = directXCom->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();


	// パイプランステートの生成
	ComPtr < ID3D12PipelineState> pipelineState = nullptr;
	result = directXCom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion
#pragma endregion
	float	angle = 0.0f;
	XMFLOAT3	position0 = { 0.0f,0.0f, 0.0f };
	XMFLOAT3	position1 = { -20.0f,0.0f, 0.0f };
	float	rotation[2] = { 0,0 };
	float	size[2] = { 1.0f,1.0f };

//--------------------------------------------//
	//変数の初期化
	scene = Scene::title;
	movie = Movie::appearance;
	eye = { 0, 0, -100 };//視点座標
	target = { 0, 0, 0 };//注視点座標
	up = { 0, 1, 0 };	//上方向ベクトル
	points = { start,start,p2,p3,p4,end,end };

	while (true)
	{
#pragma region メッセージ
		if (winApp->ProcessMessage()) {
			break;
		}
#pragma endregion

#pragma region 毎フレーム処理

		input->Update();

		//数字の0キーが押されてたら
		if (input->TriggerKey(DIK_0))
		{
			OutputDebugStringA("Hit 0\n");//出力ウィンドウに表示
		}
		////視点の回転
		//if (input->PushKey(DIK_U)|| input->PushKey(DIK_I))
		//{
		//	if (input->PushKey(DIK_I)) { angle += XMConvertToRadians(1.0f); }
		//	else if (input->PushKey(DIK_U)) { angle -= XMConvertToRadians(1.0f); }
		//
		//	//angleラジアンだけy軸周りに回転（半径は-100）
		//	eye.x = -100 * sinf(angle);
		//	eye.z = -100 * cosf(angle);
		//	
		//	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		//}
		////オブジェクトの移動
		//if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		//{
		//	//
		//	if (input->PushKey(DIK_UP)) { object3ds[0].position.y += 1.0f; }
		//	else if (input->PushKey(DIK_DOWN)) { object3ds[0].position.y -= 1.0f; }
		//	if (input->PushKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
		//	else if (input->PushKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }
		//	
		//}
		for (size_t i = 0; i < _countof(object3ds); i++)
		{
			UpdateObject3d(&object3ds[i], matView, matProjection);
		}
//-----------------------------------------更新-----------------------------------------------------------------		
		switch (scene)
		{
		case	Scene::title:
			object3ds[0].scale.x = 5.0f;
			object3ds[0].scale.y = 3.0f;
			object3ds[0].position.z = -50.0f;

			if (input->TriggerKey(DIK_SPACE))
			{
				//シーン切り替え
				scene = Scene::play;
				//初期化処理
				PlayerInitialize(directXCom->GetDevice());
				hpP = 3;
				switchTimer = 0.0f;
				timeRate = 0.0f;
				timerTrans = 0.0f;
				movie = Movie::appearance;
				enemy.position = { 0,0,28 };
				player.position = { 0,0,-15 };
			}

			break;
		case	Scene::play:
			//当たり判定
			CheckAllCollisions();
			if (movie == Movie::nonMovie)
			{
				eye = { 0,50,0 };
				target = { 0,-1,1 };

				//プレイヤー
				if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
				{
					//移動処理
					if (input->PushKey(DIK_UP)) { player.position.z += 1.0f; }
					else if (input->PushKey(DIK_DOWN)) { player.position.z -= 1.0f; }
					if (input->PushKey(DIK_RIGHT)) { player.position.x += 1.0f; }
					else if (input->PushKey(DIK_LEFT)) { player.position.x -= 1.0f; }

				}
				if (input->PushKey(DIK_SPACE))
				{
					//発射処理
				}
			}
			else
			{
				target = { 0,0,28 };
				timerTrans++;
				timeRate = timerTrans / timerTransMax;
				if (timeRate >= 1.0f)
				{
					if (startIndex < points.size() - 3)
					{
						startIndex++;
						timerTrans -= timerTransMax;
						timeRate -= 1.0f;
					}
					else
					{
						timerTrans = timerTransMax;
						timeRate = 1.0f;
					}
				}
				eye = splinePosition(points, startIndex, timeRate);
				if (eye.x == end.x &&eye.y == end.y &&eye.z == end.z)
				{
					switchTimer++;
					if (switchTimer==50)
					{
						movie = Movie::nonMovie;
					}
				}
			}
			if (input->TriggerKey(DIK_B))
			{
				scene = Scene::clear;
			}
			if (input->TriggerKey(DIK_V))
			{
				scene = Scene::over;
			}

			UpdateObject3d(&player, matView, matProjection);
			UpdateObject3d(&enemy, matView, matProjection);
			break;
		default:
			//ビュー変換行列
			eye = { 0, 0, -100 };//視点座標
			target={0, 0, 0};//注視点座標
			up={0, 1, 0};	//上方向ベクトル
			if (input->TriggerKey(DIK_SPACE))
			{
				scene = Scene::title;
			}
			break;
		}
		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));


	
		//Direct毎フレーム処理　ここから
		directXCom->PreDraw();
		constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// 4.描画コマンドここから
		// パイプラインステートとルートシグネチャの設定コマンド
		directXCom->GetCommondList()->SetPipelineState(pipelineState.Get());
		directXCom->GetCommondList()->SetGraphicsRootSignature(rootSignature.Get());

		// プリミティブ形状の設定コマンド
		directXCom->GetCommondList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		directXCom->GetCommondList()->IASetVertexBuffers(0, 1, &vbView);

		//定数バッファビュー（CBV）の設定コマンド
		directXCom->GetCommondList()->SetGraphicsRootConstantBufferView(0, constBffMarerial->GetGPUVirtualAddress());
		//SRVヒープの設定コマンド
		directXCom->GetCommondList()->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
		//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE	srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		//SRVヒープの先頭にあるSRVをルートパラメータ１番に設定
		/*if (input->PushKey(DIK_SPACE))
		{
			srvGpuHandle.ptr += incrementSize;
		}*/
		//srvGpuHandle.ptr += incrementSize;
		directXCom->GetCommondList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		//インディックスバッファビューの設定コマンド
		directXCom->GetCommondList()->IASetIndexBuffer(&ibView);
		
		//全オブジェクトについて処理
		/*for (int i = 0; i < _countof(object3ds); i++)
		{
			DrawObject3d(&object3ds[i], directXCom->GetCommandList(), vbView, ibView, _countof(indices));
		}*/
//-------------------------------------------------描画-----------------------------------------------------
		switch (scene)
		{
		case Scene::title:
			//SRVヒープの先頭にあるSRVをルートパラメータの1番に設定
			directXCom->GetCommondList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			DrawObject3d(&object3ds[0], directXCom->GetCommondList(), vbView, ibView, _countof(indices));
			break;
		case Scene::play:
			srvGpuHandle.ptr += incrementSize;
			//SRVヒープの先頭にあるSRVをルートパラメータの1番に設定
			directXCom->GetCommondList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
			DrawObject3d(&player, directXCom->GetCommondList(), vbView, ibView, _countof(indices));

			srvGpuHandle.ptr += incrementSize;
			//SRVヒープの先頭にあるSRVをルートパラメータの1番に設定
			//dxCommon->GetCommondList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
			////敵キャラの描画
			//for (const std::unique_ptr<Enemy>& enemy : maneger->enemys_) {
			//	enemy->Draw(dxCommon->GetCommondList(), vbView, ibView, _countof(indices));
			//}
			////敵の弾の描画
			//for (std::unique_ptr<EnemyBullet>& bullet : maneger->bullets_) {
			//	bullet->Draw(dxCommon->GetCommondList(), vbView, ibView, _countof(indices));
			//}
			break;
		case Scene::clear:
			srvGpuHandle.ptr += incrementSize * 5;
			//SRVヒープの先頭にあるSRVをルートパラメータの1番に設定
			directXCom->GetCommondList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
			//シーン描画
			DrawObject3d(&object3ds[0], directXCom->GetCommondList(), vbView, ibView, _countof(indices));
			break;
		case Scene::over:
			srvGpuHandle.ptr += incrementSize * 6;
			//SRVヒープの先頭にあるSRVをルートパラメータの1番に設定
			directXCom->GetCommondList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
			//シーン描画
			DrawObject3d(&object3ds[0], directXCom->GetCommondList(), vbView, ibView, _countof(indices));
			break;
		}


		//描画終了
		directXCom->PostDraw();

		//Direct毎フレーム処理　ここまで
#pragma endregion
		if (input->PushKey( DIK_ESCAPE))
		{
			break;
		}
	}
	

	winApp->Finalize();
	delete	input;
	delete winApp;
	delete	directXCom;
}