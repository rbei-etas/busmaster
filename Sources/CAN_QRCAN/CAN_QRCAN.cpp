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
#include "EXTERNAL/qrcan_api.h"

BEGIN_MESSAGE_MAP(CCAN_QRCAN, CWinApp)
END_MESSAGE_MAP()

static struct QRCanCfg sg_QRCanCfg = {};


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
typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    char pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = 0;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = nullptr;
        }
    }
} SCLIENTBUFMAP;

/* QRCAN Variable Declarations */
static HWND sg_hOwnerWnd = nullptr;
static Base_WrapperErrorLogger* sg_pIlog   = nullptr;
static HINSTANCE               hxlDll;
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT sg_unClientCnt = 0;
static UINT sg_nNoOfChannels = 0;
static CRITICAL_SECTION sg_DIL_CriticalSection;
static HANDLE sg_hEventRecv = nullptr;
static HANDLE sg_hReadThread = nullptr;
static DWORD sg_dwReadThreadId = 0;

#define CALLBACK_TYPE __stdcall
static void CALLBACK_TYPE CanRxEvent(uint32_t index, struct TCanMsg* msg, int32_t count);

/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;



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

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bClientExist(std::string pcClientName, INT& Index);
static BOOL bRemoveClient(DWORD dwClientId);
static BOOL bClientIdExist(const DWORD& dwClientId);
static DWORD dwGetAvailableClientSlot(void);
//static void vMarkEntryIntoMap(const SACK_MAP& RefObj);
//static BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID);

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
 * Starts code for the state machine
 */
enum
{
    STATE_DRIVER_SELECTED    = 0x0,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

static SYSTEMTIME sg_CurrSysTime;
static UINT64 sg_TimeStamp = 0;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_QRCAN::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;

    // Initialize both the time parameters
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;

    CAN_ManageMsgBuf(MSGBUF_CLEAR, 0, nullptr);

    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
HRESULT CDIL_CAN_QRCAN::CAN_UnloadDriverLibrary(void)
{
    if (hxlDll != nullptr)
    {
        FreeLibrary(hxlDll);
        hxlDll = nullptr;
    }

    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_QRCAN::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != 0)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                //Add msg buffer
                if (pBufObj != nullptr)
                {
                    if (sClientObj.unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.pClientBuf[sClientObj.unBufCount++] = pBufObj;
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
                //clear msg buffer
                if (pBufObj != nullptr) //Remove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = nullptr;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
            }
            else
            {
                ////ASSERT(false);
            }
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
            //clear msg buffer
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, nullptr);
            }
            hResult = S_OK;
        }
    }

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
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
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
                    _tcscpy(sg_asClientToBufMap[Index].pacClientName, pacClientName);

                    sg_asClientToBufMap[Index].dwClientID = ClientID;
                    sg_asClientToBufMap[Index].unBufCount = 0;
                }
                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].dwClientID;
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
HRESULT CDIL_CAN_QRCAN::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    HRESULT hResult = S_OK;

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
    memset(&sg_QRCanCfg, 0, sizeof(sg_QRCanCfg));

    /* Create critical section for ensuring thread
    safeness of read message function */
    InitializeCriticalSection(&sg_DIL_CriticalSection);

    /* Register Monitor client */
    DWORD dwClientID = 0;
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
    HRESULT hResult = CAN_StopHardware();

    // ------------------------------------
    // Close driver
    // ------------------------------------
    //CanDownDriver();

    // Remove all the existing clients
    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[0].dwClientID);
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
HRESULT CDIL_CAN_QRCAN::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp = sg_TimeStamp;

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
    USES_CONVERSION;
    static BOOL bInit = 1;

    nCount = 1;
    // Set the current number of channels
    sg_nNoOfChannels = 1;

    sSelHwInterface[0].m_dwIdInterface = 0;
    sSelHwInterface[0].m_acDescription = _("QRCAN Device");
    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    if (bInit){
        bInit = FALSE;
    }
    else{
        MessageBox(sg_hOwnerWnd, "Please use the \"Channel Configuration\" menu item to setup the device.", "Hardware Selection", MB_OK);
    }
    
    return S_OK;
}

/**
* \brief         Selects the hardware interface selected by the user.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*sSelHwInterface*/, INT /*nSize*/)
{
    USES_CONVERSION;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    // Check for success
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return S_OK;
}

