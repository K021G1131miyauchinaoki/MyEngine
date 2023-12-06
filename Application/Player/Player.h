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
#include<Vector3.h>
#include<Vector2.h>
#include<MyMath.h>
#include<BaseHp.h>
#include"BulletManager.h"

class Player
{
public://メンバ関数
	/// <summary>
	/// タイトル初期化
	/// </summary>
	/// <param name="tankModel_">自機</param>
	/// <param name="input_">キー操作</param>
	void TitleInitialeze(Model* tankHadModel_,Model* tankBodyModel_,Input* input_);

	/// /// <summary>
	/// プレイ初期化
	/// </summary>
	/// <param name="tankModel_">自機</param>
	/// <param name="parachuteModel_">パラシュート</param>
	/// <param name="input_">キー操作</param>
	void PlayInitialeze(Model* tankHadModel_,Model* tankBodyModel_,Model* parachuteModel_,Input* input_,BulletManager* bulletManager_);

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
	const	Vector3 GetPos() {return tankHad->GetPosition(); }

	/// <summary>
	/// スケール取得
	/// </summary>
	Vector3 GetScale() {return tankBody->GetScale();};

	/// <summary>
	/// タイトル演出
	/// </summary>
	void TitleStaging();

	/// <summary>
	/// プレイ時のスタート演出
	/// </summary>
	void StartStaging();

	/// <summary>
	/// 攻撃を食らった時のコールバック
	/// </summary>
	void OnCollision();

	/// <summary>
	/// オブジェクト衝突時のコールバック
	/// </summary>
	/// <param name="hitDirection">当たった方向</param>
	void OnCollisionPos(std::string hitDirection);

	//半径を取得
	const	float GetRadius() { return radius; }

	//デスフラグの取得
	bool IsDead()const { return hp.isDead; }

	//共通パラメータの初期化
	void ParameterCommonInitialeze();

	//共通モデルの初期化
	void ModelCommonInitialeze(Model* tankHadModel_,Model* tankBodyModel_);

	//HPの取得
	int8_t GetHp() {return hp.value;}

private://メンバ変数
	std::unique_ptr<Input>input = nullptr;
	std::unique_ptr<Object3d>tankHad = nullptr;
	std::unique_ptr<Object3d>tankBody = nullptr;
	std::unique_ptr<Object3d>parachute = nullptr;
	BulletManager* bulletManager=nullptr;
	//クールタイム
	int32_t coolTime;

	float angle = 0.0f;
	Vector3 oldPos;
	Vector3 tankPos;
	Vector3 tankScale;
	Vector3 bodyRot;
	Vector3 parachutePos;
	Vector3 velocity;
	Vector2	mouseVec ;
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
	bool isTitleStaging;
	bool isInvincible;
	float parachutePosY;
	float pStartRotZ;
	float pEndRotZ;
	float pStartScaleXZ;
	float pEndScaleXZ;
	float pLeaveTime;
	const	float pLeaveTimer=60;

};

