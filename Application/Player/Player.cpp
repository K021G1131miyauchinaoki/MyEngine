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
#include "GamePlayScene.h"
#include"TitleScene.h"
#include"Easing.h"

void Player::TitleInitialeze(Model* tankModel_,Input* input_) {
	assert(tankModel_);
	assert(input_);

	input.reset(input_);
	startPosY = 120.0f;
	endPosY = 5.0f;
	//位置
	tankPos = { 0.0f,endPosY,0.0f };
	parachutePosY = 0.0f;
	parachutePos = tankPos;
	parachutePos.y += parachutePosY;
	
	//モデル
	tank = std::make_unique<Object3d>();
	tank->Initialize();
	tank->SetModel(tankModel_);
	tank->SetPosition(tankPos);
	tank->SetScale({ 5.0f,5.0f,5.0f });
	tank->SetColor({ 0.0f,0.0f,0.5f,1.0f });
	tank->Update();

	parachute = nullptr;

	//タイム
	startEaseTime = 0;
	coolTime = 0;
	invincibleTimer = invincibleTime;
	//体力
	hp.value = NULL;
	hp.isDead = false;

	drawHp.clear();

	isTitleStaging = false;
	isInvincible = false;
}

void Player::PlayInitialeze( Model* tankModel_,Model* parachuteModel_, Input* input_) {
	assert(tankModel_);
	assert(parachuteModel_);
	assert(input_);
	
	input.reset(input_);
	startPosY = 120.0f;
	endPosY = 6.0f;
	//位置
	tankPos = { 0.0f,startPosY,0.0f };
	parachutePosY = 6.0f;
	parachutePos = tankPos;
	parachutePos.y += parachutePosY;
	//サイズ
	pStartScaleXZ = 6.0f;
	pEndScaleXZ = 0.0f;
	//角度
	pStartRotZ = 0;
	pEndRotZ = -90;
	//モデル
	tank=std::make_unique<Object3d>();
	tank->Initialize();
	tank->SetModel(tankModel_);
	tank->SetPosition(tankPos);
	tank->SetScale({ 5.0f,5.0f,5.0f });
	tank->SetColor({ 0.0f,0.0f,0.5f,1.0f });
	tank->Update();

	parachute = std::make_unique<Object3d>();
	parachute->Initialize();
	parachute->SetModel(parachuteModel_);
	parachute->SetPosition(parachutePos);
	parachute->SetScale({ pStartScaleXZ,4.0f,pStartScaleXZ });
	parachute->SetRotation({ 0.0f,0.0f,0.0f });
	parachute->Update();

	//タイム
	startEaseTime = 0;
	pLeaveTime = 0;
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
			drawHp[i].sprite = std::make_unique<Sprite>();
			drawHp[i].sprite->Initialize(SpriteCommon::GetInstance(), 4);
			drawHp[i].sprite->SetSize({ 50.0f,50.0f });
			drawHp[i].sprite->SetPosition(XMFLOAT2{ 10.0f + (60.0f * i),10 });
			drawHp[i].sprite->SetIsInvisible(drawHp[i].isDraw);
		}
	}
	isTitleStaging = false;
	isInvincible = false;
}

void Player::Reset() {
	startEaseTime = 0;
	coolTime = 0;
	invincibleTimer = invincibleTime;
}

