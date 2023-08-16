#include "MyMath.h"
#include<cmath>	
#define PI (3.14f)

namespace MyMath
{
	Vector3 normaleizeVec3(Vector3 vec) {
		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		if (length != 0) {
			vec.x /= length;
			vec.y /= length;
			vec.z /= length;

			return vec;
		}

		return vec;
	}

	Vector2 normaleizeVec2(Vector2 vec) {
		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
		if (length != 0) {
			vec.x /= length;
			vec.y /= length;

			return vec;
		}

		return vec;
	}

	//ƒ‰ƒWƒAƒ“•ÏŠ·
	float RadianTransform(float degree) {
		float radian = degree * (PI / 180.0f);
		return radian;
	}

	//“x”•ÏŠ·
	float DegreeTransform(float radian) {
		float degree = radian * (180.0f / PI);
		return degree;
	}

	Vector3 lens(Vector3 vec1, Vector3 vec2) {
		Vector3 vec = vec1 -= vec2;
		return vec;
	}

	float length(Vector3 vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); }

}