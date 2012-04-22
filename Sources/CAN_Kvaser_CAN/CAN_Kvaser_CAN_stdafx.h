/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      CAN_Kvaser_CAN_stdafx.h
 * \brief     include file for standard system include files
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once

#define WIN32_LEAN_AND_MEAN  	// Exclude rarely-used stuff from Windows headers

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif

//#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
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

// TODO: reference additional headers your program requires here
const int ERR_INVALID_PROCDIL       = -1;
const int ERR_INVALID_PARENTHANDLE  = -2;
const int INFO_RETAINED_CONFDATA    = 0;
const int INFO_CHANGED_CONFDATA     = 1;

const int WARNING_NOTCONFIRMED      = -3;
const int ERR_CONFIRMED_CONFIGURED  = -4;
const int INFO_INIT_DATA_CONFIRMED  = 2;
const int INFO_CONFIRMED_CONFIGURED = 3;

// First define the callback function type
typedef BOOL (*DILCALLBACK)(BYTE Argument, PBYTE pDatStream, int Length);
