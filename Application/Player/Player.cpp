/**
 * @file Player.cpp
 * @brief プレイヤーのソースファイル
 */
#include "Player.h"
#include<WinApp.h>	
#include<cassert>
#include<SpriteCommon.h>
#include<Map.h>
#include<SceneManager.h>
#include"TitleScene.h"
#include"Easing.h"

void Player::Initialeze( Model* model_, Input* input_) {
	assert(model_);
	assert(input_);
	
	model = model_;
	input = input_;
	//モデル
	startPosY = 120.0f;
	endPosY = 5.0f;
	obj=std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 0.0f,startPosY,0.0f });
	obj->SetScale({ 5.0f,5.0f,5.0f });
	obj->SetColor({ 0.0f,0.0f,0.5f,1.0f });
	obj->Update();
	//タイム
	startEaseTime = 0;
	coolTime = 0;
	invincibleTimer = invincibleTime;
	//体力
	hp.value = 3;
	hp.isDead = false;

	//サイズを決定
	if (SceneManager::sceneNum == SceneManager::play)
	{
		drawHp.resize(hp.value);
		for (size_t i = 0; i < hp.value; i++)
		{
			drawHp[i].isDraw = false;
			drawHp[i].Sprite = std::make_unique<Sprite>();
			drawHp[i].Sprite->Initialize(SpriteCommon::GetInstance(), 4);
			drawHp[i].Sprite->SetSize({ 50.0f,50.0f });
			drawHp[i].Sprite->SetPosition(XMFLOAT2{ 10.0f + (60.0f * i),10 });
			drawHp[i].Sprite->SetIsInvisible(drawHp[i].isDraw);
		}
	}
	isTitleStaging = false;
	isInvincible = false;
	isStart = true;
}

void Player::Reset() {
	startEaseTime = 0;
	coolTime = 0;
	invincibleTimer = invincibleTime;
}

void Player::Update() {
	TitleStaging();
	if (SceneManager::sceneNum == SceneManager::play)
	{
		if ( isStart )
		{
			StartStaging();
		}
		else
		{
			//デスフラグの立った弾を削除
			bullets_.remove_if([ ] (std::unique_ptr<Bullet>& bullet)
			{
			 return bullet->IsDead();
			});

			//マウスカーソルの位置取得
			mausePos = input->GetMausePos();
			vector = { 0.0f,0.0f };
			//ウィンドウの中心点とマウスの現在点のベクトルをとる
			vector.x = mausePos.x - WinApp::width / 2;
			vector.y = mausePos.y - WinApp::height / 2;
			//正規化
			vector = MyMath::normaleizeVec2(vector);
			//角度を算出
			angle = atan2(vector.y,vector.x);

			Shot();

			//度数変換
			angle = MyMath::DegreeTransform(angle);

			Rotate();
			Move();
			//HPのスプライト
			for ( size_t i = 0; i < hp.value; i++ )
			{
				drawHp[ i ].Sprite->Update();
			}
			//点滅表現
			if ( isInvincible )
			{
				invincibleTimer--;
			}
			if ( invincibleTimer < 0 )
			{
				invincibleTimer = invincibleTime;
				isInvincible = false;
			}
			//弾
			for ( std::unique_ptr<Bullet>& bullet : bullets_ )
			{
				bullet->Update();
			}
		}
	}
	//オブジェクト
	obj->Update();
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
	if (SceneManager::sceneNum == SceneManager::play)
	{
		//スプライト
		for (size_t i = 0; i < hp.value; i++)
		{
			drawHp[i].Sprite->Draw();
		}
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
	Vector3 rot = { 0,angle,0 };

	obj->SetRotation(rot);
}

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

void Player::TitleStaging() {
	if ( SceneManager::sceneNum == SceneManager::title )
	{
		if ( TitleScene::movieCount == 0 )
		{
			if ( isTitleStaging )//カウントを加算する
			{
				if ( obj->GetPosition().x>0.0f )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else//初期位置
			{
				obj->SetPosition({ -30.0f,5.0f,0.0f });
				isTitleStaging = true;
			}
		}
		else if ( TitleScene::movieCount == 1 )
		{
			if ( isTitleStaging )
			{
				if ( obj->GetPosition().x > 0.0f )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else
			{
				obj->SetPosition({ -45.0f,5.0f,0.0f });
				isTitleStaging = true;
			}
		}
		else if ( TitleScene::movieCount == 2 )
		{
			if ( isTitleStaging )
			{
				if ( easeTime > titleEaseTimer )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else
			{
				easeTime=0.0f;
				obj->SetPosition({ -45.0f,5.0f,0.0f });
				isTitleStaging = true;
			}
		}
		//移動
		if ( TitleScene::movieCount < 2 )
		{
			Vector3 move = obj->GetPosition();
			const float speed = 0.25f;
			move.x += speed;
			obj->SetPosition(move);
		}
		else if ( easeTime < titleEaseTimer )
		{
			//イージング
			Vector3 start = { -45.0f,5.0f,0.0f };
			Vector3 end = { 0.0f,5.0f,0.0f };
			Vector3 move = obj->GetPosition();
			move.x = start.x + ( end.x - start.x ) * Easing::easeOutSine(easeTime / titleEaseTimer);
			obj->SetPosition(move);

			easeTime+=0.3f;
		}
	}
}

void Player::StartStaging() {
	Vector3 pos = obj->GetPosition();
	if ( startEaseTime<startEaseTimer )
	{
		startEaseTime++;
		pos.y= startPosY + ( endPosY - startPosY ) * Easing::easeOutBounce(startEaseTime/ startEaseTimer);
	}
	else
	{
		startEaseTime = startEaseTimer;
		isStart = false;
	}
	obj->SetPosition(pos);
	obj->SetRotation({ 0.0f,-90.0f,0.0f });

}