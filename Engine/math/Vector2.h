/**
 * @file Vector2.h
 * @brief x,y成分をもつ
 */
#pragma once
namespace MyEngin
{
	class Vector2
	{
	public:
		float	x,y;//x,y成分

	public:
		//コンストラクタ
		Vector2();
		Vector2(float	x,float	y);

		//メンバ関数
		//長さ
		float	length()const;
		//正規化
		Vector2& normaleize();
		//内積
		float	dot(const Vector2& v)const;
		//外積
		float	cross(const Vector2& v)const;

		//単項演算子オーバーロード
		Vector2	operator+()const;
		Vector2	operator-()const;

		//代入演算子オーバーロード
		Vector2& operator-=(const	Vector2& v);
		Vector2& operator+=(const	Vector2& v);
		Vector2& operator*=(float	s);
		Vector2& operator/=(float	s);
	};

	//2項演算子オーバーロード
	//※色んな引数(引数の型と順序)のパターンに対応するため、以下のように準備している
	const	Vector2	operator+(const	Vector2& v1,const	Vector2& v2);
	const	Vector2	operator-(const	Vector2& v1,const	Vector2& v2);
	const	Vector2	operator*(const	Vector2& v,float	s);
	const	Vector2	operator*(float	s,const	Vector2& v);
	const	Vector2	operator/(const	Vector2& v,float	s);
}