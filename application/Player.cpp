#include "Player.h"
#include<cassert>
XMFLOAT3& normaleize(XMFLOAT3 vec) {
	float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (length != 0) {
		 vec.x /= length;
		 vec.y /= length;
		 vec.z /= length;

		 return vec;
	}

	return vec;
}

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


void Player::Initialeze( Model* model_, Input* input_,Aimposition* aim_) {
	assert(model_);
	assert(input_);
	assert(aim_);
	
	model = model_;
	input = input_;
	aim = aim_;
	obj=std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,0.0f,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->Update();
	coolTime = 0;
}

void Player::Updata() {
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

void Player::Draw() {
	obj->Draw();
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::Move() {
	XMFLOAT3 move = obj->GetPosition();

	const float speed = 0.2f;
	if (input->PushKey(DIK_W)) {
		move.z += speed;
	}
	else if (input->PushKey(DIK_S)){
		move.z +=-speed;
	}
	else if (input->PushKey(DIK_A)) {
		move.x +=-speed;
	}
	else if (input->PushKey(DIK_D) ){
		move.x += speed;
		//move = { 0,0,5 };
	}
	obj->SetPosition(move);
}

void Player::Shot() {
	coolTime--;
	//弾の速度
	const float kBulletSpeed = 1.0f;
	velocity.x =0.0f;
	velocity.y =0.0f;
	velocity.z =0.0f;

	XMFLOAT3 pPos = obj->GetPosition();
	XMFLOAT3 ePos = aim->GetPosition();

	XMFLOAT3 len;
	len.x = ePos.x - pPos.x;
	len.y = ePos.y - pPos.y;
	len.z = ePos.z - pPos.z;
	velocity = normaleize(len);

	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = Vec_rot(velocity, worldTransform_.matWorld_);
	if (input->PushKey(DIK_SPACE)&&coolTime<0) {
		

		//弾を生成し、初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet->Initialize(model, obj->GetPosition(), velocity,obj->GetRotation());

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
		
		//タイムリセット
		coolTime = 30;
	}
}

void Player::Rotate() {
	XMFLOAT3 a = obj->GetPosition();
	//XMFLOAT3 a = obj->GetRotation();
	XMFLOAT3 b = aim->GetPosition();
	XMFLOAT3 vec = { 0,0,0 };
	vec.x = b.x - a.x;
	vec.z = b.z - a.z;
	vec = normaleize(vec);
	float angle = -((atan2(vec.z, vec.x)));
	angle = (180.0f / 3.14f) * angle;
	obj->SetRotation({0.0f,angle,0.0f});
	obj->GetRotation();
}