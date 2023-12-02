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
	//デスフラグの立った弾を削除
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	if (!GamePlayScene::isStart )
	{
		switch ( phase )
		{
		case Phase::wait:
			Wait();
			break;
		case Phase::move:
			Move();
			break;
		case Phase::atack:
			Shot();

			break;
		}
	}
	obj->Update();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets)
	{
		bullet->Update();
	}
	if ( isInvincible )
	{
		invincibleTime--;
	}
	if ( invincibleTime < 0 )
	{
		invincibleTime = invincibleTimer;
		isInvincible = false;
	}
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
	vector = MyMath::normaleizeVec3(len);
	//角度を算出
	angle[1] = -atan2(vector.z, vector.x);
	vector.x = 0.0f;
	vector.z = 0.0f;
	vector.y=MyMath::DegreeTransform(angle[1]);
	velocity *= kBulletSpeed;

	//角度を格納
	obj->SetRotation(vector);
	//速度ベクトルを自機の向きに合わせて回転させる
	//ImgM = Vec_rot(ImgM, worldTransform_.matWorld_);
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(obj->GetPosition(), velocity, obj->GetRotation());

	//弾を登録する
	bullets.push_back(std::move(newBullet));

	//フェーズの切り替え
	phase = Phase::wait;
	shotTimer = shotTime;
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

void Enemy::OnCollisionPos(std::string hitDirection)
{
	BaseEnemy::OnCollisionPos(hitDirection);
}
Enemy::Enemy() {}
