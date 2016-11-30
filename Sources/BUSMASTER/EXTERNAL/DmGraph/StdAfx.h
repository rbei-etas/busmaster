// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#pragma once

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 // Windows 2000 and later
#endif
#define _ATL_APARTMENT_THREADED

//#pragma comment(lib, "atlthunk.lib")

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#ifndef PROP_ENTRY_TYPE
#define PROP_ENTRY_TYPE( szDesc, dispid, clsid, vt ) PROP_ENTRY( szDesc, dispid, clsid )
#endif // PROP_ENTRY_TYPE

#include <ocidl.h>  // Added by ClassView

#include <math.h>
#include <limits.h>
#include <time.h>

#include <commctrl.h>
#include <Commdlg.h>
#include <HtmlHelp.h>
#include <stdio.h>

//these macros work with nullptr pointers
#define wcscmp(x, y) wcscmp(x ? x : L"", y ? y : L"")
#define _wcsicmp(x, y) _wcsicmp(x ? x : L"", y ? y : L"")

void WINAPI DmOpenHelp(HWND hWndParent, LPCOLESTR pszHelpDir);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
