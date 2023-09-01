#include "Explosion.h"
#include<random>
#include<MyMath.h>

Explosion::~Explosion() {}

void Explosion::Initialize(Model* model_) {

	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
#pragma region ����
	//xyz�������_���ɕ��z
	Vector3 pos = { 0,0,0 };
	const float rnd_pos = 2.0f;
	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	oneGrain.pos += pos;

	//xyz�������_���ɕ��z
	Vector3 value = { 0,0,0 };
	const float rnd_vel = 1.0f;
	value.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	value.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	value.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	oneGrain.velocity = value;
	
	//�d�͂Ɍ����Ă�Y�̂݃����_���ɕ��z
	acc = { 0,0,0 };
	const float rnd_acc = 0.5f;
	acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
	
	oneGrain.accel = acc;

	//�����@�i�X�P�[���j
	//oneGrain.stratScale = 1.0f;
#pragma endregion
	//oneGrain.endScale = 0.0f;

	obj->SetPosition(oneGrain.pos);

}

void Explosion::Update() {
	oneGrain.stratFrame++;

	//���x�ɂ��ړ�
	oneGrain.pos += oneGrain.velocity * speed;
	//�i�s�x��0�`1�͈̔͂Ɋ��Z
	float f = (float)oneGrain.stratFrame / oneGrain.endFrame;
	//�X�P�[���̐��`���
	oneGrain.scale = (oneGrain.endScale - oneGrain.stratScale) * f;
	oneGrain.scale += oneGrain.stratScale;

	obj->SetPosition(oneGrain.pos);
	obj->SetScale({ oneGrain.scale, oneGrain.scale, oneGrain.scale });
	obj->Update();
}

void Explosion::Draw() {
	obj->Draw();
}