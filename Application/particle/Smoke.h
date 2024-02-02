	/**
 * @file Explosion.h
 * @brief BaseParticleを継承した爆発パーティクル
 */
#pragma once
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>
#include<BaseModelParticle.h>
#include<memory>
class Smoke :public BaseModelParticle
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Smoke() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Smoke()= default;
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
	
	float colorStart;
	float colorEnd;
	float alphaStart;
	float alphaEnd;
};

