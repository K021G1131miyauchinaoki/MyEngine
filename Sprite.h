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
	void TransferVertices();

public://ゲッター、セッター
	//回転
	void	SetRotation(const float& rotation_);
	const float& GetRotation()const { return rotation; }
	//座標
	void	SetPosition(const XMFLOAT2& position_);
	const XMFLOAT2& GetPosition()const {return position; }
	//色
	void	SetColor(const XMFLOAT4& color_);
	const XMFLOAT4& GetColor()const { return color; }
	//サイズ
	void	SetSize(const XMFLOAT2& size_);
	const XMFLOAT2& GetSize()const { return size; }
	//アンカーポイント
	void	SetAnchorPoint(const XMFLOAT2& anchorPoint_);
	const XMFLOAT2& GetAnchorPoint()const { return anchorPoint; }
	
	/// 左右反転の設定
	void SetIsFlipX(bool isFlipX_);

	/// 上下反転の設定
	void SetIsFlipY(bool isFlipY_);

	void	
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
	ID3D12Resource* vertBuff = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	XMMATRIX	matWorld;
	float	rotation = 0;
	XMFLOAT2	position={-1.0f,1.0f};
	XMFLOAT4	color = { 1,1,1,1 };
	XMFLOAT2	size = { 100.0f,100.0f };
	XMFLOAT2	anchorPoint = { 0.0f,0.0f };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
};

