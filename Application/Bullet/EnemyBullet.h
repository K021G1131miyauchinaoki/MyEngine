/**
 * @file EnemyBullet.h
 * @brief 敵用弾
 */

#pragma once
#include<Object3d.h>
#include<Vector3.h>
#include<memory>

//前方宣言
class Player;
using namespace MyEngin;

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

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Model*model_, const Vector3& position_, const Vector3& veclocity_, const Vector3& rotation_,Player*player_=nullptr);
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	///デスフラグを取得 
	/// </summary>
	virtual bool IsDead() const;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	virtual void OnCollision();
	//ワールド座標を取得
	virtual Vector3 GetPos();

	/// <summary>
	/// スケール取得
	/// </summary>
	virtual Vector3 GetScale();

	//半径を取得
	virtual float GetRadius();

protected://メンバ変数
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
	const float kBulletSpeed = 1.5f;

	Player* player = nullptr;
};

