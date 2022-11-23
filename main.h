#pragma once
#include"obj.h"

Object3d	player;
int	hp;

//�V�[���Ǘ�
enum Scene
{
	title,
	play,
	clear,
	over
};
int scene;

//�\����
//�v���C���[
struct Player
{

	//�ϐ�
	int32_t HP_ = 3;
	//���a
	float radius = 1.0f;
};
//�G�l�~�[
struct Enemy
{
	//�֐�
	/// ����
	void Initialize(ID3D12Device* device, uint32_t textureHandle);

	/// �X�V
	void Update(int movie);

	/// �`��
	void Draw(XMMATRIX matView);

	//���[�v���̏�����
	void	State();

	//���[���h���W���擾
	XMFLOAT3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���a��Ԃ��֐�
	float GetRadius();

	//�G�e�f���[�g
	void	BulletDelete();

	int32_t GetHP()const { return HP_; }


	//�ϐ�
	// //���[���h�g�����X�t�H�[��
	Object3d object3d_;
	// ���f��
	ID3D12Device* device_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	 
	//���x
	XMFLOAT3 velocity_ = { 0.0f,0.0f,0.1f };

	//�̗�
	int32_t HP_ = 20;

	//���a
	float radius = 15.0f;
};
