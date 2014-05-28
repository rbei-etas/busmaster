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
 * @file  DIL_CAN_IXXAT_VCI.h
 *
 * @brief
 *      Implements the access to IXXAT VCI (Windows driver) based interfaces.
 * @remarks
 *    The available CAN channels will be stored in the m_arrInternalChannel array.
 *    This is necessary because the VCI works with 64 bit values and the Busmaster
 *    works with 32 bit values for the device access.
 */
#pragma once

#include "CAN_IXXAT_VCI_stdafx.h"
#include "AllIncludes.h"
#include "ClientList.h"
#include "IxxatCanChannel.h"



// forward declaration to make it possible to compile
// without including files outside the normal
// Busmaster includes paths
typedef struct _VCIDEVICEINFO VCIDEVICEINFO;



/**
 * @class CDIL_CAN_IXXAT_VCI
 *
 * @brief
 *  This class is the DIL interface for IXXAT VCI based
 *  CAN interfaces.
 *
 */
class CDIL_CAN_IXXAT_VCI : CBaseDIL_CAN_Controller
{
public:
    CDIL_CAN_IXXAT_VCI();
    ~CDIL_CAN_IXXAT_VCI();
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT CAN_PerformInitOperations(void);
    HRESULT CAN_PerformClosureOperations(void);
    HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
    HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT CAN_DeselectHwInterface(void);
    HRESULT CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_GetCurrStatus(s_STATUSMSG& StatusData);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName);
    HRESULT CAN_ManageMsgBuf(BYTE byAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    void    LogMessage(BOOL bShowOnlyInDebug, LPCTSTR pFormat, ...);

protected:

    BOOL m_bDriverAccessLoaded; ///< flag to indicate wether the IXXAT VCI functions has loaded
    int  m_iNumberOfCANChannelsTotal; ///< Number of all available IXXAT CAN channels


    Base_WrapperErrorLogger* m_pILog; ///< The error logger object
    HWND m_hOwnerWndHandle; ///< The owner window handle

    CClientList m_ClientList; ///< List with all of registered clients

    /**
     * Starts code for the state machine
     */
    enum
    {
        STATE_HARDWARE_NONE = 0x0,
        STATE_DRIVER_LOADED,
        STATE_HW_INTERFACE_LISTED,
        STATE_HW_INTERFACE_SELECTED,
        STATE_CONNECTED
    };

    BYTE m_byCurrHardwareState; // <- current state of the hardware access

    CIxxatCanChannel m_arrIxxatCanChannels[CHANNEL_CAN_MAX];    ///< internal can channel list
    CIxxatCanChannel m_arrTmpIxxatCanChannels[CHANNEL_CAN_MAX];  ///< internal can channel list Temp

    INTERFACE_HW_LIST m_sSelHwInterface;
    INT m_anSelectedItems[CHANNEL_ALLOWED];

    int  VciDeviceInfoAddToArray(int iStartPosArray, VCIDEVICEINFO* pVciDevInfo, INTERFACE_HW_LIST& sSelHwInterface);

private:
    SYSTEMTIME    m_sCurrSysTime; ///< Current system time, set in CAN_SetAppParams()
    UINT64        m_qwTimeStamp;  ///< The time stamp variable, not used.
    long long int m_qwQueryTickCount; ///< The query tick count, not used.
};

