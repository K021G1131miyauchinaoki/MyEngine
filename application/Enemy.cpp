#include "Enemy.h"
#include<cassert>
#include<random>


//Vector3 Vec_rot(Vector3 velocity, Matrix4 amount) {
//	Vector3 Rot;
//
//	Rot.x = velocity.x * amount.m[0][0];
//	Rot.x += velocity.y * amount.m[1][0];
//	Rot.x += velocity.z * amount.m[2][0];
//
//	Rot.y = velocity.x * amount.m[0][1];
//	Rot.y += velocity.y * amount.m[1][1];
//	Rot.y += velocity.z * amount.m[2][1];
//
//	Rot.z = velocity.x * amount.m[0][2];
//	Rot.z += velocity.y * amount.m[1][2];
//	Rot.z += velocity.z * amount.m[2][2];
//
//	return Rot;
//}


void Enemy::Initialeze(Model* model_,Player*player_) {
	assert(model_);
	assert(player_);

	model = model_;
	player = player_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,0.0f,10.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.1f,0.3f,1.0f });
	obj->Update();
	moveTimer = moveTime;
	shotTimer = shotTime;
	waitTimer = waitTime[0];
	invincibleTimer = invincibleTime;
}

void Enemy::Updata() {
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
		value = MyMath::normaleize(value);
		
		angle = (atan2(value.x, value.z));
		Vector3 rot;
		//度数に変換
		rot.y = MyMath::DegreeTransform(angle);

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

		Vector3 pPos = { 0.0f,0.0f,0.0f };
		pPos.x = obj->GetPosition().x;
		pPos.y = obj->GetPosition().y;
		pPos.z = obj->GetPosition().z;
		Vector3 ePos = player->GetPos();

		Vector3 len;
		len.x = ePos.x - pPos.x;
		len.y = ePos.y - pPos.y;
		len.z = ePos.z - pPos.z;
		velocity = MyMath::normaleize(len);

		velocity *= kBulletSpeed;

		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = Vec_rot(velocity, worldTransform_.matWorld_);
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
	waitTimer--;
	if (waitTimer < 0)
	{
		if (!isWait)
		{
			isWait = true;
			waitTimer = waitTime[1];

			phase = Phase::atack;
		}
		else
		{
			isWait = false;
			waitTimer = waitTime[0];
			phase = Phase::move;

		}
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