#pragma once
#include<DirectXMath.h>

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
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Updata();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

public://�����o�ϐ�
	const int8_t width = 10;
	const int8_t heith = 10;



};

