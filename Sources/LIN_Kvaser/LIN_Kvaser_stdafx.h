/**
 * \file      LIN_Kvaser_stdafx.h
 * \author    a4@uincore.net
 * \copyright Copyright (c) 2015, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * LIN_Kvaser_stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

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
#include "afxtempl.h"
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wtypes.h>
#include <atlconv.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <locale.h>

const int ERR_INVALID_PROCDIL       = -1;
const int ERR_INVALID_PARENTHANDLE  = -2;
const int INFO_RETAINED_CONFDATA    = 0;
const int INFO_CHANGED_CONFDATA     = 1;

const int WARNING_NOTCONFIRMED      = -3;
const int ERR_CONFIRMED_CONFIGURED  = -4;
const int INFO_INIT_DATA_CONFIRMED  = 2;
const int INFO_CONFIRMED_CONFIGURED = 3;