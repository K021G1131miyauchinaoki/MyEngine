#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>

class SpriteCommon
{
public://�����o�֐�
	//������
	void	Initialize(DirectXCommon* ditectXCom_);
	DirectXCommon* GetdxCom()const { return directXCom; }
private://�����o�ϐ�
	DirectXCommon* directXCom;
	HRESULT	result;
};