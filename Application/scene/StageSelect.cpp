/**
 * @file StageSelect.cpp
 * @brief ベースシーンを継承したステージセレクト
 */

#include"SceneTransition.h"
#include<SceneManager.h>
#include<Vector3.h>
#include "StageSelect.h"

void StageSelect::Initialize() {
	waitTime = 0;
	//スプライト
	selectSprite = std::make_unique<Sprite>();
	selectSprite->Initialize(SpriteCommon::GetInstance(),13);
	selectSprite->SetAnchorPoint({ 0.5f,0.5f });
	selectSprite->SetPosition({ 640.0f,100.0f });

	for ( int8_t i = 0; i < slideNum; i++ )
	{
		XMFLOAT2 pos;
		slide[i] = std::make_unique<Sprite>();
		slide[i]->Initialize(SpriteCommon::GetInstance(),12);
		slide[i]->SetAnchorPoint({ 0.5f,0.5f });
		pos.x = ( slide[ i ]->GetTexSize().x / 2.0f ) + static_cast< float >( WinApp::width*i ) - ( slide[ i ]->GetTexSize().x * i );
		pos.y = static_cast< float >( WinApp::height / 2 );
		slide[i]->SetPosition(pos);
		if ( i==1 )
		{
			slide[ i ]->SetIsFlipX(true);
		}
		
	}

	for ( int8_t i = 0; i < stageNum; i++ )
	{
		XMFLOAT2 pos;
		pos.x = static_cast< float >( WinApp::width / 2 );
		pos.y = static_cast< float >( WinApp::height / 2 );
		stage[ i ] = std::make_unique<Sprite>();
		stage[ i ]->Initialize(SpriteCommon::GetInstance(),20+i);
		stage[ i ]->SetAnchorPoint({ 0.5f,0.5f });
		stage[ i ]->SetPosition(pos);
	}
	//操作
	input.reset(Input::GetInstance());
	//input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetEye({ 0.0f, 0.0f,-10.0f });
	Object3d::SetCamera(camera.get());
	Geometry::SetCamera(camera.get());
	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	light->SetLightDir(lightDir);
	light->Updata();
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome",true));
	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 150.0f,150.0f,150.0f });

	//ターゲットの設定
	camera->SetTarget({0.0f,0.0f,0.0f});

	//音
	//SoundManager::GetInstance()->PlayWave("BGM/over.wav",1.0f,true);
}

void StageSelect::Update() {
	selectSprite->Update();
	camera->Update();
	objSkydome->Update();
	for ( int8_t i = 0; i < slideNum; i++ )
	{
		slide[ i ]->Update();
	}

	for ( int8_t i = 0; i < stageNum; i++ )
	{
		stage[ i ]->Update();
	}
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
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		//停止
		//SoundManager::GetInstance()->StopWave("BGM/over.wav");
	}
}

void StageSelect::SpriteDraw() {
	selectSprite->Draw();

	for ( int8_t i = 0; i < slideNum; i++ )
	{
		slide[ i ]->Draw();
	}

	for ( int8_t i = 0; i < stageNum; i++ )
	{
		stage[ i ]->Draw();
	}
}

void StageSelect::ObjDraw() {
	objSkydome->Draw();
}

void StageSelect::GeometryDraw()
{

}

void StageSelect::Finalize() {}

StageSelect::StageSelect() {}
StageSelect::~StageSelect() {
	Finalize();
}