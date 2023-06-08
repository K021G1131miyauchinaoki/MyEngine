#include "FbxObject3d.h"

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* FbxObject3d::device = nullptr;
Camera* FbxObject3d::camera = nullptr;

void FbxObject3d::Initialize() {
	HRESULT result;
	//�萔�o�b�t�@�̐���

	CD3DX12_HEAP_PROPERTIES heap =CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	result = device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
}