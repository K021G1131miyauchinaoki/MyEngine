#include "Wall.h"
#include"Easing.h"
#include"GamePlayScene.h"

void Wall::Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_) {
	rotEnd = rot_;
	rotStart = { 0.0f,360.0f,0.0f };
	scaleEnd = scale_;
	scaleStart = { 0.0f,0.0f,0.0f };
	BaseBlock::Initialize(pos_,rotStart,scaleStart,model_);
	isStaging = false;
	stagingTime = 0;
}

void Wall::Update() {
	BaseBlock::Staging();
	BaseBlock::Update();
}

void Wall::Draw() {

	BaseBlock::Draw();
}
