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
 * \file      BaseDIL_Ethernet_Controller.h
 * \brief     Base class for CDIL_Ethernet class.
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for Ethernet controller classes.
 */

#if !defined BASEDIL_ETHERNET_CONTROLLER_H__INCLUDED_
#define BASEDIL_ETHERNET_CONTROLLER_H__INCLUDED_

#include "DataTypes/DIL_DataTypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"

class CBaseDIL_Ethernet_Controller
{

	public:
    virtual HRESULT Ethernet_PerformInitOperations(void) = 0;
    virtual HRESULT Ethernet_PerformClosureOperations(void) = 0;
    virtual HRESULT Ethernet_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;
    virtual HRESULT Ethernet_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;
    virtual HRESULT Ethernet_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;
    virtual HRESULT Ethernet_DeselectHwInterface(void) = 0;
    virtual HRESULT Ethernet_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) = 0;
    virtual HRESULT Ethernet_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length) = 0;
    virtual HRESULT Ethernet_StartHardware(void) = 0;
    virtual HRESULT Ethernet_StopHardware(void) = 0;
    //virtual HRESULT Ethernet_GetCurrStatus(s_STATUSMSG& StatusData) = 0;
    virtual HRESULT Ethernet_SendMsg(DWORD dwClientID, const STETHERNET_MSG& sCanTxMsg) = 0;
    virtual HRESULT Ethernet_GetLastErrorString(string& acErrorStr) = 0;
    //virtual HRESULT Ethernet_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    //MVN
    virtual HRESULT Ethernet_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) = 0;
    //~MVN
    virtual HRESULT Ethernet_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

    // Specific function set
    virtual HRESULT Ethernet_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;
    virtual HRESULT Ethernet_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseEthernetBufFSE* pBufObj) = 0;
    virtual HRESULT Ethernet_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;
    virtual HRESULT Ethernet_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;
    virtual HRESULT Ethernet_LoadDriverLibrary(void) = 0;
    virtual HRESULT Ethernet_UnloadDriverLibrary(void) = 0;
	virtual HMODULE Ethernet_GetCustomProtocolHandle() = 0;
};

#endif // BASEDIL_ETHERNET_CONTROLLER_H__INCLUDED_