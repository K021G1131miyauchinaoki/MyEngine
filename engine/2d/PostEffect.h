#pragma once
#include<DirectXMath.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"SpriteCommon.h"
#include<wrl.h>

class PostEffect:public Sprite
{
public: // �ÓI�����o�֐�
	static void	StaticInitialize(DirectXCommon* dxCommon_);
private: // �ÓI�����o�ϐ�
	static DirectXCommon* dxCommon;
	static const float clearColor[4];

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(SpriteCommon* spriteCommon_);

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �e�N�X�`��������
	/// </summary>
	void CreateTex();
	
	/// <summary>
	/// SRV������
	/// </summary>
	void CreateSRV();
	
	/// <summary>
	/// RTV������
	/// </summary>
	void CreateRTV();
	
	/// <summary>
	/// �[�x�o�b�t�@������
	/// </summary>
	void CreateDepth();
	
	/// <summary>
	/// DSV������
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// �V�[���`��O
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList_);

	/// <summary>
	/// �V�[���`���
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList_);
private://�\����
//���_�f�[�^
	struct Vertex {
		XMFLOAT3	pos;//xyz���W
		XMFLOAT2	uv;//uv���W
	};

private:
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;

	HRESULT result_;
};