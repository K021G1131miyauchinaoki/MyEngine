/**
 * @file BaseGrain.h
 * @brief パーティクルの一粒
 */

#pragma once
#include<Vector3.h>

struct BaseGrain
{
	//座標
	Vector3 pos = {};
	//速度
	Vector3 velocity = {};
	//加速度
	Vector3 accel = {};
	//現在フレーム
	int32_t stratFrame = 0;
	//終了フレーム
	int32_t endFrame = 0;
	//スケール
	float scale = 1.0f;
	//初期値
	float stratScale = 1.0f;
	//最終値
	float endScale = 0.0f;
};

