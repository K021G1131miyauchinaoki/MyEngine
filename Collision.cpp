#include "Collision.h"

using namespace DirectX;

bool Collision::CheckAphere2Plane(const Sphere& sphere, const Plane& plane, Vector3*inter) {
	//座標系の原点から球の中心座標への距離
	float distV = Vector3::StaticDot(sphere.center, plane.normal);
	//平面の原点距離を原点を減算して、平面と球の中心との距離が出る
	float	dist = distV - plane.distance;
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist)>sphere.radius)
	{
		return false;
	}
	//疑似交点を計算
	if (inter)
	{
		//平面上の最近接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}
