#include<Windows.h>
#pragma once

class WinApp
{
public://�ÓI�����o�֐�
	static	LRESULT	WindowProc(HWND hwnd, UINT	msg, WPARAM wapram, LPARAM	lparam);

public://�����o�֐�
	//������
	void	Initialize();
	//���b�Z�[�W�̏���
	bool	ProcessMessage();
	//�I��
	void	Finalize();

	WNDCLASSEX	GetWindow()const { return w; };

	HWND	GetHwnd() { return hwnd; };
public:
	//�E�B���h�E�T�C�Y
	static	const	int	window_width = 1280;//��
	static	const	int	window_height = 720;//�c

private:
	WNDCLASSEX	w{};
	//�E�B���h�E�T�C�Y
	HWND	hwnd = nullptr;
};

