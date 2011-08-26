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
 * \file      DILC_Dummy.h
 * \brief     Declaration file for dummy DIL functions
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for dummy DIL functions
 */

// Dummy.h : Declaration of the functions

HRESULT DILC_Dummy_PerformInitOperations(void);
HRESULT DILC_Dummy_PerformClosureOperations(void);
HRESULT DILC_Dummy_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
HRESULT DILC_Dummy_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
HRESULT DILC_Dummy_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
HRESULT DILC_Dummy_DeselectHwInterface(void);
HRESULT DILC_Dummy_DisplayConfigDlg(PCHAR& InitData, int& Length);
HRESULT DILC_Dummy_SetConfigData(PCHAR pInitData, int Length);
HRESULT DILC_Dummy_StartHardware(void);
HRESULT DILC_Dummy_StopHardware(void);
HRESULT DILC_Dummy_ResetHardware(void);
HRESULT DILC_Dummy_GetCurrStatus(s_STATUSMSG& StatusData);
HRESULT DILC_Dummy_GetTxMsgBuffer(BYTE*& pouTxMsgBuffer);
HRESULT DILC_Dummy_SendMsg(DWORD dwClientID, const STCAN_MSG& pouTxMsg);
HRESULT DILC_Dummy_GetBoardInfo(s_BOARDINFO& BoardInfo);
HRESULT DILC_Dummy_GetBusConfigInfo(BYTE* BusInfo);
HRESULT DILC_Dummy_GetVersionInfo(VERSIONINFO& sVerInfo);
HRESULT DILC_Dummy_GetLastErrorString(CHAR* acErrorStr, int nLength);
HRESULT DILC_Dummy_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL FlexChannel, UINT* punFrames, UINT nLength);
HRESULT DILC_Dummy_ManageMsgBuf(BYTE, DWORD ClientID, CBaseCANBufFSE*);
HRESULT DILC_Dummy_RegisterClient(BOOL, DWORD& ClientID, TCHAR* pacClientName);
HRESULT DILC_Dummy_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
HRESULT DILC_Dummy_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
HRESULT DILC_Dummy_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
