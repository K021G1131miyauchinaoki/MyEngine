/**
 * @file GameClear.cpp
 * @brief ベースシーンを継承したゲームクリアシーン
 */
#include "GameClear.h"
#include<SceneManager.h>
#include"SceneTransition.h"
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
	waitTime = 0;
}

void GameClear::Update() {
	clearSprite->Update();
	if ( waitTime <= waitTimer )
	{
		waitTime++;
	}
	if (waitTime>waitTimer&&
		!SceneTransition::GetInstance()->GetIsFadeOut() &&
		SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//キーを押したら
	if ( input->TriggerKey(DIK_RETURN)
		|| input->TriggerReleaseKey(DIK_SPACE)
		|| input->TriggerReleaseClick(Botton::LEFT)
		&& !SceneTransition::GetInstance()->GetIsFadeOut()
		&& !SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		SceneTransition::GetInstance()->IsFadeOutTrue();
	}
}

void GameClear::SpriteDraw() {
	//clearSprite->SetTexIndex(1);
	clearSprite->Draw();

}

void GameClear::ObjDraw() {
}

void GameClear::Finalize() {}
