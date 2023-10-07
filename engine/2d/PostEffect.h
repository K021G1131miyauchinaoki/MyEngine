#pragma once
#include<DirectXMath.h>
#include<DirectXCommon.h>
#include<wrl.h>
#include<Vector2.h>
#include<Vector3.h>

class PostEffect
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
	void Initialize();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// テクスチャ生成
	/// </summary>
	void CreateTex();

	/// <summary>
	/// SRV生成
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// RTV生成
	/// </summary>
	void CreateRTV();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// DSV生成
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// 頂点バッファ生成
	/// </summary>
	void CreateVertexBuffer();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

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
		DirectX::XMFLOAT3	pos;//xyz座標
		DirectX::XMFLOAT2	uv;//uv座標
	};

	enum VertNum
	{
		LB,//左下
		LT,//左上
		RB,//右下
		RT,//右上
	};

	//定数バッファ用データ（マテリアル）
	struct ConstBufferData {
		DirectX::XMFLOAT4	color;//色（RGB）
		DirectX::XMMATRIX	mat;//3D変換行列
	};
private:
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;
	//定数バッファビュー
	ConstBufferData* constMap = nullptr;
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
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
	Vertex vertices_[4];
	//カラー
	DirectX::XMFLOAT4	color = { 1,1,1,1 };
	HRESULT result_;
};