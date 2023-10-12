/**
 * @file EnemyBullet.cpp
 * @brief 敵用弾
 */
#include "EnemyBullet.h"
#include<assert.h>

std::unique_ptr<Model> EnemyBullet::model = nullptr;

bool EnemyBullet::IsDead() const { return isDead_; }
float EnemyBullet::GetRadius() {return r;}


void EnemyBullet::StaticInitialize(Model* model_) {
	if (model == nullptr)
	{
		model.reset(model_);
	}
}

void EnemyBullet::Initialize( const Vector3& position, const Vector3& velocity, const Vector3& rotation) {
	//NULLポインタチェック
	assert(model);
	/*size_t s=sizeof(model);
	size_t s = sizeof(model);*/
	velocity_ = velocity;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model.get());
	obj->SetRotation(rotation);
	obj->SetPosition(position);
	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();
}

void EnemyBullet::Update() {
	{
		Vector3 pos = obj->GetPosition();
		pos.x += velocity_.x;
		pos.y += velocity_.y;
		pos.z += velocity_.z;
		obj->SetPosition(pos);
	}
	//時間経過で消滅
	if (--deathTimer <= 0) {
		isDead_ = true;
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

//衝突したら
void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw() {
	obj->Draw();
}

//終了
void EnemyBullet::Finalize() {
	model.release();
}

//コンストラクタ
EnemyBullet::EnemyBullet() {
	deathTimer = kLifeTime;
}
//デストラクタ
EnemyBullet::~EnemyBullet() {
	
}