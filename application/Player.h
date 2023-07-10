#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Bullet.h>
#include<Aimposition.h>
#include<Vector3.h>

class Player
{
public://�ÓI�����o�֐�
	Player GetInstnce();

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialeze(Model*model_,Input*input_,Aimposition*aim_);

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

	Vector3 GetVelocity(){ return velocity; }

private://�����o�ϐ�
	Input*input = nullptr;
	Model* model = nullptr;
	Aimposition* aim;
	std::unique_ptr<Object3d>obj = nullptr;
	//�e
	std::list<std::unique_ptr<Bullet>> bullets_;
	//Bullet* one;
	//�N�[���^�C��
	int32_t coolTime;

	float angle = 0.0f;
	Vector3 velocity;
};

