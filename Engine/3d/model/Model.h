/**
 * @file Model.h
 * @brief　モデル生成
 */
#pragma once
#include"TextureManager.h"
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cassert>
#include<unordered_map>
#pragma warning( push )
#pragma warning(disable:4061)
#pragma warning(disable:4265)
#pragma warning(disable:4365)
#pragma warning(disable:4514)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4828)
#pragma warning(disable:5204)
#pragma warning(disable:5220)
#include<DirectXMath.h>
#include<Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#pragma warning( pop)

using namespace DirectX;
using namespace Microsoft::WRL;
namespace MyEngin
{
	class Model
	{
	public://静的メンバ関数
		//obj読み込み
		static Model* LoadFromOBJ(const	std::string& modelname,bool smoothing_ = false);
		//セッター
		static void SetDevice(ID3D12Device* device_);

		static void Finalize();
	public:
		//描画
		void	Draw(ID3D12GraphicsCommandList* cmdList_,UINT rootParamIndexMaterial_);
	private://メンバ関数
		//obj読み込み
		void LoadFromOBJInternal(const	std::string& modelname_,bool smoothing_);
		//テクスチャ
		void LoadTexture(const	std::string& directoryPath_,const std::string& filename_);
		//マテリアル
		void LoadMaterial(const std::string& directoryPath_,const std::string& filename_);
		//デスクリプタヒープの初期化
		void InitializeDescriptorHeap();
		//バッファ生成
		void CreateBuffers();
		//エッジ平滑化データの追加
		void AddSmoothData(unsigned short indexPosition,unsigned short indexVertex);
		//平滑化された頂点法線の計算
		void CalculateSmoothedVertexNormals();
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
				ambient = { 0.3f,0.3f, 0.3f };
				diffuse = { 0.0f,0.0f, 0.0f };
				specular = { 0.0f,0.0f, 0.0f };
				alpha = 1.0f;
			}
		};
	private:
		//デバイス
		static ComPtr<ID3D12Device> device;
	private://メンバ変数
		//頂点法線スムージング用データ
		std::unordered_map<unsigned short,std::vector<unsigned short>>smoothData;
		//頂点インデックス配列
		std::vector<VertexPosNormalUv>vertices;
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
		// デスクリプタサイズ
		UINT descriptorHandleIncrementSize;
		int	iBufferSize;
		std::vector<unsigned short>indices;
		TextureData textureData;
	};
}