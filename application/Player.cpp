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
	if (input->PushKey(DIK_SPACE)&&--coolTime<0) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		DirectX::XMFLOAT3 velocity(0, 0,0);

		XMFLOAT3 pPos = obj->GetPosition();
		XMFLOAT3 ePos = aim->GetPosition();

		XMFLOAT3 len;
		len.x = ePos.x - pPos.x;
		len.y = ePos.y - pPos.y;
		len.z = ePos.z - pPos.z;
		velocity = normaleize(len);

		len.x *= kBulletSpeed;
		len.y *= kBulletSpeed;
		len.z *= kBulletSpeed;
		
		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = Vec_rot(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet->Initialize(model, obj->GetPosition(), velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
		
		//タイムリセット
		coolTime = 30;
	}
}

void Player::Rotate() {
	//XMFLOAT3 a = obj->GetPosition();
	XMFLOAT3 a = obj->GetRotation();
	XMFLOAT3 b = aim->GetPosition();
	XMFLOAT3 vec;
	vec.x = b.x * a.x;
	vec.y = b.y * a.y;
	vec.z = b.z * a.z;
	float angle = atan2(vec.z, vec.x);
	float rotX = cos(angle);
	float rotZ = sin(angle);
	float rot = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	a.y = std::sqrt(vec.x * vec.x + vec.z * vec.z);
	obj->SetRotation({a});
}