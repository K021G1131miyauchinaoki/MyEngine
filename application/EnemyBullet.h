#pragma once
#include<DirectXMath.h>
#include<Model.h>
#include<Object3d.h>
#include<Vector3.h>

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();
	static void StaticInitialeze(Model* model_);
	void Initialize(Model* model, const Vector3& position, const Vector3& veclocity, const Vector3& rotation_);
	void Update();
	void Draw();
	bool IsDead() const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	//���[���h���W���擾
	Vector3 GetBulletPosition();
	//���a���擾
	float GetRadius() { return r; }

private:

	Model* model_ = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//���x
	Vector3 velocity_;
	//����
	static const int32_t kLifeTime = 60;
	//�f�X�^�C�}�[
	int32_t deathTimer;
	//�f�X�t���O
	bool isDead_ = false;

	//���a
	const float r = 1.0f;
};

