#pragma once
#include"obj.h"

Object3d	player;
int	hpP;
bool	isPlayer;

Object3d	enemy;
int	hpE;
bool	isEnemy;


//�V�[���Ǘ�
enum Scene
{
	title,
	play,
	clear,
	over
};
int scene;

enum Movie
{
	nonMovie,	 //���[�r�[�Ȃ�
	appearance,  //�{�X�o��
};
int movie;


// ���`���
//�擪�ƍŌ�ɐ���_��ǉ����Ă���
std::vector<XMFLOAT3>points;
//p1����X�^�[�g����
size_t startIndex;
XMFLOAT3	start = { 30,-14,26 };//�X�^�[�g�n�_
XMFLOAT3	p2 = { 20, -7 , 10 };
XMFLOAT3	p3 = { 14,-4,-6 };
XMFLOAT3	p4 = { 0,3,-35 };
XMFLOAT3	end = { 0,3,-55 };//�G���h�n�_
float timerTrans;
const	float timerTransMax = 100;
float	timeRate;

int	switchTimer;

XMFLOAT3	startTrans;
XMFLOAT3	endTrans;
const	float startFovAngle = 20.0f;
const	float endFovAngle = 45.0f;
float timerFovAngle;
const float timeFoAngleMax = 25;

//�\����
//�v���C���[
struct Obj
{
	//�֐�
		//������
	void Initialize(ID3D12Device* device, uint32_t textureHandle);

	//���[�v���̏�����
	void State();

	//�X�V
	void Update(int	movie);

	//�ړ�
	void Move();

	//��]
	void Rotate();

	//�U��
	void Attack();

	//�`��
	void Draw(XMMATRIX matView);

	//���[���h���W���擾
	XMFLOAT3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���a��Ԃ��֐�
	float GetRadius();

	int32_t GetHP()const { return HP_; }
	//�ϐ�

	// //���[���h�g�����X�t�H�[��
	Object3d object3d_;
	// ���f��
	ID3D12Device* device_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�̗�
	int32_t HP_ = 3;
	//���a
	float radius = 1.0f;
};
