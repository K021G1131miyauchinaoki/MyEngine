#pragma once
#include<Vector3.h>
#include<DirectXMath.h>

struct BaseGrain
{
	//���W
	Vector3 pos = {};
	//���x
	Vector3 velocity = {};
	//�����x
	Vector3 accel = {};
	//���݃t���[��
	int32_t stratFrame = 0;
	//�I���t���[��
	int32_t endFrame = 0;
	//�X�P�[��
	float scale = 1.0f;
	//�����l
	float stratScale = 1.0f;
	//�ŏI�l
	float endScale = 0.0f;
};

