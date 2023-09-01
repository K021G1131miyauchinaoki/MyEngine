#include "ParticleManager.h"
#include"Explosion.h"

void ParticleManager::Initialize(Model* model_) {
	assert(model_);
	model.reset(model_);
}
void ParticleManager::Add(const std::string &name,int amount, int32_t life,Vector3	position,/* Vector3	velocity, Vector3	accel,*/ float	start_scale, float	end_scale) {
	for (size_t i = 0; i < 30; i++)
	{
		BaseParticle* p;
		if (name=="1")
		{
			p = new Explosion();
		}
		else {
			p = new Explosion();
		}

		p->oneGrain.pos = position;
		p->oneGrain.endFrame = life;
		p->oneGrain.stratScale = start_scale;
		p->oneGrain.endScale = end_scale;
		p->Initialize(model.get());
		particles.emplace_front(p);
	}
	
}
void ParticleManager::Update() {
	//ƒtƒ‰ƒO‚ª—§‚Á‚½‚çíœ
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