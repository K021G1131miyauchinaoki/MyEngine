#pragma once

#include"Vector3.h"
#include<DirectXMath.h>

using namespace DirectX;

//Matrix4構造体
struct Matrix4
{
	float	m[4][4];
};

//単位行列
Matrix4	MatIdentity();

//拡大縮小行列の設定
Matrix4	MatScale(const Vector3& s);

//回転行列の作成
Matrix4	MatRotX(float angle);
Matrix4	MatRotY(float angle);
Matrix4	MatRotZ(float angle);
Matrix4 MatRot(Vector3 angle);

//平行移動
Matrix4	MatTrans(const Vector3& t);

//座標変換
Vector3	MatTransform(const Vector3& v, const Matrix4& m);

Matrix4 ConvertXMMATRIXtoMatrix4(XMMATRIX xmMatrix);

//代入演算子オーバーロード
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

//2項演算子オーバーロード
Matrix4	operator*(const Matrix4& m1, const	Matrix4& m2);
Vector3	operator*(const Vector3& v, const	Matrix4& m);