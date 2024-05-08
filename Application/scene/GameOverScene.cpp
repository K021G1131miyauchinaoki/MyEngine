/**
 * @file GameOverScene.cpp
 * @brief ベースシーンを継承したゲームオーバーシーン
 */

#include "GameOverScene.h"
#include"SceneTransition.h"
#include<SceneManager.h>
#include<Vector3.h>
#include<random>

void GameOverScene::Initialize() {
	modelM = ModelManager::GetInstance();

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

	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelM->GetModel("skydome"));
	objSkydome->SetScale({ 150.0f,150.0f,150.0f });
	//タンク
	tankBody = std::make_unique<Object3d>();
	tankBody->Initialize();
	tankBody->SetModel(modelM->GetModel("TankBody"));
	tankBody->SetPosition({ -5.0f,5.0f,8.0f });
	tankBody->SetScale({ 5.0f,5.0f,5.0f });
	tankHad = std::make_unique<Object3d>();
	tankHad->Initialize();
	tankHad->SetModel(modelM->GetModel("TankHad"));
	tankHad->SetPosition({ -1.9f,3.5f,2.0f });
	tankHad->SetScale({ 5.0f,5.0f,5.0f });
	tankHad->SetRotation({ -1.5f,40.0f,-22.0f });

	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false,modelM->GetModel("map"));
	map->LoadCSV("title");

	//パーティクルマネージャー
	particle = std::make_unique<BillboardParticle>();
	particle->Initialize();

	particlePos.x = tankBody->GetPosition().x;
	particlePos.x -= 2.0f;
	particlePos.y = tankBody->GetPosition().y;
	particlePos.y -= 3.0f;
	particlePos.z = tankBody->GetPosition().z;

	num =2;

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
	
	for ( size_t i = 0; i < num; i++ )
	{
	//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		std::uniform_real_distribution<float> velDist(-0.1f,0.1f);
		std::uniform_real_distribution<float> velDistY(0.1f,0.12f);
		//XYZ全てにランダムに分布
		XMFLOAT3	vel{};
		vel.x = velDist(engine);
		vel.y = velDistY(engine);
		vel.z = velDist(engine);

		//重力に見立ててでランダムに分布
		XMFLOAT3	acc{ 0.0f,0.0f,0.0f };

		//追加
		particle->Add(80,particlePos,vel,acc,0.5f,2.0f,{ 0.1f,0.1f, 0.1f,0.8f },{ 0.1f,0.1f, 0.1f,0.0f });
	}
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
	tankBody->Draw();
	tankHad->Draw();
}

void GameOverScene::GeometryDraw()
{

	particle->Draw();
}

void GameOverScene::Finalize() {}

GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
	Finalize();
}