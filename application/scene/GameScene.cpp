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


	//---------------------------3D----------------------------------
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,0,-20 });
	camera->Update();
	Object3d::StaticInitialize(dxCommon->GetDevice(), camera.get());

	/*---------------- - FBX------------------------*/
	fbxM = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
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
	fbxObj->PlayAnimation();
	
	// モデル読み込み
	modelSkydome = Model::LoadFromOBJ("skydome");
	/*modelGround = Model::LoadFromOBJ("ground");
	modelChr = Model::LoadFromOBJ("chr_sword");
	modelSphere = Model::LoadFromOBJ("sphere");*/
	box = Model::LoadFromOBJ("player");

	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome);
	
	/*--------------レベルエディタ----------------*/
	// レベルデータの読み込み
	levelData = LevelLoader::LoadJson("testScene");
	


	aim= std::make_unique<Aimposition>();
	aim->Initialeze(box, input);
	player = std::make_unique<Player>();
	player->Initialeze(box,input,aim.get());

	//ポストエフェクト
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);
	PostEffect::StaticInitialize(dxCommon);
	postEffect = new PostEffect;
	postEffect->Initialize();
}

void GameScene::Update(){
#pragma region メッセージ
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEndRequst = true;
	}
#pragma endregion
	input->Update();
	//imgui
	// デモ
	//ImGui::ShowDemoWindow();
	
	//------------------------------
	

	camera->Update();
	player->Updata();
	aim->Updata();
	objSkydome->Update();
	//float vec[2] = { player->GetVelocity().x,player->GetVelocity().z };
	//float posA[2] = { aim->GetPosition().x,aim->GetPosition().z };
	////imgui関連
	//velocity->Begin();
	////ここから中身を書いていく
	//ImGui::Begin("a");
	//ImGui::SliderFloat2("volocity", vec, 0.0f, static_cast<float>(WinApp::width));
	//ImGui::End();
	//ImGui::SliderFloat2("pos",posA,0.0f, WinApp::width);
	fbxObj->Update();

	//for (auto object : objects) {
	//	object->Update();
	//}
	//postEffect->Update();
}

void GameScene::Draw(){
	postEffect->PreDrawScene(dxCommon->GetCommandList());
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	fbxObj->Draw(dxCommon->GetCommandList());
	postEffect->PostDrawScene(dxCommon->GetCommandList());

	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	
	postEffect->Draw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

bool GameScene::IsEndRequst(){
	return isEndRequst;
}

void GameScene::Finalize(){
	//解放処理
	winApp->Finalize();
	
	FbxLoader::GetInstance()->Finalize();
	//safe_delete(fbxObj);
	//safe_delete(fbxM);

	delete	input;
	delete winApp;
	delete	dxCommon;
	//delete velocity;
	/*delete modelChr;
	delete modelGround;
	delete modelSkydome;
	delete modelSphere;
	delete objChr;
	delete	objSkydome;
	delete	objGround;
	delete	objSphere;*/

}