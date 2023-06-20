#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>

struct Node
{
	//名前
	std::string name;
	//----------------------自作クラスに置き換えたい	
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };		
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX glabalTransform;
	//-----------------------------------------------
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node>nodes;

private://エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFloat2 = DirectX::XMFLOAT2;
	using XMFloat3 = DirectX::XMFLOAT3;
	using XMFloat4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	//std::を省略
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://関数
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode->glabalTransform; }

private:
	//頂点バッファ
	ComPtr<ID3D12Resource>vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource>indexBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource>texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;


public://サブクラス
	//頂点データ構造体
	struct VertexPosNormalUV
	{
		DirectX::XMFLOAT3 pos;//xyz座標
		DirectX::XMFLOAT3 normal;//法線ベクトル
		DirectX::XMFLOAT2 uv;//uv座標
	};
	
	//メッシュを持つノード
	Node* meshNode = nullptr;
	
	//頂点データ配列
	std::vector<VertexPosNormalUV>vertices;
	
	//頂点インデックス配列
	std::vector<unsigned  short>indices;

	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	

};

