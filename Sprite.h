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

public://ゲッター、セッター
	//回転
	void	SetRotation(const float& rotation_) { rotation = rotation_; }
	const float& GetRotation()const { return rotation; }
	//座標
	void	SetPosition(const XMFLOAT2& position_) { position = position_; }
	const XMFLOAT2& GetPosition()const {return position; }
private://構造体
	//頂点データ
	struct Vertex {
		XMFLOAT3	pos;//xyz座標
		XMFLOAT2	uv;//uv座標
	};

	//定数バッファ用データ（マテリアル）
	struct ConstBufferData {
		XMFLOAT4	color;//色（RGB）
		XMMATRIX	mat;//3D変換行列
	};

private:
	SpriteCommon* spriteCommon;
	DirectXCommon* directXCom;
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW	ibView{};
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	XMMATRIX	matWorld;
	float	rotation = 0;
	XMFLOAT2	position={-1.0f,1.0f};
};

