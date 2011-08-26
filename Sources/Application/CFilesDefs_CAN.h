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
#if !defined CFILESDEFS_CAN_H__INCLUDED_
#define CFILESDEFS_CAN_H__INCLUDED_

#define STRUCT_FILE                    _T("struct.h")
#define MSG_STRUCT_NAME                _T("STCAN_MSG");
#define WRAPPER_NAME                   _T("Wrapper.o");
#define TOTAL_API_COUNT                18
#define TOTAL_ERROR_COUNT              5 

static const CString sg_omAPIFuncList[TOTAL_API_COUNT] = 
{
    _T("UINT EnableLogging()"), 
    _T("UINT DisableLogging()"), 
    _T("UINT WriteToLogFile(char*)"), 
    _T("UINT Connect(DWORD)"),
    _T("UINT Disconnect(DWORD)"),
    _T("UINT GoOnline()"),
    _T("UINT GoOffline()"),
    _T("UINT StartTimer(char*, UINT)"),
    _T("UINT StopTimer(char*)"),
    _T("BOOL SetTimerVal(char*, UINT)"),
    _T("BOOL EnableMsgHandlers(BOOL)"),
    _T("BOOL EnableErrorHandlers(BOOL)"),
    _T("BOOL EnableKeyHandlers(BOOL)"),
    _T("BOOL EnableDisableMsgTx(BOOL)"),
    _T("UINT SendMsg(STCAN_MSG)"),
    _T("HMODULE GetDllHandle(char*)"),
    _T("UINT Trace(char*, ...)"),
    _T("void ResetController(BOOL)")
};

static const CString sg_omAPIFuncNames[TOTAL_API_COUNT] = 
{
    _T("EnableLogging"), 
    _T("DisableLogging"), 
    _T("WriteToLogFile"), 
    _T("Connect"),
    _T("Disconnect"),
    _T("GoOnline"),
    _T("GoOffline"),
    _T("StartTimer"),
    _T("StopTimer"),
    _T("SetTimerVal"),
    _T("EnableMsgHandlers"),
    _T("EnableErrorHandlers"),
    _T("EnableKeyHandlers"),
    _T("EnableDisableMsgTx"),
    _T("SendMsg"),
    _T("GetDllHandle"),
    _T("Trace"),
    _T("ResetController")
};

static const CString sg_omMcNetErrorHandlerList[TOTAL_ERROR_COUNT] = {_T("Error Active"), _T("Error Passive"), _T("Warning Limit"), _T("Bus Off"), _T("Error Frame")};
#endif //CFILESDEFS_CAN_H__INCLUDED_