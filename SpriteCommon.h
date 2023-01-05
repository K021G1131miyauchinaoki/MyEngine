#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>
#include<array>
#include<string>
#include<vector>
using namespace DirectX;

class SpriteCommon
{
private://静的メンバ変数
	//SRVの最大枚数
	static const size_t	maxSRVCount = 2048;
	static	std::string defaultTextureDirectoryPath;
public://メンバ関数
	//初期化
	void	Initialize(DirectXCommon* ditectXCom_);
	//テクスチャ読み込み
	void	Loadtexture(uint32_t index, std::string fileName);
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();

	//描画用テクスチャコマンド
	void	SetTextureCommands(uint32_t index);
	//ゲッター
	ID3D12DescriptorHeap* GetSrvHeap()const { return srvHeap; }

	DirectXCommon* GetdxCom()const { return directXCom; }

	ID3D12Resource* GetTexBuff(uint32_t index)const { return texBuffers[index].Get(); }
public://静的メンバ関数

	//ID3D12PipelineState* GetPipelineState()const { return pipelineState; }
	//ID3D12RootSignature* GetRootSignature()const { return rootSignature; }
private://メンバ変数
	DirectXCommon* directXCom;
	ID3D12Device* device;
	HRESULT	result;
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12RootSignature* rootSignature;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, maxSRVCount>texBuffers;
	UINT	incrementSize;

};