/**
 * @file GameOverScene.cpp
 * @brief ベースシーンを継承したゲームオーバーシーン
 */
#include "GameOverScene.h"
#include"SceneTransition.h"
#include<SceneManager.h>
#include<Vector3.h>
void GameOverScene::Initialize() {
	//スプライト
	overSprite = std::make_unique<Sprite>();
	overSprite->Initialize(SpriteCommon::GetInstance(),3);
	overSprite->SetAnchorPoint({ 0.5f,0.5f });
	overSprite->SetPosition({ 640.0f,200.0f });


	//操作
	input.reset(Input::GetInstance());
	//input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetEye({ 10.0f, 4.0f,-10.0f });
	Object3d::SetCamera(camera.get());

	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	had.reset(Model::LoadFromOBJ("TankHad"));
	body.reset(Model::LoadFromOBJ("TankBody"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	//モデルのセット
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 150.0f,150.0f,150.0f });

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
	particle = std::make_unique<ParticleManager>();
	particle->Initialize(cube.get());

	//ターゲットの設定
	camera->SetTarget({ tankBody->GetPosition().x, tankBody->GetPosition().y, tankBody->GetPosition().z });
}

void GameOverScene::Update() {
	overSprite->Update();
	camera->Update();
	map->Update();
	objSkydome->Update();
	particle->Update();
	tankBody->Update();
	tankHad->Update();
	//キーを押したら
	if ( input->TriggerKey(DIK_RETURN)
		|| input->TriggerReleaseKey(DIK_SPACE)
		&& !SceneTransition::GetInstance()->GetIsFadeOut()
		&& !SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		SceneTransition::GetInstance()->IsFadeOutTrue();
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
	}

	float a[ 3 ] = { tankBody->GetPosition().x,tankBody->GetPosition().y, tankBody->GetPosition().z };
	float a2[ 3 ] = { tankBody->GetRotation().x,tankBody->GetRotation().y, tankBody->GetRotation().z };
	float b[ 3 ] = { tankHad->GetPosition().x,tankHad->GetPosition().y, tankHad->GetPosition().z };
	float b2[ 3 ] = { tankHad->GetRotation().x,tankHad->GetRotation().y, tankHad->GetRotation().z };
	//ImGui::Begin("lo");
	/*ImGui::SliderFloat3("l",a,-100.0f,100.0f);
	ImGui::SliderFloat3("i",a2, -100.0f,100.0f);
	ImGui::SliderFloat3("g",b,-30.0f,30.0f);
	ImGui::SliderFloat3("t",b2,-50.0f,50.0f);

	ImGui::End();
	tankBody->SetPosition({ a[ 0 ],a[ 1 ],a[ 2 ] });
	tankBody->SetRotation({ a2[ 0 ],a2[ 1 ],a2[ 2 ] });
	tankHad->SetPosition({ b[ 0 ],b[ 1 ],b[ 2 ] });
	tankHad->SetRotation({ b2[ 0 ],b2[ 1 ],b2[ 2 ] });*/
	Vector3 particlePos= tankBody->GetPosition();
	particlePos.x -= 2.0f;

	particlePos.y -= 2.0f;
	particle->Add("2",2,50,particlePos,2.0f,4.0f);
}

void GameOverScene::SpriteDraw() {
	//overSprite->SetTexIndex(1);
	overSprite->Draw();

}

void GameOverScene::ObjDraw() {
	objSkydome->Draw();
	map->Draw();
	particle->Draw();
	tankBody->Draw();
	tankHad->Draw();
}

void GameOverScene::Finalize() {}

GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
	Finalize();
}