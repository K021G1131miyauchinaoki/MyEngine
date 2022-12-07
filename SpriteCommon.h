#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>

class SpriteCommon
{
public://ƒƒ“ƒoŠÖ”
	//‰Šú‰»
	void	Initialize(DirectXCommon* ditectXCom_);
	DirectXCommon* GetdxCom()const { return directXCom; }
private://ƒƒ“ƒo•Ï”
	DirectXCommon* directXCom;
	HRESULT	result;
};