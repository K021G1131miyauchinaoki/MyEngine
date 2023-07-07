#pragma once
#include <imgui.h>
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <tchar.h>
#include"WinApp.h"
#include"DirectXCommon.h"

class ImguiManager
{
public://�����o�֐�
	//������
	void Initialize(WinApp* winApp_, DirectXCommon* dxCom_);
	//�I��
	void Finalize();
	//��t�J�n
	void Begin();
	//��t�I��
	void End();
	//�`��
	void Draw();
public://�ÓI�����o�֐�
private://�����o�ϐ�
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvHeap;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCom = nullptr;
	HRESULT result = NULL;
};

