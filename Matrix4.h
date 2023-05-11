#pragma once

#include"Vector3.h"
#include<DirectXMath.h>

using namespace DirectX;

//Matrix4�\����
struct Matrix4
{
	float	m[4][4];
};

//�P�ʍs��
Matrix4	MatIdentity();

//�g��k���s��̐ݒ�
Matrix4	MatScale(const Vector3& s);

//��]�s��̍쐬
Matrix4	MatRotX(float angle);
Matrix4	MatRotY(float angle);
Matrix4	MatRotZ(float angle);
Matrix4 MatRot(Vector3 angle);

//���s�ړ�
Matrix4	MatTrans(const Vector3& t);

//���W�ϊ�
Vector3	MatTransform(const Vector3& v, const Matrix4& m);

Matrix4 ConvertXMMATRIXtoMatrix4(XMMATRIX xmMatrix);

//������Z�q�I�[�o�[���[�h
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

//2�����Z�q�I�[�o�[���[�h
Matrix4	operator*(const Matrix4& m1, const	Matrix4& m2);
Vector3	operator*(const Vector3& v, const	Matrix4& m);