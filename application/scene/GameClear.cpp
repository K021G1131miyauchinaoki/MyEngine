#include "GameClear.h"
#include<SceneManager.h>

void GameClear::Initialize() {
	clearSprite = std::make_unique<Sprite>(
		2,
		XMFLOAT2{ 640.0f,375.0f },//位置
		XMFLOAT2{ 500.0f,200.0f },//サイズ
		XMFLOAT4{ 1,1,1,1 },//カラー（RGBA）
		XMFLOAT2{ 0.5f,0.5f },//アンカーポイント
		false,//左右反転
		false//上下反転
		);
	clearSprite->Initialize(SpriteCommon::GetInstance(), 2);
	input.reset(Input::GetInstance());
}

void GameClear::Update() {
	clearSprite->Update();
	//エンターキーを押したら
	if (input->TriggerKey(DIK_RETURN) || input->TriggerClick(Botton::RIGHT))
	{
		//シーンの切り替え
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