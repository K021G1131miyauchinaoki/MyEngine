#pragma once
#include"DirectXCommon.h"
#include<DirectXMath.h>
#include<array>
#include<string>
#include<vector>
using namespace DirectX;

class SpriteCommon
{
private://�ÓI�����o�ϐ�
	//SRV�̍ő喇��
	static const size_t	maxSRVCount = 2048;
	static	std::string defaultTextureDirectoryPath;
public://�����o�֐�
	//������
	void	Initialize(DirectXCommon* ditectXCom_);
	//�e�N�X�`���ǂݍ���
	void	Loadtexture(uint32_t index, std::string fileName);
	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//�`��p�e�N�X�`���R�}���h
	void	SetTextureCommands(uint32_t index);
	//�Q�b�^�[
	ID3D12DescriptorHeap* GetSrvHeap()const { return srvHeap; }

	DirectXCommon* GetdxCom()const { return directXCom; }

	ID3D12Resource* GetTexBuff(uint32_t index)const { return texBuffers[index].Get(); }
public://�ÓI�����o�֐�

	//ID3D12PipelineState* GetPipelineState()const { return pipelineState; }
	//ID3D12RootSignature* GetRootSignature()const { return rootSignature; }
private://�����o�ϐ�
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