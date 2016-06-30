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
 * \file      CAN_VSCOM.cpp
 * \author
 * \copyright Copyright (c) 2012, ETAS GmbH. All rights reserved.
 */
// CAN_VSCOM.cpp : Defines the initialization routines for the DLL.
//

#include "CAN_VSCOM_stdafx.h"
#include "CAN_VSCOM.h"
//#include "include/Error.h"
//#include "include/basedefs.h"
//#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_DataTypes.h"
//#include "DataTypes/DIL_Datatypes.h"
//#include "Include/BaseDefs.h"
//#include "Include/CAN_Error_Defs.h"
//#include "Include/Struct_CAN.h"
//#include "Include/CanUsbDefs.h"
//#include "Include/DIL_CommonDefs.h"
//#include "DIL_Interface/BaseDIL_CAN_Controller.h"

#include "BaseDIL_CAN_Controller.h"
#include "DILPluginHelperDefs.h"

#include "EXTERNAL/vs_can_api.h"

#define USAGE_EXPORT
#include "CAN_VSCOM_Extern.h"


//#define CAN_DRIVER_DEBUG

// CCAN_VSCOM

BEGIN_MESSAGE_MAP(CCAN_VSCOM, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_VSCOM construction
 */
CCAN_VSCOM::CCAN_VSCOM()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_VSCOM object
CCAN_VSCOM theApp;


/**
 * CCAN_VSCOM initialization
 */
BOOL CCAN_VSCOM::InitInstance()
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

static HANDLE sg_hEventRecv = nullptr;
static HANDLE sg_hReadThread = nullptr;
static DWORD sg_dwReadThreadId = 0;
static struct _VSCanCfg sg_VSCanCfg;


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


typedef struct tagClientBufMap
{
    DWORD m_dwClientID;
    CBaseCANBufFSE* m_pClientBuf[MAX_BUFF_ALLOWED];
    char m_acClientName[MAX_PATH];
    UINT m_unBufCount;
    tagClientBufMap()
    {
        m_dwClientID = 0;
        m_unBufCount = 0;
        memset(m_acClientName, 0, sizeof (char) * MAX_PATH);
        for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            m_pClientBuf[i] = nullptr;
        }
    }
} SCLIENTBUFMAP;


/**
 * Array of clients
 */
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

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

static HWND sg_hOwnerWnd = nullptr;

static SYSTEMTIME sg_CurrSysTime;

/* CDIL_VSCOM class definition */
class CDIL_CAN_VSCOM : public CBaseDIL_CAN_Controller
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
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS ConfigFile, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_GetCurrStatus(STATUSMSG& StatusData);
    HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT CAN_SetAppParams(HWND hWndOwner);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);
};

CDIL_CAN_VSCOM* g_pouDIL_CAN_VSCOM = nullptr;


#define CALLBACK_TYPE __stdcall

