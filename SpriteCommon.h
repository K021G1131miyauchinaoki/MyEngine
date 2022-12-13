#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>

class SpriteCommon
{
public://メンバ関数
	//初期化
	void	Initialize(DirectXCommon* ditectXCom_);
	//ゲッター

	DirectXCommon* GetdxCom()const { return directXCom; }
public://静的メンバ関数

	ID3D12PipelineState* GetPipelineState()const { return pipelineState; }
	ID3D12RootSignature* GetRootSignature()const { return rootSignature; }
private://メンバ変数
	DirectXCommon* directXCom;
	ID3D12Device* device;
	HRESULT	result;
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12RootSignature* rootSignature;
};