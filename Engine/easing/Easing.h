/**
 * @file Easing.h
 * @brief イージング処理
 */

#pragma once
namespace Easing
{
	float easeInSine(float num);
	float easeOutSine(float num);
	float easeInOutSine(float x);

	float easeOutBack(float num);

	float easeInCubic(float x);
	float easeOutCubic(float x);
	float easeInOutCubic(float x);

	float easeInQuint(float x);
	float easeOutQuint(float x);

	float easeOutCirc(float x);
	float easeInOutCirc(float x);

	float easeOutBounce(float x);

	float easeInQuad(float x);
	float easeOutQuad(float x);
	
}