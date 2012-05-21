// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__DEB8CE89_9AD6_4031_BB38_E0961D3F8DEA__INCLUDED_)
#define AFX_STDAFX_H__DEB8CE89_9AD6_4031_BB38_E0961D3F8DEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500	// Windows 2000 and later
#endif
#define _ATL_APARTMENT_THREADED

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "atlthunk.lib")

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#ifndef PROP_ENTRY_TYPE
	#define PROP_ENTRY_TYPE( szDesc, dispid, clsid, vt ) PROP_ENTRY( szDesc, dispid, clsid )
#endif // PROP_ENTRY_TYPE

#include <ocidl.h>	// Added by ClassView

#include <math.h>
#include <limits.h>
#include <time.h>

#include <commctrl.h>
#include <Commdlg.h>
#include <HtmlHelp.h>
#include <stdio.h>

//these macros work with NULL pointers
#define wcscmp(x, y) wcscmp(x ? x : L"", y ? y : L"")
#define _wcsicmp(x, y) _wcsicmp(x ? x : L"", y ? y : L"")

void WINAPI DmOpenHelp(HWND hWndParent, LPCOLESTR pszHelpDir);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DEB8CE89_9AD6_4031_BB38_E0961D3F8DEA__INCLUDED)
