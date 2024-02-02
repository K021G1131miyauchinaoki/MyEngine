/**
 * @file BaseParticle.h
 * @brief 継承用パーティクルの基盤
 */

#pragma once
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>
using namespace MyEngin;

class BaseModelParticle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Model* model_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// デスフラグの取得
	/// </summary>
	virtual bool GetIsDead()const { return isDead; }

protected:
	std::unique_ptr<Object3d>obj;
	//座標
	Vector3 pos = {};
	//回転
	Vector3 rot = {};
	//速度
	Vector3 velocity = {};
	//色
	Vector3 color = {};
	//加速度
	Vector3 accel = {};
	//現在フレーム
	int32_t stratFrame = 0;
	//終了フレーム
	int32_t endFrame = 0;
	//スケール
	float scale;
	//初期値
	float stratScale = 1.0f;
	//最終値
	float endScale = 0.0f;
	bool isDead = false;
	const float speed = 1.0f;
	Vector3 acc;
	//移動
	Vector3 move;
};