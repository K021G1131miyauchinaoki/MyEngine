/**
 * @file Vector3.h
 * @brief x,y,z成分をもつ
 */
#pragma once
namespace MyEngin
{
	class Vector3
	{
	public:
		float	x,y,z;//x,y成分	
	public:
		//コンストラクタ
		Vector3();
		Vector3(float	x,float	y,float	z);

		//メンバ関数
		float	Length()const;//長さ
		Vector3& Normaleize();//正規化
		float	Dot(const Vector3& v)const;//内積
		float	Cross(const Vector3& v)const;//外積

		//単項演算子オーバーロード
		Vector3	operator+()const;
		Vector3	operator-()const;

		//代入演算子オーバーロード
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator+=(float s);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};

	//2項演算子オーバーロード
	//※色んな引数(引数の型と順序)のパターンに対応するため、以下のように準備している
	const	Vector3	operator+(const	Vector3& v1,const	Vector3& v2);
	const	Vector3	operator-(const	Vector3& v1,const	Vector3& v2);
	const	Vector3	operator*(const	Vector3& v,float	s);
	const	Vector3	operator*(float	s,const	Vector3& v);
	const	Vector3	operator/(const	Vector3& v,float	s);
}