#include "MyMath.h"
#include<cmath>	

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
}