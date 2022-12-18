#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
using namespace DirectX;

class Sprite
{
public://�����o�֐�
	//������
	void	Initialize(SpriteCommon* spriteCommon_);
	//�`��
	void Draw();
private://�\����
	//���_�f�[�^
	struct Vertex {
		XMFLOAT3	pos;//xyz���W
		XMFLOAT2	uv;//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^
	struct ConstBufferDataMaterial {
		XMFLOAT4	color;//�F�iRGB�j
	};


private:
	SpriteCommon* spriteCommon;
	DirectXCommon* directXCom;
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW	ibView{};
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBffMarerial = nullptr;
};

