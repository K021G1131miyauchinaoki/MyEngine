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
	Map() :constStartY(0.0f) {};
	//�f�X�g���N�^
	~Map();
public://�ÓI�����o�֐�
	static void StaticInitialize(Model* model_);
	static void Finalize();

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(bool isStaging_);

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

	//����
	void Preparation();
	//���o
	void Staging(size_t y_,size_t x_);


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
public:
	static float moveLimitW;
	static float moveLimitH;
	static float mapScaleW;
	static float mapScaleH;
private://�����o�ϐ�
	//���o��邩���Ȃ����p�t���O
	bool isStaging;

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
	int8_t height;
	int16_t lineNum;

	const int time = 20;
	int timer = 0;
	int16_t nowMax;
	int16_t setPoint;
	int16_t numH;
	int16_t numW;
};

