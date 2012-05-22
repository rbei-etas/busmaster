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
 * \file      BaseDIL_CAN_Controller.h
 * \brief     Base class for CDIL_CAN class.
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CAN controller classes.
 */

#if !defined BASEDIL_CAN_CONTROLLER_H__INCLUDED_
#define BASEDIL_CAN_CONTROLLER_H__INCLUDED_

class CBaseDIL_CAN_Controller
{
public:
	virtual HRESULT CAN_PerformInitOperations(void) = 0;
	virtual HRESULT CAN_PerformClosureOperations(void) = 0;
	virtual HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;
	virtual HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;
	virtual HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;
	virtual HRESULT CAN_DeselectHwInterface(void) = 0;
	virtual HRESULT CAN_DisplayConfigDlg(PCHAR& InitData, int& Length) = 0;
	virtual HRESULT CAN_SetConfigData(PCHAR pInitData, int Length) = 0;
	virtual HRESULT CAN_StartHardware(void) = 0;
	virtual HRESULT CAN_StopHardware(void) = 0;
	virtual HRESULT CAN_ResetHardware(void) = 0;
	virtual HRESULT CAN_GetCurrStatus(s_STATUSMSG& StatusData) = 0;
	virtual HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer) = 0;
	virtual HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg) = 0;
	virtual HRESULT CAN_GetBoardInfo(s_BOARDINFO& BoardInfo) = 0;
	virtual HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo) = 0;
	virtual HRESULT CAN_GetVersionInfo(VERSIONINFO& sVerInfo) = 0;
	virtual HRESULT CAN_GetLastErrorString(CHAR* acErrorStr, int nLength) = 0;
	virtual HRESULT CAN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength) = 0;
	virtual HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
	virtual HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

	// Specific function set	
	virtual HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;
	virtual HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj) = 0;
	virtual HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName) = 0;
	virtual HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;
	virtual HRESULT CAN_LoadDriverLibrary(void) = 0;
	virtual HRESULT CAN_UnloadDriverLibrary(void) = 0;
};

#endif // BASEDIL_CAN_CONTROLLER_H__INCLUDED_