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
 * \file      Export_UserDll.h
 * \brief     This header file contains name and prototype of the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains name and prototype of the
 */

#pragma once

typedef int (*LOGENABLE) (BOOL);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLLOGPROC)(LOGENABLE);

typedef int (*WRITETOLOGFILE) (char*);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLLOGFILEPROC)(WRITETOLOGFILE);

typedef int (*WRITETOTRACE) (char*);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLTRACEPROC)(WRITETOTRACE);

typedef int (*STARTSTOPTIMER) (char*,int,BOOL,HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLSTARTSTOPTIMERPROC)(STARTSTOPTIMER);

typedef BOOL (*SETTIMERVAL) (CHAR*,UINT,HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLSETTIMERVAL)( SETTIMERVAL);

typedef int (*MSGTXONOFF) (BOOL,HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLMSGTXONOFF)(MSGTXONOFF);

typedef HMODULE (*GETNODEHANDLER) (char*);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLGETNODEHANDLER)(GETNODEHANDLER);

typedef int (*KEYPRESSED)();
typedef void (*SETKEYPRESSED)(KEYPRESSED);

typedef int (*GETMESSAGENAME)(DWORD, DWORD , char* ,DWORD );
typedef void (*DLLGETMESSAGENAME)(GETMESSAGENAME);

typedef unsigned int (*TIMENOW)();
typedef void (*DLLTIMENOW)(TIMENOW);

typedef DWORD (*GETFIRSTCANDBNAME)(char [], DWORD);
typedef void (*DLLGETFIRSTCANDBNAME)(GETFIRSTCANDBNAME);

// Name of the exported function from user-defined DLL
#define NAME_FUNC_LOG_DISABLE           "vSetDisableLoggingProcAddress"
#define NAME_FUNC_LOG_ENABLE            "vSetEnableLoggingProcAddress"
#define NAME_FUNC_LOG_FILE              "vSetWriteToLogFileProcAddress"
#define NAME_FUNC_TRACE                 "vSetTraceProcAddress"
#define NAME_FUNC_START_TIMER           "vSetStartTimerProcAddress"
#define NAME_FUNC_STOP_TIMER            "vSetStopTimerProcAddress"
#define defNAME_FUNC_SET_TIMER_VAL      "vSetSetTimerValProcAddress"
#define defNAME_FUNC_GET_PRG_VER        "bGetProgramVersion"
#define NAME_FUNC_MSGTX_ON_OFF          "vSetEnableDisableMsgTxProcAddress"
#define NAME_FUNC_SETDLLHANDLE          "vSetGetDllHandleProcAddress"
#define NAME_FUNC_GETENGVALUE           "vSetGetEngValueProcAddress"
#define NAME_FUNC_GETRAWVALUE           "vSetGetRawValueProcAddress"
