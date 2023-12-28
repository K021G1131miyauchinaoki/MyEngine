#pragma once
#include"EnemyBullet.h"

class Homing :public EnemyBullet
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model_,const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_,Player* player_ = nullptr)override;

	/// <summary>
	/// 更新
	/// </summary>
	 void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	 void Draw()override;

	/// <summary>
	///デスフラグを取得 
	/// </summary>
	 bool IsDead() const;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	 void OnCollision()override;
	//ワールド座標を取得
	 Vector3 GetPos()override;

	/// <summary>
	/// スケール取得
	/// </summary>
	 Vector3 GetScale()override;

	//半径を取得
	 float GetRadius()override;
};