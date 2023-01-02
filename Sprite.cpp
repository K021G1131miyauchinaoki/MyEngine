#include "Sprite.h"
#include"DirectXTex/DirectXTex.h"


//�C���f�b�N�X�f�[�^
unsigned	short	indices[] = {
	0,1,2,
	1,2,3,
};

XMMATRIX Sprite::matProjection;
//void	Sprite::Loadtexture(uint32_t index) {
//
//	TexMetadata	metadata{};
//	ScratchImage	scratchImg{};
//
//	result = LoadFromWICFile(
//		L"Resources/tex.png",
//		WIC_FLAGS_NONE,
//		&metadata, scratchImg);
//
//
//	ScratchImage	mipChain{};
//	result = GenerateMipMaps(
//		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT, 0, mipChain);
//	if (SUCCEEDED(result))
//	{
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//	metadata.format = MakeSRGB(metadata.format);
//	//�e�N�X�`���o�b�t�@�ݒ�
//	//�q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES	textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC	textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = metadata.format;
//	textureResourceDesc.Width = metadata.width;
//	textureResourceDesc.Height = (UINT16)metadata.height;
//	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	//�e�N�X�`���o�b�t�@�̐���
//	ID3D12Resource* texBuff = nullptr;
//	result = directXCom->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuffers[index]));
//
//	//�S�~�b�v�}�b�v�ɂ���
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
//		const	Image* img = scratchImg.GetImage(i, 0, 0);
//		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//		result = texBuffers[index]->WriteToSubresource(
//			(UINT)i,
//			nullptr,			 //�S�̈�փR�s�[
//			img->pixels,		 //���f�[�^�A�h���X
//			(UINT)img->rowPitch, //�P���C���T�C�Y
//			(UINT)img->slicePitch//�S�T�C�Y
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	
//
//	//SRV�q�[�v�̐擪�n���h�����擾
//	D3D12_CPU_DESCRIPTOR_HANDLE	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
//
//	//�V�F�[�_���\�[�X�r���[�ݒ�
//	D3D12_SHADER_RESOURCE_VIEW_DESC	srvDesc{};		//�ݒ�\����
//	D3D12_RESOURCE_DESC	resDesc = texBuffers[index]->GetDesc();
//	srvDesc.Format = resDesc.Format;//RGBA	float
//	srvDesc.Shader4ComponentMapping =
//		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;
//
//	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
//	srvHandle.ptr += index;
//	directXCom->GetDevice()->CreateShaderResourceView(texBuffers[index].Get(), 
//		&srvDesc, 
//		srvHandle);
//
//}
void	Sprite::Initialize(SpriteCommon* spriteCommon_) {
	//�ϐ��փR�s�[
	spriteCommon = spriteCommon_;
	directXCom = spriteCommon_->GetdxCom();

	


	//UINT	sizeVB=static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//���_�o�b�t�@
	D3D12_HEAP_PROPERTIES heapProp{};//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]��
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(Vertex)*4;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���_�o�b�t�@�̐���
	//ID3D12Resource* vertBuff = nullptr;
	result = directXCom->GetResult();
	result = directXCom->GetDevice()->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	Update();

	//���_�o�b�t�@�r���[�̍쐬
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeof(Vertex)*4;
	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);
	// �萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};   // �q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData)+0xff)&~0xff; // ���_�f�[�^�S�̂̃T�C�Y
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = directXCom->GetDevice()->CreateCommittedResource(
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
		0.0f, (float)directXCom->GetSwapChainDesc().Width,
		(float)directXCom->GetSwapChainDesc().Height, 0.0f,
		0.0f, 1.0f);
	//���s���e�s��̌v�Z
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, 1280,
	//	720, 0,
	//	0.0f, 1.0f);
	
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


	

	//���f�[�^���
	//delete[]	imageData;
	//srv�̍ő��
	const	size_t	kMaxSRVCount = 2056;
	
		
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

}
void	Sprite::SetRotation(const float& rotation_) {
	rotation = rotation_;
	Update();

}

void	Sprite::SetPosition(const XMFLOAT2& position_) { 
	position = position_;
	Update();
}

void	Sprite::SetColor(const XMFLOAT4& color_) {
	color =	color_;
	Update();
}

void	Sprite::SetSize(const XMFLOAT2& size_) {
	size = size_;
	Update();
}

void	Sprite::SetAnchorPoint(const XMFLOAT2& anchorPoint_) {
	anchorPoint = anchorPoint_;
	Update();
}

// ���E���]�̐ݒ�
void Sprite::SetIsFlipX(bool isFlipX_){
	isFlipX = isFlipX_;
	Update();
}

// �㉺���]�̐ݒ�
void Sprite::SetIsFlipY(bool isFlipY_){
	isFlipY = isFlipY_;
	Update();
}

// ��\��
void Sprite::SetIsInvisible(bool isInvisible_) {
	isInvisible = isInvisible_;
	Update();
}

void Sprite::Draw() {
	//srvHeap = spriteCommon->GetSrvHeap();
	comList = directXCom->GetCommandList();
	matWorld = XMMatrixIdentity();
	//matWorld.r[0].m128_f32[0] = 2.0f / directXCom->GetSwapChainDesc().Width;
	//matWorld.r[1].m128_f32[1] = -2.0f / directXCom->GetSwapChainDesc().Height;
	//matWorld *= XMMatrixScaling(1.0f, 1.0f, 0.0f);
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	constMap->color = color;
	constMap->mat = matWorld*matProjection;

	//��\��
	if (isInvisible)
	{
		return;
	}

	spriteCommon->SetTextureCommands(textureIndex);
	
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	comList->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	comList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	

	//�C���f�B�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	comList->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//comList->SetGraphicsRootConstantBufferView(2, constBuff->GetGPUVirtualAddress());
	// �`��R�}���h
	comList->DrawIndexedInstanced(_countof(indices), textureIndex, 0, 0, 0);//�S�Ă̒��_���g���ĕ`��
}

void Sprite::Update()
{
	enum vertexNum
	{
		LB,//����
		LT,//����
		RB,//�E��
		RT,//�E��
	};
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.x) * size.y;
	float bottom = (1.0f - anchorPoint.x) * size.y;

	if (isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	//���_�f�[�^
	Vertex vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	vertices[LB].uv = {0.0f,1.0f }; // ����
	vertices[LT].uv = {0.0f,0.0f }; // ����
	vertices[RB].uv = {1.0f,1.0f }; // �E��
	vertices[RT].uv = {1.0f,0.0f }; // �E��

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}