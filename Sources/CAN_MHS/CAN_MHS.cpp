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
 * \file      CAN_MHS.cpp
 * \brief     Source file for MHS-Elektronik Tiny-CAN DIL functions
 * \author    Klaus Demlehner
 * \copyright Copyright (c) 2011, MHS-Elektronik GmbH & Co. KG
 *
 * Defines the initialization routines for the DLL.
 */

#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */
#define VC_EXTRALEAN        /* Exclude rarely-used stuff from Windows headers */

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

/* MFC includes */
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <atlconv.h>
#include <tchar.h>
#include <windows.h>
#include <wtypes.h>

/* C includes */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

/* C++ includes */
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

/* Project includes */
#include "CAN_MHS.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/BaseDefs.h"
#include "Include/CAN_Error_Defs.h"
#include "Include/Struct_CAN.h"
#include "Include/CanUsbDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"

#include "mhstcan.h"
#include "mhsbmcfg.h"

#define USAGE_EXPORT
#include "CAN_MHS_Extern.h"

using namespace std;

//#define CAN_DRIVER_DEBUG

// CCAN_MHS

BEGIN_MESSAGE_MAP(CCAN_MHS, CWinApp)
END_MESSAGE_MAP()

/**
 * CCAN_MHS construction
 */
CCAN_MHS::CCAN_MHS()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_MHS object
CCAN_MHS theApp;


/**
 * CCAN_MHS initialization
 */
BOOL CCAN_MHS::InitInstance()
{
    CWinApp::InitInstance();
    return(TRUE);
}

/**
 * Client and Client Buffer map
 */
#define MAX_BUFF_ALLOWED 16
#define MAX_CLIENT_ALLOWED 16

static UINT sg_unClientCnt = 0;
static UINT sg_nNoOfChannels = 0;

#ifdef CAN_DRIVER_DEBUG
static char CanInitStr[] = {"MainThreadPriority=4;CanRxDMode=1;CanTxDFifoSize=2048;CanRxDBufferSize=800;LogFlags=0xFFFF;LogFile=log.txt"};
#else
static char CanInitStr[] = {"MainThreadPriority=4;CanRxDMode=1;CanTxDFifoSize=2048;CanRxDBufferSize=800"};
#endif

/**
 * Starts code for the state machine
 */
enum
{
    STATE_DRIVER_SELECTED    = 0x0,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};


class SCLIENTBUFMAP
{
public:
    DWORD m_dwClientID;
    CBaseCANBufFSE* m_pClientBuf[MAX_BUFF_ALLOWED];
    string m_acClientName;
    UINT m_unBufCount;
    SCLIENTBUFMAP()
    {
        m_dwClientID = 0;
        m_unBufCount = 0;
        m_acClientName = "";

        for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            m_pClientBuf[i] = NULL;
        }
    }
};


/**
 * Array of clients
 */
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

const INT MAX_MAP_SIZE = 3000;

typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)
    {
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel));
    }
} SACK_MAP;

typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;

static BYTE sg_bCurrState = STATE_DRIVER_SELECTED;
static CRITICAL_SECTION sg_DIL_CriticalSection;

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;
static struct TMhsCanCfg sg_MhsCanCfg;

/* CDIL_MHS class definition */
class CDIL_CAN_MHS : public CBaseDIL_CAN_Controller
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
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetLastErrorString(string& acErrorStr);
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

CDIL_CAN_MHS* g_pouDIL_CAN_MHS = NULL;


#define CALLBACK_TYPE __stdcall

static void CALLBACK_TYPE CanPnPEvent(uint32_t index, int32_t status);
static void CALLBACK_TYPE CanStatusEvent(uint32_t index, struct TDeviceStatus* status);
static void CALLBACK_TYPE CanRxEvent(uint32_t index, struct TCanMsg* msg, int32_t count);

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bClientExist(TCHAR* pcClientName, INT& Index);
static BOOL bRemoveClient(DWORD dwClientId);
static BOOL bClientIdExist(const DWORD& dwClientId);
static DWORD dwGetAvailableClientSlot(void);
static void vMarkEntryIntoMap(const SACK_MAP& RefObj);
static BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID);
static int str_has_char(char* s);


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult;
    hResult = S_OK;

    if (!g_pouDIL_CAN_MHS)
    {
        g_pouDIL_CAN_MHS = new CDIL_CAN_MHS;

        if (!(g_pouDIL_CAN_MHS))
        {
            hResult = S_FALSE;
        }
    }

    *ppvInterface = (void*)g_pouDIL_CAN_MHS;  /* Doesn't matter even if g_pouDIL_CAN_MHS is null */
    return(hResult);
}


