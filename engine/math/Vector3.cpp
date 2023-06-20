#include"Vector3.h"
#include<cmath>//sqrt


float Vector3::StaticDot(const Vector3& v1, const Vector3& v2) {
	return	v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3::Vector3()
	:x(0),y(0), z(0)
{

}

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
	
}

float	Vector3::length()const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normaleize()
{
	float	len = length();
	if (len!=0)
	{
		return	*this/= len;
	}

	return	*this;
}

float	Vector3::dot(const Vector3& v)const
{
	return	x * v.x + y * v.y+z*v.z;
}

float	Vector3::cross(const Vector3& v)const
{
	return  x * v.x - y * v.y - z * v.z;
}

Vector3	Vector3::operator+()const
{
	return	*this;
}

Vector3	Vector3::operator-()const
{
	return	Vector3(-x, -y,-z);
}

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
//Vector2 �N���X�ɑ����Ȃ��֐��Q
//�񍀉��Z�q
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