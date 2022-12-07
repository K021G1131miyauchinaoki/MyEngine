#include "SpriteCommon.h"
#include<d3dcompiler.h>
#pragma	comment(lib,"d3dcompiler.lib")

void	SpriteCommon::Initialize(DirectXCommon* directXCom_) {
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	directXCom = directXCom_;
	result = directXCom->GetResult();
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
}