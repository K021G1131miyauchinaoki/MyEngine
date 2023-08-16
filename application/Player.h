#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Bullet.h>
#include<Aimposition.h>
#include<Vector3.h>
#include<Vector2.h>
#include<MyMath.h>

class Player
{
public://�ÓI�����o�֐�
	//Player GetInstnce();

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialeze(Model*model_,Input*input_,Aimposition*aim_);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

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

	/// <summary>
	/// �ʒu
	/// </summary>
	/// <returns></returns>
	const	Vector3 GetPos() {return obj->GetPosition(); }

	void OnCollision();

	Vector3 GetVelocity(){ return velocity; }

	const	float GetRadius() { return radius; }

	std::list<std::unique_ptr<Bullet>>& GetBullets() { return bullets_; };


private://�����o�ϐ�
	Input*input = nullptr;
	Model* model = nullptr;
	Aimposition* aim;
	std::unique_ptr<Object3d>obj = nullptr;
	//�e
	std::list<std::unique_ptr<Bullet>> bullets_;
	//�N�[���^�C��
	int32_t coolTime;

	float angle = 0.0f;
	Vector3 velocity;
	Vector2	vector ;
	Vector2	 mausePos;
	//���a
	float radius = 5.0f;

	bool isInvincible = false;
	const int16_t invincibleTime=11;
	int16_t invincibleTimer;
};

