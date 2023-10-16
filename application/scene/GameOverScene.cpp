/**
 * @file GameOverScene.cpp
 * @brief ベースシーンを継承したゲームオーバーシーン
 */
#include "GameOverScene.h"
#include"SceneTransition.h"
#include<SceneManager.h>

void GameOverScene::Initialize() {
	overSprite = std::make_unique<Sprite>(
		3,
		XMFLOAT2{ 640.0f,375.0f },//位置
		XMFLOAT2{ 500.0f,200.0f },//サイズ
		XMFLOAT4{ 1,1,1,1 },//カラー（RGBA）
		XMFLOAT2{ 0.5f,0.5f },//アンカーポイント
		false,//左右反転
		false//上下反転
		);
	overSprite->Initialize(SpriteCommon::GetInstance(), 3);
	input.reset(Input::GetInstance());
}

void GameOverScene::Update() {
	overSprite->Update();
	//キーを押したら
	if ( input->TriggerKey(DIK_RETURN)
		|| input->TriggerReleaseKey(DIK_SPACE)
		|| input->TriggerReleaseClick(Botton::LEFT)
		&& !SceneTransition::GetInstance()->GetIsBlackOut()
		&& !SceneTransition::GetInstance()->GetIsLightChange() )
	{
		SceneTransition::GetInstance()->IsBlackOutTrue();
	}
	if ( waitTime <= waitTimer )
	{
		waitTime++;
	}
	if ( waitTime > waitTimer &&
		!SceneTransition::GetInstance()->GetIsBlackOut() &&
		SceneTransition::GetInstance()->GetIsLightChange() )
	{
		//シーンの切り替え
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