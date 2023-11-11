/**
 * @file Explosion.h
 * @brief BaseParticleを継承した爆発パーティクル
 */

#pragma once
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>
#include<BaseGrain.h>
#include<BaseParticle.h>
#include<memory>

class Explosion:public BaseParticle
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Explosion();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model_)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:
	std::unique_ptr<Object3d>obj;
	const float speed = 1.0f;
	Vector3 acc;
	//移動
	Vector3 move;
};

