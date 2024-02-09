/**
 * @file StageSelect.cpp
 * @brief ベースシーンを継承したステージセレクト
 */

#include"SceneTransition.h"
#include<SceneManager.h>
#include<Vector3.h>
#include "StageSelect.h"

//矩形（アンカーポイントが｛0.5,0.5｝の場合	
bool CheckHit(XMFLOAT2 pos,XMFLOAT2 size,Vector2 mouse) {
	float cl1 = pos.x - size.x;
	float cr1 = pos.x + size.x;
	float cl2 = mouse.x;

	float cu1 = pos.y - size.y;
	float cd1 = pos.y + size.y;
	float cu2 = mouse.y;

	if ( cl1<=cl2&& cr1>=cl2)
	{
		if ( cu1 <= cu2 && cd1 >= cu2 )
		{
			return true;
		}
	}

	return	false;
}

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
		pos.x = ( slide[ i ]->GetTexSize().x ) + static_cast< float >( WinApp::width*i ) - ( slide[ i ]->GetTexSize().x*2.0f * i );
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
	input=Input::GetInstance();
	//input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetEye({ 0.0f, 0.0f,-10.0f });
	Object3d::SetCamera(camera.get());
	BillboardParticle::SetCamera(camera.get());
	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	light->SetLightDir(lightDir);
	light->Update();
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
	CheckAllCollision();
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

	stage[ SceneManager::stage ]->Draw();
}

void StageSelect::ObjDraw() {
	objSkydome->Draw();
}

void StageSelect::GeometryDraw()
{

}

void StageSelect::Finalize() {}

void StageSelect::CheckAllCollision()
{
	for ( int8_t i = 0; i < slideNum; i++ )
	{
		if ( CheckHit(slide[ i ]->GetPosition(),slide[ i ]->GetTexSize(),input->GetMausePos()))
		{
			XMFLOAT2 size = slide[ i ]->GetTexSize();
			size.x *= scale;
			size.y *= scale;
			slide[ i ]->SetSize(size);
			if ( input->TriggerReleaseClick(Botton::LEFT) )
			{
				//右矢印
				if ( i == slideNum-1 && SceneManager::stage < stageNum - 1 )
				{
					SceneManager::stage++;
					BaseScene::DecisionSound();
				}
				//左矢印
				else if ( i ==0&& SceneManager::stage > 0 )
				{
					SceneManager::stage--;
					BaseScene::DecisionSound();
				}
			}
		}
		else
		{
			slide[ i ]->SetSize(slide[i]->GetTexSize());
		}
	}
	XMFLOAT2 pos,size;
	pos.x = static_cast< float >( WinApp::width / 2 );
	pos.y = static_cast< float >( WinApp::height / 2 );
	size.x = 300.0f;
	size.y = 200.0f;
	if ( CheckHit(pos,	size,input->GetMausePos()) )
	{
		size = stage[ SceneManager::stage ]->GetTexSize();
		size.x *= scale;
		size.y *= scale;
		stage[ SceneManager::stage ]->SetSize(size);
		if ( input->TriggerReleaseClick(Botton::LEFT)
		&& !SceneTransition::GetInstance()->GetIsFadeOut()
		&& !SceneTransition::GetInstance()->GetIsFadeIn() )
		{
			SceneTransition::GetInstance()->IsFadeOutTrue();
			BaseScene::DecisionSound();
		}
	}
	else
	{
		stage[ SceneManager::stage ]->SetSize(stage[ SceneManager::stage ]->GetTexSize());
	}
}

StageSelect::StageSelect() {}
StageSelect::~StageSelect() {
	Finalize();
}