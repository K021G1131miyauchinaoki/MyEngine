#pragma once
#include<DirectXMath.h>
#include<Model.h>
#include<Object3d.h>
#include<Input.h>

class Aimposition
{
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialeze(Model* model_, Input* input_);

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

private://メンバ変数
	Input* input = nullptr;
	Model* model = nullptr;
	std::unique_ptr<Object3d>obj = nullptr;
	//クールタイム
	int32_t coolTime;
};


