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
 * \file      NodeSimEx_stdafx.h
 * \brief     include file for standard system include files
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * include file for standard system include files,
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
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxext.h>
#include <afxtempl.h>
#include <afxcview.h>   // Standard tree view header file
#include <afxrich.h>    //Interface for Richedit classes
#include <afxmt.h>
#include <afxrich.h>
#include <shlwapi.h>
#include <fstream>
#include <io.h>
#include <direct.h>             // for getting curr working directory
#include <Mmsystem.h>

//In 32 bit
#define COMMANINT   INT
#define COMMANUINT  UINT
#define COMMANDWORD DWORD
//In 64 Bit
//#define COMMANINT   INT_PTR
//#define COMMANUINT  UINT_PTR
//#define COMMANDWORD DWORD_PTR


using namespace std;
