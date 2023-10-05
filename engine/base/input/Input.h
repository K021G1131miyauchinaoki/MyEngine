#include<Windows.h>
#pragma	comment(lib,"dinput8.lib")
#pragma	comment(lib,"dxguid.lib")
#define	DIRECTINPUT_VERSION	0x0800 //DirectInputのバージョン指定
#include<dinput.h>
#include<cassert>
#include<wrl.h>
#include"WinApp.h"
#include<Vector2.h>

#pragma once

enum Botton {
	LEFT,
	RIGHT,
	CENTER
};

class Input
{
public:
template<class T>using	ComPtr= Microsoft::WRL::ComPtr<T>;

public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	/*キーボード*/
	//押したとき
	bool PushKey(BYTE	keyNumber);
	//押した瞬間
	bool TriggerKey(BYTE	keyNumber);
	
	/*マウス*/
	//押したとき
	bool PushClick(Botton botton);
	//押した瞬間
	bool TriggerClick(Botton botton);
	//2D座標の取得
	const Vector2 GetMausePos() { return mPos; }

	static Input* GetInstance();

private:
	WinApp* winApp = nullptr;

	HRESULT	result = 0;

	ComPtr<	IDirectInput8> directInput = nullptr;

	ComPtr < IDirectInputDevice8> keyboard = nullptr;
	ComPtr < IDirectInputDevice8> mouseDev = nullptr;
	//全キーの入力状態を取得する
	BYTE	key[256] = {};
	//前回の全キーの状態
	BYTE	keyPre[256] = {};

	DIMOUSESTATE mouse;
	DIMOUSESTATE oldMouse;
	Vector2 mPos;

};

