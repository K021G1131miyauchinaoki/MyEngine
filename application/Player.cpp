#include "Player.h"
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
	obj->SetColor({ 0.0f,0.2f,0.0f,1.0f });

	obj->Update();
	coolTime = 0;
	invincibleTimer = invincibleTime;
}

void Player::Updata() {
	//�f�X�t���O�̗������e���폜
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
	if (isInvincible)
	{
		invincibleTimer--;
	}
	if (invincibleTimer<0)
	{
		invincibleTimer = invincibleTime;
		isInvincible = false;
	}
	if (invincibleTimer%2==1)
	{
		obj->Draw();
	}
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::Move() {
	Vector3 move = obj->GetPosition();

	const float speed = 0.5f;
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
	
	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	velocity.x =0.0f;
	velocity.y =0.0f;
	velocity.z =0.0f;

	Vector3 pPos = {0.0f,0.0f,0.0f};
	pPos.x = obj->GetPosition().x;
	pPos.y = obj->GetPosition().y;
	pPos.z = obj->GetPosition().z;
	Vector3 ePos = aim->GetPosition();

	Vector3 len;
	len.x = ePos.x - pPos.x;
	len.y = ePos.y - pPos.y;
	len.z = ePos.z - pPos.z;
	velocity = MyMath::normaleize(len);

	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = Vec_rot(velocity, worldTransform_.matWorld_);
	coolTime--;
	if (input->PushKey(DIK_SPACE)||input->PushClick(Botton::LEFT)) {
		if (coolTime < 0)
		{
			//�e�𐶐����A������
			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet->Initialize(obj->GetPosition(), velocity,obj->GetRotation());

			//�e��o�^����
			bullets_.push_back(std::move(newBullet));
			
			//�^�C�����Z�b�g
			coolTime = 30;

		}
	}
	//if (input->TriggerClick(Botton::LEFT)) {
	//	
	//	//�e�𐶐����A������
	//	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//	newBullet->Initialize(model, obj->GetPosition(), velocity, obj->GetRotation());

	//	//�e��o�^����
	//	bullets_.push_back(std::move(newBullet));
	//}
}

void Player::Rotate() {
	Vector3 a = obj->GetPosition();
	//XMFLOAT3 a = obj->GetRotation();
	Vector3 b = aim->GetPosition();
	Vector3 vec = { 0,0,0 };
	vec.x = b.x - a.x;
	vec.z = b.z - a.z;
	vec = MyMath::normaleize(vec);
	angle = ((atan2(vec.x, vec.z)));
	angle = MyMath::DegreeTransform(angle);
	Vector3 rot = { 0,angle,0 };

	obj->SetRotation(rot);
}

//�Փ˂�����
void Player::OnCollision() 
{ 
	if (!isInvincible)
	{
		isInvincible = true; 
	}
}