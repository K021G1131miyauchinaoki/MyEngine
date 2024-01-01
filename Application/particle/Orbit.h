#pragma once
#include "BaseParticle.h"
#include"Object3d.h"
class Orbit :   public BaseParticle
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Orbit() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Orbit() = default;
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
	float alphaStart;
	float alphaEnd;
};

