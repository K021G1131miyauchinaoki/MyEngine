/**
 * @file Enemy.cpp
 * @brief エネミークラス
 */

#include "Enemy.h"
#include<cassert>
#include<random>
#include<SceneManager.h>
#include<Map.h>
#include<MyMath.h>
#include"GamePlayScene.h"
#include<cmath>

void Enemy::Initialeze(Model* model_,Player*player_,const Vector3& pos_,const Vector3& rot_) {
	BaseEnemy::Initialeze(model_,player_,pos_,rot_);
	obj->SetColor({ 0.0f,0.1f,0.2f,1.0f });
	obj->Update();
}

void Enemy::Update() {
	BaseEnemy::Update();
}

void Enemy::Draw() {
	BaseEnemy::Draw();
}

void Enemy::Move() {
	BaseEnemy::Move();
}

void Enemy::Shot() {
	Vector3 pos;

	//弾の速度
	const float kBulletSpeed = 1.5f;
	velocity = { 0.0f,0.0f,0.0f };

	pos = obj->GetPosition();
	playerPos = player->GetPos();

	len= playerPos - pos;
	velocity = MyMath::normaleizeVec3(len);
	// 正規化
	rot = MyMath::normaleizeVec3(len);
	//角度を算出
	angle = -atan2(rot.z, rot.x);
	rot.x = 0.0f;
	rot.z = 0.0f;
	rot.y=MyMath::DegreeTransform(angle);
	velocity *= kBulletSpeed;

	//角度を格納
	obj->SetRotation(rot);
	//速度ベクトルを自機の向きに合わせて回転させる
	//ImgM = Vec_rot(ImgM, worldTransform_.matWorld_);
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(obj->GetPosition(), velocity, obj->GetRotation());

	//弾を登録する
	bullets.push_back(std::move(newBullet));

	BaseEnemy::Shot();
}

void Enemy::Rotate() {
	
}

void Enemy::Wait() {
	BaseEnemy::Wait();
}

//衝突したら
void Enemy::OnCollision()
{
	BaseEnemy::OnCollision();
}

void Enemy::OnCollisionPos(const std::string &hitDirection)
{
	BaseEnemy::OnCollisionPos(hitDirection);
}
Enemy::Enemy() {}
Enemy::~Enemy() {}
