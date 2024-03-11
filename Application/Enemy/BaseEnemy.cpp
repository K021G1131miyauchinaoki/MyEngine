/**
 * @file BaseEnemy.cpp
 * @brief エネミークラス
 */

#include<cassert>
#include<cmath>
#include<random>
#include "BaseEnemy.h"
#include<SceneManager.h>
#include<Map.h>
#include<MyMath.h>
#include"GamePlayScene.h"
#include"Easing.h"
#include<SoundManager.h>


void BaseEnemy::Initialize(Model* model_,Model* parachuteModel_,Player* player_,const Vector3& pos_,const Vector3& rot_,BulletManager* bulletManager_) {
	assert(model_);
	assert(player_);
	//位置
	startPosY = 60.0f;
	endPosY = pos_.y;
	parachutePosY = 6.0f;
	parachutePos = pos_;
	parachutePos.y += parachutePosY;
	//サイズ
	pStartScaleXZ = 6.0f;
	pEndScaleXZ = 0.0f;
	//角度
	pStartRotZ = 0;
	pEndRotZ = -90;
	stratPos = pos_;
	stratPos.y = startPosY;
	endPos = pos_;

	player=player_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition(stratPos);
	obj->SetRotation(rot_);
	obj->SetScale({ radius,radius,radius });


	parachute = std::make_unique<Object3d>();
	parachute->Initialize();
	parachute->SetModel(parachuteModel_);
	parachute->SetPosition(parachutePos);
	parachute->SetScale({ pStartScaleXZ,4.0f,pStartScaleXZ });
	parachute->SetRotation({ 0.0f,0.0f,0.0f });
	parachute->Update();

	moveTimer = moveTime;
	shotTimer = 20;
	shiftChangeTimer = shiftChangeTime;
	waitTime = 0.0f;
	invincibleTime = invincibleTimer;
	waitAngle = 0.0f;
	moveAngle = 0.0f;
	//体力
	hp.value = 3;
	hp.isDead = false;
	bulletManager = bulletManager_;

	//移動フラグ
	isMove = false;
	//待機フラグ
	isWait = false;
	//点滅
	isInvincible = false;
	//時計回り
	isClockwise = false;
	//方向転換フラグ
	isShiftChange = false;
	//発射フラグ
	isShot = false;
}

void BaseEnemy::Update() {
	//スタート演出
	if ( GamePlayScene::isStart )
	{
		StartStaging();
	}
	//プレイ中
	else
	{
		switch ( phase )
		{
		case Phase::wait:
			Wait();
			break;
		case Phase::move:
			Move();
			break;
		case Phase::atack:
			Shot();
			break;
		}
	}
	obj->Update();
	parachute->Update();
	//点滅
	if ( isInvincible )
	{
		invincibleTime--;
	}
	if ( invincibleTime < 0 )
	{
		invincibleTime = invincibleTimer;
		isInvincible = false;
	}
}

void BaseEnemy::Draw() {
	if ( GamePlayScene::startCount >= GamePlayScene::Bound2 )
	{
		if ( invincibleTime % 2 == 1 )
		{
			obj->Draw();
		}
		parachute->Draw();
	}
}

void BaseEnemy::Move() {
	Vector3 pos={0.0f,0.0f,0.0f};
	rot={0.0f,0.0f,0.0f};
	float r;
	//敵の速度
	const float speed = 0.5f;
	//加算用角度
	const float addAngle = 1.0f;
	//パーセント
	float percent = 0.1f;
	if ( !isMove&&!isShiftChange)
	{
		//向きに対してプラスする角度
		float shift = 180.0f;
		float radiusShift = 5.0f;
		rot = { 0.0f,0.0f,0.0f };
		#pragma region 乱数
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		
		
		std::uniform_real_distribution<float> rotDist(-shift,shift);
		std::uniform_real_distribution<float> radiusDist(0.1f,radiusShift);
		std::uniform_int_distribution<int> Dist(false,true);

		isClockwise = Dist(engine);

		#pragma endregion
		isMove = true;
	}
	if ( isShot )
	{
		shotTimer--;
		if ( shotTimer<0 )
		{
			phase = Phase::wait;
			shotTimer = shotTime;
			isShot = false;
		}
	}

	//動作用タイマーとフラグのリセット
	if ( moveTimer < 0 )
	{
		moveTimer = moveTime;
		isMove = false;
	}
	if ( shiftChangeTimer < 0.0f )
	{
		shiftChangeTimer = shiftChangeTime;
		isShiftChange = false;
	}
	//蛇行
	if ( !isShiftChange )
	{
		moveTimer--;
	}
	//旋回
	else
	{
		shiftChangeTimer--;
	}
	oldPos = obj->GetPosition();
	pos = obj->GetPosition();
	//弧度法に変換
	moveAngle = MyMath::AngleCorrection(moveAngle);//角度の補正
	r = MyMath::RadianTransform(moveAngle);

	value = { std::cos(r) * speed,0.0f,std::sin(r) * speed };
	pos += value;
	rot.y -= moveAngle;
	obj->SetRotation(rot);
	if ( moveTimer>moveTime/4 || (shiftChangeTimer > shiftChangeTime *percent&&isShiftChange))
	{
		
		//時計回り
		if ( isClockwise )
		{
			moveAngle += addAngle;
		}
		//反時計回り
		else
		{
			moveAngle -= addAngle;
		}
	}
	//方向転換のフラグ
	if (  pos.x >= Map::moveLimitW - radius   //xがw-rより大きい
		||pos.x <= -Map::moveLimitW + radius  //xが-w+rより小さい
		||pos.z >= Map::moveLimitH - radius	 //zがh-rより大きい
		||pos.z <= -Map::moveLimitH + radius )//zが-h+rより小さい
	{
		if ( !isShiftChange )
		{
			isShiftChange = true;

		}
	}
	//移動範囲の制限
	if ( pos.x > Map::moveLimitW - radius )
	{
		pos.x = Map::moveLimitW - radius;
	}
	else if ( pos.x < -Map::moveLimitW + radius )
	{
		pos.x = -Map::moveLimitW + radius;
	}

	if ( pos.z > Map::moveLimitH - radius )
	{
		pos.z = Map::moveLimitH - radius;
	}
	else if ( pos.z < -Map::moveLimitH + radius )
	{
		pos.z = -Map::moveLimitH + radius;
	}


	obj->SetPosition(pos);
}

