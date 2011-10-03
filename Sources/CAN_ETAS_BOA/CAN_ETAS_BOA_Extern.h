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
 * \file      CAN_ETAS_BOA/CAN_ETAS_BOA_Extern.h
 * \brief     Exports API functions for BOA CAN Hardware interface
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exports API functions for BOA CAN Hardware interface
 */
#if !defined CAN_ETAS_BOA_EXTERN_H__INCLUDED_
#define CAN_ETAS_BOA_EXTERN_H__INCLUDED_


#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

/*  Exported function list */
USAGEMODE HRESULT CAN_ETAS_BOA_PerformInitOperations(void);
USAGEMODE HRESULT CAN_ETAS_BOA_PerformClosureOperations(void);
USAGEMODE HRESULT CAN_ETAS_BOA_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
USAGEMODE HRESULT CAN_ETAS_BOA_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
USAGEMODE HRESULT CAN_ETAS_BOA_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
USAGEMODE HRESULT CAN_ETAS_BOA_DeselectHwInterface(void);
USAGEMODE HRESULT CAN_ETAS_BOA_DisplayConfigDlg(PCHAR& InitData, int& Length);
USAGEMODE HRESULT CAN_ETAS_BOA_SetConfigData(PCHAR pInitData, int Length);
USAGEMODE HRESULT CAN_ETAS_BOA_StartHardware(void);
USAGEMODE HRESULT CAN_ETAS_BOA_StopHardware(void);
USAGEMODE HRESULT CAN_ETAS_BOA_ResetHardware(void);
USAGEMODE HRESULT CAN_ETAS_BOA_GetCurrStatus(s_STATUSMSG& StatusData);
USAGEMODE HRESULT CAN_ETAS_BOA_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
USAGEMODE HRESULT CAN_ETAS_BOA_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
USAGEMODE HRESULT CAN_ETAS_BOA_GetBoardInfo(s_BOARDINFO& BoardInfo);
USAGEMODE HRESULT CAN_ETAS_BOA_GetBusConfigInfo(BYTE* BusInfo);
USAGEMODE HRESULT CAN_ETAS_BOA_GetVersionInfo(VERSIONINFO& sVerInfo);
USAGEMODE HRESULT CAN_ETAS_BOA_GetLastErrorString(CHAR* acErrorStr, int nLength);
USAGEMODE HRESULT CAN_ETAS_BOA_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
USAGEMODE HRESULT CAN_ETAS_BOA_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
USAGEMODE HRESULT CAN_ETAS_BOA_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

// Specific function set
USAGEMODE HRESULT CAN_ETAS_BOA_LoadDriverLibrary(void);
USAGEMODE HRESULT CAN_ETAS_BOA_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
USAGEMODE HRESULT CAN_ETAS_BOA_UnloadDriverLibrary(void);
USAGEMODE HRESULT CAN_ETAS_BOA_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
USAGEMODE HRESULT CAN_ETAS_BOA_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName);
USAGEMODE HRESULT CAN_ETAS_BOA_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);



#ifdef __cplusplus
}
#endif

#endif // CAN_ETAS_BOA_EXTERN_H__INCLUDED_