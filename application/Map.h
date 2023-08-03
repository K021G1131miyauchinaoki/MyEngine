#pragma once
#include<DirectXMath.h>
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>


class Map
{
public:
	//�R���X�g���N�^
	Map();
	//�f�X�g���N�^
	~Map();
public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model*model_);

	/// <summary>
	/// �X�V
	/// </summary>
	void Updata();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
private://�\����
	//�u���b�N
	struct Block {
		std::unique_ptr<Object3d> obj;
		Vector3	pos;
	};
private://�ÓI�����o�ϐ�
	static const int8_t width = 10;
	static const int8_t heith = 10;

private://�����o�ϐ�
	Vector3 scale;
	Block block[heith][width];

};

