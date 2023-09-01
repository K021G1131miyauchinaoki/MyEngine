#pragma once
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>
#include<BaseGrain.h>
#include<BaseParticle.h>
#include<memory>

class Explosion:public BaseParticle
{
public:
	~Explosion();

	void Initialize(Model* model)override;

	void Update()override;

	void Draw()override;

private:
	std::unique_ptr<Object3d>obj;
	float speed = 1.0f;
	Vector3 acc;
	//ˆÚ“®
	Vector3 move;
};

