#pragma once
#include<DirectXMath.h>
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>

class Map
{
public:
	//�R���X�g���N�^
	Map(float value) :constStartY(value) {};
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

	Vector3 get() { return block[1][1].obj->GetPosition(); }

private://�\����
	//�u���b�N
	struct Block {
		std::unique_ptr<Object3d> obj;
		Vector3	pos;
		bool isUp = false;
		float frame;
		float y = 0.0f;
	};
private://�ÓI�����o�ϐ�
	static const int8_t width = 5;
	static const int8_t heith = 7;

private://�����o�ϐ�
	//�C�[�W���O
	float endFrame;
	float startY;
	float endY;
	//�X�P�[��
	Vector3 scale;
	//�u���b�N
	Block block[heith][width];
	int16_t num;
	const float constStartY;

	const int time = 60;
	int timer = 0;
	int16_t nowMax;
	int16_t setPoint;
	int16_t numH;
	int16_t numW;
};

