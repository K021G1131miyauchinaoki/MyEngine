/**
 * @file GameOverScene.cpp
 * @brief ベースシーンを継承したゲームオーバーシーン
 */

#include "GameOverScene.h"
#include"SceneTransition.h"
#include<SceneManager.h>
#include<Vector3.h>

void GameOverScene::Initialize() {
	waitTime = 0;

	//スプライト
	overSprite = std::make_unique<Sprite>();
	overSprite->Initialize(SpriteCommon::GetInstance(),3);
	overSprite->SetAnchorPoint({ 0.5f,0.5f });
	overSprite->SetPosition({ 640.0f,200.0f });

	pushKey = std::make_unique<Sprite>();
	pushKey->Initialize(SpriteCommon::GetInstance(),10);
	pushKey->SetAnchorPoint({ 0.5f,0.5f });
	pushKey->SetPosition({ 640.0f,600.0f });
	//操作
	input=Input::GetInstance();
	//input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialize();
	camera->SetEye({ 10.0f, 4.0f,-10.0f });
	Object3d::SetCamera(camera.get());
	BillboardParticle::SetCamera(camera.get());
	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome",true));
	cube.reset(Model::LoadFromOBJ("smoke"));
	had.reset(Model::LoadFromOBJ("TankHad"));
	body.reset(Model::LoadFromOBJ("TankBody"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	//モデルのセット
	Map::StaticInitialize(modelMap.get());


	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 150.0f,150.0f,150.0f });
	//タンク
	tankBody = std::make_unique<Object3d>();
	tankBody->Initialize();
	tankBody->SetModel(body.get());
	tankBody->SetPosition({ -5.0f,5.0f,8.0f });
	tankBody->SetScale({ 5.0f,5.0f,5.0f });
	tankHad = std::make_unique<Object3d>();
	tankHad->Initialize();
	tankHad->SetModel(had.get());
	tankHad->SetPosition({ -1.9f,3.5f,2.0f });
	tankHad->SetScale({ 5.0f,5.0f,5.0f });
	tankHad->SetRotation({ -1.5f,40.0f,-22.0f });

	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false);
	map->LoadCSV("title");

	//パーティクルマネージャー
	particle = std::make_unique<ModelParticleManager>();
	particle->Initialize(cube.get());

	//ターゲットの設定
	camera->SetTarget({ tankBody->GetPosition().x, tankBody->GetPosition().y, tankBody->GetPosition().z });

	flashTime = 0;
	isFlash = true;

	//音
	SoundManager::GetInstance()->PlayWave("BGM/over.wav",1.0f,true);

}

void GameOverScene::Update() {
	overSprite->Update();
	pushKey->Update();
	camera->Update();
	map->Update();
	objSkydome->Update();
	particle->Update();
	tankBody->Update();
	tankHad->Update();
	
	//キーを押したら
	if ( input->TriggerKey(DIK_RETURN)
		|| input->TriggerReleaseKey(DIK_SPACE)
		|| input->TriggerReleaseClick(Botton::LEFT)
		&& !SceneTransition::GetInstance()->GetIsFadeOut()
		&& !SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		SceneTransition::GetInstance()->IsFadeOutTrue();
		BaseScene::DecisionSound();

	}
	if ( waitTime <= waitTimer )
	{
		waitTime++;
	}
	if ( waitTime > waitTimer &&
		!SceneTransition::GetInstance()->GetIsFadeOut() &&
		SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
		//停止
		SoundManager::GetInstance()->StopWave("BGM/over.wav");
	}
	
	Vector3 particlePos= tankBody->GetPosition();
	particlePos.x -= 2.0f;

	particlePos.y -= 2.0f;
	particle->Add("smoke",2,50,particlePos,2.0f,5.0f);

	flashTime++;
	if ( flashTime >= flashTimer )
	{
		isFlash ^= 1;
		flashTime = 0;
	}
}

void GameOverScene::SpriteDraw() {
	overSprite->Draw();
	if ( isFlash )
	{
		pushKey->Draw();
	}
}

void GameOverScene::ObjDraw() {
	objSkydome->Draw();
	map->Draw();
	particle->Draw();
	tankBody->Draw();
	tankHad->Draw();
}

void GameOverScene::GeometryDraw()
{

}

void GameOverScene::Finalize() {}

GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
	Finalize();
}