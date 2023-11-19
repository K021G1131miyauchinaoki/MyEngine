#pragma once
#include<memory>
#include<Object3d.h>
#include<Vector3.h>
//ブロック
struct BaseBlock
{
	std::unique_ptr<Object3d> obj;
	bool isUp = false;
	bool isDrow = false;
	float frame;
	float stagingTime = 0.0f;
};