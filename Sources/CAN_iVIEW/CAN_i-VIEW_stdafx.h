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
 * \file    CAN_i_VIEW/CAN_i_VIEW_stdafx.h
 * \author  D Southworth
 * \copyright Copyright (c) 2013, Robert Bosch Automotive Service Solutions. All rights reserved.
 *
 * stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include "afxtempl.h"
#endif // _AFX_NO_AFXCMN_SUPPORT

// Windows & C RunTime Header Files
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wtypes.h>
#include <atlconv.h>
#include <stdio.h>
#include <winsock2.h>

#include <include/struct_can.h>

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
typedef BOOL (*DILCALLBACK)(BYTE Argument, PSCONTROLLER_DETAILS pDatStream, int Length);
