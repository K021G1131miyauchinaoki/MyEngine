#include "EnemyBullet.h"
#include<assert.h>

std::unique_ptr < Model>  EnemyBullet::model=nullptr;

void EnemyBullet::StaticInitialize(Model* model_) {
	model.reset(model_);
}

void EnemyBullet::Initialize( const Vector3& position, const Vector3& velocity, const Vector3& rotation) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	velocity_ = velocity;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model.get());
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

//�I��
void EnemyBullet::Finalize() {
	model.release();
}

//�R���X�g���N�^
EnemyBullet::EnemyBullet() {

}
//�f�X�g���N�^
EnemyBullet::~EnemyBullet() {
	
}