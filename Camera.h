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
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	// 視点座標
	XMFLOAT3 eye;
	// 注視点座標
	XMFLOAT3 target;
	// 上方向ベクトル
	XMFLOAT3 up;
};

