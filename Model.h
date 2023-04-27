#pragma once
#include<Windows.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cassert>
#include<DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class Model
{
public://�ÓI�����o�֐�
	//obj�ǂݍ���
	static Model*LoadFromOBJ(const	std::string& modelname);
	//�Z�b�^�[
	static void SetDevice(ID3D12Device* device_) { Model::device = device_; }
	//�`��
	void	Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParamIndexMaterial);
private://�����o�֐�
	//obj�ǂݍ���
	void LoadFromOBJInternal(const	std::string& modelname);
	//�e�N�X�`��
	void LoadTexture(const	std::string& directoryPath, const std::string& filename);
	//�}�e���A��
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//�f�X�N���v�^�q�[�v�̏�����
	void InitializeDescriptorHeap();
	//�o�b�t�@����
	void CreateBuffers();
public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//�A���r�G���g�W��
		float pad1;		 //�p�f�B���O
		XMFLOAT3 diffuse;//�f�B�t���[�Y�W��
		float pad2;		 //�p�f�B���O
		XMFLOAT3 specular;//�X�y�L�����[�W��
		float alpha;	 //�A���t�@
	};

	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		XMFLOAT3 ambient;//�A���r�G���g�e���x
		XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;//�X�y�L�����[�e���x
		float alpha;//�A���t�@
		std::string textureFilename;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
private:
	//�f�o�C�X
	static	ID3D12Device* device;
private://�����o�ϐ�
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short>indices;
	std::vector<VertexPosNormalUv>vertices;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	//�}�e���A��
	Material	material;
	//ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@
};