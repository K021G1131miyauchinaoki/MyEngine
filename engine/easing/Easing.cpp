#include "Easing.h"
#include<MyMath.h>
#include<cmath>

float Easing::easeInSine(float num){
	return 1 - std::cos((num * MyMath::PI_F) / 2);
}
float Easing::easeOutSine(float num){
	return 1-std::sin((num * MyMath::PI_F) / 2);
}
float Easing::easeOutBack(float num) {
	const float c1 = 1.70158f;
	const float c2 = c1 + 1;

	return 1 +c2*pow(num-1.0f,3.0f)+c1 * pow(num - 1.0f, 2.0f);
}

float Easing::easeOutCubic(float x) {
	return 1 - (float)pow(1 - x, 3);
}