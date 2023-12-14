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

void Wall::Updata() {
	Staging();
	BaseBlock::Updata();
}

void Wall::Draw() {

	BaseBlock::Draw();
}

void Wall::Staging() {
	if ( stagingTime<stagingTimer&&GamePlayScene::startCount>=GamePlayScene::Wait)
	{
		Vector3 rot = obj->GetRotation();
		Vector3 scale = obj->GetScale();
		rot = rotStart + ( rotEnd - rotStart ) * Easing::easeOutQuint(stagingTime / stagingTimer);
		scale = scaleStart + ( scaleEnd - scaleStart ) * Easing::easeOutQuint(stagingTime / stagingTimer);
		obj->SetRotation(rot);
		obj->SetScale(scale);
		stagingTime++;
	}
}