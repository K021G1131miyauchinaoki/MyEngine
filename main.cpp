#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex/DirectXTex.h"
#include<cassert>
#include<vector>
#include<list>
#include<memory>

#pragma	comment(lib,"d3dcompiler.lib")
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

//自作クラス
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#include<random>

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


float length(XMFLOAT3 vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);}

XMFLOAT3 lens(XMFLOAT3 vec1, XMFLOAT3 vec2) {
	XMFLOAT3 vec;
	vec.x = vec1.x - vec2.x;
	vec.y = vec1.y - vec2.y;
	vec.z=	vec1.z - vec2.z;
	return vec;
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
	spriteCommon->Loadtexture(1, "title.png");
	spriteCommon->Loadtexture(2, "gameover.png");

#pragma	endregion
#pragma	region	最初のシーンの初期化
	//一度しか宣言しない
	Object3d::StaticInitialize(directXCom->GetDevice(), WinApp::window_width, WinApp::window_height);
	//スプライト
	Sprite* spriteTitle = new	Sprite();
	spriteTitle->Initialize(spriteCommon,1);
	Sprite* spriteOver = new	Sprite();
	spriteOver->Initialize(spriteCommon,2);
	//サイズ
	spriteTitle->SetSize({ 1280,720 });
	spriteOver->SetSize({1280,720});
	//座標
	spriteTitle->SetPosition({ 0,0 });
	spriteOver->SetPosition({ 0,0 });
	//モデル
	Model* modelPlayer = Model::LoadFromOBJ("circle");
	Model* modelObstacle = Model::LoadFromOBJ("box");
	//3dオブジェクト生成
	Object3d* player = Object3d::Create();
	std::list<std::unique_ptr<Object3d>>obstacle;
	//= Object3d::Create();
	//modelクラスをひも付け
	player->SetModel(modelPlayer);
	player->SetPosition({ 0,0,-100 });
	//obj3ds->SetModel(model2);
	//obj3ds->SetPosition({ 0,0,+50 });

	//変数
	Object3d::SetEye({0,30,-250});
	Object3d::SetTarget({0,-1,10});
	enum Scene
	{
		title,
		play,
		over,
	};
	int	scene = Scene::title;
	int	timer = 20;
	const float rangeX = 150.0f;
	const float rangeY = 80.0f;
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

		switch (scene)
		{
		//----------タイトル---------------	
		case Scene::title:
			//スペースキーを押したら
			if (input->TriggerKey(DIK_SPACE))
			{
				scene = Scene::play;
				//
				player->SetPosition({ 0,0,-100 });
				//強制消去
				obstacle.remove_if([](std::unique_ptr<Object3d>& obj) { return true; });
				//タイム
				timer = 20;

			}
			break;
		//----------プレイ-----------
		case Scene::play:
		{
			//デスフラグが立ったら消去
			obstacle.remove_if([](std::unique_ptr<Object3d>& obj) { return obj->IsDead(); });
			//当たり判定
			for (const std::unique_ptr<Object3d>& obj : obstacle)
			{
				//プレイヤーの座標
				XMFLOAT3 playerPos = player->GetPosition();
				//一つの障害物の座標
				XMFLOAT3  obstaclePos = obj->GetPosition();
				XMFLOAT3 vecPos = lens(playerPos, obstaclePos);
				float dis = length(vecPos);
				float	radius = 25.0f;
				//当たったら
				if (dis <= radius)
				{
					scene = Scene::over;
				}
			}

			//オブジェクトの生成
			if (timer-- < 0)
			{
				//生成
				std::unique_ptr<Object3d>newObj = std::make_unique<Object3d>();
				//初期化
				newObj->Initialize();
				newObj->SetScale({ 10,10,10 });
				//乱数シード生成器
				std::random_device seed_gen;
				//メルセンヌ・ツイスター
				std::mt19937_64 enigine(seed_gen());
				//x
				std::uniform_real_distribution<float> x(-rangeX, rangeX);
				//y
				std::uniform_real_distribution<float> y(-rangeY, rangeY);
				//位置
				newObj->SetPosition(XMFLOAT3{ x(enigine),y(enigine),50 });
				//モデル
				newObj->SetModel(modelObstacle);
				//登録
				obstacle.push_back(std::move(newObj));
				//タイムのリセット
				timer = 10;
			}

			// オブジェクト移動
			if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
			{
				// 現在の座標を取得
				XMFLOAT3 position = player->GetPosition();

				// 移動後の座標を計算
				if (input->PushKey(DIK_W)) { position.y += 1.0f; }
				else if (input->PushKey(DIK_S)) { position.y -= 1.0f; }
				if (input->PushKey(DIK_D)) { position.x += 1.0f; }
				else if (input->PushKey(DIK_A)) { position.x -= 1.0f; }
			
				//移動範囲の設定
				position.x = max(position.x, -rangeX);
				position.x = min(position.x, +rangeX);
				position.y = max(position.y, -rangeY);
				position.y = min(position.y, +rangeY);
				// 座標の変更を反映
				player->SetPosition(position);
			}
			//プレイヤーの更新
			player->Update();
			//障害物達の更新
			for (std::unique_ptr<Object3d>& obj : obstacle)
			{
				XMFLOAT3 pos = obj->GetPosition();
				pos.z -= 1.0f;
				if (pos.z < -250)
				{
					obj->OnCollision();
				}
				obj->SetPosition(pos);
				obj->Update();
			}
		}
			break;
		//----------ゲームオーバー----------
		case Scene::over:
			//スペースキーを押したら
			if (input->TriggerKey(DIK_SPACE))
			{
				scene = Scene::title;
			}
			break;
		}
		
		//-------------------描画処理-------------------
		//Direct毎フレーム処理　ここから
		directXCom->PreDraw();
		//---------------3D描画-------------------
		Object3d::PreDraw(directXCom->GetCommandList());
		switch (scene)
		{
		case Scene::title:
			break;
		case Scene::play:
			//プレイヤー
			player->Draw();
			//障害物
			for (std::unique_ptr<Object3d>& obj : obstacle)
			{
				obj->Draw();
			}
			break;
		case Scene::over:
			break;
		}
		Object3d::PostDraw();

		//---------------2D描画-------------------
		spriteCommon->PreDraw();
		//sprite->SetIsInvisible(true);
		switch (scene)
		{
		case Scene::title:
			spriteTitle->SetTexIndex(1);
			spriteTitle->Draw();
			break;
		case Scene::play:
			break;
		case Scene::over:
			spriteOver->SetTexIndex(2);
			spriteOver->Draw();				
			break;
		}

		//
		spriteCommon->PostDraw();
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
	delete	spriteTitle;
	delete modelPlayer;
	delete modelObstacle;
	delete player;
	//delete obj3ds;
#pragma	endregion
	return 0;
}