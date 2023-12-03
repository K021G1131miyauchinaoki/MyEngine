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

void ShotgunEnemy::Initialeze(Model* model_,Player* player_,const Vector3& pos_,const Vector3& rot_) {
	BaseEnemy::Initialeze(model_,player_,pos_,rot_);
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
	const float kBulletSpeed = 1.5f;
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
		const float shift = 30.0f;
		float radian = -MyMath::RadianTransform(angle+shift*static_cast<float>(i));
		velocity = { std::cos(radian),0.0f,std::sin(radian) };
		velocity = MyMath::normaleizeVec3(velocity);
		velocity *= kBulletSpeed;
		//弾を生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(obj->GetPosition(),velocity,obj->GetRotation());

		//弾を登録する
		bullets.push_back(std::move(newBullet));
	}
	

	BaseEnemy::Shot();

	//len = MyMath::normaleizeVec3(len);
	//float criteriaRot = MyMath::DegreeTransform(( atan2(len.z,len.x) ));
	//
	//else if ( lenght < 20.0f )
	//{
	//	criteriaRot = -MyMath::DegreeTransform(( atan2(len.z,len.x) ));
	//}
	//std::uniform_real_distribution<float> rotDist(-shift,shift);

	//angle = rotDist(engine) + criteriaRot;
	//angle = MyMath::RadianTransform(angle);
	////乱数エンジンを渡し、指定範囲かっランダムな数値を得る
	//value = { std::cos(angle),0.0f,std::sin(angle) };
	////値を正規化
	//value = MyMath::normaleizeVec3(value);

	//angle = -MyMath::DegreeTransform(angle);//角度の算出
	//angle = MyMath::AngleCorrection(angle);//角度の補正
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
