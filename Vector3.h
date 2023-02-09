#pragma once
class Vector3
{
public:
	float	x, y,z;//x,y����
public://�ÓI�����o�֐�
	static float StaticDot(const Vector3& v1,const Vector3& v2);
	
public:
	//�R���X�g���N�^
	Vector3();
	Vector3(float	x, float	y, float	z);

	//�����o�֐�
	float	length()const;
	Vector3& normaleize();
	float	dot(const Vector3& v)const;
	float	cross(const Vector3& v)const;

	//�P�����Z�q�I�[�o�[���[�h
	Vector3	operator+()const;
	Vector3	operator-()const;

	//������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const	Vector3&	v);
	Vector3& operator-=(const	Vector3&	v);
	Vector3& operator*=(float	s);
	Vector3& operator/=(float	s);
};
//2�����Z�q�I�[�o�[���[�h
//���F��Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const	Vector3	operator+(const	Vector3& v1, const	Vector3& v2);
const	Vector3	operator-(const	Vector3& v1, const	Vector3& v2);
const	Vector3	operator*(const	Vector3& v, float	s);
const	Vector3	operator*(float	s, const	Vector3& v);
const	Vector3	operator/(const	Vector3& v, float	s);