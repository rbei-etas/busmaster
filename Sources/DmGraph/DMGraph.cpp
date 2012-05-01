// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      DMGraph.cpp: Implementation of DLL Exports.
//
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old NTGraph.cpp
//

// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL,
//      run nmake -f DMGraphps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "DMGraph.h"

#include "DMGraph_i.c"
#include "DMGraphCtrl.h"
#include "ElementPropPage.h"
#include "AnnoPropPage.h"
#include "CursorPropPage.h"
#include "FormatPropPage.h"
#include "DMGraphPropPage.h"

typedef HWND (WINAPI* fct_HtmlHelpW)(
    HWND hwndCaller,
    LPCWSTR pszFile,
    UINT uCommand,
    DWORD_PTR dwData
);

static fct_HtmlHelpW _HtmlHelpW = NULL;

static HMODULE hHelpLib = NULL;
static HWND hWndHelp = NULL;

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_DMGraphCtrl, CDMGraphCtrl)
OBJECT_ENTRY(CLSID_ElementPropPage, CElementPropPage)
OBJECT_ENTRY(CLSID_AnnoPropPage, CAnnoPropPage)
OBJECT_ENTRY(CLSID_CursorPropPage, CCursorPropPage)
OBJECT_ENTRY(CLSID_FormatPropPage, CFormatPropPage)
OBJECT_ENTRY(CLSID_DMGraphPropPage, CDMGraphPropPage)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        INITCOMMONCONTROLSEX cc;
        cc.dwSize = sizeof(INITCOMMONCONTROLSEX);
        cc.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES ;
        InitCommonControlsEx(&cc);
        _Module.Init(ObjectMap, hInstance, &LIBID_DMGraphLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if(hHelpLib)
        {
            if(hWndHelp)
            {
                ::PostMessage(hWndHelp, WM_CLOSE, 0, 0);
            }

            _HtmlHelpW = NULL;
            FreeLibrary(hHelpLib);
            hHelpLib = NULL;
        }

        _Module.Term();
    }

    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


void WINAPI DmOpenHelp(HWND hWndParent, LPCOLESTR /*pszHelpDir*/)
{
    if(hHelpLib == NULL)
    {
        HRESULT hr;
        LPCTSTR szLib = _T("HHCTRL.ocx");
        hHelpLib = LoadLibrary(szLib);  //load and keep it in memory

        if(hHelpLib)
        {
            _HtmlHelpW = (fct_HtmlHelpW)GetProcAddress(hHelpLib, "HtmlHelpW");

            if(_HtmlHelpW == NULL)
            {
                hr = HRESULT_FROM_WIN32( GetLastError() );
                ATLTRACE(_T("Error %#x\n"), hr);
                MessageBoxW(NULL, L"Help cannot be loaded - HtmlHelpW could not be found", NULL, MB_OK|MB_ICONSTOP);
            }
        }
        else
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            ATLTRACE(_T("Error %#x\n"), hr);
            MessageBoxW(NULL, L"Help cannot be loaded - HHCTRL.ocx could not be found", NULL, MB_OK|MB_ICONSTOP);
        }
    }

    if(_HtmlHelpW == NULL)
    {
        return;
    }

    OLECHAR szFullHelpPath[_MAX_PATH], *ptr;
    GetModuleFileNameW(_Module.GetModuleInstance(), szFullHelpPath, _MAX_PATH);
    ptr = wcsrchr(szFullHelpPath, '.');

    if(ptr == NULL)
    {
        return;
    }

    wcscpy(ptr+1, L"chm");
    hWndHelp = _HtmlHelpW(hWndParent, szFullHelpPath, HH_DISPLAY_TOPIC, NULL);

    if(hWndHelp == NULL)
    {
        MessageBoxW(hWndParent, L"Help cannot be loaded - chm file could not be found", NULL, MB_OK|MB_ICONSTOP);
    }
}
