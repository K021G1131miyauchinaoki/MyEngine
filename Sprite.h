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

public://�Q�b�^�[�A�Z�b�^�[
	//��]
	void	SetRotation(const float& rotation_) { rotation = rotation_; }
	const float& GetRotation()const { return rotation; }
	//���W
	void	SetPosition(const XMFLOAT2& position_) { position = position_; }
	const XMFLOAT2& GetPosition()const {return position; }
private://�\����
	//���_�f�[�^
	struct Vertex {
		XMFLOAT3	pos;//xyz���W
		XMFLOAT2	uv;//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�i�}�e���A���j
	struct ConstBufferData {
		XMFLOAT4	color;//�F�iRGB�j
		XMMATRIX	mat;//3D�ϊ��s��
	};

private:
	SpriteCommon* spriteCommon;
	DirectXCommon* directXCom;
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW	ibView{};
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	XMMATRIX	matWorld;
	float	rotation = 0;
	XMFLOAT2	position={-1.0f,1.0f};
};

