#include "Fixedgun.h"
#include"MyMath.h"
#include"GamePlayScene.h"
#include<cmath>

void Fixedgun::Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_) {
	BaseBlock::Initialize(pos_,rot_,scale_,model_);
	const float kBulletSpeed = 1.5f;
	float radian = -MyMath::RadianTransform(rot_.y);
	velocity = { std::cos(radian),0.0f,std::sin(radian) };
	velocity = MyMath::normaleizeVec3(velocity);
	velocity *= kBulletSpeed;
	waitTime = 0;
	pos = GetPos();
	pos.x = GetPos().x + std::cos(radian) * ( GetScale().x + 2.0f );
	pos.z = GetPos().z + std::sin(radian) * ( GetScale().z + 2.0f );
}

void Fixedgun::Updata() {
	BaseBlock::Updata();
	if ( !GamePlayScene::isStart )
	{
		waitTime++;
		if ( waitTime>waitTimer )
		{
			waitTime = 0;
			Shot();
		}
	}
}

void Fixedgun::Draw() {
	BaseBlock::Draw();
}
void Fixedgun::SetParameter(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_) {
	BaseBlock::SetParameter(pos_,rot_,scale_);
}

void Fixedgun::SetIsUp(const bool& isUp_) {
	isUp = isUp_;
}

void Fixedgun::Shot() {
	bulletManager->EnemyBulletCreate(pos,velocity,GetRot());
}

Vector3 Fixedgun::GetPos(){
	return BaseBlock::GetPos();
}
//回転
Vector3 Fixedgun::GetRot(){
	return BaseBlock::GetRot();
}
//スケール
Vector3 Fixedgun::GetScale(){
	return BaseBlock::GetScale();
}