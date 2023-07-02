#pragma once
#include<DirectXMath.h>
#include<Model.h>
#include<Object3d.h>

class Bullet
{
public:
	void Initialize(Model* model, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& veclocity);
	void Update();
	void Draw();
	bool IsDead() const { return isDead_; }
	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	//ワールド座標を取得
	DirectX::XMFLOAT3 GetBulletPosition();
	//半径を取得
	float GetRadius() { return r; }

private:
	
	Model* model_ = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//速度
	DirectX::XMFLOAT3 velocity_;
	//消滅
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	//半径
	const float r = 1.0f;
};

