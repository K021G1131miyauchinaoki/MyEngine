/**
 * @file BaseHp.h
 * @brief 体力
 */
#pragma once
#include<Sprite.h>
#include<memory>

struct BaseHp
{
	int8_t value;
	bool isDead = false;

}; 
struct DrawHp
{
	bool isDraw = false;
	std::unique_ptr<Sprite> sprite;
};
