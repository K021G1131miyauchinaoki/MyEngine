#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<forward_list>
#include"Camera.h"

/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
//class Object3d
class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	//struct VertexPosNormalUv
	//{
	//	XMFLOAT3 pos; // xyz���W
	//	XMFLOAT3 normal; // �@���x�N�g��
	//	XMFLOAT2 uv;  // uv���W
	//};
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float	scale;//�X�P�[��
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX	matBillboard;//�r���{�[�h�s��
	};

	//�p�[�e�B�N��1��
	struct Particle
	{
		//directx���ȗ�
		using	XMFLOAT3 = DirectX::XMFLOAT3;
		//���W
		XMFLOAT3	position = {};
		//���x
		XMFLOAT3	velocity = {};
		//�����x
		XMFLOAT3	accel = {};
		//���݃t���[��
		int	frame = 0;
		//�I���t���[��
		int	num_frame = 0;
		//�X�P�[��
		float	scale = 1.0f;
		//�����l
		float	s_scale = 1.0f;
		//�ŏI�l
		float	e_scale = 1.0f;
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	//static const int vertexCount = planeCount * 3;		// ���_��
	//static const int vertexCount = 1;		// ���_��
	static const int vertexCount = 1024;		// ���_��
	//static const int	indexCount = 3 * 2;//�C���f�b�N�X��
	static const int	indexCount = 1;//�C���f�b�N�X��

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	static void StaticInitialize(ID3D12Device* device,Camera*camera_ );

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create(uint32_t texIndex_);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTexture(uint32_t index, std::string fileName);

	//�e�N�X�`���ԍ�
	void	SetTexIndex(const uint32_t texIndex_) { texIndex = texIndex_; }
	const	uint32_t& GetTexIndex()const { return texIndex; }

private: // �ÓI�����o�ϐ�
	//SRV�̍ő喇��
	static const size_t	maxSRVCount = 2048;
	static	UINT	incrementSize;
	static	std::string defaultTextureDirectoryPath;
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ID3D12DescriptorHeap* descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	static	ComPtr<ID3D12Resource>texBuffers[maxSRVCount];
	//static ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	//static D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	static VertexPos vertices[vertexCount];
	//�J����
	static Camera* camera;
private:// �ÓI�����o�֐�
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// �J����������
	/// </summary>
	static void InitializeCamera(Camera* camera_);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

	static void SetTextureCommands(uint32_t index);

	

public: // �����o�֐�
	bool Initialize(uint32_t texIndex_);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void	Add(int life, XMFLOAT3	position, XMFLOAT3	velocity, XMFLOAT3	accel
		, float	start_scale, float	end_scale);


	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	//const XMFLOAT3& GetPosition() const { return position; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	//void SetPosition(const XMFLOAT3& position) { this->position = position; }

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// �F
	//XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	//XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	//XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	//XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	//ParticleManager* parent = nullptr;
	//�r���{�[�h�s��
	static	XMMATRIX	matBillboard;
	//Y������̃r���{�[�h�s��
	static	XMMATRIX	matBillboardY;

	//�p�[�e�B�N���z��
	std::forward_list<Particle>particles;
	//�e�N�X�`���̔ԍ�
	uint32_t texIndex = 0;
};

