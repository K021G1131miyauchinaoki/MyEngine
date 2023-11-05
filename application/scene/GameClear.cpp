/**
 * @file GameClear.cpp
 * @brief ベースシーンを継承したゲームクリアシーン
 */
#include "GameClear.h"
#include<SceneManager.h>
#include"SceneTransition.h"
void GameClear::Initialize() {
	waitTime = 0;

	//スプライト
	clearSprite = std::make_unique<Sprite>();
	clearSprite->Initialize(SpriteCommon::GetInstance(), 2);
	clearSprite->SetAnchorPoint({ 0.5f,0.5f });
	clearSprite->SetPosition({ 640.0f,200.0f });


	//操作
	input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetEye({ 10.0f, 4.0f,10.0f });
	Object3d::SetCamera(camera.get());

	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	light->SetLightDir(lightDir);
	light->Updata();
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
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

	tankBody = std::make_unique<Object3d>();
	tankBody->Initialize();
	tankBody->SetModel(body.get());
	tankBody->SetPosition({ 0.0f,5.0f,0.0f });
	tankBody->SetScale({ 5.0f,5.0f,5.0f });
	tankHad = std::make_unique<Object3d>();
	tankHad->Initialize();
	tankHad->SetModel(had.get());
	tankHad->SetPosition({ 0.0f,5.0f,0.0f });
	tankHad->SetScale({ 5.0f,5.0f,5.0f });

	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false);
	map->LoadCSV("title");

	//ターゲットの設定
	camera->SetTarget({ tankBody->GetPosition().x, tankBody->GetPosition().y, tankBody->GetPosition().z });
}

void GameClear::Update() {
	camera->Update();
	map->Update();
	objSkydome->Update();
	tankBody->Update();
	tankHad->Update();
	clearSprite->Update();
	if ( waitTime <= waitTimer )
	{
		waitTime++;
	}
	if (waitTime>waitTimer&&
		!SceneTransition::GetInstance()->GetIsFadeOut() &&
		SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//キーを押したら
	if ( input->TriggerKey(DIK_RETURN)
		|| input->TriggerReleaseKey(DIK_SPACE)
		//|| input->TriggerReleaseClick(Botton::LEFT)
		&& !SceneTransition::GetInstance()->GetIsFadeOut()
		&& !SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		SceneTransition::GetInstance()->IsFadeOutTrue();
	}
}

void GameClear::SpriteDraw() {
	//clearSprite->SetTexIndex(1);
	clearSprite->Draw();
	
}

void GameClear::ObjDraw() {
	objSkydome->Draw();
	map->Draw();
	tankBody->Draw();
	tankHad->Draw();
}

void GameClear::Finalize() {}
