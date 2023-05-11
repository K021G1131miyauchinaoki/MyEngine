#include "WorldTransform.h"

void WorldTransform::Initialize() {

	scale = { 1 , 1 , 1 };
	rotation = { 0 , 0 , 0 };
	translation = { 0 , 0 , 0 };
	matWorld=MatIdentity();

	Update();

}

//���[���h�ϊ��s��̍X�V
void WorldTransform::Update() {

	//���[���h�ϊ��s���p��
	Matrix4 affineMat;
	affineMat=MatIdentity();

	//���[���h�ϊ��s��ɃX�P�[�����O,��],���s�ړ��̍s�������
	affineMat*= MatScale(scale);
	affineMat*=MatRot(rotation);
	affineMat*=MatTrans(translation);

	//���[���h�s��ɒP�ʍs�����
	matWorld=MatIdentity();

	//�s��̌v�Z
	matWorld *= affineMat;

	//�����e������ꍇ
	if (parent) {
		//�e�̃��[���h�s��Ƃ̌v�Z���s��
		matWorld *= parent->matWorld;
	}

}