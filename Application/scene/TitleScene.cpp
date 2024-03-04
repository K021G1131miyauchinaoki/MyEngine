/**
 * @file TitleScene.cpp
 * @brief ベースシーンを継承したタイトルシーン
 */
#include "TitleScene.h"
#include"SceneTransition.h"
#include<SceneManager.h>
#include<Easing.h>
#include<random>
#include<SoundManager.h>

int8_t TitleScene::movieCount = NULL;

void TitleScene::Initialize() {
	//スプライト
	titleSprite = std::make_unique<Sprite>();
	titleSprite->Initialize(SpriteCommon::GetInstance(), 1);
	titleSprite->SetAnchorPoint({ 0.5f,0.5f });
	titleSprite->SetPosition({ 640.0f,200.0f });

	blackOutSprite = std::make_unique<Sprite>();
	blackOutSprite->Initialize(SpriteCommon::GetInstance(),5);
	blackOutSprite->SetPosition(XMFLOAT2{ 0.0f,0.0f });
	blackOutSprite->SetSize(XMFLOAT2{ 1280.0f,720.0f });
	blackOutSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,0.0f });
	blackOutSprite->SetAnchorPoint(XMFLOAT2{ 0.0f,0.0f });

	pushKey = std::make_unique<Sprite>();
	pushKey->Initialize(SpriteCommon::GetInstance(),10);
	pushKey->SetAnchorPoint({ 0.5f,0.5f });
	pushKey->SetPosition({ 640.0f,600.0f });

	//操作
	input=Input::GetInstance();
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialize();
	Object3d::SetCamera(camera.get());
	camera->SetTarget({ 0.0f, 2.0f,0.0f });

	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	light->SetLightDir(lightDir);
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome",true));
	cube.reset(Model::LoadFromOBJ("cube"));
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

	//プレイヤー
	player = std::make_unique<Player>();
	player->TitleInitialeze(had.get(),body.get(),input);
	
	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false);
	map->LoadCSV("title");

	isMovie = true;
	movieTime=0;
	movieCount = 0;
	isFadeOut = false;
	isFadeIn = false;
	transTime = 0.0f;
	flashTime = 0;
	isFlash = true;

	SoundManager::GetInstance()->PlayWave("BGM/title.wav",0.3f,true);
	color.x = 1.0f;
	color.y = 1.0f;
	color.z = 1.0f;
	color.w = 1.0f;
}

void TitleScene::Update() {


	/*float a[4];
	a[ 0 ] = color.x;
	a[ 1 ] = color.y;
	a[ 2 ] = color.z;
	a[ 3 ] = color.w;
	ImGui::Begin("lo");
	ImGui::ColorEdit4("lightDir",a,ImGuiColorEditFlags_Float);
	ImGui::End();
	color.x= a[ 0 ];
	color.y = a[ 1 ];
	color.z = a[ 2 ];
	color.w = a[ 3 ];
	objSkydome->SetColor(color);*/
	/*シェイク処理*/
	//カメラ位置
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数　（回転）
	std::uniform_real_distribution<float> posDist(-0.025f,0.025f);
	//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
	shake = { posDist(engine),posDist(engine),0.0f };
	if ( movieCount== Staging::CameraFirst )
	{
		DirectX::XMFLOAT3 eye = { 8.0f + shake.x,0.5f + shake.y,-15.0f };
		//DirectX::XMFLOAT3 eye = { 8.0f,2.0f,-0.0f };
		camera->SetEye(eye);
	}
	else if ( movieCount == Staging::CameraSecond )
	{
		DirectX::XMFLOAT3 eye = { 10.0f + shake.x,8.0f + shake.y,15.0f };
		camera->SetEye(eye);
	}
	else if ( movieCount == Staging::CameraThird )
	{
		camera->SetTarget({ 0.0f, player->GetPos().y, player->GetPos().z });
		DirectX::XMFLOAT3 eye = { 15.0f + shake.x,8.0f + shake.y,-15.0f };
		camera->SetEye(eye);
	}
	else if ( movieCount == Staging::Title )
	{
		DirectX::XMFLOAT3 eye = { 15.0f ,8.0f,-15.0f };
		camera->SetEye(eye);
		movieTime++;
		if ( movieTime>movieTimer )
		{
			movieCount = Staging::CameraFirst;
			movieTime = 0;
		}
	}

	//暗転
	
	BlackOutStaging();
	light->Update();
	titleSprite->Update();
	blackOutSprite->Update();
	pushKey->Update();
	player->Update();
	camera->Update();
	objSkydome->Update();
	map->Update();
	/*シーン遷移*/
	//キーを押したら
	if (input->TriggerReleaseKey(DIK_SPACE)
		|| input->TriggerReleaseClick(Botton::LEFT)
		&&!SceneTransition::GetInstance()->GetIsFadeOut()
		&&!SceneTransition::GetInstance()->GetIsFadeIn() )
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
		SceneManager::GetInstance()->ChangeScene("Select");
		//音停止
		SoundManager::GetInstance()->StopWave("BGM/title.wav");
	}
	//点滅処理
	if ( movieCount == Staging::Title)
	{
		flashTime++;
		if ( flashTime>=flashTimer )
		{
			isFlash ^= 1;
			flashTime = 0;
		}		
	}
	else
	{
		flashTime = 0;
		isFlash = true;
	}
}

void TitleScene::SpriteDraw() {
	if ( movieCount == Staging::Title )
	{
		titleSprite->SetTexIndex(1);
		titleSprite->Draw();
		if ( isFlash )
		{
			pushKey->Draw();
		}
	}
	blackOutSprite->SetTexIndex(5);
	blackOutSprite->Draw();
}

void TitleScene::ObjDraw() {
	objSkydome->Draw();
	player->ObjDraw();
	map->Draw();
}

void TitleScene::Finalize() {}

TitleScene::TitleScene() {}
TitleScene::~TitleScene(){
	Finalize();
}

void TitleScene::AddMovieCount() {
	movieCount++;
}

void TitleScene::BlackOutStaging() {
	if ( movieCount < 2 && player->GetPos().x>-1.0f )
	{
		isFadeOut = true;
	}
	const float min = 0.0f;
	const float max = 1.0f;
	if ( isFadeOut )
	{
		alpha = blackOutSprite->GetColor().w;
		alpha = min + ( max - min ) * Easing::easeOutSine(transTime / transTimer);
		blackOutSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		transTime += 0.15f;
		if ( transTime > transTimer )
		{
			transTime = 0.0f;
			isFadeOut = false;
			isFadeIn = true;
		}
	}
	if ( isFadeIn )
	{
		alpha = blackOutSprite->GetColor().w;
		alpha = max + ( min - max ) * Easing::easeOutSine(transTime / transTimer);
		blackOutSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		transTime += 0.15f;
		if ( transTime > transTimer )
		{
			transTime = 0.0f;
			isFadeIn = false;
		}
	}
}