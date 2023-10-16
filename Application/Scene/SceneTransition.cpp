/**
 * @file SceneTransition.cpp
 * @brief シーン遷移
 */
#include "SceneTransition.h"
#include"SpriteCommon.h"
#include"Easing.h"

SceneTransition* SceneTransition::GetInstance()
{
	static SceneTransition instance;
	return &instance;
}

void SceneTransition::Initialize(){
	transSprite = std::make_unique<Sprite>();
	transSprite->Initialize(SpriteCommon::GetInstance(),5);
	transSprite->SetPosition(XMFLOAT2{ 0.0f,0.0f });
	transSprite->SetSize(XMFLOAT2{ 1280.0f,720.0f });
	transSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,0.0f });
	transSprite->SetAnchorPoint(XMFLOAT2{ 0.0f,0.0f });
}

void SceneTransition::Updata() {
	const float min = 0.0f;
	const float max = 1.0f;
	if ( isBlackOut )
	{
		alpha = transSprite->GetColor().w;
		alpha = min + ( max - min ) * Easing::easeOutSine(blackOutTime / blackOutTimer);
		transSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		blackOutTime += 0.15f;
		if ( blackOutTime > blackOutTimer )
		{
			blackOutTime = 0.0f;
			isBlackOut = false;
			isLightChange = true;
		}
	}
	if ( isLightChange )
	{
		alpha = transSprite->GetColor().w;
		alpha = max + ( min - max ) * Easing::easeOutSine(blackOutTime / blackOutTimer);
		transSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		blackOutTime += 0.15f;
		if ( blackOutTime > blackOutTimer )
		{
			blackOutTime = 0.0f;
			isLightChange = false;
		}
	}
	transSprite->Update();
}
void SceneTransition::Draw() {
	transSprite->Draw();
}

void SceneTransition::IsBlackOutTrue() {
	isBlackOut = true;
}
