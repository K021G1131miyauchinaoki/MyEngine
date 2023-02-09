#include "Collision.h"

using namespace DirectX;

bool Collision::CheckAphere2Plane(const Sphere& sphere, const Plane& plane, Vector3*inter) {
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = Vector3::StaticDot(sphere.center, plane.normal);
	//���ʂ̌��_���������_�����Z���āA���ʂƋ��̒��S�Ƃ̋������o��
	float	dist = distV - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist)>sphere.radius)
	{
		return false;
	}
	//�^����_���v�Z
	if (inter)
	{
		//���ʏ�̍ŋߐړ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}
