#include "BaseBlock.h"

void BaseBlock::Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_) {
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition(pos_);
	obj->SetRotation(rot_);
	obj->SetScale(scale_);
	obj->Update();
	frame = 0;
}

void BaseBlock::Updata() {
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

//Vector3 BaseBlock::GetPos() {
//	return obj->GetPosition();
//}
//
//Vector3 BaseBlock::GetRot() {
//	return obj->GetRotation();
//}
//
//Vector3 BaseBlock::GetScale() {
//	return obj->GetScale();
//}