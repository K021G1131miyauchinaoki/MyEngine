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

void BaseBlock::StartStaging() {
	if (GamePlayScene::isStart&&
		GamePlayScene::startCount >= GamePlayScene::Redy&&
		stagingTime < stagingTimer)
	{
		Vector3 rot = obj->GetRotation();
		Vector3 scale = obj->GetScale();
		rot = rotStart + ( rotEnd - rotStart ) * Easing::easeOutQuint(stagingTime / stagingTimer);
		scale = scaleStart + ( scaleEnd - scaleStart ) * Easing::easeOutQuint(stagingTime / stagingTimer);
		obj->SetRotation(rot);
		obj->SetScale(scale);
		stagingTime++;
	}
	//タイマーのリセット
	if (!GamePlayScene::isStart&&
		!GamePlayScene::isOut )
	{
		stagingTime = 0;
	}
}

void BaseBlock::OutStaging()
{
	if ( GamePlayScene::isOut &&
		GamePlayScene::outCount >= GamePlayScene::Roll &&
		stagingTime < stagingTimer )
	{
		//回転、スケールをイージング
		Vector3 rot = obj->GetRotation();
		Vector3 scale = obj->GetScale();
		rot = rotEnd + ( rotStart - rotEnd ) * Easing::easeOutQuint(stagingTime / stagingTimer);
		scale = scaleEnd + ( scaleStart - scaleEnd ) * Easing::easeOutQuint(stagingTime / stagingTimer);
		obj->SetRotation(rot);
		obj->SetScale(scale);
		stagingTime++;
		//タイマーが一定値超えたらカウントを進める
		if ( stagingTime >= stagingTimer )
		{
			GamePlayScene::outCount++;
		}
	}
}
