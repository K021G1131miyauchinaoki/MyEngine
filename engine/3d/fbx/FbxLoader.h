/**
 * @file FbxLoader.h
 * @brief　FBXを読み込む
 */
#pragma once
#pragma warning( push )
#pragma warning( disable : 4324 )
#pragma warning( disable : 4828 )
#include<fbxsdk.h>
#include <d3d12.h>
#include <d3dx12.h>
#pragma warning( pop)
#include"FbxModel.h"
#include<wrl.h>
#include<string>

class FbxLoader
{
private://エイリアス
	//std::を省略
	using string = std::string;
public://定数
	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み</param>
	/// <param name="src">書き込み</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst_, const FbxAMatrix& src_);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj_) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj_) = delete;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	void Initialize(ID3D12Device* device_);
	
	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();
	
	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	FbxModel* LoadModelFromFile(const string& modelName_);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	/// <param name="parent">親ノード</param>
	void ParseNodeRecursive(FbxModel* model_, FbxNode* fbxNode_,Node*parent_=nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* model_, FbxNode* fbxNode_);

	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* model_, FbxMesh* fbxMesh_);
	//面情報読み取り
	void ParseMeshFaces(FbxModel* model_, FbxMesh* fbxMesh_);
	//マテリアル読み取り
	void ParseMeshMaterial(FbxModel* model_, FbxNode* fbxNode_);
	//テクスチャ読み取り
	void LoadTexture(FbxModel* model_, const std::string& fullpath_);
	//スキニング情報の読み取り
	void ParseSkin(FbxModel* model_, FbxMesh* fbxMesh_);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path_);

private:
	//d3d12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	FbxImporter* fbxImporter = nullptr;
};