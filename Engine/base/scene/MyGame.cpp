/**
 * @file GameClear.cpp
 * @brief フレームワークを継承したゲームシーン
 */
#include "MyGame.h"
#include<MyMath.h>
#include<SceneFactory.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void MyGame::Initialize() {
	Framework::Initialize();
	ImguiManager::GetInstance()->Initialize(winApp.get(), dxCommon.get());
	
	gpu = std::make_unique<Gpup>();
	gpu->Initialize(1000,dxCommon.get());
}

void MyGame::Update(){
	//imgui関連
	ImguiManager::GetInstance()->Begin();
	
	Framework::Update();
	//------------------------------
}	

void MyGame::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//ジオメトリ
	Geometry::PreDraw(dxCommon->GetCommandList());
	Geometry::PostDraw();

	//スプライト描画
	SpriteCommon::GetInstance()->PreDraw();
	SpriteCommon::GetInstance()->PostDraw();

	//imgui
	ImguiManager::GetInstance()->End();
	ImguiManager::GetInstance()->Draw();
	//pe->ObjDraw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

void MyGame::Finalize(){
	
	Framework::Finalize();
}

