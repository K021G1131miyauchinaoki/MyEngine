/**
 * @file Framework.cpp
 * @brief シーン用フレームワーク
 */
#include "Framework.h"

void Framework::Initialize(){
	winApp.reset(WinApp::GetInstance());
	winApp->Initialize();

	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	Input::GetInstance()->Initialize();
	input .reset(Input::GetInstance());
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	//デバイスをセット
	Object3d::StaticInitialize(dxCommon->GetDevice());
	Light::StaticInitialize(dxCommon->GetDevice());
	//ポストエフェクト
	PostEffect::StaticInitialize(dxCommon.get());
	//スプライトコモン
	SpriteCommon::GetInstance()->Initialize(dxCommon.get());
	SpriteCommon::GetInstance()->Loadtexture(1, "title.png");
	SpriteCommon::GetInstance()->Loadtexture(2, "clear.png");
	SpriteCommon::GetInstance()->Loadtexture(3, "over.png");
	SpriteCommon::GetInstance()->Loadtexture(4,"heart.png");
	SpriteCommon::GetInstance()->Loadtexture(5,"white1.png");
	SpriteCommon::GetInstance()->Loadtexture(6,"sceneTransition.png");
	SpriteCommon::GetInstance()->Loadtexture(7,"stage/ready.png");
	SpriteCommon::GetInstance()->Loadtexture(8,"stage/stage1.png");
	SpriteCommon::GetInstance()->Loadtexture(9, "star.png");
}

void Framework::Update(){
	input->Update();
	//sceneManager->Update();
}

void Framework::Draw(){}

void Framework::Finalize(){
	winApp->Finalize();
	/*FbxLoader::GetInstance()->Finalize();
	FbxObject3d::Finalize();
	Model::Finalize();
	Object3d::Finalize();*/
}

bool Framework::IsEndRequst() {
	if (winApp->ProcessMessage() || input->PushKey(DIK_ESCAPE)) {
		return true;
	}
	return false;
}

void Framework::Run() {
	//初期化
	Initialize();

	//ループ
	while (true) {
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequst())
		{
			break;
		}

		//描画
		Draw();
	}
	//ゲーム終了
	Finalize();
}