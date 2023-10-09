#include "TitleScene.h"
#include<SceneManager.h>

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
	tSprite[0] = std::make_unique<Sprite>();
	tSprite[ 1 ] = std::make_unique<Sprite>();
	titleSprite->Initialize(SpriteCommon::GetInstance(), 1);
	for ( size_t i = 0; i < tSprite.size(); i++ )
	{
		tSprite[i]->Initialize(SpriteCommon::GetInstance(),5);
		tSprite[ i ]->SetPosition(XMFLOAT2{ 0.0f,50+620.0f*static_cast<float>(i) });
		tSprite[ i ]->SetSize(XMFLOAT2{ 1280.0f,100.0f });
		tSprite[ i ]->SetColor(XMFLOAT4{ 0,0,0,1 });
		tSprite[ i ]->SetAnchorPoint(XMFLOAT2{ 0.0f,0.5f });

	}
	input.reset(Input::GetInstance());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());


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
	//camera->SetEye({ 8.0f,0.5f,-15.0f });
	camera->SetEye({ 10.0f,8.0f,15.0f });
	//camera->SetEye({ 15.0f,8.0f,-15.0f });
	//マップ
	map = std::make_unique<Map>();
	map->Initialize(false);
	map->LoadCSV("title");

	isMovie = true;
	movieTimer=300;
	titleTimer=600;
	movieTime=movieTimer;

}

void TitleScene::Update() {
	titleSprite->Update();
	for ( size_t i = 0; i < tSprite.size(); i++ )
	{
		tSprite[ i ]->Update();
	}
	camera->Update();
	player->Update();
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
	titleSprite->SetTexIndex(1);
	titleSprite->Draw();
	for ( size_t i = 0; i < tSprite.size(); i++ )
	{
		tSprite[ i ]->SetTexIndex(5);
		//tSprite[ i ]->Draw();
	}
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