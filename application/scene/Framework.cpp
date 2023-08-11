#include "Framework.h"

void Framework::Initialize(){
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	input = std::make_unique<Input>();
	input->Initialize(winApp.get());
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	//デバイスをセット
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//ポストエフェクト
	PostEffect::StaticInitialize(dxCommon.get());

}

void Framework::Update(){
	input->Update();
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