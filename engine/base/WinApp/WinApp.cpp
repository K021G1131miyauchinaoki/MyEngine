#include "WinApp.h"
#include<imgui_impl_win32.h>
#pragma comment(lib,"winmm.lib")

int	WinApp::width = 1280; 
int	WinApp::height = 720;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

WinApp* WinApp::GetInstance()
{
	static WinApp instance;
	return &instance;
}
WNDCLASSEX	WinApp::GetWindow()const {
	return w;
};

//ウィンドウプロシージャ
LRESULT	WinApp::WindowProc(HWND hwnd, UINT	msg, WPARAM wparam, LPARAM	lparam) {
	//ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd,msg,wparam,lparam)){return true;}
	
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破棄された
	case	WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return	0;
	}
	//標準のメッセージを行う
	return	DefWindowProc(hwnd, msg, wparam, lparam);
}

void	WinApp::Initialize() {
	//コンソールへの文字出力
	//OutputDebugStringA("Hello,DirectX!!\n");

	//ウィンドウクラスの設定

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = L"BattleTank";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	//ウィンドウサイズ｛x,range,z,横,縦｝
	RECT	wrc = { 0,0,width,height };
	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,
		L"BattleTank",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

bool	WinApp::ProcessMessage() {
	MSG	msg{};

	//メッセージがある ?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}
	return	false;
}

void	WinApp::Finalize() {
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}
