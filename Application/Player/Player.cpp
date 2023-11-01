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

void Player::ParameterCommonInitialeze() {
	startPosY = 24.0f;
	endPosY = 1.0f;
	//サイズ
	tankScale = { radius,radius,radius };
	isTitleStaging = false;
	isInvincible = false;
}

void Player::ModelCommonInitialeze(Model* tankHadModel_,Model* tankBodyModel_) {
	tankHad = std::make_unique<Object3d>();
	tankHad->Initialize();
	tankHad->SetModel(tankHadModel_);
	tankHad->SetPosition(tankPos);
	tankHad->SetScale(tankScale);
	tankHad->Update();

	tankBody = std::make_unique<Object3d>();
	tankBody->Initialize();
	tankBody->SetModel(tankBodyModel_);
	tankBody->SetPosition(tankPos);
	tankBody->SetScale(tankScale);
	tankBody->Update();
}

void Player::TitleInitialeze(Model* tankHadModel_,Model* tankBodyModel_,Input* input_) {
	assert(tankHadModel_);
	assert(tankBodyModel_);
	assert(input_);

	input.reset(input_);
	//共通のパラメータ初期化
	ParameterCommonInitialeze();

	//位置
	tankPos = { 0.0f,endPosY,0.0f };
	parachutePosY = 0.0f;
	parachutePos = tankPos;
	parachutePos.y += parachutePosY;
	
	//モデル
	ModelCommonInitialeze(tankHadModel_,tankBodyModel_);
	parachute = nullptr;

	//タイム
	startEaseTime = 0;
	coolTime = 0;
	invincibleTimer = invincibleTime;
	//体力
	hp.value = NULL;
	hp.isDead = false;

	drawHp.clear();
}

void Player::PlayInitialeze(Model* tankHadModel_,Model* tankBodyModel_,Model* parachuteModel_, Input* input_) {
	assert(tankHadModel_);
	assert(tankBodyModel_);
	assert(parachuteModel_);
	assert(input_);
	
	input.reset(input_);
	//共通のパラメータ初期化
	ParameterCommonInitialeze();
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
	ModelCommonInitialeze(tankHadModel_,tankBodyModel_);

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
	drawHp.resize(hp.value);
	for (size_t i = 0; i < hp.value; i++)
	{
		drawHp[i].isDraw = false;
		drawHp[i].sprite = std::make_unique<Sprite>();
		drawHp[i].sprite->Initialize(SpriteCommon::GetInstance(), 4);
		drawHp[i].sprite->SetSize({ 50.0f,50.0f });
		drawHp[i].sprite->SetPosition(XMFLOAT2{ 10.0f + (60.0f * i),10 });
		drawHp[i].sprite->SetIsInvisible(drawHp[i].isDraw);
		drawHp[ i ].sprite->Update();
	}	
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
	tankBody->Update();
	tankHad->Update();
	if ( parachute != nullptr )
	{
		parachute->Update();

	}
}

void Player::ObjDraw() {
	//パラシュート
	if ( parachute!=nullptr )
	{
		//parachute->Draw();
	}

	//プレイヤー
	if (invincibleTimer%2==1)
	{
		tankHad->Draw();
		tankBody->Draw();
	}
	//弾
	for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::SpriteDraw() {
	if (SceneManager::sceneNum == SceneManager::play
		&&!GamePlayScene::isStart)
	{
		//スプライト
		for (size_t i = 0; i < hp.value; i++)
		{
			drawHp[i].sprite->Draw();
		}
	}
}

void Player::Move() {
	Vector3 move = { 0.0f,0.0f,0.0f };
	
	float speed = 0.1f;
	if (input->PushKey(DIK_W)|| input->PushKey(DIK_S))
	{
		if (input->PushKey(DIK_A)|| input->PushKey(DIK_D))
		{
			speed = speed / 1.414213562f;
		}
	}

	if (input->PushKey(DIK_W)) {
		move.z += speed;
		bodyRot = { 0.0f,-90.0f,0.0f };
	}
	if (input->PushKey(DIK_S)){
		move.z += -speed;
		bodyRot = { 0.0f,90.0f,0.0f };
	}
	if (input->PushKey(DIK_A)) {
		move.x += -speed;
		bodyRot = { 0.0f,180.0f,0.0f };
	}
	if (input->PushKey(DIK_D) ){
		move.x += speed;
		bodyRot = { 0.0f,0.0f,0.0f };
	}

	if ( input->PushKey(DIK_W) )
	{
		if ( input->PushKey(DIK_A) )
		{
			bodyRot = { 0.0f,-135.0f,0.0f };
		}
		else if ( input->PushKey(DIK_D) )
		{
			bodyRot = { 0.0f,-45.0f,0.0f };
		}
	}if ( input->PushKey(DIK_S) )
	{
		if ( input->PushKey(DIK_A) )
		{
			bodyRot = { 0.0f,135.0f,0.0f };
		}
		else if ( input->PushKey(DIK_D) )
		{
			bodyRot = { 0.0f,45.0f,0.0f };
		}
	}


	move += tankHad->GetPosition();
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



	tankHad->SetPosition(move);
	tankBody->SetPosition(move);
	tankBody->SetRotation(bodyRot);
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
			newBullet->Initialize(tankHad->GetPosition(), velocity,tankHad->GetRotation());

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

	tankHad->SetRotation(rot);
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
				if ( tankHad->GetPosition().x>0.0f )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else//初期位置
			{
				const Vector3 pos = { -30.0f,5.0f,0.0f };
				tankHad->SetPosition(pos);
				tankBody->SetPosition(pos);
				isTitleStaging = true;
			}
		}
		else if ( TitleScene::movieCount == 1 )
		{
			if ( isTitleStaging )
			{
				if ( tankHad->GetPosition().x > 0.0f )
				{
					isTitleStaging = false;
					TitleScene::AddMovieCount();
				}
			}
			else
			{
				const Vector3 pos = { -45.0f,5.0f,0.0f };

				tankHad->SetPosition(pos);
				tankBody->SetPosition(pos);
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
				const Vector3 pos = { -45.0f,5.0f,0.0f };
				tankHad->SetPosition(pos);
				tankBody->SetPosition(pos);
				isTitleStaging = true;
			}
		}
		//移動
		if ( TitleScene::movieCount < 2 )
		{
			Vector3 move = tankHad->GetPosition();
			const float speed = 0.25f;
			move.x += speed;
			tankHad->SetPosition(move);
			tankBody->SetPosition(move);
		}
		else if ( easeTime < titleEaseTimer )
		{
			//イージング
			Vector3 start = { -45.0f,5.0f,0.0f };
			Vector3 end = { 0.0f,5.0f,0.0f };
			Vector3 move = tankHad->GetPosition();
			move.x = start.x + ( end.x - start.x ) * Easing::easeOutSine(easeTime / titleEaseTimer);
			tankHad->SetPosition(move);
			tankBody->SetPosition(move);

			easeTime+=0.3f;
		}
	}
}

void Player::StartStaging() {
	Vector3 pos = tankHad->GetPosition();
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
	tankHad->SetPosition(pos);
	tankHad->SetRotation({ 0.0f,-90.0f,0.0f });
	tankBody->SetPosition(pos);
	tankBody->SetRotation({ 0.0f,-90.0f,0.0f });
	parachute->SetPosition(parachutePos);
}