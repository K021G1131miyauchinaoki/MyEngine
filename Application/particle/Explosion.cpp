/**
 * @file Explosion.cpp
 * @brief BaseParticleを継承した爆発パーティクル
 */

#include "Explosion.h"
#include<random>
#include<MyMath.h>

Explosion::~Explosion() {}

void Explosion::Initialize(Model* model_) {

	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
#pragma region 乱数
	//xyzをランダムに分布
	Vector3 pos = { 0,0,0 };
	const float rnd_pos = 2.0f;
	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	oneGrain.pos += pos;

	//xyzをランダムに分布
	Vector3 value = { 0,0,0 };
	const float rnd_vel = 1.0f;
	value.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	value.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	value.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	oneGrain.velocity = value;

	//乱数　（スケール）
	//oneGrain.stratScale = 1.0f;
#pragma endregion
	//oneGrain.endScale = 0.0f;

	obj->SetPosition(oneGrain.pos);

}

void Explosion::Update() {
	oneGrain.stratFrame++;

	//速度による移動
	oneGrain.pos += oneGrain.velocity * speed;
	//進行度を0～1の範囲に換算
	float f = (float)oneGrain.stratFrame / oneGrain.endFrame;
	//スケールの線形補間
	oneGrain.scale = (oneGrain.endScale - oneGrain.stratScale) * f;
	oneGrain.scale += oneGrain.stratScale;

	obj->SetPosition(oneGrain.pos);
	obj->SetScale({ oneGrain.scale, oneGrain.scale, oneGrain.scale });
	obj->Update();
}

void Explosion::Draw() {
	obj->Draw();
}