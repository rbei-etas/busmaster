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
 * \file      DIL_Interface/DILC_Dummy.cpp
 * \brief     Source file for dummy DIL functions
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for dummy DIL functions
 */
#include "DIL_Interface_stdafx.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"

HRESULT DILC_Dummy_PerformInitOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_PerformClosureOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetTimeModeMapping(SYSTEMTIME& /*CurrSysTime*/, UINT64& /*TimeStamp*/, LARGE_INTEGER* /*QueryTickCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_ListHwInterfaces(INTERFACE_HW_LIST& /*asSelHwInterface*/, INT& /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_DeselectHwInterface(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_DisplayConfigDlg(PCHAR& /*InitData*/, int& /*Length*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_SetConfigData(PCHAR /*pInitData*/, int /*Length*/)
{
    return WARN_DUMMY_API;
}

/*HRESULT DILC_Dummy_Connect(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_Disconnect(void)
{
    return WARN_DUMMY_API;
}*/

HRESULT DILC_Dummy_StartHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_StopHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_ResetHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetCurrStatus(s_STATUSMSG& /*StatusData*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_SendMsg(DWORD /*dwClientID*/, const STCAN_MSG& /*sCanTxMsg*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetBoardInfo(s_BOARDINFO& /*BoardInfo*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetVersionInfo(VERSIONINFO& /*sVerInfo*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetLastErrorString(CHAR* /*acErrorStr*/, int /*nLength*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_FilterFrames(FILTER_TYPE /*FilterType*/, TYPE_CHANNEL /*Channel*/, UINT* /*punMsgIds*/, UINT /*nLength*/)
{
	return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_ManageMsgBuf(BYTE /*bAction*/, DWORD /*ClientID*/, CBaseCANBufFSE* /*pBufObj*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_RegisterClient(BOOL /*bRegister*/, DWORD& /*ClientID*/, TCHAR* /*pacClientName*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetControllerParams(LONG& /*lParam*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

HRESULT DILC_Dummy_GetErrorCount(SERROR_CNT& /*sErrorCnt*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}