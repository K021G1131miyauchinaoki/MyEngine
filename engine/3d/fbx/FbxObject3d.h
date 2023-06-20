#pragma once

#include"FbxModel.h"
#include"Camera.h"

#include<Windows.h>
#include<wrl.h>
#include<d3dx12.h>
#include <d3d12.h>
#include<DirectXMath.h>
#include<string>

class FbxObject3d
{
protected://エイリアス
	//Microsoft::WRL::を省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;//ビュープロジェクション行列
		XMMATRIX world;//ワールド行列
		XMFLOAT3 cameraPos;//カメラ座標（ワールド座標）
	};
public://静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }
	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// /// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }


protected://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//x,y,z軸周りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	FbxModel* fbxModel = nullptr; 
};

