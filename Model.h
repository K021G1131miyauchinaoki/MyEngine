#pragma once
#include<Windows.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cassert>
#include<DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class Model
{
public://静的メンバ関数
	//obj読み込み
	static Model*LoadFromOBJ(const	std::string& modelname);
	//セッター
	static void SetDevice(ID3D12Device* device_) { Model::device = device_; }
	//描画
	void	Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParamIndexMaterial);
private://メンバ関数
	//obj読み込み
	void LoadFromOBJInternal(const	std::string& modelname);
	//テクスチャ
	void LoadTexture(const	std::string& directoryPath, const std::string& filename);
	//マテリアル
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//デスクリプタヒープの初期化
	void InitializeDescriptorHeap();
	//バッファ生成
	void CreateBuffers();
public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//アンビエント係数
		float pad1;		 //パディング
		XMFLOAT3 diffuse;//ディフューズ係数
		float pad2;		 //パディング
		XMFLOAT3 specular;//スペキュラー係数
		float alpha;	 //アルファ
	};

	//マテリアル
	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient;//アンビエント影響度
		XMFLOAT3 diffuse;//ディフューズ影響度
		XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//アルファ
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
private:
	//デバイス
	static	ID3D12Device* device;
private://メンバ変数
	//頂点インデックス配列
	std::vector<unsigned short>indices;
	std::vector<VertexPosNormalUv>vertices;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	//マテリアル
	Material	material;
	//ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
};