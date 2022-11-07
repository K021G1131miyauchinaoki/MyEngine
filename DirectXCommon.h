#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<cassert>
#include<vector>
#include"WinApp.h"

#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")


using	namespace Microsoft::WRL;
#pragma once
class DirectXCommon
{
public://�����o�֐�
	//������
	void Initialize(WinApp* winApp_);
	//�f�o�C�X�̏�����
	void InitializeDevice();
	//�R�}���h�̏�����
	void InitializeCommand();
	//�X���b�v�`�F�[���̏�����
	void InitializeSwapchain();
	//�����_�[�^�[�Q�b�g�r���[
	void IntializeRenderTargetView();
	//�[�x�o�b�t�@�̏�����
	void InitializeDepthBuffer();
	//�t�F���X�̏�����
	void InitializeFence();
private:
	//DirectX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device>device;
	//DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory7>dxgiFactory;
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC	commandQueueDesc{};
	ComPtr < IDXGISwapChain4> swapChain = nullptr;
	ComPtr < ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> comList = nullptr;
	ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	//�X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1	swapChainDesc{};
private:
	WinApp* winApp = nullptr;
	HRESULT	result;
};

