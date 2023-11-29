/**
 * @file Explosion.cpp
 * @brief BaseParticleを継承した爆発パーティクル
 */
#include "Smoke.h"
#include<random>
#include<MyMath.h>
#include<Easing.h>

void Smoke::Initialize(Model* model_) {

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
	Vector3 pos = { 0,0,0 };
	pos= { posDist(engine),0.0f,posDist(engine) };
	oneGrain.pos += pos;

	//xyzをランダムに分布
	Vector3 value = { 0,0,0 };
	value = { valueDistXZ(engine),valueDistY(engine),valueDistXZ(engine) };

	oneGrain.velocity = value;

	Vector3 rot = { 0,0,0 };
	rot = { rotValue(engine),rotValue(engine),rotValue(engine) };
	//rot.y = rotY(engine);

	//乱数　（スケール）
	//oneGrain.stratScale = 1.0f;
#pragma endregion
	//oneGrain.endScale = 0.0f;

	obj->SetPosition(oneGrain.pos);
	obj->SetRotation(oneGrain.rot);

}

void Smoke::Update() {
	oneGrain.stratFrame++;

	//速度による移動
	oneGrain.pos += oneGrain.velocity * speed;
	//進行度を0～1の範囲に換算
	float f = ( float ) oneGrain.stratFrame / oneGrain.endFrame;
	//スケールの線形補間
	oneGrain.scale = ( oneGrain.endScale - oneGrain.stratScale ) * f;
	
	//カラー
	//float color;
	float alpha;
	//color = colorStart+( colorEnd - colorStart ) * Easing::easeOutCubic(f);
	alpha = alphaStart+( alphaEnd - alphaStart ) * Easing::easeOutSine(f);
	obj->SetPosition(oneGrain.pos);
	obj->SetScale({ oneGrain.scale, oneGrain.scale, oneGrain.scale });
	obj->SetColor({ colorStart,colorStart,colorStart,alpha });
	obj->Update();
}

void Smoke::Draw() {
	obj->Draw();
}