/* void __cdecl ShowErrorMessage(const char *title, const char *text, ...)
{
va_list argptr;
char out[512];

va_start(argptr, text);
_vstprintf(out, text, argptr);
va_end(argptr);
MessageBox(NULL, out, title, MB_ICONEXCLAMATION | MB_OK);
} */


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_MHS::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;
    CAN_ManageMsgBuf(MSGBUF_CLEAR, NULL, NULL);
    return(S_OK);
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
HRESULT CDIL_CAN_MHS::CAN_UnloadDriverLibrary(void)
{
    return(S_OK);
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_MHS::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    UINT unClientIndex;
    UINT i;

    if (ClientID != 0)
    {
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];

            if (bAction == MSGBUF_ADD)
            {
                // **** Add msg buffer
                if (pBufObj)
                {
                    if (sClientObj.m_unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.m_pClientBuf[sClientObj.m_unBufCount++] = pBufObj;
                            hResult = S_OK;
                        }
                        else
                        {
                            hResult = ERR_BUFFER_EXISTS;
                        }
                    }
                }
            }
            else if (bAction == MSGBUF_CLEAR)
            {
                // **** Clear msg buffer
                if (pBufObj != NULL) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.m_pClientBuf, sClientObj.m_unBufCount, pBufObj);
                }
                else // Remove all
                {
                    for (i = 0; i < sClientObj.m_unBufCount; i++)
                    {
                        sClientObj.m_pClientBuf[i] = NULL;
                    }

                    sClientObj.m_unBufCount = 0;
                }

                hResult = S_OK;
            }

            ////else
            ////  ASSERT(FALSE);
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (bAction == MSGBUF_CLEAR)
        {
            // **** clear msg buffer
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].m_dwClientID, NULL);
            }
        }

        hResult = S_OK;
    }

    return(hResult);
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls
 */
HRESULT CDIL_CAN_MHS::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName)
{
    HRESULT hResult = S_FALSE;
    INT Index;

    if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            Index = 0;

            if (!bClientExist(pacClientName, Index))
            {
                //Currently store the client information
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    //First slot is reserved to monitor node
                    ClientID = 1;
                    sg_asClientToBufMap[0].m_acClientName = pacClientName;
                    sg_asClientToBufMap[0].m_dwClientID = ClientID;
                    sg_asClientToBufMap[0].m_unBufCount = 0;
                }
                else
                {
                    if (!bClientExist(CAN_MONITOR_NODE, Index))
                    {
                        Index = sg_unClientCnt + 1;
                    }
                    else
                    {
                        Index = sg_unClientCnt;
                    }

                    ClientID = dwGetAvailableClientSlot();
                    sg_asClientToBufMap[Index].m_acClientName = pacClientName;
                    sg_asClientToBufMap[Index].m_dwClientID = ClientID;
                    sg_asClientToBufMap[Index].m_unBufCount = 0;
                }

                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].m_dwClientID;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }

    return(hResult);
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the controller status. hEvent will be registered
 * and will be set whenever there is change in the controller
 * status.
 */
HRESULT CDIL_CAN_MHS::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    (void)unCntrlStatus;
    (void)hEvent;
    //unCntrlStatus = defCONTROLLER_ACTIVE; //Temporary solution. TODO
    return(S_OK);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
 */
HRESULT CDIL_CAN_MHS::CAN_LoadDriverLibrary(void)
{
    return(S_OK);
}

