/**
 * @file Explosion.h
 * @brief BaseParticleを継承した爆発パーティクル
 */

#pragma once

#include<BaseModelParticle.h>
#include<memory>

class Explosion:public BaseModelParticle
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Explosion();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:
	Vector3 rotation = { 0,0,0 };
};

