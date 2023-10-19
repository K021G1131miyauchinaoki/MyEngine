/**
 * @file FbxModel.h
 * @brief　FBXのモデル生成
 */
#pragma once
#include<string>
#include<vector>
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#pragma warning( push )
#pragma warning( disable : 4324 )
#pragma warning( disable : 4828 )
#include<DirectXMath.h>
#include<DirectXTex.h>
#include<fbxsdk.h>
#pragma warning( pop)


struct Node
{
	//----------------------自作クラスに置き換えたい	
	//ローカルスケール
	alignas(16)DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//ローカル回転角
	alignas(16)DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	alignas(16)DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	alignas(16)DirectX::XMMATRIX transform;
	//グローバル変形行列
	alignas(16)DirectX::XMMATRIX glabalTransform;
	//-----------------------------------------------
	//親ノード
	Node* parent = nullptr;
	//名前
	std::string name;
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

public://定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

public://サブクラス
	//頂点データ構造体
	struct VertexPosNormalUVSkin
	{
		DirectX::XMFLOAT3 pos;//xyz座標
		DirectX::XMFLOAT3 normal;//法線ベクトル
		DirectX::XMFLOAT2 uv;//uv座標
		uint32_t boneIndex[MAX_BONE_INDICES];//ボーン　番号
		float boneWeight[MAX_BONE_INDICES];//ボーン　重み
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		Bone(const std::string& name) {
			this->name = name;
		}
	};

public://メンバ関数
	//デストラクタ
	~FbxModel();
	//バッファ生成
	void CreateBuffers(ID3D12Device* device_);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList_);
	//モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode->glabalTransform; }
	//-------getter------
	//ボーン
	std::vector<Bone>& GetBones() { return bones; }
	//FBXシーン
	FbxScene* GetFbxScene() { return fbxScene; }

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
public:
	
	//メッシュを持つノード
	Node* meshNode = nullptr;
	
	//頂点データ配列
	std::vector<VertexPosNormalUVSkin>vertices;
	
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

private://メンバ変数
	//ボーン配列
	std::vector<Bone>bones;
	//FBXシーン
	FbxScene*fbxScene=nullptr;
};

