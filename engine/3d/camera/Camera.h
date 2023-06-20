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
	//setter
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }
	void SetView(XMMATRIX matView) { this->matView = matView; }
	void SetProjection(XMMATRIX matProjection) { this->matProjection = matProjection; }
	//getter
	const XMMATRIX& GetView() { return matView; }
	const XMMATRIX& GetProjection() { return matProjection; }
	const XMFLOAT3& GetEye() { return eye; }
	const XMFLOAT3& GetTarget() { return target; }
	const XMFLOAT3& GetUp() { return up; }


private:
	// �r���[�s��
	XMMATRIX matView;
	// �ˉe�s��
	XMMATRIX matProjection;
	// ���_���W
	XMFLOAT3 eye;
	// �����_���W
	XMFLOAT3 target;
	// ������x�N�g��
	XMFLOAT3 up;
};

