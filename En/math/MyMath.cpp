/**
 * @file MyMath.cpp
 * @brief 計算式を保持
 */

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

	//ラジアン変換
	float RadianTransform(float degree) {
		float radian = degree * (PI / 180.0f);
		return radian;
	}

	//度数変換
	float DegreeTransform(float radian) {
		float degree = radian * (180.0f / PI);
		return degree;
	}

	Vector3 lens(Vector3 vec1, Vector3 vec2) {
		Vector3 vec = vec1 -= vec2;
		return vec;
	}

	float Length(Vector3 vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); }

	float LerpShortAngle(float a, float b, float t) {
		float diff = b - a;

		diff = static_cast<float>(std::fmod(diff, 360));
		// 2πまたはπに補正
		if (diff > 180) {
			diff -= 360;
		}
		else if (diff < -180) {
			diff += 360;
		}
		float total = a + diff * t;

		return total;
	}
	float AngleCorrection(float a) {
		a = static_cast< float >( std::fmod(a,360) );
		// 2πまたはπに補正
		if ( a > 180 )
		{
			a -= 360;
		}
		else if ( a < -180 )
		{
			a += 360;
		}

		return a;
	}

}