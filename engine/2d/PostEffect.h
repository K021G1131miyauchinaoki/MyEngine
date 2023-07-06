#pragma once
#include<DirectXMath.h>
#include<DirectXCommon.h>

class PostEffect
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://静的メンバ関数
	static void StaticInitialize(DirectXCommon* dxCommon_);
	

private://静的メンバ変数
// 射影行列計算
	static	XMMATRIX	matProjection;
	//SRVの最大枚数
	static const size_t	maxSRVCount = 2056;
	//DirectXCommon
	static	DirectXCommon* dxCommon;
public://メンバ関数
	//初期化
	void	Initialize();
	void Update();
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);


public://ゲッター、セッター
	//座標
	void	SetPosition(const XMFLOAT2& position_);
	const XMFLOAT2& GetPosition()const { return position; }
	//色
	void	SetColor(const XMFLOAT4& color_);
	const XMFLOAT4& GetColor()const { return color; }
	//サイズ
	void	SetSize(const XMFLOAT2& size_);
	const XMFLOAT2& GetSize()const { return size; }

private://構造体
	//頂点データ
	struct Vertex {
		XMFLOAT3	pos;//xyz座標
	};

	//定数バッファ用データ（マテリアル）
	struct ConstBufferData {
		XMFLOAT4	color;//色（RGB）
		XMMATRIX	mat;//3D変換行列
	};

private:
	
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW	ibView{};
	ID3D12Resource* vertBuff = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	//回転
	float	rotation = 0;
	//座標
	XMFLOAT2	position = { -1.0f,1.0f };
	//カラー
	XMFLOAT4	color = { 1,1,1,1 };
	//サイズ
	XMFLOAT2	size = { 100.0f,100.0f };
	//アンカーポイント
	XMFLOAT2	anchorPoint = { 0.0f,0.0f };
	
	//3D変換行列
	XMMATRIX	matWorld;
};

