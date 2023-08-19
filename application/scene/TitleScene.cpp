#include "TitleScene.h"
#include<SceneManager.h>
//��ŏ���
#include<GamePlayScene.h>

void TitleScene::Initialize() {
	titleSprite = std::make_unique<Sprite>(
		1,
		XMFLOAT2{500.0f,500.0f},
		XMFLOAT2{100.0f,100.0f},
		XMFLOAT4{1,1,1,1},
		XMFLOAT2{0.5f,0.5f},
		false,
		false
		);
	titleSprite->Initialize(SpriteCommon::GetInstance(), 1);
	input.reset(Input::GetInstance());
}

void TitleScene::Update() {
	titleSprite->Update();
	//�G���^�[�L�[����������
	if (input->TriggerKey(DIK_RETURN))
	{
		//���V�[������
		BaseScene* scene = new GamePlayScene;
		//�V�[���̐؂�ւ�
		sceneManager->SetNextScene(scene);
	}
}

void TitleScene::SpriteDraw() {

}

void TitleScene::ObjDraw() {
	titleSprite->SetTexIndex(1);
	titleSprite->Draw();
}

void TitleScene::Finalize() {}