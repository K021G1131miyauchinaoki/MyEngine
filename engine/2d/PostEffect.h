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

private:
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
};