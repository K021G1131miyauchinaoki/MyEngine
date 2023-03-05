#include "Input.h"

void	Input::Initialize(WinApp* winApp_) {
	this->winApp = winApp_;
	//DirectInputの初期化
	result = DirectInput8Create(
		winApp->GetWindow().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	//ComPtr < IDirectInputDevice8> keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void	Input::Update() {
	//キーボード情報の取得開始
	keyboard->Acquire();

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

bool	Input::PushKey(BYTE keyNumber) {
	//キーが押されていればtrue
	if (key[keyNumber])
	{
		return	true;
	}

	//そうでなければfalseを返す
	return	false;
}

bool	Input::TriggerKey(BYTE keyNumber) {
	//キーが押された瞬間にtrue
	if (key[keyNumber]&&!keyPre[keyNumber])
	{
		return	true;
	}

	//そうでなければfalseを返す
	return	false;
}