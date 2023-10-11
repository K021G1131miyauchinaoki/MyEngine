#pragma once
#include<DirectXMath.h>	
#include<d3dx12.h>
#include<wrl.h>

class Light
{
public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		DirectX::XMVECTOR lightv;	//方向を示すベクトル
		DirectX::XMFLOAT3 lightcolor;//ライトの色
	};
public://静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(ID3D12Device* device_);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	static Light* Create();

private://静的メンバ変数
	static ID3D12Device* device;
private://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファの転送
	/// </summary>
	void TransferConstBuffer();

public://メンバ関数
	/// <summary>
	/// ライト方向をセット
	/// </summary>
	void SetLightDir(const DirectX::XMVECTOR& lightdir_);

	/// <summary>
	/// ライト色をセット
	/// </summary>
	void SetLightColor(const DirectX::XMFLOAT3& lightcolor_);

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList*cmdList,UINT rootParameterindex);

private://メンバ変数
	//ライト光線方向(単位ベクトル)
	DirectX::XMVECTOR lightdir = { 1,0,0,0 };
	//ライト色
	DirectX::XMFLOAT3 lightcolor = { 1,1,1 };
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>constBuff;
	//ダーティフラグ
	bool dirty = false;

	HRESULT result;
};

