#pragma once
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_6.h>
#include<memory>
#include<DirectXMath.h>
#include<DirectXCommon.h>
using namespace MyEngin;

class Gpup
{
public:
	void Initialize(size_t MAX_PARTICLE_,DirectXCommon*dxCom_);
	void Updata(float deltaTime_);
	void Draw();
	void CreateFreePipeline();
	void CreateEmitPipeline();
	void CreateParticlePipeline();
	void ConstMapEmit();
public:
	//パーティクル1粒
	struct Particle
	{
		DirectX::XMFLOAT4 color;	// 色 (RGBA)
		DirectX::XMFLOAT3 pos; // xyz座標
		float age;
		DirectX::XMFLOAT3 velocity;
		float isAlive;
		DirectX::XMFLOAT3 accel;
		float scale;//スケール
		float lifeTime;
		DirectX::XMFLOAT3 pad;
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		DirectX::XMMATRIX mat;	// ３Ｄ変換行列
		DirectX::XMMATRIX matBillboard;//ビルボード行列
	};

	struct EmitData
	{
		//座標
		DirectX::XMFLOAT3	pos = {};
		//寿命
		float lifeTime;
		DirectX::XMFLOAT3 velocity;
		//加速度
		DirectX::XMFLOAT3	accel = {};
		//スケール
		float	scale = 1.0f;
		//色
		DirectX::XMFLOAT4 startColor = { 1,1,1,1 };
		//終了色
		DirectX::XMFLOAT4 endColor = { 1,1,1,1 };
		//経過時間
		float deltaTime;

		int32_t maxParticle;
	};

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;;
	Microsoft::WRL::ComPtr<ID3D12Resource>drawList;
	Microsoft::WRL::ComPtr<ID3D12Resource>freeList;
	Microsoft::WRL::ComPtr<ID3D12Resource>particlePool;
	//エミッター用
	Microsoft::WRL::ComPtr<ID3D12Resource>BuffEmitData;
	//ビルボード用
	Microsoft::WRL::ComPtr<ID3D12Resource>BuffBill;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>cmdList;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> freeRootSignature;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> emitRootSignature;
	// パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState>freePipelineState;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>emitPipelineState;

	D3D12_GPU_DESCRIPTOR_HANDLE freeListGpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE particlePoolHandle;

	ID3D12Device* device;
	
	//最大数
	size_t MAX_PARTICLE = 1024;

	EmitData emitData;

	float emitTimeCounter;//
	float timeBetweenEmit;//生成までの時間
};

