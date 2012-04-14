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

#pragma once

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
#include <string.h>
using namespace std;
extern void Truncate_str(char *type,char *pcToken,bool bToLog);
extern void Truncate_str(char *type,string &strTemp,bool bToLog);
