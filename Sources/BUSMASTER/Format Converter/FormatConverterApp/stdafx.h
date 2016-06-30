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
 * \file      stdafx.h
 * \brief     Standard system include files
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Include file for standard system include files,
 * or project specific include files that are used frequently,
 * but are changed infrequently
 */

#pragma once

/* MFC includes */
#define _SECURE_ATL 1
#define VC_EXTRALEAN                        /* Exclude rarely-used stuff from Windows headers */
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  /* some CString constructors will be explicit */
#define _AFX_ALL_WARNINGS                   /* turns off MFC's hiding of some common and often safely ignored warning messages */

#include <afxwin.h>         /* MFC core and standard components */
#include <afxext.h>         /* MFC extensions */
#include <afxdisp.h>        /* MFC Automation classes */
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         /* MFC support for Windows Common Controls */
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdlgs.h>

/* C++ includes */
#include <iostream>
using namespace std;
