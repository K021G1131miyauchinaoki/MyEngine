#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex/DirectXTex.h"
#include<cassert>
#include<vector>

#pragma	comment(lib,"d3dcompiler.lib")
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

//自作クラス
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"

using namespace DirectX;
using	namespace Microsoft::WRL;

struct ConstBufferDataMaterial
{
	XMFLOAT4	color;//(RGBA)
};

struct ConstBufferDataTransform
{
	XMMATRIX	mat;//３D変換行列
};

struct Vertex
{
	XMFLOAT3	pos;//xyz座標
	XMFLOAT3	normal;//法線ベクトル
	XMFLOAT2	uv;	//uv座標
};
float	R = 1.0f;
float	G = 0.0f;
float	B = 0.0f;

struct Object3d
{
	//定数バッファ（行列用）
	ID3D12Resource* constBuffTransform = 0;
	//定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform = 0;
	//アフィン変換情報
	XMFLOAT3	scale = { 1,1,1 };
	XMFLOAT3	rotation = { 0,0,0 };
	XMFLOAT3	position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX	matWorld{};
	//親オブジェクトへのポインタ
	Object3d* parent = nullptr;
};

void	InitializeObject3d(Object3d* object, ID3D12Device* device)
{
	HRESULT	result;

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES	heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC	resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTransform));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);//マッピング
	assert(SUCCEEDED(result));
}

void	UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matProjection) {
	XMMATRIX	matScale, matRot, matTrans;
	//スケール、回転、平行移動の行列計算
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);
	matTrans = XMMatrixTranslation(object->position.x, object->position.y, object->position.z);

	//行列の合成
	object->matWorld = XMMatrixIdentity();
	object->matWorld *= matScale;
	object->matWorld *= matRot;
	object->matWorld *= matTrans;

	//親オブジェクトがあれば
	if (object->parent != nullptr)
	{
		object->matWorld *= object->parent->matWorld;
	}
	//定数バッファへデータ転送
	object->constMapTransform->mat = object->matWorld * matview * matProjection;
}

void	DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
D3D12_INDEX_BUFFER_VIEW& ibView, UINT	numIndices) {
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}



//windowsアプリでのエントリーポイント(main関数)
int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma	region	基盤システムの初期化
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
	//スプライト
	SpriteCommon* spriteCommon = nullptr;
	//スプライト共通部分の初期化
	spriteCommon = new	SpriteCommon;
	spriteCommon->Initialize(directXCom);
	spriteCommon->Loadtexture(1, "mario.jpg");
	spriteCommon->Loadtexture(2, "tex.png");


#pragma	endregion
#pragma	region	最初のシーンの初期化
	Sprite* sprite = new	Sprite();
	sprite->Initialize(spriteCommon);
	Sprite* sprite2 = new	Sprite();
	sprite2->Initialize(spriteCommon);
	sprite2->SetPosition(XMFLOAT2( 50.0f, 50.0f));
	sprite2->SetColor(XMFLOAT4(0.1f, 0.0f, 0.0f, 0.5f));
	sprite2->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	//sprite2->SetIsFlipX(true);
	//sprite2->SetIsFlipY(true);
	sprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	//変数
#pragma	endregion
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
		//スプライトの回転
		{
			float rotation = sprite->GetRotation();
			if (input->PushKey(DIK_O))
			{
				rotation += 10.0f;
			}
			else if (input->PushKey(DIK_P))
			{
				rotation -= 10.0f;
			}
			sprite->SetRotation(rotation);
			//sprite2->SetSize(XMFLOAT2(150.0f, 50.0f));
			sprite2->SetRotation(rotation);
		}
		//スプライトの座標
		{
			XMFLOAT2 position = sprite->GetPosition();
			if (input->PushKey(DIK_UP))
			{
				position.y -= 10.0f;
			}
			else if (input->PushKey(DIK_DOWN))
			{
				position.y += 10.0f;
			}
			if (input->PushKey(DIK_LEFT))
			{
				position.x -= 10.0f;
			}
			else if (input->PushKey(DIK_RIGHT))
			{
				position.x += 10.0f;

			}
			sprite->SetPosition(position);
		}
		//Direct毎フレーム処理　ここから
		directXCom->PreDraw();
		//sprite->SetIsInvisible(true);
		sprite->SetTexIndex(1);
		sprite2->SetTexIndex(2);
		sprite->Draw();
		sprite2->Draw();

		//
		directXCom->PostDraw();

		//Direct毎フレーム処理　ここまで
	#pragma endregion
		if (input->PushKey( DIK_ESCAPE))
		{
			break;
		}
	}
#pragma	region	最初のシーンの終了
	winApp->Finalize();
	delete	input;
	delete winApp;
	delete	directXCom;
	delete	spriteCommon;
	delete	sprite;
#pragma	endregion
	return 0;
}