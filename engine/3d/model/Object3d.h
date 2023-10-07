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
//#pragma warning(disable: 4820) // 警告 C4820 を無効にする
#include"Model.h"
#include"Camera.h"
#include"Vector3.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
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
	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device_);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList_);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();

	static void SetCamera(Camera* camera_);

	static void Finalize();

private: // 静的メンバ変数
	// デバイス
	static ComPtr<ID3D12Device> device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//カメラ
	static	Camera* camera;

private:// 静的メンバ関数
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

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

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position_);
	
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetRotation() const;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetRotation(const Vector3& rotation_);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetScale() const;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetScale(const Vector3& scale_);

	//モデルセッター
	void SetModel(Model* model_);

	//ペアレント設定
	void SetParent(Object3d* parent_);

	//カラー
	void SetColor(XMFLOAT4 color_);

private: // メンバ変数
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	Vector3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation = { 0,0,0 };
	// ローカル座標
	Vector3 position = { 0,0,0 };
	//モデル
	Model* model = nullptr;
	// 親オブジェクト
	Object3d* parent = nullptr;
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
};