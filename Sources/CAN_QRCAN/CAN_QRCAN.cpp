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
 * \file      CAN_QRCAN.cpp
 * \author    
 * \copyright Copyright (c) ((Year)), QRTECH AB. All rights reserved.
 *
 * Implementation of QRCAN
 */

/* C++ includes */
#include <string>
#include <vector>

/* Project includes */
#include "CAN_QRCAN_stdafx.h"
#include "CAN_QRCAN.h"
#include "include/Error.h"
#include "Include/Struct_CAN.h"
#include "Include/BaseDefs.h"
#include "Include/CanUSBDefs.h"
#include "Include/DIL_CommonDefs.h"

#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "BusEmulation/BusEmulation.h"
#include "BusEmulation/BusEmulation_i.c"
#include "DataTypes/DIL_DataTypes.h"
#include "Utility/Utility_Thread.h"
#include "Utility/Utility.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"
//#include "../Application/GettextBusmaster.h"

#include "Utility\MultiLanguageSupport.h"

#define USAGE_EXPORT
#include "CAN_QRCAN_Extern.h"

BEGIN_MESSAGE_MAP(CCAN_QRCAN, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_QRCAN construction
 */
CCAN_QRCAN::CCAN_QRCAN()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/**
 * The one and only CCAN_QRCAN object
 */
CCAN_QRCAN theApp;

/**
 * CCAN_QRCAN initialization
 */
BOOL CCAN_QRCAN::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

/**
 * Client and Client Buffer map
 */
#define MAX_CLIENT_ALLOWED 16
#define MAX_BUFF_ALLOWED 16

/**
 * Client and Client Buffer map
 */
class SCLIENTBUFMAP
{
public:
    DWORD dwClientID;
    HANDLE hClientHandle;
    HANDLE hPipeFileHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    std::string pacClientName;
    UINT unBufCount;
    SCLIENTBUFMAP()
    {
        dwClientID = 0;
        hClientHandle = nullptr;
        hPipeFileHandle = nullptr;
        unBufCount = 0;
        pacClientName = "";

        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = nullptr;
        }
    }
};
typedef SCLIENTBUFMAP* PSCLIENTBUFMAP;

/* QRCAN Variable Declarations */
static HWND sg_hOwnerWnd = nullptr;

/* CDIL_CAN_QRCAN class definition */
class CDIL_CAN_QRCAN : public CBaseDIL_CAN_Controller
{
public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT CAN_PerformInitOperations(void);
    HRESULT CAN_PerformClosureOperations(void);
    HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT CAN_DeselectHwInterface(void);
    HRESULT CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_GetCurrStatus(s_STATUSMSG& StatusData);
    HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);
};

static CDIL_CAN_QRCAN* g_pouDIL_CAN_QRCAN = nullptr;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult;

    hResult = S_OK;
    if (!g_pouDIL_CAN_QRCAN)
    {
        g_pouDIL_CAN_QRCAN = new CDIL_CAN_QRCAN;
        if (!(g_pouDIL_CAN_QRCAN))
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*)g_pouDIL_CAN_QRCAN;  /* Doesn't matter even if g_pouDIL_CAN_VSCOM is null */

    return(hResult);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_QRCAN::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
	sg_hOwnerWnd = hWndOwner;
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
HRESULT CDIL_CAN_QRCAN::CAN_UnloadDriverLibrary(void)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_QRCAN::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_OK;

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls
 */
HRESULT CDIL_CAN_QRCAN::CAN_RegisterClient(BOOL bRegister,DWORD& ClientID, char* pacClientName)
{
	HRESULT hResult = S_OK;
    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the controller status. hEvent will be registered
 * and will be set whenever there is change in the controller
 * status.
 */
HRESULT CDIL_CAN_QRCAN::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    HRESULT hResult = S_OK;
	(void)unCntrlStatus;
	(void)hEvent;

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
 */
HRESULT CDIL_CAN_QRCAN::CAN_LoadDriverLibrary(void)
{
    return S_OK;
}

/**
* \brief         Performs intial operations.
*                Initializes filter, queue, controller config with default values.
* \param         void
* \return        S_OK if the open driver call successfull otherwise S_FALSE
*/
HRESULT CDIL_CAN_QRCAN::CAN_PerformInitOperations(void)
{
    DWORD dwClientID;
    /* Register Monitor client */
    dwClientID = 0;
    CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE);

    return(S_OK);
}

