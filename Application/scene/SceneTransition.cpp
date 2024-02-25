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

void SceneTransition::Update() {
	const float min = 0.0f;
	const float max = 1.0f;
	if ( isFadeOut )
	{
		alpha = transSprite->GetColor().w;
		alpha = min + ( max - min ) * Easing::easeOutSine(transTime / transTimer);
		transSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		transTime += 0.15f;
		if ( transTime > transTimer )
		{
			transTime = 0.0f;
			isFadeOut = false;
			isFadeIn = true;
		}
	}
	if ( isFadeIn )
	{
		alpha = transSprite->GetColor().w;
		alpha = max + ( min - max ) * Easing::easeOutSine(transTime / transTimer);
		transSprite->SetColor(XMFLOAT4{ 0.0f,0.0f,0.0f,alpha });
		transTime += 0.15f;
		if ( transTime > transTimer )
		{
			transTime = 0.0f;
			isFadeIn = false;
		}
	}
	transSprite->Update();
}
void SceneTransition::Draw() {
	transSprite->Draw();
}

void SceneTransition::IsFadeOutTrue() {
	isFadeOut = true;
}

SceneTransition::SceneTransition() {}
SceneTransition::~SceneTransition(){}