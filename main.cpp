#include<Windows.h>

LRESULT	WindowProc(HWND hwnd, UINT	msg, WPARAM wapram, LPARAM	lparam) {
	switch (msg)
	{
	case	WM_DESTROY:
		PostQuitMessage(0);
		return	0;
	}
	return	DefWindowProc(hwnd, msg, wapram, lparam);
}

int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	OutputDebugStringA("Hello,DirectX!!\n");

	const	int	window_width = 1280;//â°
	const	int	window_height = 720;//èc

	WNDCLASSEX	w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = L"DirectXGame";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w);

	RECT	wrc = { 0,0,window_width,window_height };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND	hwnd = CreateWindow(w.lpszClassName,
		L"DirectXGame",
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

	MSG	msg{};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message==WM_QUIT)
		{
			break;
		}
	}
}



