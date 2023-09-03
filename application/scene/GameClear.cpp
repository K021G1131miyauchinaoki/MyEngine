#include "GameClear.h"
#include<SceneManager.h>

void GameClear::Initialize() {
	clearSprite = std::make_unique<Sprite>(
		2,
		XMFLOAT2{ 640.0f,375.0f },//�ʒu
		XMFLOAT2{ 500.0f,200.0f },//�T�C�Y
		XMFLOAT4{ 1,1,1,1 },//�J���[�iRGBA�j
		XMFLOAT2{ 0.5f,0.5f },//�A���J�[�|�C���g
		false,//���E���]
		false//�㉺���]
		);
	clearSprite->Initialize(SpriteCommon::GetInstance(), 2);
	input.reset(Input::GetInstance());
}

void GameClear::Update() {
	clearSprite->Update();
	//�G���^�[�L�[����������
	if (input->TriggerKey(DIK_RETURN) || input->TriggerClick(Botton::RIGHT))
	{
		//�V�[���̐؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameClear::SpriteDraw() {
	//clearSprite->SetTexIndex(1);
	clearSprite->Draw();

}

void GameClear::ObjDraw() {
}

void GameClear::Finalize() {}

GameClear::GameClear() {}
GameClear::~GameClear() {
	Finalize();
}