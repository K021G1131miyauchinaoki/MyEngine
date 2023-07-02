#include "Aimposition.h"
#include<cassert>

void Aimposition::Initialeze(Model* model_, Input* input_) {
	assert(model_);
	assert(input_);

	model = model_;
	input = input_;
	obj = std::make_unique<Object3d>();
	obj->Initialize();
	obj->SetModel(model_);
	obj->SetPosition({ 6.0f,0.0f,0.0f });
	obj->SetScale({ 2.0f,2.0f,2.0f });
	obj->Update();
	coolTime = 0;
}

void Aimposition::Updata() {
	Move();
	obj->Update();
}

void Aimposition::Draw() {
	obj->Draw();
}

void Aimposition::Move() {
	XMFLOAT3 move = obj->GetPosition();

	const float speed = 0.2f;
	if (input->PushKey(DIK_UP)) {
		move.z += speed;
	}
	else if (input->PushKey(DIK_DOWN)) {
		move.z += -speed;
	}
	else if (input->PushKey(DIK_LEFT)) {
		move.x += -speed;
	}
	else if (input->PushKey(DIK_RIGHT)) {
		move.x += speed;
	}
	obj->SetPosition(move);
}

//ワールド座標を渡す
DirectX::XMFLOAT3 Aimposition::GetPosition() {
	//座標を格納
	DirectX::XMFLOAT3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos = obj->GetPosition();

	return worldPos;
}
