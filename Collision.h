#pragma once
#include"CollisionPrimitive.h"

class Collision
{
public:
	//���ƕ���
	static	bool CheckAphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
};

