/**
 * @file Camera.h
 * @brief　カメラ
 */
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
	//ビュー行列更新
	void UpdateView();
	//射影行列更新
	void UpdateProjection();
	//ビュープロジェクション行列更新
	void UpdateViewProjection();
	//位置セット
	void SetEye(XMFLOAT3 eye_);
	//ターゲットセット
	void SetTarget(XMFLOAT3 target_);
	//ベクトルのセット
	void SetUp(XMFLOAT3 up_);
	//ビュー行列セット
	void SetView(XMMATRIX matView_);
	//射影行列セット
	void SetProjection(XMMATRIX matProjection_);
	//画角のセット
	void SetFovAngle(float fovAngle_);

	//ビュー行列を取得
	const XMMATRIX& GetView();
	//射影行列を取得
	const XMMATRIX& GetMatProjection();
	//位置を取得
	const XMFLOAT3& GetEye();
	//ターゲットの取得
	const XMFLOAT3& GetTarget();
	//ベクトルの取得
	const XMFLOAT3& GetUp();
	//ビュープロジェクション行列の取得
	const XMMATRIX& GetViewProjection();
	//画角の取得
	const float& GetFovAngle();
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
	//画角
	float fovAngle;
	float oldFovAngle;
};

