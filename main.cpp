#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex/DirectXTex.h"
#include<cassert>
#include<vector>
#include"imgui/imgui.h"


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
#include"ImguiManager.h"
#include"SoundManager.h"
#include "ParticleManager.h"
#include"Camera.h"

//bgm
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using	namespace Microsoft::WRL;



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
	//SoundManager::CreateAudio();

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
	//カメラ初期化
	std::unique_ptr<Camera>camera = std::make_unique<Camera>();
	camera->Initialeze();
	//一度しか宣言しない
	Object3d::StaticInitialize(directXCom->GetDevice(), WinApp::window_width, WinApp::window_height,camera.get());
	//スプライト
	Sprite* sprite = new	Sprite();
	sprite->Initialize(spriteCommon, 1);
	sprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	sprite->SetPosition(XMFLOAT2(100.0f, 100.0f));
	sprite->SetSize(XMFLOAT2(100.0f, 100.0f));
	Sprite* sprite2 = new	Sprite();
	sprite2->Initialize(spriteCommon, 2);
	sprite2->SetPosition(XMFLOAT2(50.0f, 50.0f));
	sprite2->SetColor(XMFLOAT4(0.1f, 0.0f, 0.0f, 0.5f));
	sprite2->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	//sprite2->SetIsFlipX(true);
	//sprite2->SetIsFlipY(true);

	sprite2->SetSize(XMFLOAT2(100.0f, 100.0f));
	//モデル
	Model* model = Model::LoadFromOBJ("triangle_mat");
	Model* model2 = Model::LoadFromOBJ("box_mat");
	//3dオブジェクト生成
	Object3d* obj3d = Object3d::Create();
	Object3d* obj3d2 = Object3d::Create();
	//modelクラスをひも付け
	obj3d->SetModel(model);
	obj3d2->SetModel(model2);
	obj3d->SetPosition({ -5,0,-5 });
	obj3d2->SetPosition({ +5,0,+50 });
	//imguiクラス
	//ImguiManager* imguiM = new ImguiManager;
	//imguiM->Initialize(winApp, directXCom);
	//変数
	//float position[2] = {100,100};

	//wav読み込み
	SoundManager* audio = new SoundManager;
	audio->Initialize();
	audio->LoadWave("0321.wav");
	//パーティクル
	ParticleManager::StaticInitialize(directXCom->GetDevice(), camera.get());
	ParticleManager::LoadTexture(1, "effect1.png");
	ParticleManager::LoadTexture(2, "effect2.png");
	ParticleManager* particle = nullptr;
	particle = ParticleManager::Create(1);
	particle->Update();

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
		////imgui関連
		//imguiM->Begin();
		////ここから中身を書いていく
		////デモウィンドウの表示オン
		//ImGui::ShowDemoWindow();
		//ImGui::SliderFloat2("mario",position ,  0.0f, WinApp::window_width);
		//imguiM->End();
		//sprite->SetPosition(XMFLOAT2{ position[0], position[1] });

		{
			XMFLOAT3 eye = camera->GetEye();
			XMFLOAT3 traget = camera->GetTarget();
			//eye.z -= 0.1f;

			camera->SetEye(eye);
			camera->Update();
		}
		//パーティクル
		if (input->TriggerKey(DIK_F))
		{
			//パーティクル
			for (int i = 0; i < 100; i++)
			{
				//XYZ全て[-5.0f,+5.0f]でランダムに分布
				const	float	rnd_pos = 20.0f;
				XMFLOAT3	pos{};
				pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
				pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
				pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

				//XYZ全て[-0.05f,+0.05f]でランダムに分布
				const	float	rnd_vel = 0.1f;
				XMFLOAT3	vel{};
				vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
				vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

				//重力に見立ててYのみ[-0.001f,0]でランダムに分布
				const	float	rnd_acc = 0.001f;
				XMFLOAT3	acc{};
				acc.y = (float)rand() / RAND_MAX * rnd_acc;

				particle->Add(100, pos, vel, acc, 1.0f, 0.0f);
			}
		}
		//テクスチャの切り替え
		//一枚目
		if (input->TriggerKey(DIK_V))
		{
			particle->SetTexIndex(1);
		}
		//二枚目
		if (input->TriggerKey(DIK_C))
		{
			particle->SetTexIndex(2);
		}
		//数字の0キーが押されてたら
		if (input->TriggerKey(DIK_0))
		{
			//音声再生
			audio->PlayWave("0321.wav");
			
			//OutputDebugStringA("Hit 0\n");//出力ウィンドウに表示
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
		obj3d->Update();
		obj3d2->Update();
		particle->Update();

		//-------------------描画処理-------------------
		//Direct毎フレーム処理　ここから
		directXCom->PreDraw();
		Object3d::PreDraw(directXCom->GetCommandList());
		obj3d->Draw();
		obj3d2->Draw();
		Object3d::PostDraw();
		// 3Dオブジェクト描画前処理
		ParticleManager::PreDraw(directXCom->GetCommandList());

		// 3Dオブクジェクトの描画
		
		particle->Draw();

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		ParticleManager::PostDraw();
		//sprite->SetIsInvisible(true);
		sprite->SetTexIndex(1);
		sprite2->SetTexIndex(2);
		sprite->Draw();
		sprite2->Draw();

		//imgui
		//imguiM->Draw();

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
	//imguiM->Finalize();
	audio->Finalize();
	delete	input;
	delete winApp;
	delete	directXCom;
	delete	spriteCommon;
	delete	sprite;
	delete model;
	delete model2;
	delete obj3d;
	delete obj3d2;
	//delete imguiM;
	delete audio;
	

#pragma	endregion
	return 0;
}