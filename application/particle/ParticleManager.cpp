/**
 * @file ParticleManager.cpp
 * @brief パーティクルのマネージャー
 */
#include "ParticleManager.h"
#include"Explosion.h"
#include"Smoke.h"

void ParticleManager::Initialize(Model* model_) {
	assert(model_);
	model.reset(model_);
}
void ParticleManager::Add(const std::string& name_,int amount_,int32_t life_,Vector3 position_,float startScale_,float endScale_) {
	for (size_t i = 0; i < amount_; i++)
	{
		BaseParticle* p;
		if (name_=="1")
		{
			p = new Explosion();
		}
		else {
			p = new Smoke();
		}

		p->oneGrain.pos = position_;
		p->oneGrain.endFrame = life_;
		p->oneGrain.stratScale = startScale_;
		p->oneGrain.endScale = endScale_;
		p->Initialize(model.get());
		particles.emplace_front(p);
	}
	
}
void ParticleManager::Update() {
	//フラグが立ったら削除
	particles.remove_if([](std::unique_ptr<BaseParticle>& p) {
		return p->oneGrain.stratFrame >= p->oneGrain.endFrame;
		});

	for (std::unique_ptr<BaseParticle>& p : particles) {
		p->Update();
		if (p->oneGrain.stratFrame >= p->oneGrain.endFrame)
		{
			//p->isDead = true;
		}
	}
}

void ParticleManager::Draw() {
	for (std::unique_ptr<BaseParticle>& p : particles) {
		p->Draw ();
	}
}