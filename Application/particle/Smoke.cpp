/**
 * @file Explosion.cpp
 * @brief BaseParticleを継承した爆発パーティクル
 */
#include "Smoke.h"
#include<random>
#include<MyMath.h>
#include<Easing.h>

void Smoke::Initialize(Model* model_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_) {
	BaseModelParticle::Initialize(model_,life_,position_,startScale_,endScale_,color_);

	colorStart = 0.1f;
	colorEnd=0.8f;
	alphaStart=1.0f;
	alphaEnd=0.0f;

	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetColor({ colorStart,colorStart,colorStart,alphaStart });
#pragma region 乱数
	//xyzをランダムに分布
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数
	std::uniform_real_distribution<float> posDist(-0.75f,0.75f);
	std::uniform_real_distribution<float> valueDistXZ(-0.1f,0.1f);
	std::uniform_real_distribution<float> valueDistY(0.15f,0.2f);
	std::uniform_real_distribution<float> rotValue(-180.0f,180.0f);
	//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
	pos+= { posDist(engine),0.0f,posDist(engine) };

	//xyzをランダムに分布
	Vector3 value = { 0,0,0 };
	value = { valueDistXZ(engine),valueDistY(engine),valueDistXZ(engine) };

	velocity = value;

	rot = { rotValue(engine),rotValue(engine),rotValue(engine) };
	//rot.y = rotY(engine);

	//乱数　（スケール）
	//stratScale = 1.0f;
#pragma endregion
	//endScale = 0.0f;

	obj->SetPosition(pos);
	obj->SetRotation(rot);

}

void Smoke::Update() {
	//速度による移動
	pos += velocity * speed;
	//進行度を0～1の範囲に換算
	float f = ( float ) stratFrame / endFrame;
	//スケールの線形補間
	scale = ( endScale - stratScale ) * f;
	
	//カラー
	//float color;
	float alpha;
	//color = colorStart+( colorEnd - colorStart ) * Easing::easeOutCubic(f);
	alpha = alphaStart+( alphaEnd - alphaStart ) * Easing::easeOutSine(f);
	obj->SetPosition(pos);
	obj->SetScale({ scale, scale, scale });
	obj->SetColor({ colorStart,colorStart,colorStart,alpha });
	BaseModelParticle::Update();
}

void Smoke::Draw() {
	obj->Draw();
}