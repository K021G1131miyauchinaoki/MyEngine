#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
#include<array>
using namespace DirectX;

class Sprite
{
private://静的メンバ変数
	// 射影行列計算
	static	XMMATRIX	matProjection;
	//SRVの最大枚数
	static const size_t	maxSRVCount = 2056;
public://メンバ関数
	//初期化
	void	Initialize(SpriteCommon* spriteCommon_, uint32_t texIndex_ = UINT32_MAX);
	//描画
	void Draw();
	void Update();


public://ゲッター、セッター
	//回転
	void	SetRotation(const float& rotation_);
	const float& GetRotation()const { return rotation; }
	//座標
	void	SetPosition(const XMFLOAT2& position_);
	const XMFLOAT2& GetPosition()const { return position; }
	//色
	void	SetColor(const XMFLOAT4& color_);
	const XMFLOAT4& GetColor()const { return color; }
	//サイズ
	void	SetSize(const XMFLOAT2& size_);
	const XMFLOAT2& GetSize()const { return size; }
	//アンカーポイント
	void	SetAnchorPoint(const XMFLOAT2& anchorPoint_);
	const XMFLOAT2& GetAnchorPoint()const { return anchorPoint; }
	// 左右反転の設定
	void SetIsFlipX(const	bool isFlipX_);
	// 上下反転の設定
	void SetIsFlipY(const	bool isFlipY_);
	//非表示
	void	SetIsInvisible(const	bool isInvisible_);
	//テクスチャ番号
	void	SetTexIndex(const uint32_t texIndex_) { texIndex = texIndex_; }
	const	uint32_t& GetTexIndex()const { return texIndex; }
	//サイズ
	void	SetTexSize(const XMFLOAT2& texSize_);
	const XMFLOAT2& GetTexSize()const { return texSize; }
	//サイズ
	void	SetTexLeftTop(const XMFLOAT2& texLeftTop_);
	const XMFLOAT2& GetTexLeftTop()const { return texLeftTop; }

private:
	//テクスチャサイズをイメージに合わせる
	void	AdjustTexSize();

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
	//回転
	float	rotation = 0;
	//座標
	XMFLOAT2	position = { -1.0f,1.0f };
	//カラー
	XMFLOAT4	color = { 1,1,1,1 };
	//サイズ
	XMFLOAT2	size = { 100.0f,100.0f };
	//アンカーポイント
	XMFLOAT2	anchorPoint = { 0.0f,0.0f };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	//非表示
	bool	isInvisible = false;
	//3D変換行列
	XMMATRIX	matWorld;
	//テクスチャの番号
	uint32_t texIndex = 0;
	//テクスチャ左上座標
	XMFLOAT2	texLeftTop = { 0.0f,0.0f };
	//サイズ
	XMFLOAT2	texSize = { 256.0f,256.0f };

};