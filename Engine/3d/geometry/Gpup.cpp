#include "Gpup.h"
#include <d3dcompiler.h>
#include<DirectXCommon.h>
#include<d3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")
void Gpup::Initialize(size_t MAX_PARTICLE_,DirectXCommon* dxCom_)
{
	HRESULT result = S_FALSE;
	device = dxCom_->GetDevice();
	cmdList = dxCom_->GetCommandList();
	emitData.maxParticle = (int32_t)MAX_PARTICLE_;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 10; // シェーダーリソースビュー10つ
	result = device->CreateDescriptorHeap(&descHeapDesc,IID_PPV_ARGS(&descHeap));//生成

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = descHeap->GetGPUDescriptorHandleForHeapStart();

	size_t byteSize = MAX_PARTICLE_ * sizeof(int32_t);
	const uint32_t alignment = 4096;
	size_t countBufferOffset =( (uint32_t)byteSize + ( alignment - 1 ) ) & ~( alignment - 1 );

	// ヒーププロパティを生成	
	D3D12_HEAP_PROPERTIES heapPro = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//リソース設定
	D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(int32_t) +countBufferOffset,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	//drawListの生成
	result = device->CreateCommittedResource(
			&heapPro,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&drawList));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = (UINT)MAX_PARTICLE_;
	uavDesc.Buffer.StructureByteStride = sizeof(int32_t);
	uavDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
	uavDesc.Buffer.Flags=D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.ViewDimension=D3D12_UAV_DIMENSION_BUFFER;

	device->CreateUnorderedAccessView(drawList.Get(),drawList.Get(),&uavDesc,cpuHandle);

	gpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = (UINT)MAX_PARTICLE_;
	srvDesc.Buffer.StructureByteStride = sizeof(int32_t);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

	//freeListの生成
	result = device->CreateCommittedResource(
			&heapPro,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&freeList));
	device->CreateUnorderedAccessView(freeList.Get(),freeList.Get(),&uavDesc,cpuHandle);
	freeListGpuHandle = gpuHandle;
	gpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	{
	//リソース設定
		D3D12_RESOURCE_DESC resDesc2 = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Particle)*MAX_PARTICLE_,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		//particleListの生成
		result = device->CreateCommittedResource(
				&heapPro,
				D3D12_HEAP_FLAG_NONE,
				&resDesc2,
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
				nullptr,
				IID_PPV_ARGS(&particlePool));

		D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
		lUavResDesc.Format = DXGI_FORMAT_UNKNOWN;
		lUavResDesc.Buffer.NumElements = ( UINT ) MAX_PARTICLE_;
		lUavResDesc.Buffer.StructureByteStride = sizeof(Particle);
		lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		lUavResDesc.Buffer.FirstElement = 0;
		lUavResDesc.Buffer.CounterOffsetInBytes = 0;

		device->CreateUnorderedAccessView(particlePool.Get(),nullptr,&lUavResDesc,cpuHandle);

		particlePoolHandle = gpuHandle;
		gpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(( sizeof(EmitData) + 0xff ) & ~0xff);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&BuffEmitData));
	assert(SUCCEEDED(result));

	resourceDesc =CD3DX12_RESOURCE_DESC::Buffer(( sizeof(EmitData) + 0xff ) & ~0xff);
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&BuffEmitData));
	assert(SUCCEEDED(result));

	ConstMapEmit();
	CreateFreePipeline();
	CreateEmitPipeline();
	ID3D12DescriptorHeap* descriptorHeaps[ ] = { descHeap.Get() };

	cmdList->SetDescriptorHeaps(_countof(descriptorHeaps),descriptorHeaps);
	cmdList->SetPipelineState(freePipelineState.Get());
	cmdList->SetComputeRootSignature(freeRootSignature.Get());

	cmdList->SetComputeRootDescriptorTable(1,freeListGpuHandle);
	cmdList->SetComputeRootConstantBufferView(0,BuffEmitData->GetGPUVirtualAddress());

	cmdList->Dispatch(static_cast< UINT >( emitData.maxParticle / 1024 ) + 1,1,1);
}