/**
* \brief         Performs intial operations.
*                Initializes filter, queue, controller config with default values.
* \param         void
* \return        S_OK if the open driver call successfull otherwise S_FALSE
*/
HRESULT CDIL_CAN_MHS::CAN_PerformInitOperations(void)
{
    HRESULT hResult;
    DWORD dwClientID;
    hResult = S_FALSE;
    sg_MhsCanCfg.CanSnrStr[0] = '\0';
    sg_MhsCanCfg.CanSpeed = 125;
    sg_MhsCanCfg.CanBtrValue = 0;
    /* Create critical section for ensuring thread
    safeness of read message function */
    InitializeCriticalSection(&sg_DIL_CriticalSection);
    /* Register Monitor client */
    dwClientID = 0;
    CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE);

    // ------------------------------------
    // Init Driver
    // ------------------------------------
    if (CanInitDriver(CanInitStr) >= 0)
    {
        // **** AutoConnect auf 1
        //CanSetOptions("AutoConnect=1;AutoReopen=0");
        // **** Event Funktionen setzen
        CanSetPnPEventCallback(&CanPnPEvent);
        CanSetStatusEventCallback(&CanStatusEvent);
        CanSetRxEventCallback(&CanRxEvent);
        // **** Alle Events freigeben
        CanSetEvents(EVENT_ENABLE_ALL);
        hResult = S_OK;
    }

    return(hResult);
}


/**
* \brief         Performs closure operations.
* \param         void
* \return        S_OK if the CAN_StopHardware call successfull otherwise S_FALSE
*/
HRESULT CDIL_CAN_MHS::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;
    hResult = CAN_StopHardware();
    // ------------------------------------
    // Close driver
    // ------------------------------------
    CanDownDriver();

    // Remove all the existing clients
    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[0].m_dwClientID);
    }

    /* Delete the critical section */
    DeleteCriticalSection(&sg_DIL_CriticalSection);
    sg_bCurrState = STATE_DRIVER_SELECTED;
    return(hResult);
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
HRESULT CDIL_CAN_MHS::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    (void)CurrSysTime;
    (void)TimeStamp;
    (void)QueryTickCount;
    /*CurrSysTime = sg_CurrSysTime;
    TimeStamp   = sg_TimeStamp;
    if(QueryTickCount != NULL)
      *QueryTickCount = sg_QueryTickCount; */
    return(S_OK);
}


/**
* \brief         Lists the hardware interface available.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    nCount = 1;
    //set the current number of channels
    sg_nNoOfChannels = 1;
    asSelHwInterface[0].m_dwIdInterface = 0;
    asSelHwInterface[0].m_acDescription = "0";
    sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    return(S_OK);
}


/**
* \brief         Selects the hardware interface selected by the user.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{
    USES_CONVERSION;
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);
    /* Check for the success */
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    return(S_OK);
}


/**
* \brief         Deselects the selected hardware interface.
* \param         void
* \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;
    hResult = CAN_ResetHardware();
    sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    return hResult;
}


/**
* \brief         Displays the controller configuration dialog.
* \param[out]    InitData, is SCONTROLLER_DETAILS structure
* \param[out]    Length , is INT
* \return        S_OK for success
*/
HRESULT CDIL_CAN_MHS::CAN_DisplayConfigDlg(PCHAR& InitData, INT& Length)
{
    (void)Length;
    HRESULT result;
    struct TMhsCanCfg cfg;
    SCONTROLLER_DETAILS* cntrl;
    TCHAR* str;
    result = WARN_INITDAT_NCONFIRM;
    cntrl = (SCONTROLLER_DETAILS*)InitData;

    if (!str_has_char(cntrl[0].m_omStrBaudrate))
    {
        cfg.CanSpeed = _tcstol(cntrl[0].m_omStrBaudrate, &str, 0);
        cfg.CanBtrValue = 0;
    }
    else
    {
        cfg.CanSpeed = 0;
        cfg.CanBtrValue = _tcstol(cntrl[0].m_omStrBTR0, &str, 0);
    }

    strcpy_s(cfg.CanSnrStr, cntrl[0].m_omHardwareDesc);

    if (ShowCanSetup(sg_hOwnerWnd, &cfg))
    {
        strcpy_s(cntrl[0].m_omHardwareDesc, cfg.CanSnrStr);

        if (cfg.CanBtrValue)
        {
            cntrl[0].m_omStrBaudrate[0] = '\0';
            sprintf_s(cntrl[0].m_omStrBTR0, _T("%d"), cfg.CanBtrValue);
        }
        else
        {
            sprintf_s(cntrl[0].m_omStrBaudrate, _T("%d"), cfg.CanSpeed);
            cntrl[0].m_omStrBTR0[0] = '\0';
        }

        if ((result = CAN_SetConfigData(InitData, 1)) == S_OK)
        {
            result = INFO_INITDAT_CONFIRM_CONFIG;
        }
    }

    return(result);
}


