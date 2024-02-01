#include "BaseBlock.h"
#include"Easing.h"
#include"GamePlayScene.h"

void BaseBlock::Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_) {
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition(pos_);
	obj->SetRotation(rot_);
	obj->SetScale(scale_);
	obj->Update();
}

void BaseBlock::Update() {
	obj->Update();
}

void BaseBlock::Draw() {
	obj->Draw();
}
void BaseBlock::SetParameter(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_) {
	obj->SetPosition(pos_);
	obj->SetRotation(rot_);
	obj->SetScale(scale_);
}

void BaseBlock::SetIsStaging(const bool& isStaging_) {
	isStaging = isStaging_;
}

void BaseBlock::Staging() {
	if ( stagingTime < stagingTimer && GamePlayScene::startCount >= GamePlayScene::Wait )
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