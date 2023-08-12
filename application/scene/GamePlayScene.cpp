#include "GamePlayScene.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon, Input* input) {
	/*変数*/
	mapStratY = -200;
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
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });

	//レクティル
	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input);
	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input, aim.get());
	//エネミー
	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//マップ
	map = std::make_unique<Map>(mapStratY);
	map->Initialize();
	map->LoadCSV("1");
}

void GamePlayScene::Update(){
	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Update();
	enemy->Update();
	aim->Update();
	map->Update();
	objSkydome->Update();
}

void GamePlayScene::SpriteDraw() {

}

void GamePlayScene::ObjDraw(){
	enemy->Draw();
	objSkydome->Draw();
	player->Draw();
	aim->Draw();
	map->Draw();
}

void GamePlayScene::Finalize(){}