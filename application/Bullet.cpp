#include "Bullet.h"

#include<assert.h>
void Bullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity,const Vector3& rotation){
	//NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model);
	obj->SetRotation(rotation);
	obj->SetPosition(position);
	obj->Update();
	deathTimer = kLifeTime;
}

void Bullet::Update() {
	{
		Vector3 pos=obj->GetPosition();
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
Vector3 Bullet::GetBulletPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos = obj->GetPosition();

	return worldPos;
}

//衝突したら
void Bullet::OnCollision() { isDead_ = true; }

void Bullet::Draw() {
	obj->Draw();
}

//コンストラクタ
Bullet::Bullet() {
	
}
//デストラクタ
Bullet::~Bullet() {
	
}