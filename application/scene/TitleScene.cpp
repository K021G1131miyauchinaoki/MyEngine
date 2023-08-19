#include "TitleScene.h"
#include<SceneManager.h>
//後で消す
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
	//エンターキーを押したら
	if (input->TriggerKey(DIK_RETURN))
	{
		//次シーン生成
		BaseScene* scene = new GamePlayScene;
		//シーンの切り替え
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