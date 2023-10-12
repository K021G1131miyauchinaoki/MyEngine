/**
 * @file WinApp.h
 * @brief WindowsAPIクラス
 */
#pragma once
#pragma warning (push)
#pragma warning (disable:4668)
#include<Windows.h>
#include<imgui_impl_dx12.h>
#pragma warning (pop)

class WinApp
{
public://静的メンバ関数
	static	LRESULT	WindowProc(HWND hwnd, UINT	msg, WPARAM wapram, LPARAM	lparam);
	static WinApp* GetInstance();
public://メンバ関数
	//初期化
	void	Initialize();
	//メッセージの処理
	bool	ProcessMessage();
	//終了
	void	Finalize();


	WNDCLASSEX	GetWindow()const;

	HWND	GetHwnd() { return hwnd; };
public:
	//ウィンドウサイズ
	static	int	width ;//横
	static	int	height;//縦

private:
	WNDCLASSEX	w{};
	//ウィンドウサイズ
	HWND	hwnd = nullptr;
};

