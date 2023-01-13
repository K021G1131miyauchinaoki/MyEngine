#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
#include<array>
using namespace DirectX;

class Sprite
{
private://�ÓI�����o�ϐ�
	// �ˉe�s��v�Z
	static	XMMATRIX	matProjection;
	//SRV�̍ő喇��
	static const size_t	maxSRVCount = 2056;
public://�����o�֐�
	//������
	void	Initialize(SpriteCommon* spriteCommon_, uint32_t texIndex_ = UINT32_MAX);
	//�`��
	void Draw();
	void Update();


public://�Q�b�^�[�A�Z�b�^�[
	//��]
	void	SetRotation(const float& rotation_);
	const float& GetRotation()const { return rotation; }
	//���W
	void	SetPosition(const XMFLOAT2& position_);
	const XMFLOAT2& GetPosition()const { return position; }
	//�F
	void	SetColor(const XMFLOAT4& color_);
	const XMFLOAT4& GetColor()const { return color; }
	//�T�C�Y
	void	SetSize(const XMFLOAT2& size_);
	const XMFLOAT2& GetSize()const { return size; }
	//�A���J�[�|�C���g
	void	SetAnchorPoint(const XMFLOAT2& anchorPoint_);
	const XMFLOAT2& GetAnchorPoint()const { return anchorPoint; }
	// ���E���]�̐ݒ�
	void SetIsFlipX(const	bool isFlipX_);
	// �㉺���]�̐ݒ�
	void SetIsFlipY(const	bool isFlipY_);
	//��\��
	void	SetIsInvisible(const	bool isInvisible_);
	//�e�N�X�`���ԍ�
	void	SetTexIndex(const uint32_t texIndex_) { texIndex = texIndex_; }
	const	uint32_t& GetTexIndex()const { return texIndex; }
	//�T�C�Y
	void	SetTexSize(const XMFLOAT2& texSize_);
	const XMFLOAT2& GetTexSize()const { return texSize; }
	//�T�C�Y
	void	SetTexLeftTop(const XMFLOAT2& texLeftTop_);
	const XMFLOAT2& GetTexLeftTop()const { return texLeftTop; }

private:
	//�e�N�X�`���T�C�Y���C���[�W�ɍ��킹��
	void	AdjustTexSize();

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
	ID3D12Resource* vertBuff = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	//��]
	float	rotation = 0;
	//���W
	XMFLOAT2	position = { -1.0f,1.0f };
	//�J���[
	XMFLOAT4	color = { 1,1,1,1 };
	//�T�C�Y
	XMFLOAT2	size = { 100.0f,100.0f };
	//�A���J�[�|�C���g
	XMFLOAT2	anchorPoint = { 0.0f,0.0f };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	//��\��
	bool	isInvisible = false;
	//3D�ϊ��s��
	XMMATRIX	matWorld;
	//�e�N�X�`���̔ԍ�
	uint32_t texIndex = 0;
	//�e�N�X�`��������W
	XMFLOAT2	texLeftTop = { 0.0f,0.0f };
	//�T�C�Y
	XMFLOAT2	texSize = { 256.0f,256.0f };

};