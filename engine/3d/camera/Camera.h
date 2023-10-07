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
	void SetEye(XMFLOAT3 eye_);
	void SetTarget(XMFLOAT3 target_);
	void SetUp(XMFLOAT3 up_);
	void SetView(XMMATRIX matView_);
	void SetProjection(XMMATRIX matProjection_);

	//getter
	const XMMATRIX& GetView();
	const XMMATRIX& GetProjection();
	const XMFLOAT3& GetEye();
	const XMFLOAT3& GetTarget();
	const XMFLOAT3& GetUp();
	const XMMATRIX& GetViewProjection();
private:
	// 視点座標
	XMFLOAT3 eye;
	// 注視点座標
	XMFLOAT3 target;
	// 上方向ベクトル
	XMFLOAT3 up;
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	//ビュープロジェクション行列
	XMMATRIX matViewProjection;
	//ビューポート行列
	XMMATRIX matViewPort;
};

