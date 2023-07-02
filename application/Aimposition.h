#pragma once
#include<DirectXMath.h>
#include<Model.h>
#include<Object3d.h>
#include<Input.h>

class Aimposition
{
public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialeze(Model* model_, Input* input_);

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

private://�����o�ϐ�
	Input* input = nullptr;
	Model* model = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//�N�[���^�C��
	int32_t coolTime;
};


