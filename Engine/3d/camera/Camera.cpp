/**
 * @file Camera.cpp
 * @brief　カメラ
 */

#include "Camera.h"
#include"WinApp.h"
namespace MyEngin
{
	void	Camera::Initialize() {
		eye = { 10, 10, 10 };
		target = { 0, 0, 0 };
		up = { 0, 1, 0 };
		fovAngle = 60.0f;
		UpdateView();
		UpdateProjection();
	}

	void	Camera::Update() {
		UpdateView();
		UpdateViewProjection();
		if ( oldFovAngle != fovAngle )
		{
			UpdateProjection();
		}
	}

	void	Camera::UpdateView() {
		// ビュー行列の生成
		matView = DirectX::XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up));
	}

	void	Camera::UpdateProjection() {
		// 透視投影による射影行列の生成
		oldFovAngle = fovAngle;
		matProjection = DirectX::XMMatrixPerspectiveFovLH(
			XMConvertToRadians(fovAngle),
			( float ) WinApp::width / WinApp::height,
			0.1f,1000.0f);
	}

	void	Camera::UpdateViewProjection() {
		matViewProjection = matView * matProjection;
	}


	//setter
	void Camera::SetEye(XMFLOAT3 eye_) {
		this->eye = eye_;
	}
	void Camera::SetTarget(XMFLOAT3 target_) {
		this->target = target_;
	}
	void Camera::SetUp(XMFLOAT3 up_) {
		this->up = up_;
	}
	void Camera::SetView(XMMATRIX matView_) {
		this->matView = matView_;
	}
	void Camera::SetProjection(XMMATRIX matProjection_) {
		this->matProjection = matProjection_;
	}
	void Camera::SetFovAngle(float fovAngle_) {
		oldFovAngle = fovAngle;
		fovAngle = fovAngle_;
	}

	//getter
	const XMMATRIX& Camera::GetView() {
		return matView;
	}
	const XMMATRIX& Camera::GetMatProjection() {
		return matProjection;
	}
	const XMFLOAT3& Camera::GetEye() {
		return eye;
	}
	const XMFLOAT3& Camera::GetTarget() {
		return target;
	}
	const XMFLOAT3& Camera::GetUp() {
		return up;
	}
	const XMMATRIX& Camera::GetViewProjection() {
		return matViewProjection;
	}
	const float& Camera::GetFovAngle() {
		return fovAngle;
	}
}