static void CALLBACK_TYPE CanPnPEvent(uint32_t index, int32_t status);
static void CALLBACK_TYPE CanStatusEvent(uint32_t index, struct TDeviceStatus* status);
static void CALLBACK_TYPE CanRxEvent(uint32_t index, struct TCanMsg* msg, int32_t count);

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bClientExist(std::string pcClientName, INT& Index);
static BOOL bRemoveClient(DWORD dwClientId);
static BOOL bClientIdExist(const DWORD& dwClientId);
static DWORD dwGetAvailableClientSlot(void);
static void vMarkEntryIntoMap(const SACK_MAP& RefObj);
static BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID);

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult;

    hResult = S_OK;
    if (!g_pouDIL_CAN_VSCOM)
    {
        g_pouDIL_CAN_VSCOM = new CDIL_CAN_VSCOM;
        if (!(g_pouDIL_CAN_VSCOM))
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*)g_pouDIL_CAN_VSCOM;  /* Doesn't matter even if g_pouDIL_CAN_VSCOM is null */

    return(hResult);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_VSCOM::CAN_SetAppParams(HWND hWndOwner)
{
    sg_hOwnerWnd = hWndOwner;
    CAN_ManageMsgBuf(MSGBUF_CLEAR, 0, nullptr);
    return(S_OK);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
HRESULT CDIL_CAN_VSCOM::CAN_UnloadDriverLibrary(void)
{
    return(S_OK);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_VSCOM::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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
                if (pBufObj != nullptr) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.m_pClientBuf, sClientObj.m_unBufCount, pBufObj);
                }
                else // Remove all
                {
                    for (i = 0; i < sClientObj.m_unBufCount; i++)
                    {
                        sClientObj.m_pClientBuf[i] = nullptr;
                    }
                    sClientObj.m_unBufCount = 0;
                }
                hResult = S_OK;
            }
            ////else
            ////  ASSERT(false);
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
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].m_dwClientID, nullptr);
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
HRESULT CDIL_CAN_VSCOM::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
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
                    _tcscpy(sg_asClientToBufMap[0].m_acClientName, pacClientName);
                    sg_asClientToBufMap[0].m_dwClientID = ClientID;
                    sg_asClientToBufMap[0].m_unBufCount = 0;
                }
                else
                {
                    /*if (!bClientExist(CAN_MONITOR_NODE, Index))
                    {
                        Index = sg_unClientCnt + 1;
                    }
                    else
                    {
                        Index = sg_unClientCnt;
                    }*/
                    Index = sg_unClientCnt;
                    ClientID = dwGetAvailableClientSlot();
                    _tcscpy(sg_asClientToBufMap[Index].m_acClientName, pacClientName);
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
HRESULT CDIL_CAN_VSCOM::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
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
HRESULT CDIL_CAN_VSCOM::CAN_LoadDriverLibrary(void)
{
    return(S_OK);
}

/**
* \brief         Performs intial operations.
*                Initializes filter, queue, controller config with default values.
* \param         void
* \return        S_OK if the open driver call successfull otherwise S_FALSE
*/
HRESULT CDIL_CAN_VSCOM::CAN_PerformInitOperations(void)
{
    DWORD dwClientID;

    memset(&sg_VSCanCfg, 0, sizeof(sg_VSCanCfg));

    /* Create critical section for ensuring thread
    safeness of read message function */
    InitializeCriticalSection(&sg_DIL_CriticalSection);
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
HRESULT CDIL_CAN_VSCOM::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = CAN_StopHardware();

    // ------------------------------------
    // Close driver
    // ------------------------------------
    //CanDownDriver();

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
HRESULT CDIL_CAN_VSCOM::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& /* TimeStamp */, LARGE_INTEGER& /* QueryTickCount */)
{
    CurrSysTime = sg_CurrSysTime;
    return S_OK;
}

/**
* \brief         Lists the hardware interface available.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    static BOOL bInit = 1;

    nCount = 1;
    //set the current number of channels
    sg_nNoOfChannels = 1;
    asSelHwInterface[0].m_dwIdInterface = 0;
    asSelHwInterface[0].m_acDescription = "VScom CAN Device";
    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    if (bInit)
    {
        bInit = FALSE;
    }
    else
    {
        MessageBox(sg_hOwnerWnd, "Please use the \"Channel Configuration\" menu item to setup the device.", "Hardware Selection", MB_OK);
    }

    return(S_OK);
}

/**
* \brief         Selects the hardware interface selected by the user.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
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
HRESULT CDIL_CAN_VSCOM::CAN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

/**
* \brief         Displays the controller configuration dialog.
* \param[out]    InitData, is SCONTROLLER_DETAILS structure
* \param[out]    Length , is INT
* \return        S_OK for success
*/
HRESULT CDIL_CAN_VSCOM::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
    (void)Length;
    HRESULT result;
    SCONTROLLER_DETAILS* cntrl;
    char temp[32];

    result = WARN_INITDAT_NCONFIRM;
    cntrl = (SCONTROLLER_DETAILS*)InitData;

    if (ShowCanVsComSetup(sg_hOwnerWnd, &sg_VSCanCfg))
    {
        if (sg_VSCanCfg.vSpeed)
        {
            switch ((int)sg_VSCanCfg.vSpeed)
            {
                case VSCAN_SPEED_20K:
                    cntrl[0].m_omStrBaudrate = "20";
                    break;
                case VSCAN_SPEED_50K:
                    cntrl[0].m_omStrBaudrate = "50";
                    break;
                case VSCAN_SPEED_100K:
                    cntrl[0].m_omStrBaudrate = "100";
                    break;
                case VSCAN_SPEED_125K:
                    cntrl[0].m_omStrBaudrate = "125";
                    break;
                case VSCAN_SPEED_250K:
                    cntrl[0].m_omStrBaudrate = "250";
                    break;
                case VSCAN_SPEED_500K:
                    cntrl[0].m_omStrBaudrate = "500";
                    break;
                case VSCAN_SPEED_800K:
                    cntrl[0].m_omStrBaudrate = "800";
                    break;
                default:
                    cntrl[0].m_omStrBaudrate = "1000";
                    break;
            }
        }
        else
        {
            cntrl[0].m_omStrBaudrate = "";
            sprintf(temp, "%02X", sg_VSCanCfg.btr.Btr0);
            cntrl[0].m_omStrBTR0 = temp;
            sprintf(temp, "%02X", sg_VSCanCfg.btr.Btr1);
            cntrl[0].m_omStrBTR1 = temp;
        }

        cntrl[0].m_bDebug = sg_VSCanCfg.bDebug;

        cntrl[0].m_omStrLocation = sg_VSCanCfg.szLocation;

        if (sg_VSCanCfg.dwMode == VSCAN_MODE_LISTEN_ONLY)
        {
            cntrl[0].m_bPassiveMode = 1;
            cntrl[0].m_bSelfReception = 0;
        }
        else if (sg_VSCanCfg.dwMode == VSCAN_MODE_SELF_RECEPTION)
        {
            cntrl[0].m_bPassiveMode = 0;
            cntrl[0].m_bSelfReception = 1;
        }
        else
        {
            cntrl[0].m_bPassiveMode = 0;
            cntrl[0].m_bSelfReception = 0;
        }

        cntrl[0].m_bHWTimestamps = sg_VSCanCfg.bTimestamps;

        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Code >> 0) & 0xFF);
        cntrl[0].m_omStrAccCodeByte1[0] = temp;
        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Code >> 8) & 0xFF);
        cntrl[0].m_omStrAccCodeByte2[0] = temp;
        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Code >> 16) & 0xFF);
        cntrl[0].m_omStrAccCodeByte3[0] = temp;
        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Code >> 24) & 0xFF);
        cntrl[0].m_omStrAccCodeByte4[0] = temp;

        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Mask >> 0) & 0xFF);
        cntrl[0].m_omStrAccMaskByte1[0] = temp;
        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Mask >> 8) & 0xFF);
        cntrl[0].m_omStrAccMaskByte2[0] = temp;
        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Mask >> 16) & 0xFF);
        cntrl[0].m_omStrAccMaskByte3[0] = temp;
        sprintf(temp, "%X", (sg_VSCanCfg.codeMask.Mask >> 24) & 0xFF);
        cntrl[0].m_omStrAccMaskByte4[0] = temp;

        cntrl[0].m_bAccFilterMode = sg_VSCanCfg.bDualFilter;

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
HRESULT CDIL_CAN_VSCOM::CAN_SetConfigData(PSCONTROLLER_DETAILS ConfigFile, int Length)
{
    (void)Length;
    SCONTROLLER_DETAILS* cntrl;
    char* tmp;
    std::string str;

    cntrl = (SCONTROLLER_DETAILS*)ConfigFile;
    if (cntrl[0].m_omStrBaudrate.length() > 0)
    {
        switch(_tcstol(cntrl[0].m_omStrBaudrate.c_str(), &tmp, 0))
        {
            case 20:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_20K;
                break;
            case 50:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_50K;
                break;
            case 100:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_100K;
                break;
            case 125:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_125K;
                break;
            case 250:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_250K;
                break;
            case 500:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_500K;
                break;
            case 800:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_800K;
                break;
            default:
                sg_VSCanCfg.vSpeed = VSCAN_SPEED_1M;
                break;
        }
    }
    else
    {
        sg_VSCanCfg.btr.Btr0 = (UCHAR)_tcstol(cntrl[0].m_omStrBTR0.c_str(), &tmp, 0);
        sg_VSCanCfg.btr.Btr1 = (UCHAR)_tcstol(cntrl[0].m_omStrBTR1.c_str(), &tmp, 0);
    }

    sg_VSCanCfg.bDebug = cntrl[0].m_bDebug;

    strcpy(sg_VSCanCfg.szLocation, cntrl[0].m_omStrLocation.c_str());

    if (cntrl[0].m_bPassiveMode)
    {
        sg_VSCanCfg.dwMode = VSCAN_MODE_LISTEN_ONLY;
    }
    else if (cntrl[0].m_bSelfReception)
    {
        sg_VSCanCfg.dwMode = VSCAN_MODE_SELF_RECEPTION;
    }
    else
    {
        sg_VSCanCfg.dwMode = VSCAN_MODE_NORMAL;
    }

    sg_VSCanCfg.bTimestamps = cntrl[0].m_bHWTimestamps;

    str = cntrl[0].m_omStrAccCodeByte4[0];
    str += cntrl[0].m_omStrAccCodeByte3[0];
    str += cntrl[0].m_omStrAccCodeByte2[0];
    str += cntrl[0].m_omStrAccCodeByte1[0];
    sg_VSCanCfg.codeMask.Code = strtoul(str.c_str(), nullptr, 16);
    str = cntrl[0].m_omStrAccMaskByte4[0];
    str += cntrl[0].m_omStrAccMaskByte3[0];
    str += cntrl[0].m_omStrAccMaskByte2[0];
    str += cntrl[0].m_omStrAccMaskByte1[0];
    sg_VSCanCfg.codeMask.Mask = strtoul(str.c_str(), nullptr, 16);

    sg_VSCanCfg.bDualFilter = cntrl[0].m_bAccFilterMode;

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
                //Tx for sender node
                if (/*(i == CAN_MONITOR_NODE_INDEX)  ||*/ (bClientExists && (i == Index)))
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

// sadly GetTickCount64() is no option
static LONGLONG MyGetTickCount(void)
{
    static LONGLONG myNewTickCount = 0;
    static DWORD oldTickCount = GetTickCount();
    DWORD newTickCount = GetTickCount();

    // when there is no new frame between 49,7 days,
    // we will lost this time window, but ...
    if (newTickCount < oldTickCount)
    {
        myNewTickCount += (LONGLONG)((DWORD)~0 - oldTickCount) + newTickCount;
    }
    else
    {
        myNewTickCount += (LONGLONG)(newTickCount - oldTickCount);
    }

    oldTickCount = newTickCount;

    return myNewTickCount;
}

// we use the timestamps from our hardware and must handle
// the overflow every 60000ms
static LONGLONG GetSysTimestamp(LONG Timestamp)
{
    static LONGLONG oldTickCount = MyGetTickCount();
    static LONGLONG oldTimestamp = 0, sysTimestamp = oldTickCount;
    LONGLONG timeDiff, newTickCount, temp;

    newTickCount = MyGetTickCount();
    temp = (newTickCount - oldTickCount) / 60000;  // minutes since last read

    // for Tx
    if (Timestamp == ~0)
    {
        return sysTimestamp + (newTickCount - oldTickCount) + (temp * 60000);
    }

    // for Rx
    oldTickCount = newTickCount;

    if (Timestamp < oldTimestamp)
    {
        timeDiff = (60000 - oldTimestamp) + Timestamp;
    }
    else
    {
        timeDiff = Timestamp - oldTimestamp;
    }
    oldTimestamp = Timestamp;

    timeDiff += (temp * 60000);
    sysTimestamp += timeDiff;

    return sysTimestamp;
}

static void CopyMsg2CanData(STCANDATA* can_data, VSCAN_MSG* msg, unsigned char flags)
{
    memset(can_data, 0, sizeof(*can_data));
    can_data->m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
    can_data->m_uDataInfo.m_sCANMsg.m_unMsgID = msg->Id;
    can_data->m_uDataInfo.m_sCANMsg.m_ucDataLen = msg->Size;
    can_data->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = (msg->Flags & VSCAN_FLAGS_EXTENDED)?1:0;
    can_data->m_uDataInfo.m_sCANMsg.m_ucRTR = (msg->Flags & VSCAN_FLAGS_REMOTE)?1:0;
    can_data->m_ucDataType = flags;
    if (flags & TX_FLAG)
    {
        can_data->m_lTickCount.QuadPart = GetSysTimestamp(~0) * 10;
    }
    else
    {
        can_data->m_lTickCount.QuadPart = GetSysTimestamp(msg->Timestamp) * 10;
    }
    memcpy(can_data->m_uDataInfo.m_sCANMsg.m_ucData, msg->Data, 8);
}

// RxD Event-Funktion
static DWORD WINAPI CanRxEvent(LPVOID /* lpParam */)
{
    static STCANDATA can_data;
    can_data.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    DWORD dwTemp;
    VSCAN_MSG msg;

    for (;;)
    {
        if (WaitForSingleObject(sg_hEventRecv, INFINITE) == WAIT_OBJECT_0)
        {
            for (;;)
            {
                if (VSCAN_Read(sg_VSCanCfg.hCan, &msg, 1, &dwTemp) != VSCAN_ERR_OK)
                {
                    Sleep(100);
                    continue;
                }

                if (dwTemp == 1)
                {
                    CopyMsg2CanData(&can_data, &msg, RX_FLAG);
                    //Write the msg into registered client's buffer
                    EnterCriticalSection(&sg_DIL_CriticalSection);
                    vWriteIntoClientsBuffer(can_data);
                    LeaveCriticalSection(&sg_DIL_CriticalSection);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            Sleep(100);
        }

    }
}

/**
* \brief         connects to the channels and initiates read thread.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_StartHardware(void)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    /* If no device available, exit */
    if (strcmp(sg_VSCanCfg.szLocation, "") == 0 ||
            strcmp(sg_VSCanCfg.szLocation, "\\\\.\\") == 0  )
    {
        return S_FALSE;
    }

    if (sg_VSCanCfg.bDebug)
    {
        VSCAN_Ioctl(0, VSCAN_IOCTL_SET_DEBUG_MODE, VSCAN_DEBUG_MODE_FILE);
        VSCAN_Ioctl(0, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_HIGH);
    }
    else
    {
        VSCAN_Ioctl(0, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_NONE);
    }

    sg_VSCanCfg.hCan = VSCAN_Open(sg_VSCanCfg.szLocation, sg_VSCanCfg.dwMode);
    if (sg_VSCanCfg.hCan > 0)
    {
        if (VSCAN_Ioctl(sg_VSCanCfg.hCan, VSCAN_IOCTL_SET_TIMESTAMP, sg_VSCanCfg.bTimestamps?VSCAN_TIMESTAMP_ON:VSCAN_TIMESTAMP_OFF) != VSCAN_ERR_OK)
        {
            return (S_FALSE);
        }

        if (VSCAN_Ioctl(sg_VSCanCfg.hCan, VSCAN_IOCTL_SET_FILTER_MODE, sg_VSCanCfg.bDualFilter?VSCAN_FILTER_MODE_DUAL:VSCAN_FILTER_MODE_SINGLE) != VSCAN_ERR_OK)
        {
            return (S_FALSE);
        }

        if (VSCAN_Ioctl(sg_VSCanCfg.hCan, VSCAN_IOCTL_SET_ACC_CODE_MASK, &sg_VSCanCfg.codeMask) != VSCAN_ERR_OK)
        {
            return (S_FALSE);
        }

        if (sg_VSCanCfg.vSpeed)
        {
            if (VSCAN_Ioctl(sg_VSCanCfg.hCan, VSCAN_IOCTL_SET_SPEED, (void*)sg_VSCanCfg.vSpeed) != VSCAN_ERR_OK)
            {
                return (S_FALSE);
            }
        }
        else
        {
            if (VSCAN_Ioctl(sg_VSCanCfg.hCan, VSCAN_IOCTL_SET_BTR, &sg_VSCanCfg.btr) != VSCAN_ERR_OK)
            {
                return (S_FALSE);
            }
        }

        sg_hEventRecv = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (sg_hEventRecv == nullptr)
        {
            hResult = S_FALSE;
        }

        if (VSCAN_SetRcvEvent(sg_VSCanCfg.hCan, sg_hEventRecv) != VSCAN_ERR_OK)
        {
            hResult = S_FALSE;
        }

        sg_hReadThread = CreateThread(nullptr, 0, CanRxEvent, nullptr, 0, &sg_dwReadThreadId);
        if (sg_hReadThread == nullptr)
        {
            hResult = S_FALSE;
        }

        hResult = S_OK;

        sg_bCurrState = STATE_CONNECTED;
    }
    else
    {
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return(hResult);
}

/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    if (sg_hReadThread != nullptr)
    {
        TerminateThread(sg_hReadThread, 0);
        sg_hReadThread = nullptr;
    }

    if (sg_hEventRecv != nullptr)
    {
        CloseHandle(sg_hEventRecv);
        sg_hEventRecv = nullptr;
    }

    if (sg_VSCanCfg.hCan > 0)
    {
        VSCAN_Close(sg_VSCanCfg.hCan);
        sg_VSCanCfg.hCan = 0;
    }

    return(S_OK);
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_GetCurrStatus(STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
    return(S_OK);
}

/**
* \brief         Gets the Tx queue configured.
* \param[out]    pouFlxTxMsgBuffer, is BYTE*
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return(S_OK);
}

/**
* \brief         Sends STCAN_MSG structure from the client dwClientID.
* \param[in]     dwClientID is the client ID
* \param[in]     sMessage is the application specific CAN message structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{
    VSCAN_MSG msg;
    DWORD dwTemp;
    static SACK_MAP sAckMap;
    HRESULT hResult;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= sg_nNoOfChannels)
        {
            memset(&msg, 0, sizeof(msg));

            if (sMessage.m_ucEXTENDED == 1)
            {
                msg.Flags |= VSCAN_FLAGS_EXTENDED;
            }
            if (sMessage.m_ucRTR == 1)
            {
                msg.Flags |= VSCAN_FLAGS_REMOTE;
            }
            msg.Id = sMessage.m_unMsgID;
            msg.Size = sMessage.m_ucDataLen;
            memcpy(msg.Data, &sMessage.m_ucData, msg.Size);
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_Channel  = sMessage.m_ucChannel;
            sAckMap.m_MsgID    = msg.Id;
            vMarkEntryIntoMap(sAckMap);
            if (VSCAN_Write(sg_VSCanCfg.hCan, &msg, 1, &dwTemp) == VSCAN_ERR_OK && dwTemp == 1)
            {
                static STCANDATA can_data;
                CopyMsg2CanData(&can_data, &msg, TX_FLAG);
                EnterCriticalSection(&sg_DIL_CriticalSection);
                //Write the msg into registered client's buffer
                vWriteIntoClientsBuffer(can_data);
                LeaveCriticalSection(&sg_DIL_CriticalSection);
                hResult = S_OK;
            }
            else
            {
                hResult = S_FALSE;
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
* \brief         Gets bus config info.
* \param[out]    BusInfo, is BYTE
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return(S_OK);
}

/**
* \brief         Gets last occured error and puts inside acErrorStr.
* \param[out]    acErrorStr, is CHAR contains error string
* \param[in]     nLength, is INT
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_VSCOM::CAN_GetLastErrorString(std::string& /* acErrorStr */)
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
HRESULT CDIL_CAN_VSCOM::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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

HRESULT CDIL_CAN_VSCOM::CAN_SetControllerParams(int /* nValue */, ECONTR_PARAM /* eContrparam */)
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
HRESULT CDIL_CAN_VSCOM::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
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
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    UINT i;
    for (i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName.c_str(), sg_asClientToBufMap[i].m_acClientName))
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

    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = 0;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            sg_asClientToBufMap[unClientIndex].m_dwClientID = 0;
            memset (sg_asClientToBufMap[unClientIndex].m_acClientName, 0, sizeof (char) * MAX_PATH);
            for (i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].m_pClientBuf[i] = nullptr;
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
HRESULT CDIL_CAN_VSCOM::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount)
{
    return S_OK;
}