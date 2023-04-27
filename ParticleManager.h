#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<forward_list>
#include"Camera.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
//class Object3d
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	//struct VertexPosNormalUv
	//{
	//	XMFLOAT3 pos; // xyz座標
	//	XMFLOAT3 normal; // 法線ベクトル
	//	XMFLOAT2 uv;  // uv座標
	//};
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float	scale;//スケール
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX	matBillboard;//ビルボード行列
	};

	//パーティクル1粒
	struct Particle
	{
		//directxを省略
		using	XMFLOAT3 = DirectX::XMFLOAT3;
		//座標
		XMFLOAT3	position = {};
		//速度
		XMFLOAT3	velocity = {};
		//加速度
		XMFLOAT3	accel = {};
		//現在フレーム
		int	frame = 0;
		//終了フレーム
		int	num_frame = 0;
		//スケール
		float	scale = 1.0f;
		//初期値
		float	s_scale = 1.0f;
		//最終値
		float	e_scale = 1.0f;
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	//static const int vertexCount = planeCount * 3;		// 頂点数
	//static const int vertexCount = 1;		// 頂点数
	static const int vertexCount = 1024;		// 頂点数
	//static const int	indexCount = 3 * 2;//インデックス数
	static const int	indexCount = 1;//インデックス数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device,Camera*camera_ );

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create(uint32_t texIndex_);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static void LoadTexture(uint32_t index, std::string fileName);

	//テクスチャ番号
	void	SetTexIndex(const uint32_t texIndex_) { texIndex = texIndex_; }
	const	uint32_t& GetTexIndex()const { return texIndex; }

private: // 静的メンバ変数
	//SRVの最大枚数
	static const size_t	maxSRVCount = 2048;
	static	UINT	incrementSize;
	static	std::string defaultTextureDirectoryPath;
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ID3D12DescriptorHeap* descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	static	ComPtr<ID3D12Resource>texBuffers[maxSRVCount];
	//static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	//static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	static VertexPos vertices[vertexCount];
	//カメラ
	static Camera* camera;
private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// カメラ初期化
	/// </summary>
	static void InitializeCamera(Camera* camera_);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

	static void SetTextureCommands(uint32_t index);

	

public: // メンバ関数
	bool Initialize(uint32_t texIndex_);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void	Add(int life, XMFLOAT3	position, XMFLOAT3	velocity, XMFLOAT3	accel
		, float	start_scale, float	end_scale);


	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	//const XMFLOAT3& GetPosition() const { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	//void SetPosition(const XMFLOAT3& position) { this->position = position; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// 色
	//XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	//XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	//XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	//XMMATRIX matWorld;
	// 親オブジェクト
	//ParticleManager* parent = nullptr;
	//ビルボード行列
	static	XMMATRIX	matBillboard;
	//Y軸周りのビルボード行列
	static	XMMATRIX	matBillboardY;

	//パーティクル配列
	std::forward_list<Particle>particles;
	//テクスチャの番号
	uint32_t texIndex = 0;
};

