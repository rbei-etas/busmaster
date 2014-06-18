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
 * \file      CFilesDefs_CAN.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#define STRUCT_FILE                    "CANIncludes.h"
#define MSG_STRUCT_NAME                "STCAN_MSG";
#define MSG_STRUCT_NAME_LIN            "STLIN_MSG";
#define WRAPPER_NAME                   "Wrapper.o";
#define STRUCT_FILE_J1939              "J1939Includes.h"
#define STRUCT_FILE_LIN                "LINIncludes.h"
#define MSG_STRUCT_NAME_J1939          "STJ1939_MSG";
#define WRAPPER_NAME_J1939             "Wrapper_J1939.o";
#define WRAPPER_NAME_LIN               "Wrapper_LIN.o";
#define TOTAL_API_COUNT                18
#define TOTAL_API_COUNT_LIN            11
#define TOTAL_ERROR_COUNT              5

static const CString sg_omAPIFuncList[TOTAL_API_COUNT] =
{
    "UINT EnableLogging()",
    "UINT DisableLogging()",
    "UINT WriteToLogFile(char*)",
    "UINT Connect(DWORD)",
    "UINT Disconnect(DWORD)",
    "UINT GoOnline()",
    "UINT GoOffline()",
    "UINT StartTimer(char*, UINT)",
    "UINT StopTimer(char*)",
    "BOOL SetTimerVal(char*, UINT)",
    "BOOL EnableMsgHandlers(BOOL)",
    "BOOL EnableErrorHandlers(BOOL)",
    "BOOL EnableKeyHandlers(BOOL)",
    "BOOL EnableDisableMsgTx(BOOL)",
    "UINT SendMsg(STCAN_MSG)",
    "HMODULE GetDllHandle(char*)",
    "UINT Trace(char*, ...)"
};

static const CString sg_omAPIFuncNames[TOTAL_API_COUNT] =
{
    "EnableLogging",
    "DisableLogging",
    "WriteToLogFile",
    "Connect",
    "Disconnect",
    "GoOnline",
    "GoOffline",
    "StartTimer",
    "StopTimer",
    "SetTimerVal",
    "EnableMsgHandlers",
    "EnableErrorHandlers",
    "EnableKeyHandlers",
    "EnableDisableMsgTx",
    "SendMsg",
    "GetDllHandle",
    "Trace"
};

static const CString sg_omAPIFuncListLIN[TOTAL_API_COUNT_LIN] =
{
    "UINT Connect(DWORD)",
    "UINT Disconnect(DWORD)",
    "UINT StartTimer(char*, UINT)",
    "UINT StopTimer(char*)",
    "BOOL SetTimerVal(char*, UINT)",
    "BOOL EnableMsgHandlers(BOOL)",
    "BOOL EnableErrorHandlers(BOOL)",
    "BOOL EnableKeyHandlers(BOOL)",
    "UINT SendMsg(STLIN_MSG)",
    "HMODULE GetDllHandle(char*)",
    "UINT Trace(char*, ...)"
};

static const CString sg_omAPIFuncNamesLIN[TOTAL_API_COUNT_LIN] =
{
    "Connect",
    "Disconnect",
    "StartTimer",
    "StopTimer",
    "SetTimerVal",
    "EnableMsgHandlers",
    "EnableErrorHandlers",
    "EnableKeyHandlers",
    "SendMsg",
    "GetDllHandle",
    "Trace"
};

#define TOTAL_API_COUNT_J1939 21
static const CString sg_omAPIFuncListJ1939[TOTAL_API_COUNT_J1939] =
{
    "UINT EnableLogging()",
    "UINT DisableLogging()",
    "UINT WriteToLogFile(char*)",
    "UINT GoOnline()",
    "UINT GoOffline()",
    "UINT StartTimer(char*, UINT)",
    "UINT StopTimer(char*)",
    "BOOL SetTimerVal(char*, UINT)",
    "BOOL EnableAllHandlers(BOOL)",
    "BOOL EnableMsgHandlers(BOOL)",
    "BOOL EnableKeyHandlers(BOOL)",
    "BOOL EnableDisableMsgTx(BOOL)",
    "UINT SendMsg(STJ1939_MSG* psMsg)",
    "UINT RequestPGN(UINT PGN, BYTE DestAdres, UINT Channel)",
    "UINT SendAckMsg(UINT PGN, BYTE AckType, BYTE DestAdres, UINT Channel)",
    "UINT ClaimAddress(BYTE Address, UINT Channel)",
    "UINT RequestAddress(UINT Channel)",
    "UINT CommandAddress(UINT64 EcuName, BYTE NewAddress, UINT Channel)",
    "UINT SetTimeout(BYTE TimeoutType, UINT TimeoutValue)",
    "HMODULE GetDllHandle(char*)",
    "UINT Trace(char*, ...)"
};

static const CString sg_omAPIFuncNamesJ1939[TOTAL_API_COUNT_J1939] =
{
    "EnableLogging",
    "DisableLogging",
    "WriteToLogFile",
    "GoOnline",
    "GoOffline",
    "StartTimer",
    "StopTimer",
    "SetTimerVal",
    "EnableAllHandlers",
    "EnableMsgHandlers",
    "EnableKeyHandlers",
    "EnableDisableMsgTx",
    "SendMsg",
    "RequestPGN",
    "SendAckMsg",
    "ClaimAddress",
    "RequestAddress",
    "CommandAddress",
    "SetTimeout",
    "GetDllHandle",
    "Trace"
};


static const CString sg_omMcNetErrorHandlerList[TOTAL_ERROR_COUNT] = {"Error Active", "Error Passive", "Warning Limit", "Bus Off", "Error Frame"};
