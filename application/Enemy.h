#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<EnemyBullet.h>
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

	/// <summary>
	/// �ʒu
	/// </summary>
	/// <returns></returns>
	const	Vector3 GetPos() { return obj->GetPosition(); }

	const	float GetRadius() { return radius; }

	void OnCollision();

	//�e���X�g���擾
	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
private://�\����
	


private://�����o�ϐ�
	Input* input = nullptr;
	Model* model = nullptr;
	
	std::unique_ptr<Object3d>obj = nullptr;
	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
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
	Vector3 value;
	//�f�X�t���O
	bool isDead = false;

	float radius = 5.0f;

	//��
	bool isInvincible = false;
	const int16_t invincibleTime = 11;
	int16_t invincibleTimer;
};