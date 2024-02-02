/**
 * @file BillboardParticle.h
 * @brief　モデル操作
 */
#pragma once
#pragma warning(push)
#pragma warning(disable:4365)
#pragma warning(disable:4265)
#pragma warning(disable:4820)
#pragma warning(disable:4514)
#pragma warning(disable:4668)
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include<d3dx12.h>
#pragma warning(pop)
#include<memory>
#include"Camera.h"
#include"Vector3.h"
#include<forward_list>
namespace MyEngin
{
/// <summary>
/// ジオメトリ
/// </summary>
	class BillboardParticle
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
		struct VertexPos
		{
			XMFLOAT3 pos; // xyz座標
			float	scale;//スケール
			XMFLOAT4 color;	// 色 (RGBA)
		};
		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
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
			//色
			XMFLOAT4 color = { 1,1,1,1 };
			//色
			XMFLOAT4 startColor = { 1,1,1,1 };
			//終了色
			XMFLOAT4 endColor = { 1,1,1,1 };
			//現在フレーム
			int8_t	frame = 0;
			//終了フレーム
			int8_t	endFrame = 0;
			//スケール
			float	scale = 1.0f;
			//初期値
			float	startScale = 1.0f;
			//最終値
			float	endScale = 1.0f;
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
		static void StaticInitialize(ID3D12Device* device);

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
		static BillboardParticle* Create();

		//カメラ
		static void SetCamera(Camera* camera_) {
			camera = camera_;
		}

	private: // 静的メンバ変数
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
		static ComPtr<ID3D12DescriptorHeap> descHeap;
		// 頂点バッファ
		static ComPtr<ID3D12Resource> vertBuff;
		// インデックスバッファ
		//static ComPtr<ID3D12Resource> indexBuff;
		// テクスチャバッファ
		static ComPtr<ID3D12Resource> texbuff;
		// シェーダリソースビューのハンドル(CPU)
		static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(CPU)
		static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// ビュー行列
		static XMMATRIX matView;
		// 頂点バッファビュー
		static D3D12_VERTEX_BUFFER_VIEW vbView;
		// インデックスバッファビュー
		//static D3D12_INDEX_BUFFER_VIEW ibView;
		// 頂点データ配列
		static VertexPos vertices[ vertexCount ];
		// 頂点インデックス配列
		//static unsigned short indices[indexCount];

		//カメラ
		static	Camera* camera;

	private:// 静的メンバ関数
		/// <summary>
		/// デスクリプタヒープの初期化
		/// </summary>
		static void InitializeDescriptorHeap();

		/// <summary>
		/// グラフィックパイプライン生成
		/// </summary>
		/// <returns>成否</returns>
		static void InitializeGraphicsPipeline();

		/// <summary>
		/// テクスチャ読み込み
		/// </summary>
		static void LoadTexture();

		/// <summary>
		/// モデル作成
		/// </summary>
		static void CreateModel();

		/// <summary>
		/// ビュー行列を更新
		/// </summary>
		static void UpdateViewMatrix();

	public: // メンバ関数
		bool Initialize();
		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		void	Add(int8_t life,XMFLOAT3	position,XMFLOAT3	velocity,XMFLOAT3	accel
					,float	start_scale,float	end_scale,XMFLOAT4 startColor_ = { 1,1,1,1 },XMFLOAT4 endColor_ = { 1,1,1,1 });


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

		//ビルボード行列
		static	XMMATRIX	matBillboard;
		//Y軸周りのビルボード行列
		static	XMMATRIX	matBillboardY;

		//パーティクル配列
		std::forward_list<Particle>particles;

	};
}