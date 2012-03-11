#include "stdafx.hpp"
#include "util.hpp"
#include "resource.h"

namespace {

	bool RegisterWindowClass(HINSTANCE hInstance, LPCTSTR lpClassName, WNDPROC lpfnWndProc) {
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(wcex);
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.lpfnWndProc = lpfnWndProc;
		wcex.lpszClassName = lpClassName;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.hIcon = wcex.hIconSm = LoadIcon(0, IDI_APPLICATION);
		wcex.style = 0;
		wcex.hCursor = LoadCursor(0, IDC_ARROW);
		wcex.lpszMenuName = 0;
		return RegisterClassEx(&wcex) != 0;
	}

}

void ShowError(HINSTANCE hInstance, UINT uID, HWND hWnd) {
	TCHAR szMessage[1024];
	TCHAR szAppName[256];

	LoadString(hInstance, uID, szMessage, countof(szMessage));
	LoadString(hInstance, IDS_APP_NAME, szAppName, countof(szAppName));

	MessageBox(hWnd, szMessage, szAppName, MB_OK | MB_ICONERROR);
}

HWND CreateHiddenWindow(HINSTANCE hInstance, LPCTSTR lpClassName, WNDPROC lpfnWndProc) {
	if(!RegisterWindowClass(hInstance, lpClassName, lpfnWndProc)) {
		return 0;
	}
	return CreateWindowEx(0, lpClassName, 0, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
}
