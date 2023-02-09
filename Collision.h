#pragma once
#include"CollisionPrimitive.h"

class Collision
{
public:
	//‹…‚Æ•½–Ê
	static	bool CheckAphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
};

