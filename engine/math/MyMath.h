/**
 * @file MyMath.h
 * @brief 計算やπなどの処理をまとめたヘッダー
 */

#pragma once
#include<Vector3.h>
#include<Vector2.h>


namespace MyMath
{
	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="vec">ベクトル</param>
	Vector3 normaleizeVec3(Vector3 vec);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="vec">ベクトル</param>
	Vector2 normaleizeVec2(Vector2 vec);
	
	/// <summary>
	/// ラジアン変換
	/// </summary>
	/// <param name="degree">度数</param>
	float RadianTransform(float degree);

	/// <summary>
	/// 度数変換
	/// </summary>
	/// <param name="degree">ラジアン</param>
	float DegreeTransform(float radian);

	/// <summary>
	/// 2点の距離を算出
	/// </summary>
	/// <param name="vec1">ベクトル</param>
	/// <param name="vec2">ベクトル</param>
	Vector3 lens(Vector3 vec1, Vector3 vec2);

	/// <summary>
	/// 距離を算出
	/// </summary>
	/// <param name="vec">ベクトル</param>
	float length(Vector3	vec);

	/// <summary>
	/// 円周率
	/// </summary>
	const float PI_F = 3.14f;

	/// <summary>
	/// 最短の角度を算出
	/// </summary>
	/// <param name="a">現在の角度</param>
	/// <param name="b">目標の角度</param>
	/// <param name="t">タイム</param>
	float LerpShortAngle(float a, float b, float t);

	/// <summary>
	/// 角度補正
	/// </summary>
	/// <param name="a">角度</param>
	float AngleCorrection(float a);
};

