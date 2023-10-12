/**
 * @file TitleScene.cpp
 * @brief ベースシーンを継承したタイトルシーン
 */
#include "TitleScene.h"
#include<SceneManager.h>
#include<Easing.h>
#include<random>

int8_t TitleScene::movieCount = NULL;

void TitleScene::Initialize() {
	titleSprite = std::make_unique<Sprite>(
		1,
		XMFLOAT2{ 640.0f,200.0f },//位置
		XMFLOAT2{ 500.0f,200.0f },//サイズ
		XMFLOAT4{ 1,1,1,1 },//カラー（RGBA）
		XMFLOAT2{ 0.5f,0.5f },//アンカーポイント
		false,//左右反転
		false//上下反転
		);
	blackOutSprite = std::make_unique<Sprite>();
	titleSprite->Initialize(SpriteCommon::GetInstance(), 1);
	
	blackOutSprite->Initialize(SpriteCommon::GetInstance(),5);
	blackOutSprite->SetPosition(XMFLOAT2{ 0.0f,0.0f });
	blackOutSprite->SetSize(XMFLOAT2{ 1280.0f,720.0f });
	blackOutSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,0.0f });
	blackOutSprite->SetAnchorPoint(XMFLOAT2{ 0.0f,0.0f });

	
	input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());

	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 0.75f,0.75f,0.75f });
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
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

	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input.get());
	

	camera->SetTarget({ 0.0f, 2.0f,0.0f });
	
	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false);
	map->LoadCSV("title");

	isMovie = true;
	movieTime=0;
	movieCount = 0;
	isBlackOut = false;
	isLightChange = false;
	blackOutTime = 0.0f;
}

void TitleScene::Update() {

	
	if ( input->PushKey(DIK_W) )lightDir.m128_f32[ 1 ] += 1.0f;
	if ( input->PushKey(DIK_S) )lightDir.m128_f32[ 1 ] -= 1.0f;
	if ( input->PushKey(DIK_D) )lightDir.m128_f32[ 0 ] += 1.0f;
	if ( input->PushKey(DIK_A) )lightDir.m128_f32[ 0 ]-= 1.0f;
	if ( input->PushKey(DIK_Q) )lightDir.m128_f32[ 2 ] += 1.0f;
	if ( input->PushKey(DIK_E) )lightDir.m128_f32[ 2 ] -= 1.0f;

	light->SetLightDir(lightDir);
	float a[4];
	a[ 0 ] = lightDir.m128_f32[ 0 ];
	a[ 1 ] = lightDir.m128_f32[ 1 ];
	a[ 2 ] = lightDir.m128_f32[ 2 ];
	a[ 3 ] = lightDir.m128_f32[ 3 ];
	ImGui::Begin("lo");
	ImGui::SliderFloat4("lightDir",a, -100.0f,100.0f);
	ImGui::End();
	
	//カメラ位置
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数　（回転）
	std::uniform_real_distribution<float> posDist(-0.025f,0.025f);
	//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
	shake = { posDist(engine),posDist(engine),0.0f };
	if ( movieCount==0 )
	{
		DirectX::XMFLOAT3 eye = { 8.0f + shake.x,0.5f + shake.y,-15.0f };
		//DirectX::XMFLOAT3 eye = { 8.0f,2.0f,-0.0f };
		camera->SetEye(eye);
	}
	else if ( movieCount == 1 )
	{
		DirectX::XMFLOAT3 eye = { 10.0f + shake.x,8.0f + shake.y,15.0f };
		camera->SetEye(eye);
	}
	else if ( movieCount == 2 )
	{
		camera->SetTarget({ 0.0f, player->GetPos().y, player->GetPos().z });
		DirectX::XMFLOAT3 eye = { 15.0f + shake.x,8.0f + shake.y,-15.0f };
		camera->SetEye(eye);
	}
	else if ( movieCount == 3 )
	{
		DirectX::XMFLOAT3 eye = { 15.0f ,8.0f,-15.0f };
		camera->SetEye(eye);
		movieTime++;
		if ( movieTime>movieTimer )
		{
			movieCount = 0;
			movieTime = 0;
		}
	}

	//暗転
	if ( movieCount < 2 &&player->GetPos().x>-1.0f)
	{
		isBlackOut = true;
	}
	BlackOutStaging();
	light->Updata();
	titleSprite->Update();
	blackOutSprite->Update();
	player->Update();
	camera->Update();
	objSkydome->Update();
	map->Update();
	//エンターキーを押したら
	if (input->TriggerKey(DIK_RETURN)||input->TriggerClick(Botton::RIGHT))
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::SpriteDraw() {
	if ( movieCount == 3 )
	{
		titleSprite->SetTexIndex(1);
		titleSprite->Draw();
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
	const float min = 0.0f;
	const float max = 1.0f;
	if ( isBlackOut )
	{
		alpha = blackOutSprite->GetColor().w;
		alpha = min + ( max - min ) * Easing::easeOutSine(blackOutTime / blackOutTimer);
		blackOutSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		blackOutTime += 0.15f;
		if ( blackOutTime > blackOutTimer )
		{
			blackOutTime = 0.0f;
			isBlackOut = false;
			isLightChange = true;
		}
	}
	if ( isLightChange )
	{
		alpha = blackOutSprite->GetColor().w;
		alpha = max + ( min - max ) * Easing::easeOutSine(blackOutTime / blackOutTimer);
		blackOutSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		blackOutTime += 0.15f;
		if ( blackOutTime > blackOutTimer )
		{
			blackOutTime = 0.0f;
			isLightChange = false;
		}
	}
}