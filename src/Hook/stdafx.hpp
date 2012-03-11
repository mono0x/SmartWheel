#pragma once

#define WINVER         0x0600
#define _WIN32_WINDOWS 0x0600
#define _WIN32_WINNT   0x0600
#define _WIN32_IE      0x0700

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <imm.h>
#include <shlwapi.h>
#include <commctrl.h>

#include <cstdlib>

#ifdef _DEBUG
#include <cstdarg>
#endif
