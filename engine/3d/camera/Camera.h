#pragma once
#include<DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	//初期化
	void Initialeze();
	//更新
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
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	//ビュープロジェクション行列
	XMMATRIX matViewProjection;
	// 視点座標
	XMFLOAT3 eye;
	// 注視点座標
	XMFLOAT3 target;
	// 上方向ベクトル
	XMFLOAT3 up;
};

