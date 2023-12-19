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
#include"Player.h"
#include"BulletManager.h"

void Player::ParameterCommonInitialeze() {
	startPosY = 120.0f;
	endPosY = radius;
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

void Player::PlayInitialeze(Model* tankHadModel_,Model* tankBodyModel_,Model* parachuteModel_, Input* input_,BulletManager* bulletManager_) {
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
	//コピー
	bulletManager = bulletManager_;
}

void Player::Reset() {
	startEaseTime = 0;
	coolTime = 0;
	invincibleTimer = invincibleTime;
}

void Player::Update() {
	const float half = 2.0f;
	const int16_t zero = 0;
	//タイトル演出
	TitleStaging();
	//ゲームプレイ
	if ( SceneManager::sceneNum == SceneManager::play )
	{
		//スタート演出
		if ( GamePlayScene::isStart )
		{
			StartStaging();
		}
		//プレイ
		else
		{
			//マウスカーソルの位置取得
			mausePos = input->GetMausePos();
			mouseVec = { 0.0f,0.0f };
			//ウィンドウの中心点とマウスの現在点のベクトルをとる
			mouseVec.x = mausePos.x - static_cast<float>(WinApp::width) / half;
			mouseVec.y = mausePos.y - static_cast< float >( WinApp::height ) / half;
			//正規化
			mouseVec = MyMath::normaleizeVec2(mouseVec);
			//角度を算出
			angle = atan2(mouseVec.y,mouseVec.x);

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
			if ( invincibleTimer < zero )
			{
				invincibleTimer = invincibleTime;
				isInvincible = false;
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
		parachute->Draw();
	}

	//プレイヤー
	if (invincibleTimer%2==1)
	{
		tankHad->Draw();
		tankBody->Draw();
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
	
	//移動
	Vector3 move = tankHad->GetPosition();
	float speed = 0.5f;
	float obliques = 1.414213562f;
	//制限

	//同時入力があれば45度に補正する

	if (input->PushKey(DIK_W)|| input->PushKey(DIK_S))
	{
		if (input->PushKey(DIK_A)|| input->PushKey(DIK_D))
		{
			speed = speed / obliques;
		}
	}
	//キーが押されたら
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
	//斜め入力の場合のオブジェクトの角度
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
	oldPos = tankHad->GetPosition();

	
	//移動範囲の制限
	if (move.x >Map::moveLimitW - tankScale.x ) {
		move.x = Map::moveLimitW - tankScale.x;
	}
	else if (move.x < -Map::moveLimitW + tankScale.x ) {
		move.x = -Map::moveLimitW + tankScale.x;
	}

	if (move.z > Map::moveLimitH - tankScale.z) {
		move.z = Map::moveLimitH - tankScale.z;
	}
	else if (move.z < -Map::moveLimitH + tankScale.z ) {
		move.z = -Map::moveLimitH + tankScale.z;
	}


	tankHad->SetPosition(move);
	tankBody->SetPosition(move);
	tankBody->SetRotation(bodyRot);
}

void Player::Shot() {
	
	//弾の速度
	const float kBulletSpeed = 1.5f;
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
			//弾を生成し
			bulletManager->PlayerBulletCreate(tankHad->GetPosition(),velocity,tankHad->GetRotation());
			
			//タイムリセット
			coolTime = 30;

		}
	}
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

void Player::OnCollisionPos(std::string hitDirection)
{
	Vector3 pos=tankBody->GetPosition();
	if ( hitDirection == "x" )
	{
		pos.x = oldPos.x;
	}
	else if ( hitDirection == "z" )
	{
		pos.z = oldPos.z;
	}
	tankHad->SetPosition(pos);
	tankBody->SetPosition(pos);
	tankHad->Update();
	tankBody->Update();
}

void Player::TitleStaging() {
	if ( SceneManager::sceneNum == SceneManager::title )
	{
		if ( TitleScene::movieCount == TitleScene::CameraFirst )
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
		else if ( TitleScene::movieCount == TitleScene::CameraSecond )
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
		else if ( TitleScene::movieCount == TitleScene::CameraThird )
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
		if ( TitleScene::movieCount <= TitleScene::CameraSecond )
		{
			Vector3 move = tankHad->GetPosition();
			const float speed = 0.25f;
			move.x += speed;
			tankHad->SetPosition(move);
			tankBody->SetPosition(move);
		}
		else if ( easeTime < titleEaseTimer && TitleScene::movieCount == TitleScene::CameraThird )
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
		&& GamePlayScene::startCount == GamePlayScene::Down)
	{
		startEaseTime++;
		pos.y= startPosY + ( endPosY - startPosY ) * Easing::easeOutSine(startEaseTime/ startEaseTimer);
		if ( startEaseTime >= startEaseTimer )
		{
			GamePlayScene::startCount++;
			bound = 1.5f;
		}
	}
	else if ( GamePlayScene::startCount== GamePlayScene::Bound1 )
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
	else if ( GamePlayScene::startCount == GamePlayScene::Bound2 )
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
	else if ( GamePlayScene::startCount == GamePlayScene::Wait )
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
	const float percent = 0.8f;
	const float posX = parachutePos.y + 3.0f;
	const float addX = 0.6f;
	const float subtractY = 0.1f;
	if ( startEaseTime < startEaseTimer* percent )
	{
		parachutePos = pos;
		parachutePos.y += parachutePosY;
	}
	else
	{
		if ( pLeaveTime< pLeaveTimer&& parachutePos.x>posX )
		{
			pLeaveTime++;
			rot.z = pStartRotZ + ( pEndRotZ - pStartRotZ ) * Easing::easeOutSine(pLeaveTime / pLeaveTimer);
			scale.x = pStartScaleXZ + ( pEndScaleXZ - pStartScaleXZ ) * Easing::easeOutSine(pLeaveTime / pLeaveTimer);
			scale.z = pStartScaleXZ + ( pEndScaleXZ - pStartScaleXZ ) * Easing::easeOutCirc(pLeaveTime / pLeaveTimer);
		}
		

		parachute->SetRotation(rot);
		parachute->SetScale(scale);
		parachutePos.x += addX;
		parachutePos.y -= subtractY;
	}
	tankHad->SetPosition(pos);
	tankHad->SetRotation({ 0.0f,-90.0f,0.0f });
	tankBody->SetPosition(pos);
	tankBody->SetRotation({ 0.0f,-90.0f,0.0f });
	parachute->SetPosition(parachutePos);
}