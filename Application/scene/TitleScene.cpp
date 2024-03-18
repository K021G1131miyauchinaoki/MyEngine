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
	tankScale = { 5.0f,5.0f,5.0f };
	isStaging = false;

	modelM = ModelManager::GetInstance();

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

	//オブジェクト
	for ( size_t i = 0; i < OBJ::max; i++ )
	{
		obj[ i ].reset(Object3d::Create());
	}
	obj[OBJ::skydome]->SetModel(modelM->GetModel(ModelData::skydome));
	obj[OBJ::skydome]->SetScale({ 150.0f,150.0f,150.0f });

	obj[ OBJ::had ]->SetModel(modelM->GetModel(ModelData::had));
	obj[ OBJ::had ]->SetScale(tankScale);

	obj[ OBJ::body ]->SetModel(modelM->GetModel(ModelData::body));
	obj[ OBJ::body ]->SetScale(tankScale);

	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false,modelM->GetModel(ModelData::map));
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
	Staging();

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
		camera->SetTarget({ 0.0f, obj[ OBJ::body ]->GetPosition().y, obj[ OBJ::body ]->GetPosition().z });
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
	camera->Update();
	map->Update();
	for ( size_t i = 0; i < OBJ::max; i++ )
	{
		obj[ i ]->Update();
	}
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
	map->Draw();
	for ( size_t i = 0; i < OBJ::max; i++ )
	{
		obj[ i ]->Draw();
	}
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
	if ( movieCount < 2 && obj[OBJ::body]->GetPosition().x>-1.0f )
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

void TitleScene::Staging()
{
	//最初のカメラワークの位置
	if ( TitleScene::movieCount == TitleScene::CameraFirst )
	{
		if ( isStaging )//カウントを加算する
		{
			if ( obj[OBJ::body]->GetPosition().x > 0.0f )
			{
				isStaging = false;
				TitleScene::AddMovieCount();
			}
		}
		else//初期位置
		{
			const Vector3 pos = { -30.0f,5.0f,0.0f };
			obj[ OBJ::body]->SetPosition(pos);
			obj[ OBJ::had]->SetPosition(pos);
			isStaging = true;
		}
	}
	//二回目
	else if ( TitleScene::movieCount == TitleScene::CameraSecond )
	{
		if ( isStaging )
		{
			if ( obj[OBJ::had]->GetPosition().x > 0.0f )
			{
				isStaging = false;
				TitleScene::AddMovieCount();
			}
		}
		else
		{
			const Vector3 pos = { -45.0f,5.0f,0.0f };
			obj[ OBJ::body ]->SetPosition(pos);
			obj[ OBJ::had ]->SetPosition(pos);
			isStaging = true;
		}
	}
	//三回目
	else if ( TitleScene::movieCount == TitleScene::CameraThird )
	{
		if ( isStaging )
		{
			if ( easeTime > easeTimer )
			{
				isStaging = false;
				TitleScene::AddMovieCount();
			}
		}
		else
		{
			easeTime = 0.0f;
			const Vector3 pos = { -45.0f,5.0f,0.0f };
			obj[ OBJ::body ]->SetPosition(pos);
			obj[ OBJ::had ]->SetPosition(pos);
			isStaging = true;
		}
	}
	//二回目までの移動
	if ( TitleScene::movieCount <= TitleScene::CameraSecond )
	{
		Vector3 move = obj[OBJ::had]->GetPosition();
		const float speed = 0.25f;
		move.x += speed;
		obj[OBJ::had]->SetPosition(move);
		obj[OBJ::body]->SetPosition(move);
	}
	//三回目の移動
	else if ( easeTime < easeTimer && TitleScene::movieCount == TitleScene::CameraThird )
	{
		//イージング
		Vector3 start = { -45.0f,5.0f,0.0f };
		Vector3 end = { 0.0f,5.0f,0.0f };
		Vector3 move = obj[OBJ::had]->GetPosition();
		move.x = start.x + ( end.x - start.x ) * Easing::easeOutSine(easeTime / easeTimer);
		obj[OBJ::had]->SetPosition(move);
		obj[OBJ::body]->SetPosition(move);

		easeTime += 0.3f;
	}
}
