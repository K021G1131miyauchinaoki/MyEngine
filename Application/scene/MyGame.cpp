/**
 * @file GameClear.cpp
 * @brief フレームワークを継承したゲームシーン
 */
#include "MyGame.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#include<Map.h>
#include<ModelManager.h>
#include<SceneFactory.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void MyGame::Initialize() {
	Framework::Initialize();
	/*リソース*/
	//スプライト
	SpriteCommon::GetInstance()->Loadtexture(1,"title.png");
	SpriteCommon::GetInstance()->Loadtexture(2,"clear.png");
	SpriteCommon::GetInstance()->Loadtexture(3,"over.png");
	SpriteCommon::GetInstance()->Loadtexture(4,"heart.png");
	SpriteCommon::GetInstance()->Loadtexture(5,"white1.png");
	SpriteCommon::GetInstance()->Loadtexture(6,"sight.png");
	SpriteCommon::GetInstance()->Loadtexture(7,"stage/ready.png");
	SpriteCommon::GetInstance()->Loadtexture(9,"star.png");
	SpriteCommon::GetInstance()->Loadtexture(10,"pushkey.png");
	SpriteCommon::GetInstance()->Loadtexture(11,"memo.png");
	//音
	SoundManager::GetInstance()->LoadWave("BGM/title.wav");
	SoundManager::GetInstance()->LoadWave("BGM/play.wav");
	SoundManager::GetInstance()->LoadWave("BGM/clear.wav");
	SoundManager::GetInstance()->LoadWave("BGM/over.wav");
	SoundManager::GetInstance()->LoadWave("SE/gun.wav");
	SoundManager::GetInstance()->LoadWave("SE/gun2.wav");
	SoundManager::GetInstance()->LoadWave("SE/dying.wav");
	SoundManager::GetInstance()->LoadWave("SE/decision.wav");
	ImguiManager::GetInstance()->Initialize(winApp, dxCommon.get());
	//モデル
	ModelManager::GetInstance()->LoadModel("skydome",true);
	ModelManager::GetInstance()->LoadModel("cube");
	ModelManager::GetInstance()->LoadModel("TankBody");
	ModelManager::GetInstance()->LoadModel("TankHad");
	ModelManager::GetInstance()->LoadModel("map");
	ModelManager::GetInstance()->LoadModel("enemy");
	ModelManager::GetInstance()->LoadModel("parachute");
	ModelManager::GetInstance()->LoadModel("fixedgun");
	ModelManager::GetInstance()->LoadModel("wall");
	ModelManager::GetInstance()->LoadModel("bullet");
	ModelManager::GetInstance()->LoadModel("bom");
	ModelManager::GetInstance()->LoadModel("smoke");
	ModelManager::GetInstance()->LoadModel("BreakBlock");
	
	//シーンマネージャーに最初のシーンをセット
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	SceneManager::GetInstance()->ChangeScene("TITLE");
	SceneTransition::GetInstance()->Initialize();
}

void MyGame::Update(){
	//imgui関連
	ImguiManager::GetInstance()->Begin();
	SceneTransition::GetInstance()->Update();
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

	//ジオメトリ
	BillboardParticle::PreDraw(dxCommon->GetCommandList());
	SceneManager::GetInstance()->GeometryDraw();
	BillboardParticle::PostDraw();

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
	Framework::Finalize();
}

