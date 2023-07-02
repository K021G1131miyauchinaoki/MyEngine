#pragma once
#include<Input.h>
#include<Model.h>
#include"Object3d.h"
#include<memory>
#include<list>
#include<Bullet.h>


class Player
{
public://静的メンバ関数
	Player GetInstnce();

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialeze(Model*model_,Input*input_);

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



private://メンバ変数
	Input*input = nullptr;
	Model* model = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//弾
	std::list<std::unique_ptr<Bullet>> bullets_;
	//Bullet* one;
	//クールタイム
	int32_t coolTime;
};

