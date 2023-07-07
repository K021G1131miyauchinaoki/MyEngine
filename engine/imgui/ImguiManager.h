#pragma once
#include <imgui.h>
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <tchar.h>
#include"WinApp.h"
#include"DirectXCommon.h"

class ImguiManager
{
public://メンバ関数
	//初期化
	void Initialize(WinApp* winApp_, DirectXCommon* dxCom_);
	//終了
	void Finalize();
	//受付開始
	void Begin();
	//受付終了
	void End();
	//描画
	void Draw();
public://静的メンバ関数
private://メンバ変数
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvHeap;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCom = nullptr;
	HRESULT result = NULL;
};

