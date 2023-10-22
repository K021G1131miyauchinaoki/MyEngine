/**
 * @file Easing.cpp
 * @brief イージング処理
 */
#include "Easing.h"
#include<MyMath.h>
#include<cmath>

float Easing::easeInSine(float num){
	return 1 - std::cos((num * MyMath::PI_F) / 2);
}
float Easing::easeOutSine(float num){
	return std::sin((num * MyMath::PI_F) / 2);
}
float Easing::easeOutBack(float num) {
	const float c1 = 1.70158f;
	const float c2 = c1 + 1;

	return 1 +c2*pow(num-1.0f,3.0f)+c1 * pow(num - 1.0f, 2.0f);
}

float Easing::easeOutCubic(float x) {
	return 1 - (float)pow(1 - x, 3);
}

float Easing::easeOutQuint(float x) {
	return 1 - (float)pow(1 - x, 5);
}

float Easing::easeOutCirc(float x) {
return std::sqrt(1.0f - std::pow(x - 1.0f,2.0f));
}

float Easing::easeInOutCirc(float x) {
	return x < 0.5f
		? ( 1.0f - std::sqrt(1 - std::pow(2.0f * x,2.0f)) ) / 2.0f
		: ( std::sqrt(1.0f - std::pow(-2.0f * x + 2.0f,2.0f)) + 1.0f ) / 2.0f;
}

float Easing::easeInOutSine(float x){
	return -( std::cos(MyMath::PI_F * x) - 1.0f ) / 2.0f;
}

float Easing::easeOutBounce(float x) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if ( x < 1.0f / d1 )
	{
		return n1 * x * x;
	}
	else if ( x < 2.0f / d1 )
	{
		return n1 * ( x -= 1.0f / d1 ) * x + 0.75f;
	}
	else if ( x < 2.5f / d1 )
	{
		return n1 * ( x -= 2.25f / d1 ) * x + 0.9375f;
	}
	else
	{
		return n1 * ( x -= 2.625f / d1 ) * x + 0.984375f;
	}
}