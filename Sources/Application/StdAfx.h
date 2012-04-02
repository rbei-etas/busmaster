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
 * \file      StdAfx.h
 * \brief     This header file contains the standard include header files
 * \authors   Amitesh Bharti, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the standard include header files
 */

#if !defined(AFX_STDAFX_H__E02A6C5E_1005_11D6_A606_00D0B76BEBF5__INCLUDED_)
#define AFX_STDAFX_H__E02A6C5E_1005_11D6_A606_00D0B76BEBF5__INCLUDED_

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <direct.h> // for _getcwd function.

#include <math.h>
#include <stdlib.h>
#include <afxtempl.h>
#include <Mmsystem.h>   // MultiMedia Timers
#include <fstream>    // Disk Input/Output stream
#include <afxcoll.h>
#include <afxmt.h>      // For CEvent class 
#include <afx.h>

#include <malloc.h>     // for defns of malloc and free
#include <shlwapi.h>    // For Directory related functions
#include <htmlhelp.h>   // For HTML help include
#include <AFXCTL.H>
#include <afxdisp.H>

#include <comutil.h>
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <Psapi.h>
#include <atlconv.h>

#include <map>
#include <vector>
#include <string>
#include <list>
#include <queue>

using namespace std;

const float PI = (float) 3.14159265358979323846;

#include "include/BaseDefs.h"
#include "include/Error.h"

const BYTE VERSION_MAJOR    = 1;
const BYTE VERSION_MINOR    = 6;
const BYTE VERSION_BUILD    = 1;

// Disable warning messages C4996
#pragma warning(disable: 4996)


#define FOR_ETIN
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E02A6C5E_1005_11D6_A606_00D0B76BEBF5__INCLUDED_)
