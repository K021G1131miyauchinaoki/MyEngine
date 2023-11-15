/**
 * @file Bullet.cpp
 * @brief プレイヤー用弾
 */

#include "Bullet.h"
#include<assert.h>

std::unique_ptr < Model>Bullet::model = nullptr;

void Bullet::StaticInitialize(Model* model_) {
	if (model==nullptr)
	{
		model.reset(model_);
	}
}

void Bullet::Initialize(const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_){
	//NULLポインタチェック
	assert(model);
	
	velocity = veclocity_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model.get());
	obj->SetRotation(rotation_);
	obj->SetPosition(position_);
	obj->SetScale({ r,r,r });
	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();
	deathTimer = kLifeTime;
	const float speed = 1.0f;
	velocity *= speed;
}

void Bullet::Update() {

	Vector3 pos=obj->GetPosition();
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
Vector3 Bullet::GetPos() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos = obj->GetPosition();

	return worldPos;
}

//衝突したら
void Bullet::OnCollision() { isDead = true; }

//描画
void Bullet::Draw() {
	obj->Draw();
}

//終了
void Bullet::Finalize() {
	model.release();
}
//コンストラクタ
Bullet::Bullet() {
	
}
//デストラクタ
Bullet::~Bullet() {
	
}