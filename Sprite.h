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
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ID3D12Resource* constBffMarerial = nullptr;

};

