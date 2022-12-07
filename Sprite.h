#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
using namespace DirectX;

class Sprite
{
public://ƒƒ“ƒoŠÖ”
	//‰Šú‰»
	void	Initialize(SpriteCommon* spriteCommon_);

private:
	SpriteCommon* spriteCommon;
	DirectXCommon* directXCom;
	HRESULT result;
};