/**
* \brief         Sets the controller configuration data supplied by ConfigFile.
* \param[in]     ConfigFile, is SCONTROLLER_DETAILS structure
* \param[in]     Length , is INT
* \return        S_OK for success
*/
HRESULT CDIL_CAN_MHS::CAN_SetConfigData(PCHAR ConfigFile, INT Length)
{
    (void)Length;
    SCONTROLLER_DETAILS* cntrl;
    TCHAR* str;
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    cntrl = (SCONTROLLER_DETAILS*)ConfigFile;

    if (!str_has_char(cntrl[0].m_omStrBaudrate))
    {
        sg_MhsCanCfg.CanSpeed = _tcstol(cntrl[0].m_omStrBaudrate, &str, 0);
        sg_MhsCanCfg.CanBtrValue = 0;
    }
    else
    {
        sg_MhsCanCfg.CanSpeed = 0;
        sg_MhsCanCfg.CanBtrValue = _tcstol(cntrl[0].m_omStrBTR0, &str, 0);
    }

    strcpy_s(sg_MhsCanCfg.CanSnrStr, cntrl[0].m_omHardwareDesc);

    // **** Übertragungsgeschwindigkeit einstellen
    if (sg_MhsCanCfg.CanSpeed)
    {
        if (CanSetSpeed(0, (uint16_t)sg_MhsCanCfg.CanSpeed) < 0)
        {
            return(S_FALSE);
        }
    }
    else
    {
        if (CanSetSpeedUser(0, sg_MhsCanCfg.CanBtrValue) < 0)
        {
            return(S_FALSE);
        }
    }

    return(S_OK);
}


/**
 * This function writes the message to the corresponding clients buffer
 */
static void vWriteIntoClientsBuffer(STCANDATA& can_data)
{
    UINT ClientId, i, j;
    BOOL bClientExists;
    static SACK_MAP sAckMap;
    static UINT Index = (UINT)-1;
    static STCANDATA sTempCanData;

    // Write into the client's buffer and Increment message Count
    if (can_data.m_ucDataType == TX_FLAG)
    {
        ClientId = 0;
        sAckMap.m_Channel = can_data.m_uDataInfo.m_sCANMsg.m_ucChannel;
        sAckMap.m_MsgID = can_data.m_uDataInfo.m_sCANMsg.m_unMsgID;

        if (bRemoveMapEntry(sAckMap, ClientId))
        {
            bClientExists = bGetClientObj(ClientId, Index);

            for (i = 0; i < sg_unClientCnt; i++)
            {
                //Tx for monitor nodes and sender node
                if ((i == CAN_MONITOR_NODE_INDEX)  || (bClientExists && (i == Index)))
                {
                    for (j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
                    {
                        sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&can_data);
                    }
                }
                else
                {
                    //Send the other nodes as Rx.
                    for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
                    {
                        sTempCanData = can_data;
                        sTempCanData.m_ucDataType = RX_FLAG;
                        sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sTempCanData);
                    }
                }
            }
        }
    }
    else // provide it to everybody
    {
        for (i = 0; i < sg_unClientCnt; i++)
        {
            for (j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
            {
                sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&can_data);
            }
        }
    }
}

// Plug & Play Event-Funktion
static void CALLBACK_TYPE CanPnPEvent(uint32_t /*index*/, int32_t status)
{
    if (status)
    {
    }
    else
    {
    }
}


// Status Event-Funktion
static void CALLBACK_TYPE CanStatusEvent(uint32_t /*index*/, struct TDeviceStatus* /*status*/)
{
}


