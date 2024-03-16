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
#include<SoundManager.h>

void Player::ParameterCommonInitialeze() {
	//位置
	startPosY = 120.0f;
	endPosY = radius;
	//サイズ
	tankScale = { radius,radius,radius };
	isTitleStaging = false;
	isInvincible = false;
}

void Player::ModelCommonInitialeze(Model* tankHadModel_,Model* tankBodyModel_) {
	//タンクオブジェクトの初期化
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

	input=input_;
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
	
	input = input_;

	//共通のパラメータ初期化
	ParameterCommonInitialeze();
	//位置
	tankPos = { 0.0f,startPosY,0.0f };
	parachutePosY = 6.0f;
	parachutePos = tankPos;
	parachutePos.y += parachutePosY;
	pPosX = parachutePos.x + 3.0f;

	//サイズ
	pStartScaleXZ = 6.0f;
	pEndScaleXZ = 0.0f;
	//角度
	pStartRotZ = 0;
	pEndRotZ = -90;
	bodyRot = { 0.0f,-90.0f,0.0f };
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
	shakeTimer = shakeTime;

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

			Rotate();
			Shot();
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
				if ( invincibleTimer < 0 )
				{
					invincibleTimer = invincibleTime;
					isInvincible = false;
				}
			}
			//シェイク
			if ( isShake )
			{
				shakeTimer--;
				if ( shakeTimer < 0 )
				{
					shakeTimer = shakeTime;
					isShake = false;
				}
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
	if (invincibleTimer%3==0)
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
			//弾の発射位置の調整
			Vector3 pos=tankHad->GetPosition();
			float radian = -MyMath::RadianTransform(tankHad->GetRotation().y);
			pos.x = GetPos().x + std::cos(radian) * GetScale().x;
			pos.z = GetPos().z + std::sin(radian) * GetScale().z;
			//弾を生成
			bulletManager->PlayerBulletCreate(pos,velocity,tankHad->GetRotation());
			
			//タイムリセット
			coolTime = 30;

			//音
			SoundManager::GetInstance()->PlayWave("SE/gun2.wav",0.2f);
		}
	}

	
}

void Player::Rotate() {
	//頭
	Vector3 rot = { 0,MyMath::DegreeTransform(angle),0 };

	tankHad->SetRotation(rot);
	//胴
	rot = { 0.0f,0.0f,0.0f };
	if ( rotTime<rotTimer )
	{
		rotTime++;
	}
	//何かしらのキーが押された瞬間
	if ( input->TriggerKey(DIK_W)||
		 input->TriggerKey(DIK_A)||
		 input->TriggerKey(DIK_S)||
		 input->TriggerKey(DIK_D)||
		 input->TriggerReleaseKey(DIK_W) ||
		 input->TriggerReleaseKey(DIK_A) ||
		 input->TriggerReleaseKey(DIK_S) ||
		 input->TriggerReleaseKey(DIK_D) )
	{
		rotTime=0;
	}
	//キーが押されたら
	if ( input->PushKey(DIK_W) )
	{
		bodyRot = { 0.0f,-90.0f,0.0f };
	}
	if ( input->PushKey(DIK_S) )
	{
		bodyRot = { 0.0f,90.0f,0.0f };
	}
	if ( input->PushKey(DIK_A) )
	{
		bodyRot = { 0.0f,180.0f,0.0f };
	}
	if ( input->PushKey(DIK_D) )
	{
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
	rot = tankBody->GetRotation();
	float t = rotTime / rotTimer;
	rot.y = MyMath::LerpShortAngle(rot.y,bodyRot.y,t);
	tankBody->SetRotation(rot);
}

void Player::OnCollision() 
{
	//点滅フラグ
	if (!isInvincible)
	{
		//体力減少
		hp.value--;
		//0以下ならtrue
		hp.isDead = hp.value <= 0;
		isInvincible = true;
		//一度だけシェイクする
		if ( !isShake )
		{
			isShake = true;
		}
	}
}

void Player::OnCollisionPos(std::string hitDirection)
{
	//文字列で方向の押し戻し処理
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
		//最初のカメラワークの位置
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
		//二回目
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
		//三回目
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
		//二回目までの移動
		if ( TitleScene::movieCount <= TitleScene::CameraSecond )
		{
			Vector3 move = tankHad->GetPosition();
			const float speed = 0.25f;
			move.x += speed;
			tankHad->SetPosition(move);
			tankBody->SetPosition(move);
		}
		//三回目の移動
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
	//降下
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
	//バウンド
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
	//待ち状態
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
	const float addX = 0.6f;
	const float subtractY = 0.1f;
	//自機と同じ動き
	if ( startEaseTime < startEaseTimer* percent )
	{
		parachutePos = pos;
		parachutePos.y += parachutePosY;
	}
	//自機から離れる
	else
	{
		if ( pLeaveTime< pLeaveTimer&& parachutePos.x>pPosX )
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
	tankHad->SetRotation(bodyRot);
	tankBody->SetPosition(pos);
	tankBody->SetRotation(bodyRot);
	parachute->SetPosition(parachutePos);
}