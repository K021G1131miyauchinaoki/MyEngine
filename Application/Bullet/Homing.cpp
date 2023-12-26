#include "Homing.h"
#include"Player.h"
#include<Model.h>
#include<cmath>
#include<assert.h>

bool Homing::IsDead() const {
	return isDead;
}
float Homing::GetRadius() {
	return r;
}

void Homing::Initialize(Model* model_,const Vector3& position_,const Vector3& veclocity_,const Vector3& rotation_,Player* player_) {
	//NULLポインタチェック
	EnemyBullet::Initialize(model_,position_,veclocity_,rotation_,player_);
	//タイマーを3倍に
	deathTimer *= 3;
}

void Homing::Update() {

	Vector3 pos = obj->GetPosition();
	Vector3 pPos = player->GetPos();
	Vector3 range,move;
	float angle;
	float speed = 0.5f;
	range = pPos - pos;
	angle = std::atan2(range.z,range.x);
	move.x = std::cos(angle) * speed;
	move.z = std::sin(angle) * speed;
	move.y = 0.0f;
	pos += move;
	obj->SetPosition(pos);

	//時間経過で消滅
	if ( --deathTimer <= 0 )
	{
		isDead = true;
	}
	obj->Update();
}

//ワールド座標を渡す
Vector3 Homing::GetPos() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos = obj->GetPosition();

	return worldPos;
}

Vector3 Homing::GetScale() {
	Vector3 scale;
	//スケールを取得
	scale = obj->GetScale();

	return scale;
}


//衝突したら
void Homing::OnCollision() {
	isDead = true;
}

void Homing::Draw() {
	obj->Draw();
}
