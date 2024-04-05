#include "TextureManager.h"
#include <cassert>
#include <d3dx12.h>

 /**
 * @file TextureManager.cpp
 * @brief TextureManagerの処理について書いてあります
 */

TextureManager* TextureManager::textureManager_ = nullptr;
std::string TextureManager::DefaultTextureDirectoryPath = "Resource/";

TextureData TextureManager::Load(const std::string& fileName)
{
	return TextureManager::GetInstance()->LoadTexture(fileName);
}

void TextureManager::Initialize(DirectXCommon* directXCommon)
{
	HRESULT result;

	// メンバ変数に記録
	dxCommon_ = directXCommon;

	// デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1; // 1度の描画に使うテクスチャの数
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0; // レジスタ番号
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = MaxSRVCount;
	// 設定を元にSRV用デスクリプタヒープを生成
	result = directXCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc,IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	// ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
}

TextureData TextureManager::LoadTexture(const std::string& fileName)
{
	auto it = std::find_if(
		textureDatas_.begin(),
		textureDatas_.end(),
		[ & ] (TextureData& textureData)
{
	return textureData.filePath == fileName;
 }
	);

	if ( it != textureDatas_.end() )
	{
		return *it;
	}

	if ( texCount > 1024 )
	{
		assert(0);
	}

	TextureData tmp{};

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	tmp.srvHeap = srvHeap;
	tmp.descriptorRange = descriptorRange;

	wchar_t wfilepath[ 256 ];

	MultiByteToWideChar(CP_ACP,0,fileName.c_str(),-1,wfilepath,_countof(wfilepath));

	// WICテクスチャのロード
	HRESULT result = LoadFromWICFile(
		wfilepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata,scratchImg);
	assert(SUCCEEDED(result));

	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT,0,mipChain);

	if ( SUCCEEDED(result) )
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	tmp.texBuff = CreateTexBuff(metadata,scratchImg);

	tmp.srvGpuHandle = CreateShaderResourceView(tmp.texBuff.Get(),metadata);

	tmp.width = metadata.width;
	tmp.height = metadata.height;

	texCount++;

	tmp.filePath = fileName;
	textureDatas_.push_back(tmp);

	return tmp;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(DirectX::TexMetadata& metadata,DirectX::ScratchImage& scratchImg)
{
	dxCommon_->ResetCommand();

	std::vector<D3D12_SUBRESOURCE_DATA> textureSubResource;
	for ( size_t i = 0; i < metadata.mipLevels; i++ )
	{
		D3D12_SUBRESOURCE_DATA subresouce{};

		subresouce.pData = scratchImg.GetImages()[ i ].pixels;
		subresouce.RowPitch = static_cast< LONG_PTR >( scratchImg.GetImages()[ i ].rowPitch );
		subresouce.SlicePitch = static_cast< LONG_PTR >( scratchImg.GetImages()[ i ].slicePitch );

		textureSubResource.push_back(subresouce);
	}

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		( UINT ) metadata.height,
		( UINT16 ) metadata.arraySize,
		( UINT16 ) metadata.mipLevels
		);

	Microsoft::WRL::ComPtr<ID3D12Resource> tmp;
	HRESULT result;

	// テクスチャバッファの生成
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(tmp.ReleaseAndGetAddressOf()));

	UINT64 totalBytes;

	totalBytes = GetRequiredIntermediateSize(tmp.Get(),0,static_cast< UINT >( textureSubResource.size() ));

	Microsoft::WRL::ComPtr<ID3D12Resource> staginBuffer;

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(staginBuffer.ReleaseAndGetAddressOf()));

	if ( FAILED(result) )
	{
		assert(0);
	}

	UpdateSubresources(dxCommon_->GetCommandList(),tmp.Get(),staginBuffer.Get(),0,0,static_cast< uint32_t >( textureSubResource.size() ),textureSubResource.data());

	D3D12_RESOURCE_BARRIER barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		tmp.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);

	dxCommon_->GetCommandList()->ResourceBarrier(1,&barrierTex);
	dxCommon_->ExecuteCommand(false);

	return tmp;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::CreateShaderResourceView(ID3D12Resource* texBuff,DirectX::TexMetadata& metadata)
{
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ハンドルのポインタずらし
	cpuHandle.ptr += static_cast< UINT64 > ( texCount ) * incrementSize;
	gpuHandle.ptr += static_cast< UINT64 > ( texCount ) * incrementSize;

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast< UINT >( metadata.mipLevels );

	// ハンドルの指す位置にシェーダーリソースビュー生成
	dxCommon_->GetDevice()->CreateShaderResourceView(texBuff,&srvDesc,cpuHandle);

	return gpuHandle;
}

void TextureManager::Delete()
{
	delete textureManager_;
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

ID3D12DescriptorHeap* TextureManager::GetSrvHeap()
{
	return srvHeap.Get();
}