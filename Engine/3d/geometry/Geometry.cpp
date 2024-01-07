/**
 * @file Geometry.cpp
 * @brief　モデル操作
 */

#include "Geometry.h"
#include <d3dcompiler.h>
#pragma warning( push )
#pragma warning( disable : 4828 )
#include<DirectXTex.h>
#pragma warning( pop)
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cassert>
#include<Easing.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

XMMATRIX	Geometry::matBillboard = XMMatrixIdentity();
XMMATRIX	Geometry::matBillboardY = XMMatrixIdentity();

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float Geometry::radius = 5.0f;				// 底面の半径
const float Geometry::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* Geometry::device = nullptr;
UINT Geometry::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Geometry::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Geometry::rootsignature;
ComPtr<ID3D12PipelineState> Geometry::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Geometry::descHeap;
ComPtr<ID3D12Resource> Geometry::vertBuff;
ComPtr<ID3D12Resource> Geometry::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE Geometry::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Geometry::gpuDescHandleSRV;
D3D12_VERTEX_BUFFER_VIEW Geometry::vbView{};
Geometry::VertexPos Geometry::vertices[ vertexCount ];
XMMATRIX Geometry::matView;
Camera* Geometry::camera = nullptr;

//XMFLOAT3同士の加算処理
const	DirectX::XMFLOAT3	operator+(const	DirectX::XMFLOAT3& lhs,const DirectX::XMFLOAT3& rhs) {
	XMFLOAT3	result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;

	return	result;
}


void	Geometry::Add(int8_t life,XMFLOAT3	position,XMFLOAT3	velocity,XMFLOAT3	accel,float	start_scale,float	end_scale,
					  XMFLOAT4 startColor_ ,XMFLOAT4 endColor_) {
	//リストに追加
	particles.emplace_front();
	//参照
	Particle& p = particles.front();
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.endFrame = life;
	p.startScale = start_scale;
	p.endScale = end_scale;
	p.startColor = startColor_;
	p.endColor = endColor_;
}

void Geometry::StaticInitialize(ID3D12Device* device_)
{
	// nullptrチェック
	assert(device_);

	Geometry::device = device_;

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// テクスチャ読み込み
	LoadTexture();

	// モデル生成
	CreateModel();

}

void Geometry::PreDraw(ID3D12GraphicsCommandList* cmdList_)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Geometry::cmdList == nullptr);

	// コマンドリストをセット
	Geometry::cmdList = cmdList_;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Geometry::PostDraw()
{
	// コマンドリストを解除
	Geometry::cmdList = nullptr;
}

Geometry* Geometry::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Geometry* geometry = new Geometry();
	if ( geometry == nullptr )
	{
		return nullptr;
	}

	// 初期化
	if ( !geometry->Initialize() )
	{
		delete geometry;
		assert(0);
		return nullptr;
	}

	return geometry;
}

void Geometry::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc,IID_PPV_ARGS(&descHeap));//生成
	if ( FAILED(result) )
	{
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void Geometry::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePs.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[ ] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//スケール
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//色
			"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[ 0 ] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[ 0 ] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[ 2 ];
	rootparams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 1 ].InitAsDescriptorTable(1,&descRangeSRV,D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),rootparams,1,&samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,&errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

void Geometry::LoadTexture()
{
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WICテクスチャのロード
	result = LoadFromWICFile(L"Resources/white1.png",WIC_FLAGS_NONE,&metadata,scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),scratchImg.GetImageCount(),scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,0,mipChain);
	if ( SUCCEEDED(result) )
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,metadata.width,( UINT ) metadata.height,( UINT16 ) metadata.arraySize,
		( UINT16 ) metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&heapProps,D3D12_HEAP_FLAG_NONE,&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	for ( size_t i = 0; i < metadata.mipLevels; i++ )
	{
		const Image* img = scratchImg.GetImage(i,0,0); // 生データ抽出
		result = texbuff->WriteToSubresource(
			( UINT ) i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			( UINT ) img->rowPitch,  // 1ラインサイズ
			( UINT ) img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),0,descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(),0,descriptorHandleIncrementSize);

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

void Geometry::CreateModel()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast< UINT >( sizeof(vertices) );

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps,D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	//VertexPosNormalUv* vertMap = nullptr;
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0,nullptr,( void** ) &vertMap);
	if ( SUCCEEDED(result) )
	{
		memcpy(vertMap,vertices,sizeof(vertices));
		vertBuff->Unmap(0,nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[ 0 ]);
}

