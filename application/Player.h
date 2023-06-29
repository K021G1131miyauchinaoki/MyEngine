#pragma once
#include<Input.h>
#include<Model.h>
#include<Object3d.h>


class Player
{
public://�ÓI�����o�֐�
	Player GetInstnce();

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialeze();

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

private://�����o�ϐ�
	std::unique_ptr<Input>input;
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> obj;

};

