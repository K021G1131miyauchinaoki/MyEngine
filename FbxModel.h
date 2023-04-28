#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>

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

