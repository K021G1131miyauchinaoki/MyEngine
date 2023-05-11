#include"Matrix4.h"
#include"Vector3.h"
#include<cmath>

//単位行列
Matrix4	MatIdentity()
{
	static const	Matrix4	result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return	result;
}

//拡大縮小行列の設定
Matrix4	MatScale(const Vector3& s)
{
	Matrix4	result
	{
		 s.x,0.0f,0.0f,0.0f,
		0.0f, s.y,0.0f,0.0f,
		0.0f,0.0f, s.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return	result;
}

//回転行列の作成
//X軸周り
Matrix4	MatRotX(float angle) 
{
	float	sin = std::sin(angle);
	float	cos = std::cos(angle);
	Matrix4	result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f, cos, sin,0.0f,
		0.0f,-sin, cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return	result;
}
//Y軸周り
Matrix4	MatRotY(float angle)
{
	float	sin = std::sin(angle);
	float	cos = std::cos(angle);
	Matrix4	result
	{
		 cos,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		 sin,0.0f, cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return	result;
}
//Z軸周り
Matrix4	MatRotZ(float angle)
{
	float	sin = std::sin(angle);
	float	cos = std::cos(angle);
	Matrix4	result
	{
		 cos, sin,0.0f,0.0f,
		-sin, cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return	result;
}

//回転
Matrix4	MatRot(Vector3 angle)
{
	Matrix4	result = MatIdentity();
	Matrix4	x = MatIdentity();
	Matrix4	y = MatIdentity();
	Matrix4	z = MatIdentity();

	z = MatRotZ(angle.z);
	x = MatRotX(angle.x);
	y = MatRotY(angle.y);

	result *= z;
	result *= x;
	result *= y;

	return	result;
}


//平行移動
Matrix4	MatTrans(const Vector3& t)
{
	Matrix4	result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		 t.x, t.y, t.z,1.0f
	};
	return	result;
}

//座標変換
Vector3	MatTransform(const Vector3& v, const Matrix4& m)
{
	float	w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3	result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return	result;
}

Matrix4 ConvertXMMATRIXtoMatrix4(XMMATRIX xmMatrix) {
	Matrix4 matrix4;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			matrix4.m[i][j] = xmMatrix.r[i].m128_f32[j];

		}
	}

	return matrix4;
}



//代入演算子オーバーロード
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2) 
{
	Matrix4	result{ 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;
	return	m1;
}

//2項演算子オーバーロード
Matrix4	operator*(const Matrix4& m1, const	Matrix4& m2)
{
	Matrix4	result = m1;

	return	result *= m2;
}
Vector3	operator*(const Vector3& v, const	Matrix4& m)
{
	return	MatTransform(v, m);
}