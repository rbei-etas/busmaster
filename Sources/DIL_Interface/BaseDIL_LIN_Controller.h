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
 * \file      BaseDIL_LIN_Controller.h
 * \brief     Base class for CDIL_LIN class.
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for LIN controller classes.
 */

#if !defined BASEDIL_LIN_CONTROLLER_H__INCLUDED_
#define BASEDIL_LIN_CONTROLLER_H__INCLUDED_
#include "DataTypes\Cluster.h"
#include "DataTypes\DIL_Datatypes.h"
#include "DataTypes\MsgBufAll_DataTypes.h"
#include "DataTypes\Base_WrapperErrorLogger.h"

class CBaseDIL_LIN_Controller
{
public:
    virtual HRESULT LIN_PerformInitOperations(void) = 0;
    virtual HRESULT LIN_PerformClosureOperations(void) = 0;
    virtual HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;
    virtual HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;
    virtual HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;
    virtual HRESULT LIN_DeselectHwInterface(void) = 0;
    virtual HRESULT LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) = 0;
    virtual HRESULT LIN_SetConfigData(ClusterConfig& ouConfig) = 0;
    virtual HRESULT LIN_StartHardware(void) = 0;
    virtual HRESULT LIN_PreStartHardware(void) = 0;
    virtual HRESULT LIN_StopHardware(void) = 0;
    virtual HRESULT LIN_ResetHardware(void) = 0;
    virtual HRESULT LIN_GetCurrStatus(s_STATUSMSG& StatusData) = 0;
    virtual HRESULT LIN_SendMsg(DWORD dwClientID, STLIN_MSG& sCanTxMsg) = 0;
    virtual HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg) = 0;
    virtual HRESULT LIN_ResetSlaveRespData(void) = 0;
    virtual HRESULT LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage) = 0;
    virtual HRESULT LIN_GetLastErrorString(string& acErrorStr) = 0;
    virtual HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    //MVN
    virtual HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) = 0;
    //~MVN
    virtual HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

    // Specific function set
    virtual HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;
    virtual HRESULT LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj) = 0;
    virtual HRESULT LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;
    virtual HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;
    virtual HRESULT LIN_LoadDriverLibrary(void) = 0;
    virtual HRESULT LIN_UnloadDriverLibrary(void) = 0;
};

#endif // BASEDIL_LIN_CONTROLLER_H__INCLUDED_