void Geometry::UpdateViewMatrix()
{
	//視点座標
	XMVECTOR	eyePosition = XMLoadFloat3(&camera->GetEye());

	//注視点座標
	XMVECTOR	targetPosition = XMLoadFloat3(&camera->GetTarget());

	//(仮の)上方向
	XMVECTOR	upVector = XMLoadFloat3(&camera->GetUp());

	//カメラZ軸（視線方向）
	XMVECTOR	cameraAxisZ = XMVectorSubtract(targetPosition,eyePosition);

	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ,XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector,XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化	
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	XMVECTOR	cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector,cameraAxisZ);
	//ベクトルの正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸(右方向)
	XMVECTOR	cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ,cameraAxisX);
	//ベクトルの正規化
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//カメラ回転行列
	XMMATRIX	matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[ 0 ] = cameraAxisX;
	matCameraRot.r[ 1 ] = cameraAxisY;
	matCameraRot.r[ 2 ] = cameraAxisZ;
	matCameraRot.r[ 3 ] = XMVectorSet(0,0,0,1);

	//転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	//視点座標に-1をかけた座標
	XMVECTOR	reverEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル(カメラ座標系)
	XMVECTOR	tX = XMVector3Dot(matCameraRot.r[ 0 ],reverEyePosition);
	XMVECTOR	tY = XMVector3Dot(matCameraRot.r[ 1 ],reverEyePosition);
	XMVECTOR	tZ = XMVector3Dot(matCameraRot.r[ 2 ],reverEyePosition);

	//1つのベクトルにまとめる
	XMVECTOR	transIation = XMVectorSet(tX.m128_f32[ 0 ],tY.m128_f32[ 1 ],tZ.m128_f32[ 2 ],1.0f);

	//ビュー行列に平行移動成分を設定
	matView.r[ 3 ] = transIation;

#pragma	region	全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard.r[ 0 ] = cameraAxisX;
	matBillboard.r[ 1 ] = cameraAxisY;
	matBillboard.r[ 2 ] = cameraAxisZ;
	matBillboard.r[ 3 ] = XMVectorSet(0,0,0,1);
#pragma	endregion
#pragma	region	Y軸周りのビルボード行列の計算
	//カメラX,Y,Z軸
	XMVECTOR	ybillCameraAxisX,ybillCameraAxisY,ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX,cameraAxisY);

	//Y軸周りのビルボード行列
	matBillboardY.r[ 0 ] = ybillCameraAxisX;
	matBillboardY.r[ 1 ] = ybillCameraAxisY;
	matBillboardY.r[ 2 ] = ybillCameraAxisZ;
	matBillboardY.r[ 3 ] = XMVectorSet(0,0,0,1);

#pragma	endregion
}

bool Geometry::Initialize()
{
	// nullptrチェック
	assert(device);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(( sizeof(ConstBufferData) + 0xff ) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	return true;
}

void Geometry::Update()
{
	HRESULT result;
	//XMMATRIX matScale,matRot,matTrans;
	UpdateViewMatrix();

	//パーティクルの削除
	particles.remove_if([ ] (Particle& x)
 {
	 return	x.frame >= x.endFrame;
		});
	//パーティクルの更新
	for ( std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end(); it++ )
	{
		//フレームのカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;
		//スケールの線形補間
		float	f = ( float ) it->frame / it->endFrame;
		it->scale = ( it->endScale - it->startScale ) * f;
		it->scale += it->startScale;
		//色の線形補間
		it->color.x = it->startColor.x +( it->endColor.x - it->startColor.x ) * Easing::easeOutQuint( f);
		it->color.y = it->startColor.y +( it->endColor.y - it->startColor.y ) * Easing::easeOutQuint( f);
		it->color.z = it->startColor.z +( it->endColor.z - it->startColor.z ) * Easing::easeOutQuint( f);
		it->color.w = it->startColor.w +( it->endColor.w - it->startColor.w ) * Easing::easeInQuint( f);
	}

	//頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0,nullptr,( void** ) &vertMap);
	if ( SUCCEEDED(result) )
	{
		//パーティクルの情報を1つずつ反映
		for ( std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end(); it++ )
		{
			//座標
			vertMap->pos = it->position;
			//スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			//次の頂点
			vertMap++;
		}
		vertBuff->Unmap(0,nullptr);
	}



	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0,nullptr,( void** ) &constMap);
	// 行列の合成
	constMap->mat = matView * camera->GetMatProjection();
	constMap->matBillboard = matBillboard;
	constBuff->Unmap(0,nullptr);
}

void Geometry::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Geometry::cmdList);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0,1,&vbView);
	// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[ ] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0,constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1,gpuDescHandleSRV);
	// 描画コマンド
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
	cmdList->DrawInstanced(( UINT ) std::distance(particles.begin(),particles.end()),1,0,0);
}