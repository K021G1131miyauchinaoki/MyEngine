#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>

class SpriteCommon
{
public://メンバ関数
	//初期化
	void	Initialize(DirectXCommon* ditectXCom_);
	DirectXCommon* GetdxCom()const { return directXCom; }
private://メンバ変数
	DirectXCommon* directXCom;
	HRESULT	result;
};