/**
* \brief         Deselects the selected hardware interface.
* \param         void
* \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_DeselectHwInterface(void)
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
HRESULT CDIL_CAN_QRCAN::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
    HRESULT hResult = S_OK;
    SCONTROLLER_DETAILS* cntrl;
    
    char temp[32];

    cntrl = (SCONTROLLER_DETAILS*)InitData;

    if (ShowQRCANConfig(sg_hOwnerWnd, &sg_QRCanCfg)){
        if (sg_QRCanCfg.canBaudRate){
            switch ((int)sg_QRCanCfg.canBaudRate)
            {
                case QRCAN_SPEED_20K:
                    cntrl[0].m_omStrBaudrate = "20";
                    break;
                case QRCAN_SPEED_50K:
                    cntrl[0].m_omStrBaudrate = "50";
                    break;
                case QRCAN_SPEED_100K:
                    cntrl[0].m_omStrBaudrate = "100";
                    break;
                case QRCAN_SPEED_125K:
                    cntrl[0].m_omStrBaudrate = "125";
                    break;
                case QRCAN_SPEED_250K:
                    cntrl[0].m_omStrBaudrate = "250";
                    break;
                case QRCAN_SPEED_500K:
                    cntrl[0].m_omStrBaudrate = "500";
                    break;
                case QRCAN_SPEED_800K:
                    cntrl[0].m_omStrBaudrate = "800";
                    break;
                default:
                    cntrl[0].m_omStrBaudrate = "1000";
                    break;
            }
        }
        if ((hResult = CAN_SetConfigData(InitData, 1)) == S_OK) {
            hResult = INFO_INITDAT_CONFIRM_CONFIG;
        }
    }
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
    SCONTROLLER_DETAILS* cntrl;
    char* tmp;

    cntrl = (SCONTROLLER_DETAILS*)ConfigFile;
    
    return S_OK;
}

/**
 * This function writes the message to the corresponding clients buffer
 */
static void vWriteIntoClientsBuffer(STCANDATA& sCanData, UINT unClientIndex)
{
    /* Write into the respective client's buffer */
    for (UINT j = 0; j < sg_asClientToBufMap[unClientIndex].unBufCount; j++)
    {
        sg_asClientToBufMap[unClientIndex].pClientBuf[j]->WriteIntoBuffer(&sCanData);
    }
}

static void CopyMsg2CanData(STCANDATA* sCanData, QRCAN_MSG* msg, unsigned char flags)
{

    memset(sCanData, 0, sizeof(*sCanData));
    sCanData->m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
    sCanData->m_uDataInfo.m_sCANMsg.m_unMsgID = msg->Id;
    sCanData->m_uDataInfo.m_sCANMsg.m_ucDataLen = msg->Length;
    //sCanData->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = (msg->Flags & VSCAN_FLAGS_EXTENDED)?1:0;
    //sCanData->m_uDataInfo.m_sCANMsg.m_ucRTR = (msg->Flags & VSCAN_FLAGS_REMOTE)?1:0;
    sCanData->m_ucDataType = flags;

      //The part that deals with display Time for each message
        GetLocalTime(&sg_CurrSysTime);
        //Query Tick Count
        QueryPerformanceCounter(&sg_QueryTickCount);
        // Get frequency of the performance counter
        QueryPerformanceFrequency(&sg_lnFrequency);
        // Convert it to time stamp with the granularity of hundreds of microsecond
        if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
        {
            sCanData->m_lTickCount.QuadPart = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
        }
        else
        {
            sCanData->m_lTickCount.QuadPart = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
        }

    memcpy(sCanData->m_uDataInfo.m_sCANMsg.m_ucData, msg->Data, 8);
}

static DWORD WINAPI CanRxEvent(LPVOID /* lpParam */)
{
    static STCANDATA sCanData;
    sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    DWORD dwTemp;
    QRCAN_MSG msg;

    for (;;)
    {
        if (WaitForSingleObject(sg_hEventRecv, INFINITE) == WAIT_OBJECT_0)
        {
            for (;;)
            {
                if (QRCAN_Recv(sg_QRCanCfg.hCan, &msg) != QRCAN_ERR_OK)
                {
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("QRCAN_Read failed"));
                    Sleep(100);
                    continue;
                }

                if (dwTemp == 1)
                {
                    CopyMsg2CanData(&sCanData, &msg, RX_FLAG);
                    //Write the msg into registered client's buffer
                    EnterCriticalSection(&sg_DIL_CriticalSection);
                    vWriteIntoClientsBuffer(sCanData, 0);
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
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("WaitForSingleObject failed"));
            Sleep(100);
        }
    }
}

