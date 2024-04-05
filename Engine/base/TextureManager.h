#pragma once
#pragma warning( push )
#pragma warning(disable:4828)
#include <DirectXTex.h>
#pragma warning(pop)
#include <DirectXCommon.h>

using namespace MyEngin;

/// <summary>
/// テクスチャデータ
/// </summary>
struct TextureData
{
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//GPUデスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle{};
	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	int8_t pad1[ 4 ];

	std::string filePath;

	// 横幅
	size_t width = 0;
	// 縦幅
	size_t height = 0;
};


/**
 * @class TextureManager.h
 * @brief テクスチャ関連のクラス
 */

/// <summary>
/// テクスチャマネージャ
/// </summary>
class TextureManager
{
public: // 定数
	static const size_t MaxSRVCount = 256; // テクスチャの最大枚数

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXCommon"></param>
	void Initialize(DirectXCommon* directXCommon);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="filename">テクスチャファイル名</param>
	TextureData LoadTexture(const std::string& fileName);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャハンドル</returns>
	static TextureData Load(const std::string& fileName);

	/// <summary>
	/// テクスチャバッファの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTexBuff(DirectX::TexMetadata& metadata,DirectX::ScratchImage& scratchImg);

	/// <summary>
	/// シェーダリソースビューの生成
	/// </summary>
	/// <param name="texbuff">テクスチャバッファ</param>
	/// <param name="metadata"></param>
	D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(ID3D12Resource* texBuff,DirectX::TexMetadata& metadata);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Delete();

	// インスタンス
	static TextureManager* GetInstance();

	// Getter
	ID3D12DescriptorHeap* GetSrvHeap();

private: // メンバ変数
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// テクスチャ数
	UINT texCount;
	int8_t pad1[ 4 ];

	//テクスチャデータコンテナ
	std::list<TextureData> textureDatas_;

	// デフォルトテクスチャ格納ディレクトリ
	static std::string DefaultTextureDirectoryPath;

	DirectXCommon* dxCommon_ = nullptr;
	static TextureManager* textureManager_;

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(const TextureManager&) = delete;
};

