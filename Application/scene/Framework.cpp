/**
 * @file Framework.cpp
 * @brief シーン用フレームワーク
 */

#include "Framework.h"
#include"TextureManager.h"

void Framework::Initialize(){
	winApp=WinApp::GetInstance();
	winApp->Initialize();

	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp);

	Input::GetInstance()->Initialize();
	input=Input::GetInstance();
	TextureManager::GetInstance()->Initialize(dxCommon.get());

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	//デバイスをセット
	Object3d::StaticInitialize(dxCommon->GetDevice());
	Light::StaticInitialize(dxCommon->GetDevice());
	BillboardParticle::StaticInitialize(dxCommon->GetDevice());
	//ポストエフェクト
	PostEffect::StaticInitialize(dxCommon.get());
	//スプライトコモン
	SpriteCommon::GetInstance()->Initialize(dxCommon.get());
	//音
	SoundManager::GetInstance()->Initialize();
}

void Framework::Update(){
	input->Update();
	//sceneManager->Update();
}

void Framework::Draw(){}

void Framework::Finalize(){
	winApp->Finalize();
	SoundManager::GetInstance()->Finalize();
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