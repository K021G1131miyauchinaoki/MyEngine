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

void Enemy::Initialeze(Model* model_,Player*player_) {
	assert(model_);
	assert(player_);

	model = model_;
	player = player_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,5.0f,50.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.1f,0.3f,1.0f });
	obj->Update();
	moveTime = moveTimer;
	shotTimer = shotTime;
	waitTime = 0.0f;
	invincibleTime = invincibleTimer;
	angle[0] = 0.0f;
	angle[1] = 0.0f;
	//体力
	hp.value = 3;
	hp.isDead = false;
}

void Enemy::Update() {
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
}

void Enemy::Draw() {
	if (isInvincible)
	{
		invincibleTime--;
	}
	if (invincibleTime < 0)
	{
		invincibleTime = invincibleTimer;
		isInvincible = false;
	}
	if (invincibleTime % 2 == 1)
	{
		obj->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Enemy::Move() {

	if (!isMove)
	{
		#pragma region 乱数
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		//乱数　（回転）
		std::uniform_real_distribution<float> rotDist(-0.5f,0.5f);
		//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
		value = { rotDist(engine),0.0f,rotDist(engine) };
		//値を正規化
		value = MyMath::normaleizeVec3(value);
		
		angle[0] = -(atan2(value.z, value.x));
		Vector3 rot = { 0.0f,0.0f,0.0f };
		//度数に変換
		angle[0] = MyMath::DegreeTransform(angle[0]) ;
		rot.y = angle[0];

		obj->SetRotation(rot);
		
		//敵の速度
		const float speed = 0.2f;
		move += value * speed;
		
		#pragma endregion
		isMove = true;
	}
	else if (moveTime<0) {
		move = { 0.0f,0.0f, 0.0f };
		phase = Phase::wait;
		isMove = false;
		moveTime = moveTimer;
	}
	else
	{
		moveTime--;
		Vector3 pos = obj->GetPosition();
		pos += move;
		//移動範囲の制限
		if (pos.x > Map::moveLimitW - Map::mapScaleW * 1.5) {
			pos.x = Map::moveLimitW - Map::mapScaleW * 1.5f;
		}
		else if (pos.x < -Map::moveLimitW - Map::mapScaleW / 2) {
			pos.x = -Map::moveLimitW - Map::mapScaleW / 2.0f;
		}

		if (pos.z > Map::moveLimitH - Map::mapScaleH * 1.5) {
			pos.z = Map::moveLimitH - Map::mapScaleH * 1.5f;
		}
		else if (pos.z < -Map::moveLimitH - Map::mapScaleH / 2) {
			pos.z = -Map::moveLimitH - Map::mapScaleH / 2.0f;
		}
		obj->SetPosition(pos);
	}

}

void Enemy::Shot() {
	/*shotTimer--;
	if (shotTimer < 0)
	{*/
		//弾の速度
		const float kBulletSpeed = 1.0f;
		velocity = { 0.0f,0.0f,0.0f };

		Pos = obj->GetPosition();
		playerPos = player->GetPos();

		len= playerPos - Pos;
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
	//}

}

void Enemy::Rotate() {
	
}

void Enemy::Wait() {
	waitTime++;
	Pos = obj->GetPosition();
	playerPos = player->GetPos();

	len = playerPos - Pos;
	// 正規化
	vector = MyMath::normaleizeVec3(len);
	//角度を算出
	angle[1] = MyMath::DegreeTransform(-atan2(vector.z, vector.x));

	if (!isWait)
	{
 		Vector3 rot = obj->GetRotation();
		float t = waitTime / waitTimerr[0];
		rot.y = MyMath::LerpShortAngle(rot.y, angle[1], t);
		obj->SetRotation(rot);
	}

	if (waitTime > waitTimerr[0] && !isWait)
	{
		isWait = true;
		waitTime = 0.0f;
		phase = Phase::atack;
	}
	else if(waitTime > waitTimerr[1] && isWait)
	{
		isWait = false;
		waitTime = 0.0f;
		phase = Phase::move;
	}
}

//衝突したら
void Enemy::OnCollision()
{
	if (!isInvincible)
	{
		isInvincible = true;
	}
	hp.value--;
	if (hp.value <= 0)
	{
		hp.isDead = true;
	}
}