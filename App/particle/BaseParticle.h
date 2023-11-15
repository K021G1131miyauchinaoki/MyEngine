/**
 * @file BaseParticle.h
 * @brief 継承用パーティクルの基盤
 */

#pragma once
#include<Model.h>
#include<BaseGrain.h>

class BaseParticle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Model* model_);

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

	//パーティクルの一粒
	BaseGrain oneGrain;
protected:
	Model* model;
	bool isDead = false;
};