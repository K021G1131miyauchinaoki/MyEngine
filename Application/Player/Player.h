/**
 * @file Player.h
 * @brief プレイヤーのヘッダー
 */

#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<vector>
#include<Bullet.h>
#include<Vector3.h>
#include<Vector2.h>
#include<MyMath.h>
#include<BaseHp.h>

class Player
{
public://静的メンバ関数
	//Player GetInstnce();

public://メンバ関数
	/// <summary>
	/// タイトル初期化
	/// </summary>
	/// <param name="tankModel_">自機</param>
	/// <param name="input_">キー操作</param>
	void TitleInitialeze(Model* tankModel_,Input* input_);

	/// /// <summary>
	/// プレイ初期化
	/// </summary>
	/// <param name="tankModel_">自機</param>
	/// <param name="parachuteModel_">パラシュート</param>
	/// <param name="input_">キー操作</param>
	void PlayInitialeze(Model*tankModel_,Model* parachuteModel_,/*Model* model_,*/Input* input_);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// オブジェクト描画
	/// </summary>
	void ObjDraw();

	/// <summary>
	/// スプライト描画
	/// </summary>
	void SpriteDraw();

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
	/// 位置
	/// </summary>
	/// <returns></returns>
	const	Vector3 GetPos() {return tank->GetPosition(); }

	/// <summary>
	/// タイトル演出
	/// </summary>
	void TitleStaging();

	/// <summary>
	/// プレイ時のスタート演出
	/// </summary>
	void StartStaging();

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	void OnCollision();

	//半径を取得
	const	float GetRadius() { return radius; }

	//弾のリストを取得
	std::list<std::unique_ptr<Bullet>>& GetBullets() { return bullets_; };

	//デスフラグの取得
	bool IsDead()const { return hp.isDead; }

	//スタートフラグの取得
	bool IsStart() {return isStart;	}

	//スタートカウントの取得
	int8_t GetStartCount() {
		return startCount;
	}

private://メンバ変数
	std::unique_ptr<Input>input = nullptr;
	std::unique_ptr<Object3d>tank = nullptr;
	std::unique_ptr<Object3d>parachute = nullptr;
	//弾
	std::list<std::unique_ptr<Bullet>> bullets_;
	//クールタイム
	int32_t coolTime;

	float angle = 0.0f;
	Vector3 tankPos;
	Vector3 parachutePos;
	Vector3 velocity;
	Vector2	vector ;
	Vector2	 mausePos;
	//半径
	float radius = 5.0f;

	const int16_t invincibleTime=11;
	int16_t invincibleTimer;

	BaseHp hp;
	std::vector<DrawHp> drawHp;

	float easeTime;
	const float titleEaseTimer = 80.0f;

	float startPosY;
	float endPosY;
	float bound;
	const float startEaseTimer=150;
	float startEaseTime;
	int8_t startCount;
	bool isTitleStaging;
	bool isInvincible;
	bool isStart;
	float parachutePosY;
	float pStartRotZ;
	float pEndRotZ;
	float pStartScaleXZ;
	float pEndScaleXZ;
	float pLeaveTime;
	const	float pLeaveTimer=60;

};

