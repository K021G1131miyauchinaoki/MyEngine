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
public://�ÓI�����o�֐�
	static void StaticInitialize(Model* model_);
	static void Finalize();

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	// �ǂݍ���
	void LoadCSV(const std::string& num_);

private://�\����
	//�u���b�N
	struct Block {
		std::unique_ptr<Object3d> obj;
		Vector3	pos;
		bool isUp = false;
		bool isDrow = false;
		float frame;
		float range = 0.0f;
	};
private://�ÓI�����o�ϐ�
	static	std::unique_ptr < Model> model;

private://�����o�ϐ�
	/*�C�[�W���O*/
	float endFrame;
	//�ʒu
	float posStartY;
	float posEndY;
	//�X�P�[��
	Vector3 scaleEnd;
	Vector3 scaleStart;
	//��]
	float rotEndZ;
	float rotStartZ;
	//�u���b�N
	std::vector<std::vector< Block>> blocks;
	const float constStartY;
	int8_t width;
	int8_t high;
	int16_t lineNum;

	const int time = 20;
	int timer = 0;
	int16_t nowMax;
	int16_t setPoint;
	int16_t numH;
	int16_t numW;
};

