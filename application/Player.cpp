#include "Player.h"
#include<WinApp.h>	
#include<cassert>
#include<SpriteCommon.h>
#include<Map.h>

void Player::Initialeze( Model* model_, Input* input_) {
	assert(model_);
	assert(input_);
	
	model = model_;
	input = input_;
	//���f��
	obj=std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,5.0f,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.2f,0.0f,1.0f });
	obj->Update();
	coolTime = 0;
	invincibleTimer = invincibleTime;
	//�̗�
	hp.value = 3;
	hp.isDead = false;

	//�T�C�Y������
	drawHp.resize(hp.value);
	for (size_t i = 0; i < hp.value; i++)
	{
		drawHp[i].isDraw = false;
		drawHp[i].Sprite = std::make_unique<Sprite>();
		drawHp[i].Sprite->Initialize(SpriteCommon::GetInstance(),4);
		drawHp[i].Sprite->SetSize({ 50.0f,50.0f });
		drawHp[i].Sprite->SetPosition(XMFLOAT2{ 10.0f + (60.0f * i),10 });
		drawHp[i].Sprite->SetIsInvisible(drawHp[i].isDraw);
	}
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
	
	//�I�u�W�F�N�g
	obj->Update();

	//�e
	for (std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	//HP�̃X�v���C�g
	for (size_t i = 0; i < hp.value; i++)
	{
		drawHp[i].Sprite->Update();
	}
	//�_�ŕ\��
	if (isInvincible)
	{
		invincibleTimer--;
	}
	if (invincibleTimer < 0)
	{
		invincibleTimer = invincibleTime;
		isInvincible = false;
	}
}

void Player::ObjDraw() {
	
	//�v���C���[
	if (invincibleTimer%2==1)
	{
		obj->Draw();
	}
	//�e
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::SpriteDraw() {
	//�X�v���C�g
	for (size_t i = 0; i < hp.value; i++)
	{
		drawHp[i].Sprite->Draw();
	}
}

void Player::Move() {
	Vector3 move = {0,0,0};

	float speed = 0.5f;
	if (input->PushKey(DIK_W)|| input->PushKey(DIK_S))
	{
		if (input->PushKey(DIK_A)|| input->PushKey(DIK_D))
		{
			speed = speed / 1.414213562f;
		}

	}

	if (input->PushKey(DIK_W)) {
		move.z += speed;
	}
	if (input->PushKey(DIK_S)){
		move.z += -speed;
	}
	if (input->PushKey(DIK_A)) {
		move.x += -speed;
	}
	if (input->PushKey(DIK_D) ){
		move.x += speed;
	}
	
	move += obj->GetPosition();
	//�ړ��͈͂̐���
	if (move.x >Map::moveLimitW-Map::mapScaleW*1.5) {
		move.x = Map::moveLimitW- Map::mapScaleW*1.5f;
	}
	else if (move.x < -Map::moveLimitW- Map::mapScaleW/2) {
		move.x = -Map::moveLimitW- Map::mapScaleW/2.0f;
	}

	if (move.z > Map::moveLimitH- Map::mapScaleH * 1.5) {
		move.z = Map::moveLimitH- Map::mapScaleH * 1.5f;
	}
	else if (move.z < -Map::moveLimitH- Map::mapScaleH/2) {
		move.z = -Map::moveLimitH- Map::mapScaleH/2.0f;
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
	hp.value--;
	if (hp.value<=0)
	{
		hp.isDead = true;
	}
}