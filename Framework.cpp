#include "Framework.h"

void Framework::Initialize(){}

void Framework::Update(){}

void Framework::Draw(){}

void Framework::Finalize(){}

void Framework::Run() {
	//������
	Initialize();

	//���[�v
	while (true) {
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (IsEndRequst())
		{
			break;
		}

		//�`��
		Draw();
	}
	//�Q�[���I��
	Finalize();
}