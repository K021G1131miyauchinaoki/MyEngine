/**
 * @file GameClear.cpp
 * @brief ベースシーンを継承したゲームクリアシーン
 */

#include "GameClear.h"
#include<SceneManager.h>
#include"SceneTransition.h"
#include<Easing.h>

void GameClear::Initialize() {
	//変数
	clearTime = 0.0f;
	startFovAngle=10.0f;
	endFovAngle=90.0f;
	fovAngleTime = 0;
	startRot=720;
	endRot=0;
	startScoreSize = {0,0};
	endScoreSize = { 100,100 };
	clearColor = { 1.0f,1.0f,1.0f,alphaMin };
	//スプライトx
	clearSprite = std::make_unique<Sprite>();
	clearSprite->Initialize(SpriteCommon::GetInstance(), 2);
	clearSprite->SetAnchorPoint({ 0.5f,0.5f });
	clearSprite->SetPosition({ 640.0f,150.0f });
	clearSprite->SetColor(clearColor);
	startClearSize.x = clearSprite->GetTexSize().x*1.5f;
	startClearSize.y = clearSprite->GetTexSize().y * 1.5f;
	clearSprite->SetSize(startClearSize);
	endClearSize=clearSprite->GetTexSize();
	score.resize(SceneManager::playerHP);
	for ( size_t i = 0; i < score.size(); i++ )
	{
		score[ i ].easeTime = 0;
		score[ i ].sprite = std::make_unique<Sprite>();
		score[ i ].sprite->Initialize(SpriteCommon::GetInstance(),9);
		score[ i ].sprite->SetRotation(startRot);
		score[ i ].sprite->SetSize(startScoreSize);
		score[ i ].sprite->SetPosition({ 530.0f+(i*110.0f),280.0f });
		score[ i ].sprite->SetAnchorPoint({ 0.5f,0.5f });
		score[ i ].sprite->Update();

	}
	//操作
	input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetEye({ 10.0f, 5.0f,10.0f });
	camera->SetFovAngle(startFovAngle);
	camera->SetTarget({ 0.0f,7.0f,0.0f });
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
	
}

void GameClear::Update() {
	camera->Update();
	map->Update();
	objSkydome->Update();
	tankBody->Update();
	tankHad->Update();
	clearSprite->Update();

	//スコア
	for ( size_t i = 0; i < score.size(); i++ )
	{
		const float percent = 0.8f;
		if ( score[ i ].easeTime < easeTimer )
		{
			//一番最初の要素はタイムを動かす
			if (i==0)
			{
				score[ i ].easeTime++;
			}
			//一個前のタイマーが80％以下なら
			else if ( score[ i-1 ].easeTime>easeTimer*percent )
			{
				score[ i ].easeTime++;
			}
		}
		//イージング
		XMFLOAT2 size;
		size.x = startScoreSize.x + ( endScoreSize.x - startScoreSize.x ) * Easing::easeOutCubic(score[ i ].easeTime / easeTimer);
		size.y = startScoreSize.x + ( endScoreSize.x - startScoreSize.x ) * Easing::easeOutCubic(score[ i ].easeTime / easeTimer);
		float rot= startRot + ( endRot - startRot ) * Easing::easeOutCubic(score[ i ].easeTime / easeTimer);
		score[ i ].sprite->SetRotation(rot);
		score[ i ].sprite->SetSize(size);
		score[ i ].sprite->Update();
	}
	//スコアの最後の要素のタイマーがeaseTimer以上なら
	if ( score[ score.size() - 1 ].easeTime>= easeTimer && fovAngleTime< fovAngleTimer )
	{
		//画角をイージング
		fovAngleTime++;
		float fovAngle = startFovAngle + ( endFovAngle - startFovAngle ) * Easing::easeOutCubic(fovAngleTime / fovAngleTimer);
		camera->SetFovAngle(fovAngle);
	}
	//ゲームクリアのスプライトをイージング
	if ( fovAngleTime >= fovAngleTimer && clearTime < clearTimer )
	{
		clearTime++;
		float alpha = alphaMin + ( alphaMax - alphaMin ) * Easing::easeOutCubic(clearTime / clearTimer);
		clearColor.w = alpha;
		XMFLOAT2 size;
		size.x = startClearSize.x + ( endClearSize.x - startClearSize.x ) * Easing::easeOutBounce(clearTime / clearTimer);
		size.y = startClearSize.y + ( endClearSize.y - startClearSize.y ) * Easing::easeOutBounce(clearTime / clearTimer);
		clearSprite->SetColor(clearColor);
		clearSprite->SetSize(size);
	}

	if (!SceneTransition::GetInstance()->GetIsFadeOut() &&
		SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//キーを押したら
	if (input->TriggerKey(DIK_RETURN)
		|| input->TriggerReleaseKey(DIK_SPACE)
		|| input->TriggerReleaseClick(Botton::LEFT)
		 )
	{
		if ( clearTime >= clearTimer
			&& !SceneTransition::GetInstance()->GetIsFadeOut()
			&& !SceneTransition::GetInstance()->GetIsFadeIn() )
		{
			SceneTransition::GetInstance()->IsFadeOutTrue();
		}
	}
}

void GameClear::SpriteDraw() {
	clearSprite->Draw();
	for ( size_t i = 0; i < score.size(); i++ )
	{
		score[ i ].sprite->Draw();
	}
}

void GameClear::ObjDraw() {
	objSkydome->Draw();
	map->Draw();
	tankBody->Draw();
	tankHad->Draw();
}

void GameClear::Finalize() {}
