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
	/// 初期化
	/// </summary>
	void Initialeze(Model*model_,Input*input_);

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
	const	Vector3 GetPos() {return obj->GetPosition(); }

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
	Input*input = nullptr;
	Model* model = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//弾
	std::list<std::unique_ptr<Bullet>> bullets_;
	//クールタイム
	int32_t coolTime;

	float angle = 0.0f;
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
	const float startEaseTimer=90;
	float startEaseTime;
	int8_t startCount;
	bool isTitleStaging;
	bool isInvincible;
	bool isStart;
};

