#include "PostEffect.h"
#include"DirectXTex.h"
#include<WinApp.h>
#include<d3dx12.h>
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

DirectXCommon* PostEffect::dxCommon = nullptr;
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

//�C���f�b�N�X�f�[�^
unsigned	short	indice[] = {
	0,1,2,
	1,2,3,
};

PostEffect::PostEffect() 
	:Sprite(
		100,
		{0,0},
		{500.0f,500.0f},
		{1,1,1,1},
		{0.0f,0.0f},
		false,
		false
)
{

}

void PostEffect::StaticInitialize(DirectXCommon* dxCommon_) {
	dxCommon = dxCommon_;
}

void PostEffect::Initialize(SpriteCommon* spriteCommon_) {
	

	//���N���X�Ƃ��Ă̏�����
	Sprite::Initialize(spriteCommon_);

	//InitializeTex();
	//InitializeSRV();
	//InitializeRTV();
	//InitializeDepth();
	//InitializeDSV();
	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::width,
		(int32_t)WinApp::height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	CD3DX12_HEAP_PROPERTIES texHeap = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);
	result_ = dxCommon->GetDevice()->CreateCommittedResource(
		&texHeap,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result_));

	{//�e�N�X�`����ԃN���A
		//��f��(1280*720=921600�s�N�Z��)
		const uint32_t pixelCount = WinApp::width * WinApp::height;
		//�摜1�s���̃f�[�^�T�C�Y
		const uint32_t rowPitch = sizeof(uint32_t) * WinApp::width;
		//�摜1�s���̃f�[�^�T�C�Y
		const uint32_t depthPitch = sizeof(uint32_t) * WinApp::height;
		//�摜�C���[�W
		uint32_t* img = new uint32_t[pixelCount];
		for (size_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�f�[�^�Ƀf�[�^�]��
		result_ = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result_));
		delete[]img;
	}
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//�r�q�u�p�f�X�N���v�^�q�[�v�𐶐�
	result_ = dxCommon->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	dxCommon->GetDevice()->CreateShaderResourceView(
		texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result_ = dxCommon->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result_));

	//�����_�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//�f�X�N���v�^�q�[�v��RTV�쐬
	dxCommon->GetDevice()->CreateRenderTargetView(
		texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());
	
	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, WinApp::width, WinApp::height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x�o�b�t�@�̐���
	CD3DX12_HEAP_PROPERTIES depthProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE depthVale = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result_ = dxCommon->GetDevice()->CreateCommittedResource(
		&depthProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthVale,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result_));

	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result_ = dxCommon->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result_));
	//�f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxCommon->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList_) {

	matWorld = XMMatrixIdentity();
	//matWorld.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	//matWorld.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	//matWorld *= XMMatrixScaling(1.0f, 1.0f, 0.0f);
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	constMap->color = color;
	constMap->mat = matWorld * matProjection;

	//��\��
	if (isInvisible)
	{
		return;
	}

	spriteCommon->SetTextureCommands(texIndex);
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//cmdList_->SetPipelineState(spriteCommon->GetPiprlineState());
	//cmdList_->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList_->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�C���f�B�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList_->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(SRV)�̐ݒ�R�}���h
	D3D12_GPU_DESCRIPTOR_HANDLE descHeapSRVHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
	cmdList_->SetGraphicsRootDescriptorTable(1,descHeapSRVHandle);
	// �`��R�}���h
	cmdList_->DrawIndexedInstanced(_countof(indice), texIndex, 0, 0, 0);//�S�Ă̒��_���g���ĕ`��
}

void PostEffect::InitializeTex(){

}

void PostEffect::InitializeSRV(){
	
}

void PostEffect::InitializeRTV(){
	
}

void PostEffect::InitializeDepth(){
	
}

void PostEffect::InitializeDSV(){
	
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList*	 cmdList_) {
	CD3DX12_RESOURCE_BARRIER resourceBuff = 
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	//���\�[�X�o���A��ύX�i�V�F�[�_�[���\�[�X���`��\�j
	cmdList_->ResourceBarrier(
		1,&resourceBuff);
	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//�r���[�|�[�g�̐ݒ�
	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(WinApp::width), static_cast<float>(WinApp::height));
	cmdList_->RSSetViewports(1, &viewPort);
	//�V�U�����O��`�̐ݒ�
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<float>(WinApp::width), static_cast<float>(WinApp::height));
	cmdList_->RSSetScissorRects(1, &rect);

	//�S��ʃN���A
	cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//�[�x�o�b�t�@�̃N���A
	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList_) {
	CD3DX12_RESOURCE_BARRIER resourceBuff =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//���\�[�X�o���A��ύX(�`��\���V�F�[�_�[���\�[�X)
	cmdList_->ResourceBarrier(1, &resourceBuff);
}