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
	//モデル
	obj=std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,5.0f,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.2f,0.0f,1.0f });
	obj->Update();
	coolTime = 0;
	invincibleTimer = invincibleTime;
	//体力
	hp.value = 3;
	hp.isDead = false;

	//サイズを決定
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
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->IsDead(); });
	
	//マウスカーソルの位置取得
	mausePos = input->GetMausePos();
	vector = { 0.0f,0.0f };
	//ウィンドウの中心点とマウスの現在点のベクトルをとる
	vector.x = mausePos.x - WinApp::width / 2;
	vector.y = mausePos.y - WinApp::height / 2;
	//正規化
	vector = MyMath::normaleizeVec2(vector);
	//角度を算出
	angle = atan2(vector.y, vector.x);
	
	Shot();
	
	//度数変換
	angle = MyMath::DegreeTransform(angle);
	
	Rotate();
	Move();
	
	//オブジェクト
	obj->Update();

	//弾
	for (std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	//HPのスプライト
	for (size_t i = 0; i < hp.value; i++)
	{
		drawHp[i].Sprite->Update();
	}
	//点滅表現
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
	
	//プレイヤー
	if (invincibleTimer%2==1)
	{
		obj->Draw();
	}
	//弾
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::SpriteDraw() {
	//スプライト
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
	//移動範囲の制限
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
	
	//弾の速度
	const float kBulletSpeed = 1.0f;
	velocity +=0.0f;
	
	velocity.x = std::cos(angle);
	velocity.y = 0.0f;
	velocity.z = -std::sin(angle);

	velocity *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	coolTime--;
	if (input->PushKey(DIK_SPACE)||input->PushClick(Botton::LEFT)) {
		if (coolTime < 0)
		{
			//弾を生成し、初期化
			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet->Initialize(obj->GetPosition(), velocity,obj->GetRotation());

			//弾を登録する
			bullets_.push_back(std::move(newBullet));
			
			//タイムリセット
			coolTime = 30;

		}
	}
	//if (input->TriggerClick(Botton::LEFT)) {
	//	
	//	//弾を生成し、初期化
	//	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//	newBullet->Initialize(model, obj->GetPosition(), ImgM, obj->GetRotation());

	//	//弾を登録する
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

//衝突したら
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