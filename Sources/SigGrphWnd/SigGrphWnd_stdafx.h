/*********************************************************************
  Description   :  Standard inclusion file
  Author(s)     :  Arunkumar Karri
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>          // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxctl.h>
#include <afxtempl.h>

//Graph specific includes

#include <atlbase.h>

extern CComModule _Module;

#include <atlcom.h>
#include <atlwin.h>
#include <atlhost.h>

#include <Shellapi.h>
#include <commctrl.h>
#include <Commdlg.h>
#include <stdio.h>
#include <math.h>
#include <locale.h>

extern void Message(HRESULT hr, IUnknown* pUnk, LPCOLESTR szFormat, ...);
#import "..\EXTERNAL\DMGraph\DMGraph.tlb" raw_interfaces_only no_namespace named_guids
