/**
 * @file EnemyBullet.cpp
 * @brief 敵用弾
 */

#include "EnemyBullet.h"
#include<Model.h>
#include<assert.h>
#include<GamePlayScene.h>

bool EnemyBullet::IsDead() const { return isDead; }
float EnemyBullet::GetRadius() {return r;}

void EnemyBullet::SetMotionSpeed(const float& mSpeed_)
{
	mSpeed = mSpeed_;
}

void EnemyBullet::Initialize(Model* model_,const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_,Player* player_) {
	//NULLポインタチェック
	assert(model_);
	if (player_)
	{
		player = player_;
	}
	velocity = veclocity_ * kBulletSpeed;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetRotation(rotation_);
	obj->SetPosition(position_);
	obj->SetScale({r,r,r});

	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();

	deathTimer = lifeTime;
	mSpeed = 1.0f;
	isDead = true;
}

void EnemyBullet::Update() {
	
	Vector3 pos = obj->GetPosition();
	pos += velocity * mSpeed;
	obj->SetPosition(pos);
	
	//時間経過で消滅
	if (--deathTimer <= 0&&!GamePlayScene::isSlow) {
		XORDead();
	}
	obj->Update();
}

//ワールド座標を渡す
Vector3 EnemyBullet::GetPos() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos = obj->GetPosition();

	return worldPos;
}

Vector3 EnemyBullet::GetScale() {
	Vector3 scale;
	//スケールを取得
	scale = obj->GetScale();

	return scale;
}


//衝突したら
void EnemyBullet::OnCollision() { XORDead(); }

void EnemyBullet::Draw() {
	obj->Draw();
}

//コンストラクタ
EnemyBullet::EnemyBullet() {
}
//デストラクタ
EnemyBullet::~EnemyBullet() {
	
}