#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Bullet.h>
#include<Aimposition.h>
#include<Vector3.h>

class Player
{
public://静的メンバ関数
	Player GetInstnce();

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialeze(Model*model_,Input*input_,Aimposition*aim_);

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 発射
	/// </summary>
	void Shot();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	Vector3 GetVelocity(){ return velocity; }

private://メンバ変数
	Input*input = nullptr;
	Model* model = nullptr;
	Aimposition* aim;
	std::unique_ptr<Object3d>obj = nullptr;
	//弾
	std::list<std::unique_ptr<Bullet>> bullets_;
	//Bullet* one;
	//クールタイム
	int32_t coolTime;

	float angle = 0.0f;
	Vector3 velocity;
};

