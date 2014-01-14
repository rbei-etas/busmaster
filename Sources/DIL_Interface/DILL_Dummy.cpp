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
#include "DILL_Dummy.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Cluster.h"

HRESULT CDIL_LIN_DUMMY::LIN_PerformInitOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_PerformClosureOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_GetTimeModeMapping(SYSTEMTIME& /*CurrSysTime*/, UINT64& /*TimeStamp*/, LARGE_INTEGER* /*QueryTickCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_ListHwInterfaces(INTERFACE_HW_LIST& /*asSelHwInterface*/, INT& /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_DeselectHwInterface(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS /*InitData*/, int& /*Length*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_SetConfigData(ClusterConfig& ouConfig)
{
    return WARN_DUMMY_API;
}

/*HRESULT CDIL_LIN_DUMMY::LIN_Connect(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_Disconnect(void)
{
    return WARN_DUMMY_API;
}*/

HRESULT CDIL_LIN_DUMMY::LIN_StartHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_PreStartHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_StopHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_ResetHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_GetCurrStatus(s_STATUSMSG& /*StatusData*/)
{
    return WARN_DUMMY_API;
}



HRESULT CDIL_LIN_DUMMY::LIN_SendMsg(DWORD /*dwClientID*/, STLIN_MSG& /*sLinTxMsg*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_SetSlaveRespData(const STLIN_MSG /*sLinTxMsg*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_ResetSlaveRespData(void)
{
    return WARN_DUMMY_API;
}
HRESULT CDIL_LIN_DUMMY::LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage)
{
    return WARN_DUMMY_API;
}


HRESULT CDIL_LIN_DUMMY::LIN_GetLastErrorString(string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_FilterFrames(FILTER_TYPE /*FilterType*/, TYPE_CHANNEL /*Channel*/, UINT* /*punMsgIds*/, UINT /*nLength*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_ManageMsgBuf(BYTE /*bAction*/, DWORD /*ClientID*/, CBaseLINBufFSE* /*pBufObj*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_RegisterClient(BOOL /*bRegister*/, DWORD& /*ClientID*/, char* /*pacClientName*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_GetControllerParams(LONG& /*lParam*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}
//MVN
HRESULT CDIL_LIN_DUMMY::LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    return WARN_DUMMY_API;
}
//~MVN
HRESULT CDIL_LIN_DUMMY::LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_GetErrorCount(SERROR_CNT& /*sErrorCnt*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_LoadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_LIN_DUMMY::LIN_UnloadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}