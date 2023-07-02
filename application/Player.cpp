#include "Player.h"
#include<cassert>

void Player::Initialeze( Model* model_, Input* input_) {
	assert(model_);
	assert(input_);
	
	model = model_;
	input = input_;
	obj=std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,0.0f,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->Update();
	coolTime = 0;
}

void Player::Updata() {
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
	if (input->PushKey(DIK_UP)) {
		move.z += speed;
	}
	else if (input->PushKey(DIK_DOWN)) {
		move.z +=-speed;
	}
	else if (input->PushKey(DIK_LEFT)) {
		move.x +=-speed;
	}
	else if (input->PushKey(DIK_RIGHT)) {
		move.x += speed;
		//move = { 0,0,5 };
	}
	obj->SetPosition(move);
}

void Player::Shot() {
	if (input->PushKey(DIK_SPACE)&&--coolTime<0) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		DirectX::XMFLOAT3 velocity(0, 0, kBulletSpeed);

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