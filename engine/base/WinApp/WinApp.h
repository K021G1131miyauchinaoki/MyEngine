#pragma once
#include<Windows.h>
#include<imgui_impl_dx12.h>

class WinApp
{
public://�ÓI�����o�֐�
	static	LRESULT	WindowProc(HWND hwnd, UINT	msg, WPARAM wapram, LPARAM	lparam);
	static WinApp* GetInstance();
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
	static	int	width ;//��
	static	int	height;//�c

private:
	WNDCLASSEX	w{};
	//�E�B���h�E�T�C�Y
	HWND	hwnd = nullptr;
};

