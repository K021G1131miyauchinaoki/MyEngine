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
	camera->SetEye({ 0,80,-20 });
	camera->Update();
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::width, WinApp::height, camera.get());

	/*---------------- - FBX------------------------*/
	//fbxM = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	////デバイスをセット
	//FbxObject3d::SetDevice(dxCommon->GetDevice());
	//// カメラをセット
	//FbxObject3d::SetCamera(camera.get());
	////グラフィックパイプライン生成
	//FbxObject3d::CreateGraphicsPipeline();
	////3Dオブジェクト生成とモデルセット
	//fbxObj = new FbxObject3d;
	//fbxObj->Initialize();
	//fbxObj->SetModel(fbxM);
	//fbxObj->PlayAnimation();

	// モデル読み込み
	modelSkydome = Model::LoadFromOBJ("skydome");
	cube = Model::LoadFromOBJ("cube");
	box = Model::LoadFromOBJ("player");

	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome);
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });


	aim = std::make_unique<Aimposition>();
	aim->Initialeze(box, input);
	
	player = std::make_unique<Player>();
	player->Initialeze(box, input, aim.get());

	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(cube, player.get());

	//ポストエフェクト
	PostEffect::StaticInitialize(dxCommon);
	//pe = new PostEffect;
	//pe->Initialize();
	velocity = new ImguiManager;
	velocity->Initialize(winApp, dxCommon);

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
	

	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Updata();
	enemy->Updata();
	aim->Updata();
	objSkydome->Update();
	float vec[2] = { input->GetPos().x,input->GetPos().y};
	float posA[2] = { aim->GetPosition().x,aim->GetPosition().z };
	//imgui関連
	velocity->Begin();
	//ここから中身を書いていく
	ImGui::Begin("a");
	ImGui::SliderFloat2("volocity", vec, 0.0f, WinApp::width);
	ImGui::End();
	//ImGui::SliderFloat2("pos",posA,0.0f, WinApp::width);
	//fbxObj->Update();

	//for (auto object : objects) {
	//	object->Update();
	//}
}

void GameScene::Draw(){
	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	/*for (auto object : objects) {
		object->Draw();
	}*/

	objSkydome->Draw();
	player->Draw();
	aim->Draw();
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	//fbxObj->Draw(dxCommon->GetCommandList());

	//パーティクル描画
	
	//スプライト描画

	//imgui
	velocity->End();
	velocity->Draw();
	//pe->Draw(dxCommon->GetCommandList());

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
	delete velocity;
	delete modelSkydome;
	delete cube;
	delete box;
}