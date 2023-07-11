#pragma once
#include<DirectXMath.h>
#include<Model.h>
#include<Object3d.h>

class Bullet
{
public:
	Bullet();
	~Bullet();
	void Initialize(Model* model, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& veclocity, const DirectX::XMFLOAT3& rotation_);
	void Update();
	void Draw();
	bool IsDead() const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	//���[���h���W���擾
	DirectX::XMFLOAT3 GetBulletPosition();
	//���a���擾
	float GetRadius() { return r; }

private:
	
	Model* model_ = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//���x
	DirectX::XMFLOAT3 velocity_;
	//����
	static const int32_t kLifeTime = 30;
	//�f�X�^�C�}�[
	int32_t deathTimer;
	//�f�X�t���O
	bool isDead_ = false;

	//���a
	const float r = 1.0f;
};
