/**
 * @file EnemyBullet.h
 * @brief 敵用弾
 */

#pragma once
#include<Model.h>
#include<Object3d.h>
#include<Vector3.h>
#include<memory>

class EnemyBullet
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBullet();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet();
	//代入演算子明示的
	EnemyBullet& operator=(const EnemyBullet&) = default;

	//コピーコンストラクタ明示的
	EnemyBullet(const EnemyBullet&) = default;
public://静的メンバ関数
	static void StaticInitialize(Model* model_);
	static void Finalize();

private://静的メンバ変数
	static	std::unique_ptr < Model> model;

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize( const Vector3& position_, const Vector3& veclocity_, const Vector3& rotation_);
	
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
	bool IsDead() const;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	void OnCollision();
	//ワールド座標を取得
	Vector3 GetPos();

	/// <summary>
	/// スケール取得
	/// </summary>
	Vector3 GetScale();

	//半径を取得
	float GetRadius();

private://メンバ変数
	//消滅
	static const int32_t lifeTime = 60;
	//デスタイマー
	int32_t deathTimer;
	//速度
	Vector3 velocity;
	//半径
	std::unique_ptr<Object3d>obj = nullptr;
	const float r = 1.0f;
	//デスフラグ
	bool isDead = false;

};

