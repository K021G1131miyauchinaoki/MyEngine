#include "Input.h"

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

void	Input::Initialize() {
	this->winApp = WinApp::GetInstance();
	//DirectInput�̏�����
	result = DirectInput8Create(
		winApp->GetWindow().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	/*                             �L�[�{�[�h                                         */
	//�L�[�{�[�h�f�o�C�X�̐���
	//ComPtr < IDirectInputDevice8> keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	/*                                 �}�E�X                                     */
	//�}�E�X�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = mouseDev->SetDataFormat(&c_dfDIMouse);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = mouseDev->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void	Input::Update() {
	/*�L�[�{�[�h*/
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);

	/*�}�E�X*/
	HRESULT hr;
	hr = mouseDev->Acquire();
	//�ŐV�ɂ���O�ɕۑ�
	oldMouse = mouse;
	//�ŐV�̃}�E�X���̎擾
	hr = mouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
	if (FAILED(hr))return;
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(winApp->GetHwnd(), &p);
	mPos.x = static_cast<float>(p.x);
	mPos.y = static_cast<float>(p.y);
}

bool	Input::PushKey(BYTE keyNumber) {
	//�L�[��������Ă����true
	if (key[keyNumber])
	{
		return	true;
	}

	//�����łȂ����false��Ԃ�
	return	false;
}

bool	Input::TriggerKey(BYTE keyNumber) {
	//�L�[�������ꂽ�u�Ԃ�true
	if (key[keyNumber]&&!keyPre[keyNumber])
	{
		return	true;
	}

	//�����łȂ����false��Ԃ�
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