// RxD Event-Funktion
static void CALLBACK_TYPE CanRxEvent(uint32_t index, struct TCanMsg* msg, int32_t count)
{
    (void)index;
    static STCANDATA can_data;

    for (; count; count--)
    {
        EnterCriticalSection(&sg_DIL_CriticalSection);
        can_data.m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
        can_data.m_uDataInfo.m_sCANMsg.m_unMsgID = msg->Id;
        can_data.m_uDataInfo.m_sCANMsg.m_ucDataLen = msg->MsgLen;
        can_data.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = msg->MsgEFF;
        can_data.m_uDataInfo.m_sCANMsg.m_ucRTR = msg->MsgRTR;

        if (msg->MsgTxD)
        {
            can_data.m_ucDataType = TX_FLAG;
        }
        else
        {
            can_data.m_ucDataType = RX_FLAG;
        }

        //can_data.m_lTickCount.QuadPart = (LONGLONG)(msg->TimeStamp);
        memcpy(can_data.m_uDataInfo.m_sCANMsg.m_ucData, msg->MsgData, 8);
        //Write the msg into registered client's buffer
        vWriteIntoClientsBuffer(can_data);
        LeaveCriticalSection(&sg_DIL_CriticalSection);
        msg++;
    }
}


/*static void CopyMhsToCanData(const struct TCanMsg *src, STCANDATA* dest)
{

}*/


/**
* \brief         connects to the channels and initiates read thread.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_StartHardware(void)
{
    USES_CONVERSION;
    HRESULT hResult;
    char str[100];

    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    if (!str_has_char(sg_MhsCanCfg.CanSnrStr))
    {
        sprintf_s(str, "Snr=%s", sg_MhsCanCfg.CanSnrStr);
    }
    else
    {
        str[0]='\0';
    }

    if (!CanDeviceOpen(0, str))
    {
        (void)CanSetOptions("CanTxAckEnable=1");

        // **** CAN Bus Start
        if (CanSetMode(0, OP_CAN_START, CAN_CMD_FIFOS_ERROR_CLEAR) >= 0)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not start the controller in running mode"));
        }

        sg_bCurrState = STATE_CONNECTED;
    }
    else
    {
        //log the error for open port failure
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("error opening \"Tiny-CAN\" interface"));
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return(hResult);
}


/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    (void)CanDeviceClose(0);
    return(S_OK);
}


/**
* \brief         Resets the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_ResetHardware(void)
{
    (void)CAN_StopHardware();
    return(S_OK);
}


/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
    return(S_OK);
}

/**
* \brief         Sends STCAN_MSG structure from the client dwClientID.
* \param[in]     dwClientID is the client ID
* \param[in]     sMessage is the application specific CAN message structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{
    struct TCanMsg msg;
    static SACK_MAP sAckMap;
    HRESULT hResult;
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    hResult = S_FALSE;

    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= sg_nNoOfChannels)
        {
            // msg Variable Initialisieren
            msg.MsgFlags = 0L;   // Alle Flags löschen, Stanadrt Frame Format,

            // keine RTR, Datenlänge auf 0
            if (sMessage.m_ucEXTENDED == 1)
            {
                msg.MsgEFF = 1;    // Nachricht im EFF (Ext. Frame Format) versenden
            }

            if (sMessage.m_ucRTR == 1)
            {
                msg.MsgRTR = 1;    // Nachricht als RTR Frame versenden
            }

            msg.Id = sMessage.m_unMsgID;
            msg.MsgLen = sMessage.m_ucDataLen;
            memcpy(msg.MsgData, &sMessage.m_ucData, msg.MsgLen);
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_Channel  = sMessage.m_ucChannel;
            sAckMap.m_MsgID    = msg.Id;
            vMarkEntryIntoMap(sAckMap);

            if (CanTransmit(0, &msg, 1) >= 0)
            {
                hResult = S_OK;
            }
            else
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not write can data into bus"));
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }

    return(hResult);
}

/**
* \brief         Gets last occured error and puts inside acErrorStr.
* \param[out]    acErrorStr contains error string
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_GetLastErrorString(string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}


/**
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult;
    hResult = S_OK;

    switch (eContrParam)
    {
        case NUMBER_HW     :
        {
            lParam = 1;
            break;
        }

        case NUMBER_CONNECTED_HW :
        {
            lParam = 1;
            //hResult = S_FALSE;
            break;
        }

        case DRIVER_STATUS :
        {
            lParam = true;
            break;
        }

        case HW_MODE       :
        {
            if (nChannel < sg_nNoOfChannels)
            {
                lParam = defMODE_ACTIVE;
            }
            else
                //unknown
            {
                lParam = defCONTROLLER_BUSOFF + 1;
            }

            break;
        }

        case CON_TEST      :
        {
            lParam = TRUE;
            break;
        }

        default            :
            hResult = S_FALSE;
    }

    return hResult;
}


/**
* \brief         Gets the error counter for corresponding channel.
* \param[out]    sErrorCnt, is SERROR_CNT structure
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_MHS::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    (void)eContrParam;
    (void)nChannel;
    // Tiny-CAN not support CAN-Bus Error counters
    sErrorCnt.m_ucTxErrCount = 0;
    sErrorCnt.m_ucRxErrCount = 0;
    return(S_OK);
}



static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    UINT i;
    BOOL bExist;
    bExist = FALSE;

    for (i = 0; i < sClientObj.m_unBufCount; i++)
    {
        if (pBuf == sClientObj.m_pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.m_unBufCount; //break the loop
        }
    }

    return(bExist);
}


static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    UINT i;
    BOOL bReturn;
    bReturn = TRUE;

    for (i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) //If not the last bufffer
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }

            unCount--;
        }
    }

    return(bReturn);
}


/**
 * \return Returns true if found else false.
 *
 * unClientIndex will have index to client array which has clientId dwClientID.
 */
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult;
    bResult = FALSE;

    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
            bResult = TRUE;
        }
    }

    return(bResult);
}


