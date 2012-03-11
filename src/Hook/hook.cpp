#include "stdafx.hpp"
#include "hook.hpp"

#pragma data_seg(".globals")
HWND g_hWnd = 0;
HINSTANCE g_hInstance = 0;
HHOOK g_hCallWndHook = 0;
#pragma data_seg()

void Initialize(HINSTANCE hInstance) {
	g_hInstance = hInstance;
}

LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if(nCode != HC_ACTION) {
		return CallNextHookEx(0, nCode, wParam, lParam);
	}
	LPMSG lpmsg = reinterpret_cast<LPMSG>(lParam);
	switch(lpmsg->message) {	
		case WM_MOUSEWHEEL:
			{
				POINT pt;
				pt.x = GET_X_LPARAM(lpmsg->lParam);
				pt.y = GET_Y_LPARAM(lpmsg->lParam);
				HWND hwndTarget = WindowFromPoint(pt);
				if(lpmsg->hwnd != hwndTarget) {
					PostMessage(hwndTarget, WM_MOUSEWHEEL, lpmsg->wParam, lpmsg->lParam);
					lpmsg->message = WM_NULL;
				}
			}
			break;

	default:
		break;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

BOOL WINAPI Start(HWND hWnd) {
	if(!g_hInstance) {
		return FALSE;
	}
	if(!hWnd) {
		return FALSE;
	}
	if(g_hCallWndHook) {
		return TRUE;
	}
	g_hWnd = hWnd;
	if(!g_hCallWndHook) {
		g_hCallWndHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, g_hInstance, 0);
		if(!g_hCallWndHook) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL WINAPI Finish() {
	if(!g_hInstance) {
		return FALSE;
	}

	if(g_hCallWndHook && !UnhookWindowsHookEx(g_hCallWndHook)) {
		return FALSE;
	}
	g_hCallWndHook = 0;
	PostMessage(HWND_BROADCAST, WM_NULL, 0, 0);

	return TRUE;
}
