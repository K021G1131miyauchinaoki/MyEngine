/**
 * @file Vector3.cpp
 * @brief x,y,z成分をもつ
 */
#include"Vector3.h"
#include<cmath>//sqrt

Vector3::Vector3()
	:x(0),y(0), z(0)
{

}

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
	
}

float	Vector3::Length()const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::Normaleize()
{
	float	len = Length();
	if (len!=0)
	{
		return	*this/= len;
	}

	return	*this;
}

float	Vector3::Dot(const Vector3& v)const
{
	return	x * v.x + y * v.y+z*v.z;
}

float	Vector3::Cross(const Vector3& v)const
{
	return  x * v.x - y * v.y - z * v.z;
}

//単項演算子オーバーロード
Vector3	Vector3::operator+()const
{
	return	*this;
}

Vector3	Vector3::operator-()const
{
	return	Vector3(-x, -y,-z);
}

//代入演算子オーバーロード
Vector3& Vector3::operator+=(const	Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return	*this;
}

Vector3& Vector3::operator-=(const	Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return	*this;
}

Vector3& Vector3::operator+=(float	s)
{
	x = s;
	y = s;
	z = s;
	return	*this;
}


Vector3& Vector3::operator*=(float	s)
{
	x *= s;
	y *= s;
	z *= s;
	return	*this;
}

Vector3& Vector3::operator/=(float	s)
{
	x /= s;
	y /= s;
	z /= s;
	return	*this;
}
//Vector2 クラスに属さない関数群
//二項演算子
const	Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3	temp(v1);
	return	temp += v2;
}

const	Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return	temp -= v2;
}

const	Vector3 operator*(const Vector3& v, float	s)
{
	Vector3 temp(v);
	return	temp *= s;
}

const	Vector3 operator*(float	s,const Vector3& v )
{
	return	v * s;
}

const	Vector3 operator/(const Vector3& v, float	s)
{
	Vector3 temp(v);
	return	temp /= s;
}