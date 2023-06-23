#pragma once
#include<DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	//������
	void Initialeze();
	//�X�V
	void Update();
	void UpdateView();
	void UpdateProjection();
	void UpdateViewProjection();
	//setter
	void SetEye(XMFLOAT3 eye_) { this->eye = eye_; }
	void SetTarget(XMFLOAT3 target_) { this->target = target_; }
	void SetUp(XMFLOAT3 up_) { this->up = up_; }
	void SetView(XMMATRIX matView_) { this->matView = matView_; }
	void SetProjection(XMMATRIX matProjection_) { this->matProjection = matProjection_; }

	//getter
	const XMMATRIX& GetView() { return matView; }
	const XMMATRIX& GetProjection() { return matProjection; }
	const XMFLOAT3& GetEye() { return eye; }
	const XMFLOAT3& GetTarget() { return target; }
	const XMFLOAT3& GetUp() { return up; }
	const XMMATRIX& GetViewProjection() { return matViewProjection; }

private:
	// �r���[�s��
	XMMATRIX matView;
	// �ˉe�s��
	XMMATRIX matProjection;
	//�r���[�v���W�F�N�V�����s��
	XMMATRIX matViewProjection;
	// ���_���W
	XMFLOAT3 eye;
	// �����_���W
	XMFLOAT3 target;
	// ������x�N�g��
	XMFLOAT3 up;
};

