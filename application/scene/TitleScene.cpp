#include "TitleScene.h"
#include<SceneManager.h>

void TitleScene::Initialize() {
	titleSprite = std::make_unique<Sprite>(
		1,
		XMFLOAT2{ 640.0f,200.0f },//�ʒu
		XMFLOAT2{ 500.0f,200.0f },//�T�C�Y
		XMFLOAT4{ 1,1,1,1 },//�J���[�iRGBA�j
		XMFLOAT2{ 0.5f,0.5f },//�A���J�[�|�C���g
		false,//���E���]
		false//�㉺���]
		);
	titleSprite->Initialize(SpriteCommon::GetInstance(), 1);
	input.reset(Input::GetInstance());
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

	//�v���C���[
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input.get());
	

	camera->SetTarget({ 0.0f, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x+15.0f,player->GetPos().y +5.0f, player->GetPos().z - 15.0f });
	//�}�b�v
	//map = std::make_unique<Map>(mapStratY);
	//map->Initialize();
	//map->LoadCSV("1");
}

void TitleScene::Update() {
	titleSprite->Update();
	camera->Update();
	player->Update();
	objSkydome->Update();
	//�G���^�[�L�[����������
	if (input->TriggerKey(DIK_RETURN)||input->TriggerClick(Botton::RIGHT))
	{
		//�V�[���̐؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::SpriteDraw() {
	titleSprite->SetTexIndex(1);
	titleSprite->Draw();

}

void TitleScene::ObjDraw() {
	objSkydome->Draw();
	player->ObjDraw();
}

void TitleScene::Finalize() {}

TitleScene::TitleScene() {}
TitleScene::~TitleScene(){
	Finalize();
}