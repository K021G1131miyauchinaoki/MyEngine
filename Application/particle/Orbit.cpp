#include "Orbit.h"
#include<random>
#include"Easing.h"
void Orbit::Initialize(Model* model_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_) {

	alphaStart = 1.0f;
	alphaEnd = 0.0f;

	BaseParticle::Initialize(model_,life_,position_,startScale_,endScale_,color_);
}

void Orbit::Update() {
	stratFrame++;

	//進行度を0～1の範囲に換算
	float f = ( float ) stratFrame / endFrame;
	//スケールの線形補間
	//scale = ( endScale - stratScale ) * f;
	scale = 1.0f;
	//カラー
	float alpha;
	alpha = alphaStart + ( alphaEnd - alphaStart ) * Easing::easeOutSine(f);
	
	obj->SetScale({ scale, scale, scale });
	obj->SetColor({ color.x,color.y, color.z, alpha });

	BaseParticle::Update();
}

void Orbit::Draw() {
	obj->Draw();
}