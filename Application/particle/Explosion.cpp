/**
 * @file Explosion.cpp
 * @brief BaseParticleを継承した爆発パーティクル
 */

#include "Explosion.h"
#include<random>
#include<MyMath.h>

Explosion::~Explosion() {}

void Explosion::Initialize(Model* model_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_) {

	BaseModelParticle::Initialize(model_,life_,position_,startScale_,endScale_,color_);
#pragma region 乱数
	//xyzをランダムに分布
	Vector3 position = { 0,0,0 };
	const float rnd_pos = 2.0f;
	position.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	position.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	position.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	pos += position;

	//xyzをランダムに分布
	Vector3 value = { 0,0,0 };
	const float rnd_vel = 1.0f;
	value.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	value.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	value.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	velocity = value;


	
	const float rnd_rot = 5.0f;
	rotation.x = ( float ) rand() / RAND_MAX * rnd_rot - rnd_rot / 2.0f;
	rotation.y = ( float ) rand() / RAND_MAX * rnd_rot - rnd_rot / 2.0f;
	rotation.z = ( float ) rand() / RAND_MAX * rnd_rot - rnd_rot / 2.0f;

	//乱数　（スケール）
	//stratScale = 1.0f;
#pragma endregion
	//endScale = 0.0f;

	obj->SetPosition(pos);

}

void Explosion::Update() {
	
	//速度による移動
	pos += velocity * speed;
	//進行度を0～1の範囲に換算
	float f = (float)stratFrame / endFrame;
	//スケールの線形補間
	scale = (endScale - stratScale) * f;
	scale += stratScale;
	//回転
	rot += rotation;

	obj->SetPosition(pos);
	obj->SetScale({ scale, scale, scale });
	obj->SetRotation(rot);
	BaseModelParticle::Update();
}

void Explosion::Draw() {
	obj->Draw();
}