#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<EnemyBullet.h>
#include<Aimposition.h>
#include<Vector3.h>
#include<Player.h>
#include<MyMath.h>

//自機クラスの前方前言
class Player;


//行動フェーズ
enum class Phase {
	wait,//待機
	move, //移動する
	atack,    //攻撃する
};

class Enemy
{
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialeze(Model* model_, Player*player_);

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 待機
	/// </summary>
	void Wait();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 発射
	/// </summary>
	void Shot();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	///デスフラグ 
	/// </summary>
	bool IsDead() { return isDead; }

	/// <summary>
	/// 位置
	/// </summary>
	/// <returns></returns>
	const	Vector3 GetPos() { return obj->GetPosition(); }

	const	float GetRadius() { return radius; }

	void OnCollision();

	//弾リストを取得
	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
private://構造体
	


private://メンバ変数
	Input* input = nullptr;
	Model* model = nullptr;
	
	std::unique_ptr<Object3d>obj = nullptr;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//移動フラグ
	bool isMove = false;
	//移動時間
	int16_t moveTimer;
	const int16_t  moveTime = 120;
	//発射するまでの時間
	int16_t shotTimer;
	const int16_t  shotTime = 10;
	//待機フラグ
	bool isWait = false;
	//待機時間
	int16_t waitTimer;
	int16_t waitTime[2] = { 5,10 };
	//自キャラ
	Player* player = nullptr;

	float angle = 0.0f;
	//速度
	Vector3 velocity;
	//移動
	Vector3 move;
	//フェーズ
	Phase phase = Phase::move;
	//乱数値格納
	Vector3 value;
	//デスフラグ
	bool isDead = false;

	float radius = 5.0f;

	//仮
	bool isInvincible = false;
	const int16_t invincibleTime = 11;
	int16_t invincibleTimer;
};