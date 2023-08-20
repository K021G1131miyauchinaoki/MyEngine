#include "TitleScene.h"
#include<SceneManager.h>

void TitleScene::Initialize() {
	titleSprite = std::make_unique<Sprite>(
		1,
		XMFLOAT2{ 640.0f,375.0f },//�ʒu
		XMFLOAT2{ 500.0f,200.0f },//�T�C�Y
		XMFLOAT4{ 1,1,1,1 },//�J���[�iRGBA�j
		XMFLOAT2{ 0.5f,0.5f },//�A���J�[�|�C���g
		false,//���E���]
		false//�㉺���]
		);
	titleSprite->Initialize(SpriteCommon::GetInstance(), 1);
	input.reset(Input::GetInstance());
}

void TitleScene::Update() {
	titleSprite->Update();
	//�G���^�[�L�[����������
	if (input->TriggerKey(DIK_RETURN))
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
}

void TitleScene::Finalize() {}

TitleScene::TitleScene() {}
TitleScene::~TitleScene(){
	Finalize();
}