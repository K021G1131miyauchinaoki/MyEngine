/**
 * @file Enemy.h
 * @brief エネミークラス
 */

#pragma once
#include"BaseEnemy.h"

class Enemy :public BaseEnemy
{
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialeze(Model* model_,Model* parachuteModel_,Player*player_,const Vector3& pos_,const Vector3& rot_,BulletManager* bulletManager_)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 待機
	/// </summary>
	void Wait()override;

	/// <summary>
	/// 移動
	/// </summary>
	void Move()override;

	/// <summary>
	/// 発射
	/// </summary>
	void Shot()override;

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate()override;

	/// <summary>
	/// 位置
	/// </summary>
	/// <returns></returns>
	const	Vector3 GetPos()override { return obj->GetPosition(); }

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	const	float GetRadius()override { return radius; }

	/// <summary>
	/// スケール取得
	/// </summary>
	Vector3 GetScale()override {return obj->GetScale();};

	/// <summary>
	/// 衝突時
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// オブジェクト衝突時のコールバック
	/// </summary>
	/// <param name="hitDirection">当たった方向</param>
	void OnCollisionPos(const std::string &hitDirection)override;

	/// <summary>
	/// デスフラグが立ったら
	/// </summary>
	/// <returns></returns>
	bool IsDead()const override { return hp.isDead; }

	//コンストラクタ
	Enemy();

	~Enemy()override;
private://メンバ変数

};