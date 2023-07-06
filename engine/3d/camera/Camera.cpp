#include "Camera.h"
#include"WinApp.h"

void	Camera::Initialeze() {
	eye = { 10, 10, 10 };
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };

	UpdateView();
	UpdateProjection();
}

void	Camera::Update() {
	UpdateView();
	UpdateViewProjection();
}

void	Camera::UpdateView() {
	// �r���[�s��̐���
	matView = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));
}

void	Camera::UpdateProjection() {
	// �������e�ɂ��ˉe�s��̐���
	matProjection = DirectX::XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)WinApp::width / WinApp::height,
		0.1f, 1000.0f);
}

void	Camera::UpdateViewProjection() {
	matViewProjection = matView * matProjection;
}
