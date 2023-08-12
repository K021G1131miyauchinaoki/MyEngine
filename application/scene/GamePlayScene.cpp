#include "GamePlayScene.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon, Input* input) {
	/*�ϐ�*/
	mapStratY = -200;
	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());


	// ���f���ǂݍ���
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	//���f���̃Z�b�g
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//�X�J�C�h�[��
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });

	//���N�e�B��
	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input);
	//�v���C���[
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input, aim.get());
	//�G�l�~�[
	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//�}�b�v
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