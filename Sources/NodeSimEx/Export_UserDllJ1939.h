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
 * \file      Export_UserDllJ1939.h
 * \brief     This header file contains name and prototype of the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains name and prototype of the
 * exported function from user-defined DLL which should be
 * called by the application to initialise a function pointer
 * in that DLL to enable it to call a function from the
 * application.
*/

#pragma once

#include "Export_UserDll.h"

typedef UINT (*APPSENDPROC_J) (STJ1939_MSG*, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLSENDPROC_J)(APPSENDPROC_J);

typedef UINT (*APPONLINEOFFLINE_J) (BOOL, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLONLINEOFFLINE_J)(APPONLINEOFFLINE_J);

typedef UINT (*APPREQUESTPGN_J) (UINT, BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLREQUESTPGN_J)(APPREQUESTPGN_J);

typedef UINT (*APPACKMSG_J) (UINT, BYTE, BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLACKMSG_J)(APPACKMSG_J);

typedef UINT (*APPCLAIMADDRESS_J) (BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLCLAIMADDRESS_J)(APPCLAIMADDRESS_J);

typedef UINT (*APPRQSTADDRESS_J) (UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLRQSTADDRESS_J)(APPRQSTADDRESS_J);

typedef UINT (*APPCMDADDRESS_J) (UINT64, BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLCMDADDRESS_J)(APPCMDADDRESS_J);

typedef UINT (*APPSETTIMEOUT_J) (BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLSETTIMEOUT_J)(APPSETTIMEOUT_J);

// Name of the exported function from user-defined DLL
#define NAME_FUNC_SENGMSG_J      "vSetSendMsgProcAddress"
#define NAME_FUNC_ONLINE_J       "vSetGoOnlineProcAddress"
#define NAME_FUNC_OFFLINE_J      "vSetGoOfflineProcAddress"
#define NAME_FUNC_REQUESTPGN_J   "vSetRequestPGNProcAddress"
#define NAME_FUNC_ACKMSG_J       "vSetSendAckMsgProcAddress"
#define NAME_FUNC_CLAIMADDRESS_J "vSetClaimAddressProcAddress"
#define NAME_FUNC_RQSTADDRESS_J  "vSetRequestAddressProcAddress"
#define NAME_FUNC_CMDADDRESS_J   "vSetCommandAddressProcAddress"
#define NAME_FUNC_TIMEOUT_J      "vSetSetTimeoutProcAddress"
