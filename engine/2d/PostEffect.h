#pragma once
#include<DirectXMath.h>
#include<DirectXCommon.h>

class PostEffect
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://�ÓI�����o�֐�
	static void StaticInitialize(DirectXCommon* dxCommon_);
	

private://�ÓI�����o�ϐ�
// �ˉe�s��v�Z
	static	XMMATRIX	matProjection;
	//SRV�̍ő喇��
	static const size_t	maxSRVCount = 2056;
	//DirectXCommon
	static	DirectXCommon* dxCommon;
public://�����o�֐�
	//������
	void	Initialize();
	void Update();
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);


public://�Q�b�^�[�A�Z�b�^�[
	//���W
	void	SetPosition(const XMFLOAT2& position_);
	const XMFLOAT2& GetPosition()const { return position; }
	//�F
	void	SetColor(const XMFLOAT4& color_);
	const XMFLOAT4& GetColor()const { return color; }
	//�T�C�Y
	void	SetSize(const XMFLOAT2& size_);
	const XMFLOAT2& GetSize()const { return size; }

private://�\����
	//���_�f�[�^
	struct Vertex {
		XMFLOAT3	pos;//xyz���W
	};

	//�萔�o�b�t�@�p�f�[�^�i�}�e���A���j
	struct ConstBufferData {
		XMFLOAT4	color;//�F�iRGB�j
		XMMATRIX	mat;//3D�ϊ��s��
	};

private:
	
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>comList;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW	ibView{};
	ID3D12Resource* vertBuff = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC	srvHeapDesc = {};
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

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
	
	//3D�ϊ��s��
	XMMATRIX	matWorld;
};