/**
 * \return TRUE if client exists else FALSE
 *
 * Checks for the existance of the client with the name pcClientName.
 */
static BOOL bClientExist(TCHAR* pcClientName, INT& Index)
{
    UINT i;

    for (i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName, sg_asClientToBufMap[i].m_acClientName.c_str()))
        {
            Index = i;
            return(TRUE);
        }
    }

    return(FALSE);
}


/**
 * \return TRUE if client removed else FALSE
 *
 * Removes the client with client id dwClientId.
 */
static BOOL bRemoveClient(DWORD dwClientId)
{
    INT i;
    BOOL bResult = FALSE;
    bResult = FALSE;

    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = 0;

        if (bGetClientObj(dwClientId, unClientIndex))
        {
            sg_asClientToBufMap[unClientIndex].m_dwClientID = 0;
            sg_asClientToBufMap[unClientIndex].m_acClientName = "";

            for (i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].m_pClientBuf[i] = NULL;
            }

            sg_asClientToBufMap[unClientIndex].m_unBufCount = 0;

            if ((unClientIndex + 1) < sg_unClientCnt)
            {
                sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
            }

            sg_unClientCnt--;
            bResult = TRUE;
        }
    }

    return(bResult);
}


/**
 * \return TRUE if client exists else FALSE
 *
 * Searches for the client with the id dwClientId.
 */
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    UINT i;
    BOOL bReturn;
    bReturn = FALSE;

    for (i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; // break the loop
        }
    }

    return(bReturn);
}


/**
 * Returns the available slot
 */
static DWORD dwGetAvailableClientSlot(void)
{
    INT i;
    DWORD nClientId;
    nClientId = 2;

    for (i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED;    //break the loop
        }
    }

    return(nClientId);
}


/**
 * Pushes an entry into the list at the last position
 */
static void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    EnterCriticalSection(&sg_DIL_CriticalSection); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    LeaveCriticalSection(&sg_DIL_CriticalSection); // Unlock the buffer
}

static BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator  iResult =
        std::find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );

    if (iResult != sg_asAckMapBuf.end())
    {
        bResult = TRUE;
        ClientID = (*iResult).m_ClientID;
        sg_asAckMapBuf.erase(iResult);
    }

    return bResult;
}


static int str_has_char(char* s)
{
    char c;

    if (!s)
    {
        return(-1);
    }

    while (*s)
    {
        c = *s++;

        if (c != ' ')
        {
            break;
        }
    }

    if (c)
    {
        return(0);
    }
    else
    {
        return(-1);
    }
}
