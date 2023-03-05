#include "ImguiManager.h"

void ImguiManager::Initialize(WinApp* winApp_, DirectXCommon* dxCom_) {
	winApp = winApp_;
	dxCom = dxCom_;
	//�R���e�L�X�g�̍쐬
	ImGui::CreateContext();
	//�X�^�C���̐ݒ�
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());

	//�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//�f�X�N���v�^�q�[�v����
	result = dxCom->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		dxCom->GetDevice(),
		static_cast<int>(dxCom->GetBackBufferCount()),
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB, srvHeap.Get(),
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart()
	);
	ImGuiIO& io = ImGui::GetIO();
	//�W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();
}

void ImguiManager::Finalize() {
	//��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�f�X�N���v�^�q�[�v���
	srvHeap.Reset();
}

void ImguiManager::Begin() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::End() {
	ImGui::Render();
}

void ImguiManager::Draw() {
	ID3D12GraphicsCommandList* comList = dxCom->GetCommandList();
	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	comList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), comList);
}