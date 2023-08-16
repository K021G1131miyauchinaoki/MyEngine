#pragma once
#include<Vector3.h>
#include<Vector2.h>

namespace MyMath
{
	Vector3 normaleizeVec3(Vector3 vec);
	Vector2 normaleizeVec2(Vector2 vec);

	//ƒ‰ƒWƒAƒ“•ÏŠ·
	float RadianTransform(float degree);

	//“x”•ÏŠ·
	float DegreeTransform(float radian);

	Vector3 lens(Vector3 vec1, Vector3 vec2);

	float length(Vector3	vec);

	 const float PI_F = 3.14f;

};

