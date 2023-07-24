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

private://�����o�ϐ�
	Input* input = nullptr;
	Model* model = nullptr;
	
	std::unique_ptr<Object3d>obj = nullptr;
	//�e
	std::list<std::unique_ptr<Bullet>> bullets_;
	//Bullet* one;
	//�N�[���^�C��
	int32_t coolTime;

	//���L����
	Player* player = nullptr;

	float angle = 0.0f;
	Vector3 velocity;
};