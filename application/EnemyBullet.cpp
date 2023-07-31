#include "EnemyBullet.h"
#include<assert.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& rotation) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	velocity_ = velocity;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model);
	obj->SetRotation(rotation);
	obj->SetPosition(position);
	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();
	deathTimer = kLifeTime;
}

void EnemyBullet::Update() {
	{
		Vector3 pos = obj->GetPosition();
		pos.x += velocity_.x;
		pos.y += velocity_.y;
		pos.z += velocity_.z;
		obj->SetPosition(pos);
	}
	//���Ԍo�߂ŏ���
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
	obj->Update();
}

//���[���h���W��n��
Vector3 EnemyBullet::GetBulletPosition() {
	//���W���i�[
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos = obj->GetPosition();

	return worldPos;
}

//�Փ˂�����
void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw() {
	obj->Draw();
}

//�R���X�g���N�^
EnemyBullet::EnemyBullet() {

}
//�f�X�g���N�^
EnemyBullet::~EnemyBullet() {

}