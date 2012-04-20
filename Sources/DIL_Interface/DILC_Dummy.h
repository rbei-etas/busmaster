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
 * \authors   Ratnadip Choudhury, Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for dummy DIL functions
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_DataTypes.h"
#include "BaseDIL_CAN_Controller.h"

using namespace std;

class CDIL_CAN_DUMMY : public CBaseDIL_CAN_Controller
{
public:
  /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
	HRESULT CAN_PerformInitOperations(void);
	HRESULT CAN_PerformClosureOperations(void);
	HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
	HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
	HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
	HRESULT CAN_DeselectHwInterface(void);
	HRESULT CAN_DisplayConfigDlg(PCHAR& InitData, int& Length);
	HRESULT CAN_SetConfigData(PCHAR pInitData, int Length);
	HRESULT CAN_StartHardware(void);
	HRESULT CAN_StopHardware(void);
	HRESULT CAN_ResetHardware(void);
	HRESULT CAN_GetCurrStatus(s_STATUSMSG& StatusData);
	HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
	HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
	HRESULT CAN_GetBoardInfo(s_BOARDINFO& BoardInfo);
	HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
	HRESULT CAN_GetVersionInfo(VERSIONINFO& sVerInfo);
	HRESULT CAN_GetLastErrorString(string& acErrorStr);
	HRESULT CAN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
	HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
	HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

	// Specific function set		
	HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);	
	HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
	HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName);
	HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
	HRESULT CAN_LoadDriverLibrary(void);
	HRESULT CAN_UnloadDriverLibrary(void);
};
