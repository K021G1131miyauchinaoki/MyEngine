#include "Player.h"
#include<WinApp.h>	
#include<cassert>

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
	obj->SetPosition({ 0.0f,5.0f,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.2f,0.0f,1.0f });

	obj->Update();
	coolTime = 0;
	invincibleTimer = invincibleTime;
}

void Player::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->IsDead(); });
	//�}�E�X�J�[�\���̈ʒu�擾
	mausePos = input->GetMausePos();
	vector = { 0.0f,0.0f };
	//�E�B���h�E�̒��S�_�ƃ}�E�X�̌��ݓ_�̃x�N�g�����Ƃ�
	vector.x = mausePos.x - WinApp::width / 2;
	vector.y = mausePos.y - WinApp::height / 2;
	//���K��
	vector = MyMath::normaleizeVec2(vector);
	//�p�x���Z�o
	angle = atan2(vector.y, vector.x);
	Shot();
	//�x���ϊ�
	angle = MyMath::DegreeTransform(angle);
	Rotate();
	Move();
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
	velocity +=0.0f;
	
	velocity.x = std::cos(angle);
	velocity.y = 0.0f;
	velocity.z = -std::sin(angle);

	velocity *= kBulletSpeed;

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
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
	//	newBullet->Initialize(model, obj->GetPosition(), ImgM, obj->GetRotation());

	//	//�e��o�^����
	//	bullets_.push_back(std::move(newBullet));
	//}
}

void Player::Rotate() {
	//Vector3 a = obj->GetPosition();
	////XMFLOAT3 a = obj->GetRotation();
	//Vector3 b = aim->GetPosition();
	//Vector3 vec = { 0,0,0 };
	//vec.x = b.x - a.x;
	//vec.z = b.z - a.z;
	
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