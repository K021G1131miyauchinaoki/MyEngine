/**
 * @file GameClear.cpp
 * @brief フレームワークを継承したゲームシーン
 */
#include "MyGame.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#include<Map.h>
#include<SceneFactory.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void MyGame::Initialize() {
	Framework::Initialize();
	ImguiManager::GetInstance()->Initialize(winApp.get(), dxCommon.get());
	
	//シーンマネージャーに最初のシーンをセット
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Update(){
	float vec[2] = { input->GetMausePos().x,input->GetMausePos().y};
	//imgui関連
	ImguiManager::GetInstance()->Begin();
	//ここから中身を書いていく
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, -100.0f, static_cast<float>(WinApp::width));
	ImGui::End();
	Framework::Update();
	//------------------------------
	SceneManager::GetInstance()->Update();
}	

void MyGame::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	SceneManager::GetInstance()->ObjDraw();
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//スプライト描画
	SpriteCommon::GetInstance()->PreDraw();
	SceneManager::GetInstance()->SpriteDraw();
	SpriteCommon::GetInstance()->PostDraw();

	//imgui
	ImguiManager::GetInstance()->End();
	ImguiManager::GetInstance()->Draw();
	//pe->ObjDraw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

void MyGame::Finalize(){
	Bullet::Finalize();
	EnemyBullet::Finalize();
	Map::Finalize();
	Framework::Finalize();
}

