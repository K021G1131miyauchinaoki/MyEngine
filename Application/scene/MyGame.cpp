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
	SceneTransition::GetInstance()->Initialize();
}

void MyGame::Update(){
	//imgui関連
	ImguiManager::GetInstance()->Begin();
	SceneTransition::GetInstance()->Updata();
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
	SceneTransition::GetInstance()->Draw();
	SpriteCommon::GetInstance()->PostDraw();

	//imgui
	ImguiManager::GetInstance()->End();
	ImguiManager::GetInstance()->Draw();
	//pe->ObjDraw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

void MyGame::Finalize(){
	Map::Finalize();
	Framework::Finalize();
}

