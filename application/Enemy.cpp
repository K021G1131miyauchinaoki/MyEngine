#include "Enemy.h"
#include<cassert>
#include<random>
#include<Map.h>

float LerpShortAngle(float a, float b, float t) {
	float diff = b - a;

	diff = std::fmod(diff, 360);
	// 2πまたはπに補正
	if (diff > 180) {
		diff -= 360;
	}
	else if (diff < -180) {
		diff += 360;
	}
	float total = a + diff * t;

	return total;
}

void Enemy::Initialeze(Model* model_,Player*player_) {
	assert(model_);
	assert(player_);

	model = model_;
	player = player_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,5.0f,10.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.1f,0.3f,1.0f });
	obj->Update();
	moveTimer = moveTime;
	shotTimer = shotTime;
	waitTimer = 0.0f;
	invincibleTimer = invincibleTime;
	angle[0] = 0.0f;
	angle[1] = 0.0f;
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	switch (phase) {
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
	obj->Update();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Enemy::Draw() {
	if (isInvincible)
	{
		invincibleTimer--;
	}
	if (invincibleTimer < 0)
	{
		invincibleTimer = invincibleTime;
		isInvincible = false;
	}
	if (invincibleTimer % 2 == 1)
	{
		obj->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
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
		std::uniform_real_distribution<float> rotDist(-1.0f,1.0f);
		//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
		value = { rotDist(engine),0.0f,rotDist(engine) };
		//値を正規化
		value = MyMath::normaleizeVec3(value);
		
		angle[0] = (atan2(value.x, value.z));
		Vector3 rot = { 0.0f,0.0f,0.0f };
		//度数に変換
		angle[0] = MyMath::DegreeTransform(angle[0]) - 90.0f;
		rot.y = angle[0];

		obj->SetRotation(rot);
		
		//敵の速度
		const float speed = 0.2f;
		move += value * speed;
		
		#pragma endregion
		isMove = true;
	}
	else if (moveTimer<0) {
		move = { 0.0f,0.0f, 0.0f };
		phase = Phase::wait;
		isMove = false;
		moveTimer = moveTime;
	}
	else
	{
		moveTimer--;
		Vector3 pos = obj->GetPosition();
		pos += move;
		//移動範囲の制限
		if (pos.x > Map::moveLimitW - Map::mapScaleW * 1.5) {
			pos.x = Map::moveLimitW - Map::mapScaleW * 1.5;
		}
		else if (pos.x < -Map::moveLimitW - Map::mapScaleW / 2) {
			pos.x = -Map::moveLimitW - Map::mapScaleW / 2;
		}

		if (pos.z > Map::moveLimitH - Map::mapScaleH * 1.5) {
			pos.z = Map::moveLimitH - Map::mapScaleH * 1.5;
		}
		else if (pos.z < -Map::moveLimitH - Map::mapScaleH / 2) {
			pos.z = -Map::moveLimitH - Map::mapScaleH / 2;
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

		ePos = obj->GetPosition();
		pPos = player->GetPos();

		len= pPos - ePos;
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
		bullets_.push_back(std::move(newBullet));

		//フェーズの切り替え
		phase = Phase::wait;
		shotTimer = shotTime;
	//}

}

void Enemy::Rotate() {
	
}

void Enemy::Wait() {
	waitTimer++;
	ePos = obj->GetPosition();
	pPos = player->GetPos();

	len = pPos - ePos;
	// 正規化
	vector = MyMath::normaleizeVec3(len);
	//角度を算出
	angle[1] = MyMath::DegreeTransform(-atan2(vector.z, vector.x));

	if (!isWait)
	{
 		Vector3 rot = obj->GetRotation();
		float t = waitTimer / waitTime[0];
		rot.y = LerpShortAngle(rot.y, angle[1], t);
		obj->SetRotation(rot);
	}

	if (waitTimer > waitTime[0] && !isWait)
	{
		isWait = true;
		waitTimer = 0.0f;
		phase = Phase::atack;
	}
	else if(waitTimer > waitTime[1] && isWait)
	{
		isWait = false;
		waitTimer = 0.0f;
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
}