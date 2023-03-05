#pragma once
#include<Windows.h>
#include"imgui/imgui_impl_win32.h"

class WinApp
{
public://静的メンバ関数
	static	LRESULT	WindowProc(HWND hwnd, UINT	msg, WPARAM wapram, LPARAM	lparam);

public://メンバ関数
	//初期化
	void	Initialize();
	//メッセージの処理
	bool	ProcessMessage();
	//終了
	void	Finalize();

	WNDCLASSEX	GetWindow()const { return w; };

	HWND	GetHwnd() { return hwnd; };
public:
	//ウィンドウサイズ
	static	int	window_width ;//横
	static	int	window_height;//縦

private:
	WNDCLASSEX	w{};
	//ウィンドウサイズ
	HWND	hwnd = nullptr;
};

