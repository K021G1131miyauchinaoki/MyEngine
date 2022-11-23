#pragma once
#define DIRECTINPUT_VERSION    0x0800 //DirectInput�̃o�[�W�����w��
#include<windows.h>
#include<cassert>
#include<vector>
#include<string>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<dinput.h>
#include"DirectXTex/DirectXTex.h"
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;//�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D�ϊ��s��
};

//3D�I�u�W�F�N�g�^
struct Object3d
{
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;

	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	//�e�I�u�W�F�N�g�ւ̃|�C���^
	Object3d* parent = nullptr;
};


//�������֐�
void InitializeObject3d(Object3d* object, ID3D12Device* device);
//�X�V�֐�
void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection);

//�`��֐�
void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);