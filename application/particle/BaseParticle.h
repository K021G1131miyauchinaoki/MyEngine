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
	virtual void Initialize(Model* model_);

	virtual void Update();

	virtual void Draw();

	virtual bool GetIsDead()const { return isDead; }
	
	BaseGrain oneGrain;
protected:
	Model* model;
	bool isDead = false;
};