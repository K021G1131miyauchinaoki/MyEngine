#include "Sprite.h"



//�C���f�b�N�X�f�[�^
unsigned	short	indices[] = {
	0,1,2,
	1,2,3,
};

void	Sprite::Initialize(SpriteCommon* spriteCommon_) {
	//�ϐ��փR�s�[
	spriteCommon = spriteCommon_;
	directXCom = spriteCommon_->GetdxCom();

	//���_�f�[�^
	Vertex	vertices[] = {
	{{ -0.5f, -0.5f, 0.0f },{0.0f,1.0f}}, // ����
	{{ -0.5f, +0.5f, 0.0f },{0.0f,0.0f}}, // ����
	{{ +0.5f, -0.5f, 0.0f },{1.0f,1.0f}}, // �E��
	{{ +0.5f, +0.5f, 0.0f },{1.0f,0.0f}}, // �E��
	};


	UINT	sizeVB=static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
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
	ID3D12Resource* vertBuff = nullptr;
	result = directXCom->GetResult();
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//�q����̉���
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
	// �萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};   // �q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial)); // ���_�f�[�^�S�̂̃T�C�Y
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// �萔�o�b�t�@�̐���
	//ID3D12Resource* constBffMarerial = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBffMarerial));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBffMarerial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O

	//�C���f�B�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	// ���\�[�X�ݒ�

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�B�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�B�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp,//�q�[�v�̐ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�B�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�B�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];//�C���f�B�b�N�X���R�s�[
	}
	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	//�C���f�B�b�N�X�o�b�t�@�r���[�̍쐬
	//D3D12_INDEX_BUFFER_VIEW	ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Sprite::Draw() {
	comList = directXCom->GetCommandList();

	constMapMaterial->color = XMFLOAT4(1, 1, 1, 0.5f);

	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	comList->SetPipelineState(spriteCommon->GetPipelineState());
	comList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
	//�O�p�`���X�g		TRIANGLELIST
	//�O�p�`�X�g���b�v	TRIANGLESTRIP
	//�����X�g			LINELIST
	//���X�g���b�v		LINESTRIP
	//�_���X�g			POINTLIST
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	comList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[
	comList->SetGraphicsRootConstantBufferView(0,constBffMarerial->GetGPUVirtualAddress());

	//�C���f�B�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	comList->IASetIndexBuffer(&ibView);

	// �`��R�}���h
	comList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);//�S�Ă̒��_���g���ĕ`��
}