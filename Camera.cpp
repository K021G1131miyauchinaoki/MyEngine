#include "Camera.h"
#include"WinApp.h"

void	Camera::Initialeze() {
	eye = { 0, 0, -50.0f };
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };

	UpdateView();
	UpdateProjection();
}

void	Camera::Update() {
	UpdateView();
}

void	Camera::UpdateView() {
	// �r���[�s��̐���
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));
}

void	Camera::UpdateProjection() {
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)WinApp::window_width / WinApp::window_height,
		0.1f, 1000.0f);
}
