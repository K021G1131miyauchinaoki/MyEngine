/**
 * @file Bullet.cpp
 * @brief プレイヤー用弾
 */

#include<Model.h>
#include "Bullet.h"
#include<assert.h>
#include<GamePlayScene.h>

void Bullet::Initialize(Model* model_,const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_){
	//NULLポインタチェック
	assert(model_);
	
	velocity = veclocity_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetRotation(rotation_);
	obj->SetPosition(position_);
	obj->SetScale({ r,r,r });
	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	obj->Update();
	deathTimer = kLifeTime;
	const float speed = 1.0f;
	velocity *= speed;
	mSpeed = 1.0f;
}

void Bullet::Update() {

	Vector3 pos=obj->GetPosition();
	pos += velocity * mSpeed;
	obj->SetPosition(pos);
	
	//時間経過で消滅
	if (--deathTimer <= 0&&!GamePlayScene::isSlow) {
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

void Bullet::SetMotionSpeed(const float& mSpeed_)
{
	mSpeed = mSpeed_;
}

Vector3 Bullet::GetScale() {
	Vector3 scale;
	//スケールを取得
	scale = obj->GetScale();

	return scale;
}

//衝突したら
void Bullet::OnCollision() { isDead = true; }

//描画
void Bullet::Draw() {
	obj->Draw();
}

//コンストラクタ
Bullet::Bullet() {
	
}
//デストラクタ
Bullet::~Bullet() {
	
}