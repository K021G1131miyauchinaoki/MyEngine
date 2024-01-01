/**
 * @file ParticleManager.cpp
 * @brief パーティクルのマネージャー
 */

#include "ParticleManager.h"
#include"Explosion.h"
#include"Smoke.h"
#include "Orbit.h"

void ParticleManager::Initialize(Model* model_) {
	assert(model_);
	model.reset(model_);
}
void ParticleManager::Add(const std::string& name_,int amount_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_) {
	for (size_t i = 0; i < amount_; i++)
	{
		std::unique_ptr <BaseParticle> p;
		if (name_=="explosion")
		{
			p = std::make_unique <Explosion>();
		}
		else if( name_ == "smoke" ) {
			p = std::make_unique <Smoke>();
		}
		else if ( name_ == "orbit" )
		{
			p = std::make_unique <Orbit>();
		}
		else
		{
			p = std::make_unique <Explosion>();
		}
		p->Initialize(model.get(),life_,position_,startScale_,endScale_,color_);
		particles.emplace_front(std::move(p));
	}
	
}
void ParticleManager::Update() {
	//フラグが立ったら削除
	particles.remove_if([](std::unique_ptr<BaseParticle>& p) {
		return p->GetIsDead();
		});

	for (std::unique_ptr<BaseParticle>& p : particles) {
		p->Update();
		
	}
}

void ParticleManager::Draw() {
	for (std::unique_ptr<BaseParticle>& p : particles) {
		p->Draw ();
	}
}