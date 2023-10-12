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
class ParticleManager
{
public:
	void Initialize(Model* model_);

	void Update();

	void Draw();

	void Add(const std::string& name, int amount, int32_t life, Vector3	position, /*Vector3 velocity, Vector3 accel, */ float start_scale, float end_scale);

private:
	std::forward_list<std::unique_ptr<BaseParticle>>particles;
	std::unique_ptr < Model>model;
};