void Player::Update() {
	TitleStaging();
	if ( SceneManager::sceneNum == SceneManager::play )
	{
		if ( GamePlayScene::isStart )
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
				drawHp[ i ].sprite->Update();
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
	tank->Update();
	if ( parachute != nullptr )
	{
		parachute->Update();

	}
}

void Player::ObjDraw() {
	//パラシュート
	if ( parachute!=nullptr )
	{
		parachute->Draw();
	}

	//プレイヤー
	if (invincibleTimer%2==1)
	{
		tank->Draw();
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
			drawHp[i].sprite->Draw();
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
	
	move += tank->GetPosition();
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



	tank->SetPosition(move);
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
			newBullet->Initialize(tank->GetPosition(), velocity,tank->GetRotation());

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

	tank->SetRotation(rot);
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
				if ( tank->GetPosition().x>0.0f )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else//初期位置
			{
				tank->SetPosition({ -30.0f,5.0f,0.0f });
				isTitleStaging = true;
			}
		}
		else if ( TitleScene::movieCount == 1 )
		{
			if ( isTitleStaging )
			{
				if ( tank->GetPosition().x > 0.0f )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else
			{
				tank->SetPosition({ -45.0f,5.0f,0.0f });
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
				tank->SetPosition({ -45.0f,5.0f,0.0f });
				isTitleStaging = true;
			}
		}
		//移動
		if ( TitleScene::movieCount < 2 )
		{
			Vector3 move = tank->GetPosition();
			const float speed = 0.25f;
			move.x += speed;
			tank->SetPosition(move);
		}
		else if ( easeTime < titleEaseTimer )
		{
			//イージング
			Vector3 start = { -45.0f,5.0f,0.0f };
			Vector3 end = { 0.0f,5.0f,0.0f };
			Vector3 move = tank->GetPosition();
			move.x = start.x + ( end.x - start.x ) * Easing::easeOutSine(easeTime / titleEaseTimer);
			tank->SetPosition(move);

			easeTime+=0.3f;
		}
	}
}

void Player::StartStaging() {
	Vector3 pos = tank->GetPosition();
	Vector3 rot = parachute->GetRotation();
	Vector3 scale = parachute->GetScale();
	
	if ( startEaseTime<startEaseTimer
		&& GamePlayScene::startCount ==0)
	{
		startEaseTime++;
		pos.y= startPosY + ( endPosY - startPosY ) * Easing::easeOutSine(startEaseTime/ startEaseTimer);
		if ( startEaseTime >= startEaseTimer )
		{
			GamePlayScene::startCount++;
			bound = 1.5f;
		}
	}
	else if ( GamePlayScene::startCount==1 )
	{
		const float minus = -0.5f;
		pos.y += bound;
		bound += minus;
		if ( pos.y<=endPosY )
		{
			pos.y = endPosY;
			GamePlayScene::startCount++;
			bound = 1.0f;
		}
	}
	else if ( GamePlayScene::startCount == 2 )
	{
		const float minus = -0.5;
		pos.y += bound;
		bound += minus;
		if ( pos.y <= endPosY )
		{
			GamePlayScene::startCount++;
			bound = 2.0f;
		}
	}
	else if ( GamePlayScene::startCount == 3 )
	{
		if ( pLeaveTime >= pLeaveTimer )
		{
			GamePlayScene::startCount++;
		}
	}

	else
	{
		startEaseTime = 0;
	}
	//パラシュート
	//タイマーが80％以下なら
	if ( startEaseTime < startEaseTimer*0.8f )
	{
		parachutePos = pos;
		parachutePos.y += parachutePosY;
	}
	else
	{
		if ( pLeaveTime< pLeaveTimer&& parachutePos.x>3.0f )
		{
			pLeaveTime++;
			rot.z = pStartRotZ + ( pEndRotZ - pStartRotZ ) * Easing::easeOutSine(pLeaveTime / pLeaveTimer);
			scale.x = pStartScaleXZ + ( pEndScaleXZ - pStartScaleXZ ) * Easing::easeOutSine(pLeaveTime / pLeaveTimer);
			scale.z = pStartScaleXZ + ( pEndScaleXZ - pStartScaleXZ ) * Easing::easeOutCirc(pLeaveTime / pLeaveTimer);
		}
		

		parachute->SetRotation(rot);
		parachute->SetScale(scale);
		parachutePos.x += 0.6f;
		parachutePos.y -= 0.1f;
	}
	tank->SetPosition(pos);
	tank->SetRotation({ 0.0f,-90.0f,0.0f });
	parachute->SetPosition(parachutePos);
}