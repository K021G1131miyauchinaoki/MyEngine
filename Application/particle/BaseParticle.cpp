/**
 * @file BaseParticle.cpp
 * @brief 継承用パーティクルの基盤
 */

#include "BaseParticle.h"
#include<cassert>

void BaseParticle::Initialize(Model* model_,int32_t life_,Vector3 position_,float startScale_,float endScale_,Vector3 color_) {
	assert(model_);
	pos = position_;
	rot = {0.0f,0.0f,0.0f};
	color = color_;
	stratFrame = 0;
	endFrame = life_;
	stratScale = startScale_;
	endScale = endScale_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition(pos);
	obj->SetScale({startScale_,startScale_, startScale_});
	obj->SetColor({ color.x,color.y, color.z, 1.0f });
}

void BaseParticle::Update(){
	if ( stratFrame >= endFrame )
	{
		isDead = true;
	}
	stratFrame++;
	obj->Update();
}

void BaseParticle::Draw(){}