#include "stdafx.hpp"
#include "util.hpp"
#include "resource.h"
#include "../Hook/hook.hpp"

namespace {

	LPCTSTR CLASS_NAME32 = _T("SmartWheelWindowClass32");
	LPCTSTR CLASS_NAME64 = _T("SmartWheelWindowClass64");

	LPCTSTR MUTEX_OBJECT_NAME = _T("SmartWheelMutexObject");

#ifdef WIN64
#	define CLASS_NAME CLASS_NAME64
#else
#	define CLASS_NAME CLASS_NAME32
#endif

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
		switch(Msg) {

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			default:
				break;
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	bool IsWow64() {
		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
		LPFN_ISWOW64PROCESS fnIsWow64Process;

		fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process"));
		if(fnIsWow64Process) {
			BOOL isWow64 = FALSE;
			return fnIsWow64Process(GetCurrentProcess(), &isWow64) && isWow64;
		}
		return false;
	}

	int Main() {
		HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));

		TCHAR szPath[MAX_PATH];
		GetModuleFileName(0, szPath, MAX_PATH);
		PathRemoveFileSpec(szPath);
		SetCurrentDirectory(szPath);

#ifndef WIN64
		CreateMutex(0, TRUE, MUTEX_OBJECT_NAME);
		if(GetLastError() == ERROR_ALREADY_EXISTS) {
			SendMessage(FindWindow(CLASS_NAME32, 0), WM_CLOSE, 0, 0);
			SendMessage(FindWindow(CLASS_NAME64, 0), WM_CLOSE, 0, 0);
			return 0;
		}

		if(IsWow64()) {
			STARTUPINFO si;
			GetStartupInfo(&si);
			PROCESS_INFORMATION pi;
			CreateProcess(_T("SmartWheel64"), 0, 0, 0, TRUE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);
		}
#endif

		ImmDisableIME(static_cast<DWORD>(-1));

		HWND hWnd = CreateHiddenWindow(hInstance, CLASS_NAME, WindowProc);
		if(!hWnd) {
			ShowError(hInstance, IDS_ERROR_CREATE_WINDOW);
			return 1;
		}

		ShowWindow(hWnd, SW_MINIMIZE);
		ShowWindow(hWnd, SW_HIDE);

		if(!Start(hWnd)) {
			ShowError(hInstance, IDS_ERROR_START);
			return 1;
		}

		MSG msg;
		while(GetMessage(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!Finish()) {
			ShowError(hInstance, IDS_ERROR_FINISH);
			return 1;
		}
		return static_cast<int>(msg.wParam);
	}

}

#ifdef _DEBUG
int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	return Main();
}
#else
void __cdecl WinMainCRTStartup() {
	ExitProcess(Main());
}
#endif
