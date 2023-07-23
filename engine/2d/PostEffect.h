#pragma once
#include<DirectXMath.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"SpriteCommon.h"
#include<wrl.h>

class PostEffect:public Sprite
{
public: // 静的メンバ関数
	static void	StaticInitialize(DirectXCommon* dxCommon_);
private: // 静的メンバ変数
	static DirectXCommon* dxCommon;
	static const float clearColor[4];

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteCommon* spriteCommon_);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// テクスチャ初期化
	/// </summary>
	void CreateTex();
	
	/// <summary>
	/// SRV初期化
	/// </summary>
	void CreateSRV();
	
	/// <summary>
	/// RTV初期化
	/// </summary>
	void CreateRTV();
	
	/// <summary>
	/// 深度バッファ初期化
	/// </summary>
	void CreateDepth();
	
	/// <summary>
	/// DSV初期化
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// シーン描画前
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList_);

	/// <summary>
	/// シーン描画後
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList_);
private://構造体
//頂点データ
	struct Vertex {
		XMFLOAT3	pos;//xyz座標
		XMFLOAT2	uv;//uv座標
	};

private:
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
	//RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;

	HRESULT result_;
};