/**
* \brief         connects to the channels and initiates read thread.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_StartHardware(void)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    QRCAN_Open();

    sg_bCurrState = STATE_CONNECTED;

    // Send CAN baud rate selected in the GUI to hardware
    if (QRCAN_Config(sg_QRCanCfg.hCan, &sg_QRCanCfg) != QRCAN_ERR_OK)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("set speed ioctl failed"));
        return (S_FALSE);
    }            

    // Create Receive Event
    sg_hEventRecv = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (sg_hEventRecv == nullptr)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create the receive event"));
        hResult = S_FALSE;
    }

    if (QRCAN_SetRcvEvent(sg_QRCanCfg.hCan, sg_hEventRecv) != QRCAN_ERR_OK)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("QRCAN_SetRcvEvent failed"));
        hResult = S_FALSE;
    }


    // Thread to poll reception of CAN messages
    sg_hReadThread = CreateThread(nullptr, 0, CanRxEvent, nullptr, 0, &sg_dwReadThreadId);
    if (sg_hReadThread == nullptr)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create the receive thread"));
        hResult = S_FALSE;
    }
    return hResult;
}

/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    if (QRCAN_Close() == QRCAN_ERR_OK){
        hResult = S_OK;
    }
    else{
        hResult = S_FALSE;
    }

    return hResult;
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
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
    HRESULT hResult = S_FALSE;
    QRCAN_MSG msg;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    if (bClientIdExist(dwClientID)){
        if (sCanTxMsg.m_ucChannel <= sg_nNoOfChannels){
            memset(&msg, 0, sizeof(msg));
            msg.Id = sCanTxMsg.m_unMsgID;
            msg.Length = sCanTxMsg.m_ucDataLen;
            memcpy(msg.Data, &sCanTxMsg.m_ucData, msg.Length);

            if (QRCAN_Send(sg_QRCanCfg.hCan, &msg) == QRCAN_ERR_OK){
                static STCANDATA sCanData;
                CopyMsg2CanData(&sCanData, &msg, TX_FLAG);

                EnterCriticalSection(&sg_DIL_CriticalSection);
                //Write the msg into registered client's buffer
                vWriteIntoClientsBuffer(sCanData, 0);       // 0 to denote client number
                LeaveCriticalSection(&sg_DIL_CriticalSection);
                
                hResult = S_OK;
            }
            else{
                AfxMessageBox("Could not send CAN data to the hardware");
                hResult = S_FALSE;
            }
        }
        else{
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else{
        hResult = ERR_NO_CLIENT_EXIST;
    }
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
    return WARN_DUMMY_API;
}

/**
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
*/
HRESULT CDIL_CAN_QRCAN::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    switch (eContrParam)
    {
        case NUMBER_HW:
        {
            lParam = 1;
        }
        break;
        case NUMBER_CONNECTED_HW:
        {
            lParam = 1;
        }
        break;
        case DRIVER_STATUS:
        {
            lParam = TRUE;
        }
        break;
        case HW_MODE:
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
        }
        break;
        case CON_TEST:
        {
            lParam = TRUE;
        }
        break;
        default:
        {
            hResult = S_FALSE;
        }
        break;

    }
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
HRESULT CDIL_CAN_QRCAN::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
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
    BOOL bExist = FALSE;

    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; //break the loop
        }
    }
    return(bExist);
}

static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) /* If not the last bufffer */
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

/**
 * \return Returns true if found else false.
 *
 * unClientIndex will have index to client array which has clientId dwClientID.
 */
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; // break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}

/**
 * \return TRUE if client exists else FALSE
 *
 * Checks for the existance of the client with the name pcClientName.
 */
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName.c_str(), sg_asClientToBufMap[i].pacClientName))
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * \return TRUE if client removed else FALSE
 *
 * Removes the client with client id dwClientId.
 */
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = 0;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            sg_asClientToBufMap[unClientIndex].dwClientID = 0;
            memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
            for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
            }
            sg_asClientToBufMap[unClientIndex].unBufCount = 0;
            if ((unClientIndex + 1) < sg_unClientCnt)
            {
                sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
            }
            sg_unClientCnt--;
            bResult = TRUE;
        }
    }
    return bResult;
}

/**
 * \return TRUE if client exists else FALSE
 *
 * Searches for the client with the id dwClientId.
 */
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; // break the loop
        }
    }
    return bReturn;
}

/**
 * Returns the available slot
 */
static DWORD dwGetAvailableClientSlot(void)
{
    DWORD nClientId = 2;
    for (int i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; // break the loop
        }
    }

    return nClientId;
}

HRESULT CDIL_CAN_QRCAN::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount)
{
    return S_OK;
}