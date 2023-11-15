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

void Enemy::Initialeze(Model* model_,Player*player_,Vector3 pos_,Vector3 rot_) {
	assert(model_);
	assert(player_);
	InitialezePos = pos_;
	InitialezePos.y = radius;

	model = model_;
	player = player_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition(InitialezePos);
	obj->SetRotation(rot_);
	obj->SetScale({ radius,radius,radius });
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
		pos = obj->GetPosition();
		playerPos = player->GetPos();

		len = playerPos - pos;
		//長さを算出
		float lenght = MyMath::Length(len);
		float shift = 60.0f;
		Vector3 rot = { 0.0f,0.0f,0.0f };
		#pragma region 乱数
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		//乱数　（回転）
		//値を正規化
		len = MyMath::normaleizeVec3(len);
		float criteriaRot = MyMath::DegreeTransform(( atan2(len.z,len.x) ));
		if ( lenght>100.0f )
		{
			criteriaRot = 0.0f;
			shift = 0.0f;
		}
		else if ( lenght<20.0f )
		{
			criteriaRot = -MyMath::DegreeTransform(( atan2(len.z,len.x) ));
		}
		std::uniform_real_distribution<float> rotDist(-shift,shift);
		
		angle[ 0 ] = rotDist(engine)+ criteriaRot;
		angle[ 0 ] = MyMath::RadianTransform(angle[ 0 ]);
		//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
		value = { std::cos(angle[ 0 ]),0.0f,std::sin(angle[ 0 ]) };
		//値を正規化
		value = MyMath::normaleizeVec3(value);

		angle[ 0 ] = -MyMath::DegreeTransform(angle[ 0 ]);//角度の算出
		angle[ 0 ] = MyMath::AngleCorrection(angle[ 0 ]);//角度の補正
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
		pos = obj->GetPosition();
		pos += move;
		//移動範囲の制限
		if (pos.x > Map::moveLimitW -radius) {
			pos.x = Map::moveLimitW - radius;
		}
		else if (pos.x < -Map::moveLimitW + radius ) {
			pos.x = -Map::moveLimitW + radius;
		}

		if (pos.z > Map::moveLimitH - radius ) {
			pos.z = Map::moveLimitH - radius;
		}
		else if (pos.z < -Map::moveLimitH + radius ) {
			pos.z = -Map::moveLimitH + radius;
		}
		obj->SetPosition(pos);
	}

}

void Enemy::Shot() {
	
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
	waitTime++;
	pos = obj->GetPosition();
	playerPos = player->GetPos();

	len = playerPos - pos;
	// 正規化
	vector = MyMath::normaleizeVec3(len);
	//角度を算出
	angle[1] = MyMath::DegreeTransform(-atan2(vector.z, vector.x));

	if (!isWait)
	{
 		Vector3 rot = obj->GetRotation();
		float t = waitTime / waitTimer[ before ];
		rot.y = MyMath::LerpShortAngle(rot.y, angle[1], t);
		obj->SetRotation(rot);
	}

	if (waitTime > waitTimer[ before ] && !isWait)
	{
		isWait = true;
		waitTime = 0.0f;
		phase = Phase::atack;
	}
	else if(waitTime > waitTimer[after] && isWait)
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

Enemy::Enemy() {}
Enemy::~Enemy() {}