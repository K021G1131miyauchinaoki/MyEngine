
/**
 * @file NormalEnemy.h
 * @brief エネミークラス
 */

#pragma once
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Vector3.h>
#include<Player.h>
#include<BaseHp.h>
#include<array>
#include"BulletManager.h"
#include<string>
//自機クラスの前方前言
class Player;

using namespace MyEngin;


class BaseEnemy
{
public:
	//デストラクタ
	virtual ~BaseEnemy() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Model* model_,Model* parachuteModel_,Player* player_,const Vector3 &pos_,const Vector3& rot_,BulletManager* bulletManager_);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 待機
	/// </summary>
	virtual void Wait();

	/// <summary>
	/// 移動
	/// </summary>
	virtual void Move();

	/// <summary>
	/// 発射
	/// </summary>
	virtual void Shot();

	/// <summary>
	/// 回転
	/// </summary>
	virtual void Rotate();

	/// <summary>
	/// プレイ時のスタート演出
	/// </summary>
	void StartStaging();

	void SetBulletParameter(Vector3 rot_,Vector3 velocity_,std::string type_);

	/// <summary>
	/// 位置
	/// </summary>
	/// <returns></returns>
	virtual const	Vector3 GetPos() {
		return obj->GetPosition();
	}

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	virtual const	float GetRadius() {
		return radius;
	}

	/// <summary>
	/// スケール取得
	/// </summary>
	virtual Vector3 GetScale() {
		return obj->GetScale();
	};

	/// <summary>
	/// 衝突時
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// オブジェクト衝突時のコールバック
	/// </summary>
	/// <param name="hitDirection">当たった方向</param>
	virtual void OnCollisionPos(const std::string &hitDirection);

	/// <summary>
	/// オブジェクト非衝突時のコールバック
	/// </summary>
	virtual void OffCollisionShot();

	/// <summary>
	/// デスフラグが立ったら
	/// </summary>
	/// <returns></returns>
	virtual bool IsDead()const {
		return hp.isDead;
	}

	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns></returns>
	int8_t GetHP()const {
		return hp.value;
	}

	//モーションタイマーをセット　デフォルトで1.0f
	void SetMotionSpeed(const float& mSpeed_ = 1.0f);

protected://メンバ変数
	//行動フェーズ
	enum class Phase
	{
		wait,//待機
		move, //移動する
		atack,    //攻撃する
	};
	enum class MovePhase
	{
		approach,//近づく
		leave,//離れる
		rotate,//回転する
	};
	enum
	{
		before,//前
		after,//後ろ
	};

protected://メンバ変数
	std::unique_ptr<Object3d>obj = nullptr;
	std::unique_ptr<Object3d>parachute = nullptr;
	BulletManager* bulletManager = nullptr;
	//自キャラ
	Player* player = nullptr;
	//角度
	Vector3 rot;
	//速度
	Vector3 velocity;
	//位置
	Vector3 stratPos,endPos;
	Vector3 playerPos;
	Vector3 oldPos;
	Vector3 parachutePos;
	//距離
	Vector3 len;
	//位置加算
	Vector3 addPos;
	//フェーズ
	Phase phase = Phase::move;
	MovePhase movePhase = MovePhase::approach;
	//半径
	float radius = 4.0f;

	//点滅時間
	const int16_t invincibleTimer = 11;
	int16_t invincibleTime;
	//移動時間
	int16_t moveTimer;
	const int16_t  moveTime = 120;
	//オブジェクトに当たった時の方向転換の時間
	float shiftChangeTimer;
	const float  shiftChangeTime = 90;
	//発射するまでの時間
	int16_t shotTimer;
	const int16_t  shotTime = 50;
	//待機時間
	float waitTime;
	std::array<float,2> waitTimer = { 20,60 };

	//hp
	BaseHp hp;
	float startPosY,endPosY;
	//イージング変数
	const float startEaseTimer = 80;
	float startEaseTime;
	//パラシュート用変数
	float parachutePosY;
	float pStartRotZ;
	float pEndRotZ;
	float pStartScaleXZ;
	float pEndScaleXZ;
	//角度
	float waitAngle;
	float moveAngle;
	//モーション速度
	float mSpeed;
	//移動フラグ
	bool isMove = false;
	//待機フラグ
	bool isWait = false;
	//点滅
	bool isInvincible = false;
	//時計回り
	bool isClockwise;
	//方向転換フラグ
	bool isShiftChange;
	//発射フラグ
	bool isShot;
};

