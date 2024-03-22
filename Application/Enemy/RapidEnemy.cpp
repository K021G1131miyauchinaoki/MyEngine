#include "RapidEnemy.h"
#include<cassert>
#include<random>
#include<SceneManager.h>
#include<Map.h>
#include<MyMath.h>
#include"GamePlayScene.h"
#include<cmath>
#include"BulletManager.h"

void RapidEnemy::Initialize(Model* model_,Model* parachuteModel_,Player* player_,const Vector3& pos_,const Vector3& rot_,BulletManager* bulletManager_) {
	BaseEnemy::Initialize(model_,parachuteModel_,player_,pos_,rot_,bulletManager_);
	obj->SetColor({ 0.0f,0.1f,0.2f,1.0f });
	obj->Update();
}

void RapidEnemy::Update() {
	BaseEnemy::Update();
}

void RapidEnemy::Draw() {
	BaseEnemy::Draw();
}

void RapidEnemy::Move() {
	BaseEnemy::Move();
}

void RapidEnemy::Shot() {
	Vector3 pos;

	//弾の速度
	velocity = { 0.0f,0.0f,0.0f };

	pos = obj->GetPosition();
	playerPos = player->GetPos();

	len = playerPos - pos;
	velocity = MyMath::normaleizeVec3(len);
	// 正規化
	rot = MyMath::normaleizeVec3(len);
	//角度を算出
	waitAngle = -atan2(rot.z,rot.x);
	rot.x = 0.0f;
	rot.z = 0.0f;
	rot.y = MyMath::DegreeTransform(waitAngle);

	//角度を格納
	obj->SetRotation(rot);
	//弾を生成
	BaseEnemy::SetBulletParameter(rot,velocity,"normal");
	BaseEnemy::Shot();
}

void RapidEnemy::Rotate() {

}

void RapidEnemy::Wait() {
	BaseEnemy::Wait();
}

//衝突したら
void RapidEnemy::OnCollision()
{
	BaseEnemy::OnCollision();
}

void RapidEnemy::OnCollisionPos(const std::string& hitDirection)
{
	BaseEnemy::OnCollisionPos(hitDirection);
}
RapidEnemy::RapidEnemy() {}
RapidEnemy::~RapidEnemy() {}