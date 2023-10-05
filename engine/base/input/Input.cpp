#include "Input.h"

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

void	Input::Initialize() {
	this->winApp = WinApp::GetInstance();
	//DirectInputの初期化
	result = DirectInput8Create(
		winApp->GetWindow().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	/*                             キーボード                                         */
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

	/*                                 マウス                                     */
	//マウスデバイスの生成
	result = directInput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = mouseDev->SetDataFormat(&c_dfDIMouse);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = mouseDev->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void	Input::Update() {
	/*キーボード*/
	//キーボード情報の取得開始
	keyboard->Acquire();

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);

	/*マウス*/
	HRESULT hr;
	hr = mouseDev->Acquire();
	//最新にする前に保存
	oldMouse = mouse;
	//最新のマウス情報の取得
	hr = mouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
	if (FAILED(hr))return;
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(winApp->GetHwnd(), &p);
	mPos.x = static_cast<float>(p.x);
	mPos.y = static_cast<float>(p.y);
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

bool	Input::PushClick(Botton botton) {
	if (mouse.rgbButtons[botton])
	{
		return true;
	}
	return false;
}

bool	Input::TriggerClick(Botton botton) {
	if (mouse.rgbButtons[botton] && !oldMouse.rgbButtons[botton])
	{
		return true;
	}
	return false;
}