/**
* \brief         Performs closure operations.
* \param         void
* \return        S_OK if the CAN_StopHardware call successfull otherwise S_FALSE
*/

HRESULT CDIL_CAN_QRCAN::CAN_PerformClosureOperations(void)
{
    return S_OK;
}

/**
* \brief         Gets the time mode mapping of the hardware. CurrSysTime
*                will be updated with the system time ref.
*                TimeStamp will be updated with the corresponding timestamp.
* \param[out]    CurrSysTime, is SYSTEMTIME structure
* \param[out]    TimeStamp, is UINT64
* \param[out]    QueryTickCount, is LARGE_INTEGER
* \return        S_OK for success
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    return S_OK;
}

/**
* \brief         Lists the hardware interface available.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
    return(S_OK);
}

/**
* \brief         Selects the hardware interface selected by the user.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*sSelHwInterface*/, INT /*nSize*/)
{
    return S_OK;
}

/**
* \brief         Deselects the selected hardware interface.
* \param         void
* \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_DeselectHwInterface(void)
{
    return S_OK;
}

/**
* \brief         Displays the controller configuration dialog.
* \param[out]    InitData, is SCONTROLLER_DETAILS structure
* \param[out]    Length , is INT
* \return        S_OK for success
*/
HRESULT CDIL_CAN_QRCAN::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
    HRESULT hResult = S_OK;

	ShowQRCANConfig(sg_hOwnerWnd);

	return hResult;
}

/**
* \brief         Sets the controller configuration data supplied by ConfigFile.
* \param[in]     ConfigFile, is SCONTROLLER_DETAILS structure
* \param[in]     Length , is INT
* \return        S_OK for success
*/
HRESULT CDIL_CAN_QRCAN::CAN_SetConfigData(PSCONTROLLER_DETAILS ConfigFile, int /*Length*/)
{
    return S_OK;
}

/**
* \brief         connects to the channels and initiates read thread.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_StartHardware(void)
{
    HRESULT hResult = S_OK;

    return hResult;
}

/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_StopHardware(void)
{
    HRESULT hResult = S_OK;

    return hResult;
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    return S_OK;
}

/**
* \brief         Gets the Tx queue configured.
* \param[out]    pouFlxTxMsgBuffer, is BYTE*
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return(S_OK);
}

/**
* \brief         Sends STCAN_MSG structure from the client dwClientID.
* \param[in]     dwClientID is the client ID
* \param[in]     sMessage is the application specific CAN message structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    HRESULT hResult = S_OK;

    return hResult;
}

/**
* \brief         Gets bus config info.
* \param[out]    BusInfo, is BYTE
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return(S_OK);
}

/**
* \brief         Gets last occured error and puts inside acErrorStr.
* \param[out]    acErrorStr, is CHAR contains error string
* \param[in]     nLength, is INT
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetLastErrorString(std::string& acErrorStr)
{
    return S_OK;
}

/**
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetControllerParams(LONG& lParam, UINT /*nChannel*/, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    
    return hResult;
}

HRESULT CDIL_CAN_QRCAN::CAN_SetControllerParams(int /* nValue */, ECONTR_PARAM /* eContrparam */)
{
    return S_OK;
}

/**
* \brief         Gets the error counter for corresponding channel.
* \param[out]    sErrorCnt, is SERROR_CNT structure
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT /* nChannel */, ECONTR_PARAM /* eContrParam */)
{
    return S_OK;
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = TRUE;

    return bExist;
}

static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;

    return bReturn;
}

/**
 * \return Returns true if found else false.
 *
 * unClientIndex will have index to client array which has clientId dwClientID.
 */
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = TRUE;

    return bResult;
}

/**
 * \return TRUE if client exists else FALSE
 *
 * Checks for the existance of the client with the name pcClientName.
 */
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    return(TRUE);
}

/**
 * \return TRUE if client removed else FALSE
 *
 * Removes the client with client id dwClientId.
 */
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = TRUE;
    
    return bResult;
}

HRESULT CDIL_CAN_QRCAN::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount)
{
    return S_OK;
}