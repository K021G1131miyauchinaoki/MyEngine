#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
using namespace DirectX;

class Sprite
{
public://メンバ関数
	//初期化
	void	Initialize(SpriteCommon* spriteCommon_);
	//描画
	void Draw();
private://構造体
	//定数バッファ用データ
	struct ConstBufferDataMaterial {
		XMFLOAT4	color;//色（RGB）
	};


private:
	SpriteCommon* spriteCommon;
	DirectXCommon* directXCom;
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ID3D12Resource* constBffMarerial = nullptr;

};

