/**
 * @file Bullet.h
 * @brief プレイヤー用弾
 */
#pragma once
#include<Model.h>
#include<Object3d.h>
#include<Vector3.h>

class Bullet
{
public://コンストラクタ
	Bullet();
	~Bullet();
public://静的メンバ関数
	static void StaticInitialize(Model* model_);
	static void Finalize();
public://メンバ関数
/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	///デスフラグを取得 
	/// </summary>
	bool IsDead() const { return isDead; }

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	Vector3 GetPos();

	/// <summary>
	/// /半径を取得
	/// </summary>
	float GetRadius() { return r; }

private:
	
	static	std::unique_ptr < Model>model;
	std::unique_ptr<Object3d>obj = nullptr;
	//速度
	Vector3 velocity;
	//消滅
	static const int32_t kLifeTime = 60;
	//デスタイマー
	int32_t deathTimer;
	//デスフラグ
	bool isDead = false;

	//半径
	const float r = 0.2f;
};

