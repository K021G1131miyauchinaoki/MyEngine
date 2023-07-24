#include "Enemy.h"
#include<cassert>


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
	obj->SetPosition({ 0.0f,0.0f,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->Update();
	coolTime = 10;
}

void Enemy::Updata() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->IsDead(); });
	Rotate();
	Move();
	Shot();
	obj->Update();
	for (std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Enemy::Draw() {
	obj->Draw();
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Enemy::Move() {
	
}

void Enemy::Shot() {

	//弾の速度
	const float kBulletSpeed = 1.0f;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	velocity.z = 0.0f;

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

	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = Vec_rot(velocity, worldTransform_.matWorld_);
	coolTime--;
	
	if (coolTime < 0)
	{
		//弾を生成し、初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet->Initialize(model, obj->GetPosition(), velocity, obj->GetRotation());

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		//タイムリセット
		coolTime = 30;

	}
	//if (input->TriggerClick(Botton::LEFT)) {
	//	
	//	//弾を生成し、初期化
	//	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//	newBullet->Initialize(model, obj->GetPosition(), velocity, obj->GetRotation());

	//	//弾を登録する
	//	bullets_.push_back(std::move(newBullet));
	//}
}

void Enemy::Rotate() {
	
}