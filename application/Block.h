#pragma once
#include<Object3d.h>
#include<Vector3.h>

struct Block
{
	std::unique_ptr<Object3d> obj;
	Vector3	pos;
	bool isUp = false;
	float frame;
	float y = 0.0f;
};
