#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Bullet.h>
#include<Aimposition.h>
#include<Vector3.h>
#include<Player.h>
#include<MyMath.h>

//���@�N���X�̑O���O��
class Player;


//�s���t�F�[�Y
enum class Phase {
	wait,//�ҋ@
	move, //�ړ�����
	atack,    //�U������
};

class Enemy
{
public://�����o�֐�
/// <summary>
/// ������
/// </summary>
	void Initialeze(Model* model_, Player*player_);

	/// <summary>
	/// �X�V
	/// </summary>
	void Updata();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ҋ@
	/// </summary>
	void Wait();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ����
	/// </summary>
	void Shot();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	///�f�X�t���O 
	/// </summary>
	bool IsDead() { return isDead; }
private://�\����
	


private://�����o�ϐ�
	Input* input = nullptr;
	Model* model = nullptr;
	
	std::unique_ptr<Object3d>obj = nullptr;
	//�e
	std::list<std::unique_ptr<Bullet>> bullets_;
	//�ړ��t���O
	bool isMove = false;
	//�ړ�����
	int16_t moveTimer;
	const int16_t  moveTime = 120;
	//���˂���܂ł̎���
	int16_t shotTimer;
	const int16_t  shotTime = 10;
	//�ҋ@�t���O
	bool isWait = false;
	//�ҋ@����
	int16_t waitTimer;
	int16_t waitTime[2] = { 5,10 };
	//���L����
	Player* player = nullptr;

	float angle = 0.0f;
	//���x
	Vector3 velocity;
	//�ړ�
	Vector3 move;
	//�t�F�[�Y
	Phase phase = Phase::move;
	//�����l�i�[
	float value;
	//�f�X�t���O
	bool isDead = false;
};