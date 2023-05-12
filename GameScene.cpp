#include "GameScene.h"

void GameScene::Initialize() {
	winApp = new	WinApp;
	winApp->Initialize();

	dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);

	input = new	Input;
	input->Initialize(winApp);
	//---------------------------2D----------------------------------

	//スプライト共通部分の初期化
	spriteCommon = new	SpriteCommon;
	spriteCommon->Initialize(dxCommon);
	spriteCommon->Loadtexture(1, "mario.jpg");
	spriteCommon->Loadtexture(2, "tex.png");
	mario = new Sprite;
	mario->Initialize(spriteCommon, 1);
	mario->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	mario->SetPosition(XMFLOAT2(100.0f, 100.0f));
	mario->SetSize(XMFLOAT2(100.0f, 100.0f));

	flies = new Sprite;
	flies->Initialize(spriteCommon, 2);
	flies->SetPosition(XMFLOAT2(50.0f, 50.0f));
	flies->SetColor(XMFLOAT4(0.1f, 0.0f, 0.0f, 0.5f));
	flies->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	flies->SetSize(XMFLOAT2(100.0f, 100.0f));
	
	//imgui
	imguiM = new ImguiManager;
	imguiM->Initialize(winApp, dxCommon);

	//---------------------------3D----------------------------------

	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	//モデル読み込み
	Model::SetDevice(dxCommon->GetDevice());
	model = Model::LoadFromOBJ("triangle_mat");
	model2 = Model::LoadFromOBJ("box_mat");
	//オブジェクト
	//静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height, camera.get());
	triangle = Object3d::Create();
	square = Object3d::Create();
	//modelクラスをひも付け
	triangle->SetModel(model);
	square->SetModel(model2);
	
	triangle->SetPosition({ -5,0,-5 });
	square->SetPosition({ +5,0,+50 });

	//パーティクル
	ParticleManager::StaticInitialize(dxCommon->GetDevice(), camera.get());
	ParticleManager::LoadTexture(1, "effect1.png");
	ParticleManager::LoadTexture(2, "effect2.png");
	particle = ParticleManager::Create(1);
	particle->Update();

	//音
	audio = new SoundManager;
	audio->Initialize();
	audio->LoadWave("Alarm01.wav");
}

void GameScene::Update(){
#pragma region メッセージ
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEnd = true;
	}
#pragma endregion
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
		camera->SetTarget({ eye.x, eye.y, 0 });
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
		XMFLOAT3 pos = triangle->GetRotation();
		pos.y += 1.0f;
		triangle->SetRotation(pos);
		square->SetRotation(pos);
	}


	triangle->Update();
	square->Update();
	particle->Update();
}

void GameScene::Draw(){
	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	triangle->Draw();
	square->Draw();
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//パーティクル描画
	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//particle->Draw();
	ParticleManager::PostDraw();
	
	//スプライト描画
	spriteCommon->PreDraw();
	mario->SetTexIndex(1);
	flies->SetTexIndex(2);
	mario->Draw();
	flies->Draw();
	spriteCommon->PostDraw();
	//imgui
	//imguiM->Draw();

	dxCommon->PostDraw();
}

void GameScene::Finalize(){
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
	delete model2;
	delete triangle;
	delete square;
	delete imguiM;
	delete audio;
}