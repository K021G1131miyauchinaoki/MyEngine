#include<Windows.h>
#pragma	comment(lib,"dinput8.lib")
#pragma	comment(lib,"dxguid.lib")
#define	DIRECTINPUT_VERSION	0x0800 //DirectInputのバージョン指定
#include<dinput.h>
#include<cassert>
#include<wrl.h>
#include"WinApp.h"

#pragma once


class Input
{
public:
template<class T>using	ComPtr= Microsoft::WRL::ComPtr<T>;

public://メンバ関数
	//初期化
	void	Initialize(WinApp*winApp_);
	//更新
	void	Update();

	bool	PushKey(BYTE	keyNumber);

	bool	TriggerKey(BYTE	keyNumber);

private:
	WinApp* winApp = nullptr;

	HRESULT	result = 0;

	ComPtr<	IDirectInput8> directInput = nullptr;

	ComPtr < IDirectInputDevice8> keyboard = nullptr;
	//全キーの入力状態を取得する
	BYTE	key[256] = {};
	//前回の全キーの状態
	BYTE	keyPre[256] = {};
};

