/**
 * @file BaseParticle.cpp
 * @brief 継承用パーティクルの基盤
 */

#include "BaseParticle.h"
#include<cassert>

void BaseParticle::Initialize(Model* model_) {
	assert(model_);
	model = model_;
}

void BaseParticle::Update(){

}

void BaseParticle::Draw(){}