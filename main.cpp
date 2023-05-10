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
#include"FbxLoader.h"

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
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);
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
	spriteCommon->Initialize(dxCommon);
	spriteCommon->Loadtexture(1, "mario.jpg");
	spriteCommon->Loadtexture(2, "tex.png");
	
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

#pragma	endregion
#pragma	region	最初のシーンの初期化
	//カメラ初期化
	std::unique_ptr<Camera>camera = std::make_unique<Camera>();
	camera->Initialeze();
	//一度しか宣言しない
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height,camera.get());
	//スプライト
	Sprite* mario = new	Sprite();
	mario->Initialize(spriteCommon, 1);
	mario->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	mario->SetPosition(XMFLOAT2(100.0f, 100.0f));
	mario->SetSize(XMFLOAT2(100.0f, 100.0f));
	Sprite* flies = new	Sprite();
	flies->Initialize(spriteCommon, 2);
	flies->SetPosition(XMFLOAT2(50.0f, 50.0f));
	flies->SetColor(XMFLOAT4(0.1f, 0.0f, 0.0f, 0.5f));
	flies->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	//sprite2->SetIsFlipX(true);
	//sprite2->SetIsFlipY(true);

	flies->SetSize(XMFLOAT2(100.0f, 100.0f));
	//モデル
	Model* model = Model::LoadFromOBJ("sphere");
	//3dオブジェクト生成
	Object3d* square_1 = Object3d::Create();
	Object3d* square_2 = Object3d::Create();
	//modelクラスをひも付け
	square_1->SetModel(model);
	square_2->SetModel(model);
	square_1->SetPosition({ -25,0,10 });
	square_2->SetPosition({ +25,0,10 });
	//imguiクラス
	ImguiManager* imguiM = new ImguiManager;
	imguiM->Initialize(winApp, dxCommon);
	//変数
	float position[2] = {100,100};

	//wav読み込み
	SoundManager* audio = new SoundManager;
	audio->Initialize();
	audio->LoadWave("Alarm01.wav");
	//パーティクル
	ParticleManager::StaticInitialize(dxCommon->GetDevice(), camera.get());
	ParticleManager::LoadTexture(1, "effect1.png");
	ParticleManager::LoadTexture(2, "effect2.png");
	ParticleManager* particle = nullptr;
	particle = ParticleManager::Create(1);
	particle->Update();

	//fbx
	FbxLoader::GetInstance()->LoadModelFromFile("cube");
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
		//imgui関連
		imguiM->Begin();
		//ここから中身を書いていく
		//デモウィンドウの表示オン
		//ImGui::ShowDemoWindow();
		ImGui::SliderFloat2("flies", position, 0.0f, (float)WinApp::window_width);
		imguiM->End();
		flies->SetPosition(XMFLOAT2{ position[0], position[1] });
		//------------------------------
		{
			XMFLOAT3 eye = camera->GetEye();
			if (input->PushKey(DIK_W))
			{
				eye.y += 0.5f;
			}
			else if (input->PushKey(DIK_S))
			{
				eye.y += -0.5f;
			}
			else if (input->PushKey(DIK_D))
			{
				eye.x += 0.5f;
			}
			else if (input->PushKey(DIK_A))
			{
				eye.x += -0.5f;
			}
			camera->SetEye(eye);
			camera->SetTarget({eye.x, eye.y, 0});
			camera->Update();
		}
		//--------------パーティクル--------------
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

		//--------------音声再生-----------------
		if (input->TriggerKey(DIK_0))
		{
			//audio->PlayWave("0321.wav");
			
			//OutputDebugStringA("Hit 0\n");//出力ウィンドウに表示
		}
		//--------------スプライト-----------------

		//回転
		{
			float rotation = mario->GetRotation();
			if (input->PushKey(DIK_O))
			{
				rotation += 10.0f;
			}
			else if (input->PushKey(DIK_P))
			{
				rotation -= 10.0f;
			}
			mario->SetRotation(rotation);
			//sprite2->SetSize(XMFLOAT2(150.0f, 50.0f));
			flies->SetRotation(rotation);
		}
		//座標
		{
			XMFLOAT2 position = mario->GetPosition();
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
			mario->SetPosition(position);
		}
		//---------オブジェクト---------------
		{
			XMFLOAT3 pos = square_1->GetRotation();
			pos.y += 1.0f;
			square_1->SetRotation(pos);
			square_2->SetRotation(pos);
		}


		square_1->Update();
		square_2->Update();
		particle->Update();

		//-------------------描画処理-------------------
		//Direct毎フレーム処理　ここから
		dxCommon->PreDraw();
		Object3d::PreDraw(dxCommon->GetCommandList());
		square_1->Draw();
		square_2->Draw();
		Object3d::PostDraw();
		// 3Dオブジェクト描画前処理
		ParticleManager::PreDraw(dxCommon->GetCommandList());

		// 3Dオブクジェクトの描画
		
		//particle->Draw();

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		ParticleManager::PostDraw();
		//sprite->SetIsInvisible(true);
		mario->SetTexIndex(1);
		flies->SetTexIndex(2);
		mario->Draw();
		flies->Draw();

		//imgui
		//imguiM->Draw();

		dxCommon->PostDraw();

		//Direct毎フレーム処理　ここまで
	#pragma endregion
		if (input->PushKey( DIK_ESCAPE))
		{
			break;
		}
	}
#pragma	region	最初のシーンの終了
	//解放処理
	winApp->Finalize();
	imguiM->Finalize();
	audio->Finalize();
	FbxLoader::GetInstance()->Finalize();

	delete	input;
	delete winApp;
	delete	dxCommon;
	delete	spriteCommon;
	delete	mario;
	delete model;
	delete square_1;
	delete square_2;
	delete imguiM;
	delete audio;
	

#pragma	endregion
	return 0;
}