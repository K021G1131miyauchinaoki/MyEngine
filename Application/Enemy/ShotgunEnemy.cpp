/**
 * @file ShotgunEnemy.cpp
 * @brief 散弾敵クラス
 */

#include "ShotgunEnemy.h"
#include<cassert>
#include<random>
#include<SceneManager.h>
#include<Map.h>
#include<MyMath.h>
#include"GamePlayScene.h"
#include<cmath>
#include"BulletManager.h"

void ShotgunEnemy::Initialize(Model* model_,Model* parachuteModel_,Player* player_,const Vector3& pos_,const Vector3& rot_,BulletManager* bulletManager_) {
	BaseEnemy::Initialize(model_,parachuteModel_,player_,pos_,rot_,bulletManager_);
	obj->SetColor({ 0.2f,0.1f,0.0f,1.0f });
	obj->Update();
	shiftNum = 2;
}

void ShotgunEnemy::Update() {
	BaseEnemy::Update();
}

void ShotgunEnemy::Draw() {
	BaseEnemy::Draw();
}

void ShotgunEnemy::Move() {
	BaseEnemy::Move();
}

void ShotgunEnemy::Shot() {
	Vector3 pos;

	//弾の速度
	velocity = { 0.0f,0.0f,0.0f };

	pos = obj->GetPosition();
	playerPos = player->GetPos();

	len = playerPos - pos;
	
	// 正規化
	rot = MyMath::normaleizeVec3(len);
	//角度を算出
	angle = MyMath::DegreeTransform(-atan2(rot.z,rot.x));
	rot.x = 0.0f;
	rot.z = 0.0f;
	rot.y = angle;

	//角度を格納
	obj->SetRotation(rot);
	//速度ベクトルを自機の向きに合わせて回転させる
	//ImgM = Vec_rot(ImgM, worldTransform_.matWorld_);
	for ( int8_t i = -1; i < shiftNum; i++ )
	{
		Vector3 bulletRot = { 0,0,0 };
		const float shift = 30.0f;
		float radian = -MyMath::RadianTransform(angle+shift*static_cast<float>(i));
		velocity = { std::cos(radian),0.0f,std::sin(radian) };
		velocity = MyMath::normaleizeVec3(velocity);
		
		bulletRot.y = angle + shift * static_cast< float >( i );

		BaseEnemy::SetBulletParameter(bulletRot,velocity,"normale");
	}
	

	BaseEnemy::Shot();
}

void ShotgunEnemy::Rotate() {

}

void ShotgunEnemy::Wait() {
	BaseEnemy::Wait();
}

//衝突したら
void ShotgunEnemy::OnCollision()
{
	BaseEnemy::OnCollision();
}

void ShotgunEnemy::OnCollisionPos(const std::string& hitDirection)
{
	BaseEnemy::OnCollisionPos(hitDirection);
}
ShotgunEnemy::ShotgunEnemy() {}
