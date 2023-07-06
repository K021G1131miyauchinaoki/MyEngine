#include "Bullet.h"

#include<assert.h>
void Bullet::Initialize(Model* model, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	velocity_ = velocity;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model);
	obj->SetPosition(position);
	obj->Update();
	deathTimer = kLifeTime;
}

void Bullet::Update() {
	{
		DirectX::XMFLOAT3 pos=obj->GetPosition();
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
DirectX::XMFLOAT3 Bullet::GetBulletPosition() {
	//���W���i�[
	DirectX::XMFLOAT3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos = obj->GetPosition();

	return worldPos;
}

//�Փ˂�����
void Bullet::OnCollision() { isDead_ = true; }

void Bullet::Draw() {
	obj->Draw();
}

//�R���X�g���N�^
Bullet::Bullet() {
	
}
//�f�X�g���N�^
Bullet::~Bullet() {
	
}