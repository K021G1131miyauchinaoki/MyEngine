#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Bullet.h>
#include<Aimposition.h>
#include<Vector3.h>
#include<Player.h>
#include<MyMath.h>

//自機クラスの前方前言
class Player;

class Enemy
{
public://メンバ関数
/// <summary>
/// 初期化
/// </summary>
	void Initialeze(Model* model_, Player*player_);

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

private://メンバ変数
	Input* input = nullptr;
	Model* model = nullptr;
	
	std::unique_ptr<Object3d>obj = nullptr;
	//弾
	std::list<std::unique_ptr<Bullet>> bullets_;
	//Bullet* one;
	//クールタイム
	int32_t coolTime;

	//自キャラ
	Player* player = nullptr;

	float angle = 0.0f;
	Vector3 velocity;
};