#include "GameOverScene.h"

#include<SceneManager.h>

void GameOverScene::Initialize() {
	overSprite = std::make_unique<Sprite>(
		3,
		XMFLOAT2{ 640.0f,375.0f },//�ʒu
		XMFLOAT2{ 500.0f,200.0f },//�T�C�Y
		XMFLOAT4{ 1,1,1,1 },//�J���[�iRGBA�j
		XMFLOAT2{ 0.5f,0.5f },//�A���J�[�|�C���g
		false,//���E���]
		false//�㉺���]
		);
	overSprite->Initialize(SpriteCommon::GetInstance(), 3);
	input.reset(Input::GetInstance());
}

void GameOverScene::Update() {
	overSprite->Update();
	//�G���^�[�L�[����������
	if (input->TriggerKey(DIK_RETURN))
	{
		//�V�[���̐؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::SpriteDraw() {
	//overSprite->SetTexIndex(1);
	overSprite->Draw();

}

void GameOverScene::ObjDraw() {
}

void GameOverScene::Finalize() {}

GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
	Finalize();
}