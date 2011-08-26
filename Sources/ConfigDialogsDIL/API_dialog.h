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
 * \file      API_dialog.h
 * \brief     Declaration of the exported functions
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration of the exported functions
 */

#if !defined API_DIALOG_H__INCLUDED_
#define API_DIALOG_H__INCLUDED_

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

#include "DataTypes/DIL_Datatypes.h"

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


// For the configuration dialog box

USAGEMODE int DisplayConfigurationDlg(HWND hParent, DILCALLBACK ProcDIL, 
                          PSCONTROLER_DETAILS pControllerDetails, UINT nCount, DWORD pDriverId);
USAGEMODE int ListHardwareInterfaces(HWND hParent,DWORD dwDriver,INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount);

#ifdef __cplusplus
}
#endif

#endif // API_DIALOG_H__INCLUDED_
