#include "MyMath.h"
#include<cmath>	
#define PI (3.14f)

namespace MyMath
{
	Vector3 normaleize(Vector3 vec) {
		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		if (length != 0) {
			vec.x /= length;
			vec.y /= length;
			vec.z /= length;

			return vec;
		}

		return vec;
	}

	//���W�A���ϊ�
	float RadianTransform(float degree) {
		float radian = degree * (PI / 180.0f);
		return radian;
	}

	//�x���ϊ�
	float DegreeTransform(float radian) {
		float degree = radian * (180.0f / PI);
		return degree;
	}
}