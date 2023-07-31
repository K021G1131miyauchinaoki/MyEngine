#pragma once
#include<Vector3.h>

namespace MyMath
{
	Vector3 normaleize(Vector3 vec);

	//ƒ‰ƒWƒAƒ“•ÏŠ·
	float RadianTransform(float degree);

	//“x”•ÏŠ·
	float DegreeTransform(float radian);

	Vector3 lens(Vector3 vec1, Vector3 vec2);

	float length(Vector3	vec);
};

