#include<Windows.h>
#pragma	comment(lib,"dinput8.lib")
#pragma	comment(lib,"dxguid.lib")
#define	DIRECTINPUT_VERSION	0x0800 //DirectInput�̃o�[�W�����w��
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

public://�����o�֐�
	//������
	void Initialize(WinApp*winApp_);
	//�X�V
	void Update();
	/*�L�[�{�[�h*/
	//�������Ƃ�
	bool PushKey(BYTE	keyNumber);
	//�������u��
	bool TriggerKey(BYTE	keyNumber);
	
	/*�}�E�X*/
	//�������Ƃ�
	bool PushClick(Botton botton);
	//�������u��
	bool TriggerClick(Botton botton);
	//2D���W�̎擾
	const Vector2 GetPos() { return mPos; }

private:
	WinApp* winApp = nullptr;

	HRESULT	result = 0;

	ComPtr<	IDirectInput8> directInput = nullptr;

	ComPtr < IDirectInputDevice8> keyboard = nullptr;
	ComPtr < IDirectInputDevice8> mouseDev = nullptr;
	//�S�L�[�̓��͏�Ԃ��擾����
	BYTE	key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE	keyPre[256] = {};

	DIMOUSESTATE mouse;
	DIMOUSESTATE oldMouse;
	Vector2 mPos;

};

