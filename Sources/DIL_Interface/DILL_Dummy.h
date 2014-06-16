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
 * \file      DIL_Interface/DILC_Dummy.h
 * \brief     Declaration file for dummy DIL functions
 * \authors   Ratnadip Choudhury, Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for dummy DIL functions
 */
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_DataTypes.h"
#include "BaseDIL_LIN_Controller.h"
/* CDIL_LIN_DUMMY class definition */
class CDIL_LIN_DUMMY : public CBaseDIL_LIN_Controller
{
public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT LIN_PerformInitOperations(void);
    HRESULT LIN_PerformClosureOperations(void);
    HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT LIN_DeselectHwInterface(void);
    HRESULT LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length);
    HRESULT LIN_SetConfigData(ClusterConfig& ouConfig);
    HRESULT LIN_StartHardware(void);
    HRESULT LIN_PreStartHardware(void);
    HRESULT LIN_StopHardware(void);
    HRESULT LIN_ResetHardware(void);
    HRESULT LIN_GetCurrStatus(s_STATUSMSG& StatusData);
    HRESULT LIN_SendMsg(DWORD dwClientID, STLIN_MSG& sCanTxMsg);
    HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg);
    HRESULT LIN_ResetSlaveRespData(void);
    HRESULT LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage);
    HRESULT LIN_GetLastErrorString(std::string& acErrorStr);
    HRESULT LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
    HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_GetConfiguration(sCONTROLLERDETAILSLIN[], INT& nSize);
    HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj);
    HRESULT LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT LIN_LoadDriverLibrary(void);
    HRESULT LIN_UnloadDriverLibrary(void);

    //Schedule table
    HRESULT LIN_RegisterLinScheduleTable(DWORD& dwClientId, int& nChannel, CSheduleTable ouTable, int& nHandle);
    HRESULT LIN_DeRegisterLinScheduleTabel(DWORD& dwClientId, int& nChannel, int& nTableHandle);
    HRESULT LIN_StartLinScheduleTable(DWORD& dwClientId, int& nChannel, int& nTableHandle);
    HRESULT LIN_EnableLinScheuleCommand( DWORD& dwClientId, int& nChannel, int nTableHandle, int nIndex, bool bEnable );
    HRESULT LIN_UpdateLinScheduleTable( DWORD& dwClientId, int& nChannel, int& nHandle, CSheduleTable& ouTable );

    //Individual Header commands.
    HRESULT LIN_RegisterLinHeader( DWORD& dwClientId, int& nChannel, int nId, int nCycleTimer );
    HRESULT LIN_DeRegisterLinHeader(DWORD& dwClientId, int& nChannel, int nId);
};