#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>

struct Node
{
	//���O
	std::string name;
	//----------------------����N���X�ɒu����������	
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };		
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX glabalTransform;
	//-----------------------------------------------
	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//�t�����h�N���X
	friend class FbxLoader;
private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node>nodes;
public://�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUV
	{
		DirectX::XMFLOAT3 pos;//xyz���W
		DirectX::XMFLOAT3 normal;//�@���x�N�g��
		DirectX::XMFLOAT2 uv;//uv���W
	};
	
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUV>vertices;
	
	//���_�C���f�b�N�X�z��
	std::vector<unsigned  short>indices;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};
	
};

