#pragma once
#include<DirectXMath.h>
#include<Model.h>
#include<Object3d.h>
#include<Vector3.h>

class EnemyBullet
{
public://コンストラクタ
	EnemyBullet();
	~EnemyBullet();
public://静的メンバ関数
	static void StaticInitialize(Model* model_);
	static void Finalize();

private://静的メンバ変数
	static	std::unique_ptr < Model> model;

public://メンバ関数
	void Initialize( const Vector3& position, const Vector3& veclocity, const Vector3& rotation_);
	void Update();
	void Draw();
	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	//ワールド座標を取得
	Vector3 GetPos();
	//半径を取得
	float GetRadius() { return r; }

private://メンバ変数
	std::unique_ptr<Object3d>obj = nullptr;
	//速度
	Vector3 velocity_;
	//消滅
	static const int32_t kLifeTime = 60;
	//デスタイマー
	int32_t deathTimer;
	//デスフラグ
	bool isDead_ = false;

	//半径
	const float r = 1.0f;
};

