#pragma once

#define countof(a) (sizeof(a) / sizeof(a[0]))

void ShowError(HINSTANCE hInstance, UINT uID, HWND hWnd = 0);

HWND CreateHiddenWindow(HINSTANCE hInstance, LPCTSTR lpClassName, WNDPROC lpfnWndProc);
