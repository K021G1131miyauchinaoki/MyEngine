/**
 * @file Easing.h
 * @brief イージング処理
 */

#pragma once
namespace Easing
{
	float easeInSine(float num);
	float easeOutSine(float num);
	float easeOutBack(float num);
	float easeOutCubic(float x);
	float easeInCubic(float x);
	float easeOutQuint(float x);
	float easeOutCirc(float x);
	float easeInOutCirc(float x);
	float easeInOutSine(float x);
	float easeOutBounce(float x);
	
}