void Gpup::Updata(float deltaTime_)
{
	emitData.deltaTime = deltaTime_;
	ConstMapEmit();
	if (emitTimeCounter>=timeBetweenEmit)
	{
		emitTimeCounter = 0.0f;
		ID3D12DescriptorHeap* descriptorHeaps[ ] = { descHeap.Get() };
		cmdList->SetPipelineState(emitPipelineState.Get());
		cmdList->SetComputeRootSignature(emitRootSignature.Get());

		cmdList->SetDescriptorHeaps(_countof(descriptorHeaps),descriptorHeaps);

		cmdList->SetComputeRootConstantBufferView(0,BuffEmitData->GetGPUVirtualAddress());
		cmdList->SetComputeRootDescriptorTable(1,particlePoolHandle);
		cmdList->SetComputeRootDescriptorTable(2,freeListGpuHandle);

		cmdList->Dispatch(static_cast< UINT >( emitData.maxParticle / 1024 ) + 1,1,1);
	}
	else
	{
		emitTimeCounter = deltaTime_;
	}

}

void Gpup::Draw()
{
}

void Gpup::CreateFreePipeline()
{
	HRESULT result;
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineDesc = {};
	
	Microsoft::WRL::ComPtr<ID3DBlob> csBlob; 
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleCS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","cs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&csBlob,&errorBlob);
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// グラフィックスパイプラインの流れを設定
	D3D12_COMPUTE_PIPELINE_STATE_DESC cpipeline{};
	cpipeline.CS = CD3DX12_SHADER_BYTECODE(csBlob.Get());

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,1,0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[ 2 ];
	rootparams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 1 ].InitAsDescriptorTable(1,&descRangeSRV,D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),rootparams,1,&samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,&errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&freeRootSignature));
	assert(SUCCEEDED(result));

	//シグネチャをセット
	cpipeline.pRootSignature = freeRootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateComputePipelineState(&cpipeline,IID_PPV_ARGS(&freePipelineState));
	assert(SUCCEEDED(result));
}

void Gpup::CreateEmitPipeline()
{
	HRESULT result;
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineDesc = {};

	Microsoft::WRL::ComPtr<ID3DBlob> csBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/EmitCS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","cs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&csBlob,&errorBlob);
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// グラフィックスパイプラインの流れを設定
	D3D12_COMPUTE_PIPELINE_STATE_DESC cpipeline{};
	cpipeline.CS = CD3DX12_SHADER_BYTECODE(csBlob.Get());

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeUAV;
	descRangeUAV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,1,0); // t0 レジスタ

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeUAV2;
	descRangeUAV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,1,1); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[ 3 ];
	rootparams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 1 ].InitAsDescriptorTable(1,&descRangeUAV,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 2 ].InitAsDescriptorTable(1,&descRangeUAV2,D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),rootparams,0,&samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,&errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&emitRootSignature));
	assert(SUCCEEDED(result));

	//シグネチャをセット
	cpipeline.pRootSignature = emitRootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateComputePipelineState(&cpipeline,IID_PPV_ARGS(&emitPipelineState));
	assert(SUCCEEDED(result));
}

void Gpup::CreateParticlePipeline()
{
	
}

void Gpup::ConstMapEmit()
{
	HRESULT result;

	// 定数バッファへデータ転送
	EmitData* constMap = nullptr;
	result = BuffEmitData->Map(0,nullptr,( void** ) &constMap);
	constMap->pos = emitData.pos;
	constMap->lifeTime = emitData.lifeTime;
	constMap->accel = emitData.accel;
	constMap->scale = emitData.scale;
	constMap->startColor = emitData.startColor;
	constMap->endColor = emitData.endColor;
	constMap->velocity = emitData.velocity;
	constMap->deltaTime = emitData.deltaTime;
	constMap->maxParticle = emitData.maxParticle;
	BuffEmitData->Unmap(0,nullptr);
}
