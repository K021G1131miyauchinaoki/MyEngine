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
	//•`‰æ
	void Draw();
private:
	SpriteCommon* spriteCommon;
	DirectXCommon* directXCom;
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

