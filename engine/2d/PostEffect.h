#pragma once
#include<DirectXMath.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"SpriteCommon.h"
#include<wrl.h>
#include<Vector2.h>
#include<Vector3.h>
#include<Input.h>

class PostEffect
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
	void Initialize();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, Input* input_);

	/// <summary>
	/// �e�N�X�`������
	/// </summary>
	void CreateTex();
	
	/// <summary>
	/// SRV����
	/// </summary>
	void CreateSRV();
	
	/// <summary>
	/// RTV����
	/// </summary>
	void CreateRTV();
	
	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepth();
	
	/// <summary>
	/// DSV����
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// ���_�o�b�t�@����
	/// </summary>
	void CreateVertexBuffer();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

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

	enum VertNum
	{
		LB,//����
		LT,//����
		RB,//�E��
		RT,//�E��
	};

	//�萔�o�b�t�@�p�f�[�^�i�}�e���A���j
	struct ConstBufferData {
		XMFLOAT4	color;//�F�iRGB�j
		XMMATRIX	mat;//3D�ϊ��s��
	};
private:
	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;
	//�萔�o�b�t�@�r���[
	ConstBufferData* constMap = nullptr;
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff[2];
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
	//�O���t�B�b�N�X�p�C�v���C��
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
	Vertex vertices_[4];
	//�J���[
	XMFLOAT4	color = { 1,1,1,1 };
	HRESULT result_;
};