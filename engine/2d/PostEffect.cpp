#include "PostEffect.h"
#include"DirectXTex.h"
#include<d3dx12.h>
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

DirectXCommon* PostEffect::dxCommon = nullptr;
XMMATRIX PostEffect::matProjection;

XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.0f},//����
	{-0.5f,+0.5f,0.0f},//����
	{+0.5f,-0.5f,0.0f},//�E��
	//{+0.5f,+0.5f,0.0f},//�E��
};

PostEffect::PostEffect()
{

}

void PostEffect::StaticInitialize(DirectXCommon* dxCommon_) {
	dxCommon = dxCommon_;
}

void	PostEffect::Initialize() {
	//���_�f�[�^
	//���_�f�[�^�S�̂̃T�C�Y�����_�f�[�^1���̃T�C�Y*���_�f�[�^�̗v�f��
	UINT sizeVB=static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	
	//���_�o�b�t�@
	D3D12_HEAP_PROPERTIES heapProp{};//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]��
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���_�o�b�t�@�̐���
	result = dxCommon->GetResult();
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	//Update();
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (size_t i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(XMFLOAT3);
	
	//--------------------------------�V�F�[�_-----------------------------
	ID3DBlob* vsBlob = nullptr;//���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;//�G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}


	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	pipelineDesc{};
	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O�L����

	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;//RGB�S�Ẵ`�����l����`��

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
	
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();


	// �p�C�v�����X�e�[�g�̐���
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};   // �q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff; // ���_�f�[�^�S�̂̃T�C�Y
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));
	constMap->color = color;
	constMap->mat = matProjection;

	constMap->mat = XMMatrixIdentity();
	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)dxCommon->GetSwapChainDesc().Width,
		(float)dxCommon->GetSwapChainDesc().Height, 0.0f,
		0.0f, 1.0f);
}

void PostEffect::Update()
{
	//enum vertexNum
	//{
	//	LB,//����
	//	LT,//����
	//	RB,//�E��
	//	RT,//�E��
	//};
	//float left = (0.0f - anchorPoint.x) * size.x;
	//float right = (1.0f - anchorPoint.x) * size.x;
	//float top = (0.0f - anchorPoint.x) * size.y;
	//float bottom = (1.0f - anchorPoint.x) * size.y;

	////���_�f�[�^
	//Vertex vertices[4];
	//vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	//vertices[LT].pos = { left,	top,	0.0f }; // ����
	//vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	//vertices[RT].pos = { right,	top,	0.0f }; // �E��

	//// ���_�o�b�t�@�ւ̃f�[�^�]��
	//Vertex* vertMap = nullptr;
	//result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result)) {
	//	memcpy(vertMap, vertices, sizeof(vertices));
	//	vertBuff->Unmap(0, nullptr);
	//}
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList) {
	
	matWorld = XMMatrixIdentity();
	//matWorld.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	//matWorld.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	//matWorld *= XMMatrixScaling(1.0f, 1.0f, 0.0f);
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	constMap->color = color;
	constMap->mat = matWorld * matProjection;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	comList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�C���f�B�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	comList->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//comList->SetGraphicsRootConstantBufferView(2, constBuff->GetGPUVirtualAddress());
	// �`��R�}���h
	comList->DrawInstanced(_countof(vertices), 1, 0, 0);//�S�Ă̒��_���g���ĕ`��
}