#include "GameScene.h"
#define safe_delete(p)  {delete p; p = nullptr;}

void GameScene::Initialize() {
	winApp = new	WinApp;
	winApp->Initialize();

	dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);

	input = new	Input;
	input->Initialize(winApp);
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Model::SetDevice(dxCommon->GetDevice());
	//静的初期化
	//---------------------------2D----------------------------------

	//スプライト共通部分の初期化
	
	//imgui
	

	//---------------------------3D----------------------------------
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,10,-10 });
	camera->Update();
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::width, WinApp::height, camera.get());

	/*---------------- - FBX------------------------*/
	fbxM = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(camera.get());
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	//3Dオブジェクト生成とモデルセット
	fbxObj = new FbxObject3d;
	fbxObj->Initialize();
	fbxObj->SetModel(fbxM);
	
	/*--------------レベルエディタ----------------*/
	// レベルデータの読み込み
	levelData = LevelLoader::LoadJson("testScene");

	// モデル読み込み
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelChr = Model::LoadFromOBJ("chr_sword");
	modelSphere = Model::LoadFromOBJ("sphere");
	
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objChr = Object3d::Create();
	objSphere = Object3d::Create();

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objChr->SetModel(modelChr);
	objSphere->SetModel(modelSphere);
	
	objSkydome->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	objGround->SetPosition(XMFLOAT3(0.0f, 0.0f, -4.8f));
	objChr->SetPosition(XMFLOAT3(-7.5f, 0.0f, 0.0f));
	objSphere->SetPosition(XMFLOAT3(7.0f, 0.0f, 0.0f));


	models.insert(std::make_pair("skydome", modelSkydome));
	models.insert(std::make_pair("ground", modelGround));
	models.insert(std::make_pair("chr_sword", modelChr));
	models.insert(std::make_pair("sphere", modelSphere));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);
		

		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->SetRotation(rot);

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);

		// 配列に登録
		objects.push_back(newObject);
	}

	//spriteCommon->Loadtexture(1,)
}

void GameScene::Update(){
#pragma region メッセージ
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEnd = true;
	}
#pragma endregion
	input->Update();
	//imgui
	// デモ
	//ImGui::ShowDemoWindow();
	
	//------------------------------
	
	objSkydome->Update();
	objGround->Update();
	objChr->Update();
	objSphere->Update();

	fbxObj->Update();

	for (auto object : objects) {
		object->Update();
	}

}

void GameScene::Draw(){
	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//triangle->Draw();
	//square->Draw();
	for (auto object : objects) {
		object->Draw();
	}
	//objSkydome->Draw();
	//objGround->Draw();
	//objChr->Draw();
	//objSphere->Draw();


	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	fbxObj->Draw(dxCommon->GetCommandList());

	//パーティクル描画
	
	//スプライト描画

	//imgui
	//imguiM->Draw();

	dxCommon->PostDraw();
}

void GameScene::Finalize(){
	//解放処理
	winApp->Finalize();
	
	FbxLoader::GetInstance()->Finalize();
	safe_delete(fbxObj);
	safe_delete(fbxM);

	delete	input;
	delete winApp;
	delete	dxCommon;
	delete	spriteCommon;
	delete modelChr;
	delete modelGround;
	delete modelSkydome;
	delete modelSphere;
	/*delete objChr;
	delete	objSkydome;
	delete	objGround;
	delete	objSphere;*/

}