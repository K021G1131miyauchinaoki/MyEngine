#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>

class SpriteCommon
{
public://�����o�֐�
	//������
	void	Initialize(DirectXCommon* ditectXCom_);
	//�Q�b�^�[

	DirectXCommon* GetdxCom()const { return directXCom; }
public://�ÓI�����o�֐�

	ID3D12PipelineState* GetPipelineState()const { return pipelineState; }
	ID3D12RootSignature* GetRootSignature()const { return rootSignature; }
private://�����o�ϐ�
	DirectXCommon* directXCom;
	ID3D12Device* device;
	HRESULT	result;
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12RootSignature* rootSignature;
};