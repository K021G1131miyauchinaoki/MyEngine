/**
 * @file WorldTransform.h
 * @brief 変換行列
 */

#pragma once
#include"Matrix4.h"
#include"Vector3.h"

class WorldTransform
{
public://メンバ関数
	void Initialize();

	//ワールド変換行列の更新
	void Update();
public://メンバ変数
	//ワールド変換行列
	Matrix4 matWorld;
	//スケール
	Vector3 scale;
	//XYZ軸回転
	Vector3 rotation;
	//ローカル座標
	Vector3 translation;
	//親オブジェクトのワールド変換のポインタ
	WorldTransform* parent = nullptr;
	//パディング
	int8_t PADING[1];
};

