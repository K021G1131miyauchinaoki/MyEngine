/**
 * @file Enemy.h
 * @brief エネミークラス
 */

#pragma once
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<EnemyBullet.h>
#include<Vector3.h>
#include<Player.h>
#include<BaseHp.h>
#include<array>

//自機クラスの前方前言
class Player;



class BaseEnemy
{
public:
	//デストラクタ
	virtual ~BaseEnemy() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialeze(Model* model_,Player* player_,Vector3 pos_,Vector3 rot_);

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
	virtual void OnCollisionPos(std::string hitDirection);

	/// <summary>
	/// デスフラグが立ったら
	/// </summary>
	/// <returns></returns>
	virtual bool IsDead()const {
		return hp.isDead;
	}

	//弾リストを取得
	virtual std::list<std::unique_ptr<EnemyBullet>>& GetBullets() {
		return bullets;
	}


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
	Model* model = nullptr;

	std::unique_ptr<Object3d>obj = nullptr;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	//移動フラグ
	bool isMove = false;
	//移動時間
	int16_t moveTime;
	const int16_t  moveTimer = 120;
	//発射するまでの時間
	int16_t shotTimer;
	const int16_t  shotTime = 10;
	//待機フラグ
	bool isWait = false;
	//待機時間
	float waitTime;
	//std::array<float>
	float waitTimer[ 2 ] = { 5,60 };
	//自キャラ
	Player* player = nullptr;
	//角度
	Vector3 vector;
	float angle[ 2 ];
	//速度
	Vector3 velocity;
	//移動
	Vector3 move;
	//フェーズ
	Phase phase = Phase::move;
	MovePhase movePhase = MovePhase::approach;
	//乱数値格納
	Vector3 value;
	//デスフラグ
	float radius = 5.0f;

	//仮
	bool isInvincible = false;
	const int16_t invincibleTimer = 11;
	int16_t invincibleTime;

	//位置
	Vector3 InitialezePos;
	Vector3 playerPos;
	Vector3 oldPos;
	//距離
	Vector3 len;
	//hp
	BaseHp hp;

};