void BaseEnemy::Shot() {
	//フェーズの切り替え
	phase = Phase::wait;
	//音
	SoundManager::GetInstance()->PlayWave("SE/gun.wav",0.2f);
}

void BaseEnemy::Rotate() {

}

void BaseEnemy::Wait() {
	Vector3 pos;
	rot = { 0.0f,0.0f,0.0f };
	waitTime++;
	pos = obj->GetPosition();
	playerPos = player->GetPos();

	len = playerPos - pos;
	// 正規化
	rot = MyMath::normaleizeVec3(len);
	//角度を算出
	waitAngle = MyMath::DegreeTransform(-atan2(rot.z,rot.x));

	//回転
	if ( !isWait )
	{
		rot = obj->GetRotation();
		float t = waitTime / waitTimer[ before ];
		rot.y = MyMath::LerpShortAngle(rot.y,waitAngle,t);
		obj->SetRotation(rot);
	}

	if ( waitTime > waitTimer[ before ] && !isWait )
	{
		isWait = true;
		waitTime = 0.0f;
		phase = Phase::atack;
	}
	else if ( waitTime > waitTimer[ after ] && isWait )
	{
		isWait = false;
		waitTime = 0.0f;
		phase = Phase::move;
	}
}

void BaseEnemy::StartStaging() {
	Vector3 pos = obj->GetPosition();
	Vector3 pRot = parachute->GetRotation();
	float percent = 0.9f;

	if ( startEaseTime < startEaseTimer
		&& GamePlayScene::startCount >= GamePlayScene::Bound2 )
	{
		startEaseTime++;
		if ( startEaseTime < (startEaseTimer*percent))
		{
			pos.y = startPosY + ( endPosY - startPosY ) * Easing::easeInSine(startEaseTime / (startEaseTimer*percent));
		}
		else
		{
			pos.y = startPosY + ( endPosY - startPosY ) * Easing::easeOutBounce(startEaseTime / startEaseTimer);
		}
		
		
	}
	//パラシュート
	//タイマーが50％以下なら
	percent = 0.8f;
	const float addX = 0.5f;
	float subtractY = 0.6f;
	if ( startEaseTime < startEaseTimer * percent )
	{
		parachutePos = pos;
		parachutePos.y += parachutePosY;
	}
	else
	{
		
		parachutePos.x += addX;
		parachutePos.y += subtractY;
		pRot.y++;
		parachute->SetRotation(pRot);
	}
	obj->SetPosition(pos);
	parachute->SetPosition(parachutePos);
}

//衝突したら
void BaseEnemy::OnCollision()
{
	if ( !isInvincible )
	{
		isInvincible = true;
	}
	hp.value--;
	if ( hp.value <= 0 )
	{
		hp.isDead = true;
	}
}

void BaseEnemy::OnCollisionPos(const std::string& hitDirection)
{
	Vector3 pos = obj->GetPosition();
	if ( hitDirection == "x" )
	{
		pos.x = oldPos.x;
	}
	else if ( hitDirection == "z" )
	{
		pos.z = oldPos.z;
	}

	obj->SetPosition(pos);
	obj->Update();

	isShiftChange = true;
}

void BaseEnemy::OffCollisionShot()
{
	isShot = true;
}

void BaseEnemy::SetBulletParameter(Vector3 rot_,Vector3 velocity_,std::string type_) {
	Vector3 pos = obj->GetPosition();
	//弾の発射位置の調整
	float radian = -MyMath::RadianTransform(rot_.y);
	//調整
	float adjustment = 2.0f;
	//x半径
	float r = obj->GetScale().x * adjustment;
	pos.x = obj->GetPosition().x + std::cos(radian) * r;
	//z半径
	r = obj->GetScale().z * adjustment;
	pos.z = obj->GetPosition().z + std::sin(radian) * r;
	bulletManager->EnemyBulletCreate(pos,velocity_,rot_,type_);

}