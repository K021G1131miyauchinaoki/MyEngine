#pragma once
#include<Input.h>
#include<Model.h>
#include<Object3d.h>


class Player
{
public://静的メンバ関数
	Player GetInstnce();

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialeze();

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
	std::unique_ptr<Input>input;
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> obj;

};

