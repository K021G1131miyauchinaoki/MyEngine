#pragma once
#include"SpriteCommon.h"

class Sprite
{
public://メンバ関数
	//初期化
	void	Initialize(SpriteCommon* spriteCommon_);

private:
	SpriteCommon* spriteCommon;

};

