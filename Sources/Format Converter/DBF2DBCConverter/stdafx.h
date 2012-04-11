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
 * \brief     include file for standard system include files.
 * \authors   RBIN/EBS1 - Mahesh.B.S, Padmaja.A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * include file for standard system include files.
 */
/**
* \file       Signal.h
* \brief      include file for standard system include files.
* \authors    Mahesh.B.S, Padmaja.A
* \date       15.11.2004 Created
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#if !defined(AFX_STDAFX_H__CCC27482_CB86_48E1_84E7_AC7A6E6C6D2E__INCLUDED_)
#define AFX_STDAFX_H__CCC27482_CB86_48E1_84E7_AC7A6E6C6D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>
#include "tag.h"
#include "Definitions.h"
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <iostream>
using namespace std;
extern void Truncate_str(char *type,char *pcToken,bool bToLog);
extern void Truncate_str(char *type,CString &strTemp,bool bToLog);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CCC27482_CB86_48E1_84E7_AC7A6E6C6D2E__INCLUDED_)
