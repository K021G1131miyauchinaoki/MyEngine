#pragma once
#include<Vector3.h>

namespace MyMath
{
	Vector3 normaleize(Vector3 vec);

	//���W�A���ϊ�
	float RadianTransform(float degree);

	//�x���ϊ�
	float DegreeTransform(float radian);

	Vector3 lens(Vector3 vec1, Vector3 vec2);

	float length(Vector3	vec);
};

