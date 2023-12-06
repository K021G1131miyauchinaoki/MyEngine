/**
 * @file EnemyBullet.cpp
 * @brief 敵用弾
 */

#include "EnemyBullet.h"
#include<Model.h>
#include<assert.h>

bool EnemyBullet::IsDead() const { return isDead; }
float EnemyBullet::GetRadius() {return r;}

void EnemyBullet::Initialize(Model* model_,const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_) {
	//NULLポインタチェック
	assert(model_);
	velocity = veclocity_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetRotation(rotation_);
	obj->SetPosition(position_);
	obj->SetScale({r,r,r});

	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();
}

void EnemyBullet::Update() {
	
	Vector3 pos = obj->GetPosition();
	pos.x += velocity.x;
	pos.y += velocity.y;
	pos.z += velocity.z;
	obj->SetPosition(pos);
	
	//時間経過で消滅
	if (--deathTimer <= 0) {
		isDead = true;
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
void EnemyBullet::OnCollision() { isDead = true; }

void EnemyBullet::Draw() {
	obj->Draw();
}

//コンストラクタ
EnemyBullet::EnemyBullet() {
	deathTimer = lifeTime;
}
//デストラクタ
EnemyBullet::~EnemyBullet() {
	
}