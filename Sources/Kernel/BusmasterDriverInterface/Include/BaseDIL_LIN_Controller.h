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
 * @brief Base class for CDIL_LIN class.
 * @author Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for LIN controller classes.
 */

#pragma once

#include "LINDriverDefines.h"
#include "../../BusmasterDBNetwork/Include/IBMNetWorkGetService.h"
class CBaseDIL_LIN_Controller
{
public:
    virtual HRESULT LIN_PerformInitOperations(void) = 0;
    virtual HRESULT LIN_PerformClosureOperations(void) = 0;
    virtual HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount) = 0;
    virtual HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;
    virtual HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;
    virtual HRESULT LIN_DeselectHwInterface(void) = 0;

    virtual HRESULT LIN_SetConfigData(IBMNetWorkGetService* ouConfig) = 0;
    virtual HRESULT LIN_StartHardware(void) = 0;
    virtual HRESULT LIN_PreStartHardware(void) = 0;
    virtual HRESULT LIN_StopHardware(void) = 0;
    virtual HRESULT LIN_ResetHardware(void) = 0;
    virtual HRESULT LIN_GetCurrStatus( STATUSMSG& StatusData ) = 0;
    virtual HRESULT LIN_SendMsg(DWORD dwClientID, STLIN_MSG& sCanTxMsg) = 0;
    virtual HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg) = 0;
    virtual HRESULT LIN_ResetSlaveRespData(void) = 0;
    virtual HRESULT LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage) = 0;
    virtual HRESULT LIN_GetLastErrorString(std::string& acErrorStr) = 0;
    virtual HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    virtual HRESULT LIN_GetConfiguration( SCONTROLLER_DETAILS_LIN[], int& nCount ) = 0;
    virtual HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) = 0;
    virtual HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    virtual HRESULT LIN_SetAppParams(HWND hWndOwner) = 0;
    virtual HRESULT LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj) = 0;
    virtual HRESULT LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;
    virtual HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;
    virtual HRESULT LIN_LoadDriverLibrary(void) = 0;
    virtual HRESULT LIN_UnloadDriverLibrary(void) = 0;


    //Schedule table
    virtual HRESULT LIN_RegisterLinScheduleTable(DWORD& dwClientId, int& nChannel, CSheduleTable ouTable, int& nHandle) = 0;
    virtual HRESULT LIN_DeRegisterLinScheduleTabel(DWORD& dwClientId, int& nChannel, int& nTableHandle) = 0;
    virtual HRESULT LIN_StartLinScheduleTable(DWORD& dwClientId, int& nChannel, int& nTableHandle) =0;
    virtual HRESULT LIN_EnableLinScheuleCommand( DWORD& dwClientId, int& nChannel, int nTableHandle, int nIndex, bool bEnable ) =0;
    virtual HRESULT LIN_UpdateLinScheduleTable( DWORD& dwCliendId, int& nChannel, int& nHandle, CSheduleTable& ouTable ) =0;

    //Individual Header commands.
    virtual HRESULT LIN_RegisterLinHeader( DWORD& dwClientId, int& nChannel, int nId, int nCycleTimer ) = 0;
    virtual HRESULT LIN_DeRegisterLinHeader( DWORD& dwClientId, int& nChannel, int nId) = 0;
};
