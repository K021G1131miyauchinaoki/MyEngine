#pragma once
#include<Windows.h>
#include"imgui/imgui_impl_win32.h"

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
	static	int	window_width ;//��
	static	int	window_height;//�c

private:
	WNDCLASSEX	w{};
	//�E�B���h�E�T�C�Y
	HWND	hwnd = nullptr;
};

