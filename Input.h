#include<Windows.h>
#pragma	comment(lib,"dinput8.lib")
#pragma	comment(lib,"dxguid.lib")
#define	DIRECTINPUT_VERSION	0x0800 //DirectInput�̃o�[�W�����w��
#include<dinput.h>
#include<cassert>
#include<wrl.h>
#pragma once


class Input
{
public:
template<class T>using	ComPtr= Microsoft::WRL::ComPtr<T>;

public://�����o�֐�
	//������
	void	Initialize(HINSTANCE hInstance,HWND	hwnd);
	//�X�V
	void	Update();

	bool	PushKey(BYTE	keyNumber);

	bool	TriggerKey(BYTE	keyNumber);

private:
	HRESULT	result;

	ComPtr<	IDirectInput8> directInput = nullptr;

	ComPtr < IDirectInputDevice8> keyboard = nullptr;
	//�S�L�[�̓��͏�Ԃ��擾����
	BYTE	key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE	keyPre[256] = {};
};

