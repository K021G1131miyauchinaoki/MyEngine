/**
 * @file ParticleManager.h
 * @brief パーティクルのマネージャー
 */

#pragma once
#include"BaseParticle.h"
#include<list>
#include<memory>
#include<Vector3.h>
#include<forward_list>
using namespace MyEngin;

class ParticleManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 追加
	/// </summary>
	void Add(const std::string& name_, int amount_, int32_t life_, Vector3	position_,float startScale_, float endScale_,Vector3 color_={1.0f,1.0f,1.0f});

private:
	std::forward_list<std::unique_ptr<BaseParticle>>particles;
	std::unique_ptr <Model>model;
};

