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
 * \file      Export_UserDllCAN.h
 * \brief     This header file contains name and prototype of the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains name and prototype of the
 */

#pragma once

#include "Export_UserDll.h"

typedef int (*APPFUNCPROC) (STCAN_MSG,HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLFUNCPROC)(APPFUNCPROC);

typedef void (*RESETCONTROLLER) (BOOL);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLRESETCONTROLLERPROC)(RESETCONTROLLER);

typedef int (*CONTROLLERMODE) (BOOL);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLCONTROLLERMODEPROC)(CONTROLLERMODE);

typedef int (*GOONLINEOFFLINE) (BOOL,HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLONLINEOFFLINEPROC)(GOONLINEOFFLINE);

typedef int (*DISCONNECT) (BOOL);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLDISCONNECTPROC)(DISCONNECT);

// Name of the exported function from user-defined DLL
#define NAME_FUNC_USERDLL               "vSetSendMsgProcAddress"
#define NAME_FUNC_RESET_CONTROLLER      "vSetResetControllerProcAddress"
#define NAME_FUNC_CONTROLLER_MODE       "vSetControllerModeProcAddress"
#define NAME_FUNC_ONLINE                "vSetGoOnlineProcAddress"
#define NAME_FUNC_OFFLINE               "vSetGoOfflineProcAddress"
#define NAME_FUNC_DISCONNECT            "vSetDisconnectProcAddress"
#define NAME_FUNC_CONNECT               "vSetConnectProcAddress"
