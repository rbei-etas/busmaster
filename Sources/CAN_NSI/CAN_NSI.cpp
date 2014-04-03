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
 * \file      CAN_NSI.cpp
 * \brief     Source file for NSI CAN DIL functions
 * \author    Gregory MERCHAT
 * \copyright Copyright (c) 2013, NSI by Altran. All rights reserved.
 *
 * Source file for NSI CAN DIL functions
 */
// CAN_NSI.cpp : Defines the initialization routines for the DLL.
//
#include "CAN_NSI_stdafx.h"
#include "CAN_NSI.h"
#include "CAN_NSI_Resource.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Utility/Utility_Thread.h"
#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "Include/CanUsbDefs.h"
#include "Include/Can_Error_Defs.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "HardwareListing.h"
#include "ChangeRegisters.h"
#include "../Application/MultiLanguage.h"
#include "EXTERNAL/NSI/Canpcex.h"

#define USAGE_EXPORT
#include "CAN_NSI_Extern.h"


// CCAN_NSI

BEGIN_MESSAGE_MAP(CCAN_NSI, CWinApp)
END_MESSAGE_MAP()

typedef struct
{
    unsigned long ident;
    HANDLE hEvent;
    HANDLE hThread;
} t_ThreadContext;

/**
 * NSI Functions
 */
static char* GetCodeString(short cr);
ULONGLONG GetTimeStamp();
void GetEventString(t_CANevent* pEvent, UINT unDrvChannel);
DWORD Thread(HANDLE hEvent);
static int nInitDriver(UINT unDrvChannel);
static int nExitDriver(UINT unDrvChannel);
static int nInitBusParam(UINT unDrvChannel, t_CANbusParams canBus, t_CANaddressing adressing, t_CANpadding padding);
static int nInitRxMask(UINT unDrvChannel, unsigned long code, unsigned long mask, t_CANidentType identType);
static int nInitInterface(UINT unDrvChannel, t_Interface mode);

/**
 * NSI Variables
 */
#define MAX_BUFF_ALLOWED 16
short cr;
unsigned long cardCount, NSI_code, NSI_mask;
unsigned char data[_CAN_MAX_DATA_EX];
t_CANerr NSI_canErr;                                // Hardware error level
t_CANstat NSI_canEtat;                              // Hardware state
t_CANobj NSI_canObj;                                // Initialization structure for a CAN message
t_CANbusParams NSI_canBus;                          // Initialization structures for CAN bus
t_CardData NSI_cardData[MAX_BUFF_ALLOWED];          // Structure array to receive channel configuration data
t_CANdeviceInfo NSI_deviceInfo[MAX_BUFF_ALLOWED];   // Structure array to receive device info
BOOL flagLowSpeed = FALSE;
BOOL flagSendError = FALSE;
BOOL flagConnect = FALSE;
HANDLE NSI_hCanal[MAX_BUFF_ALLOWED];                // Open channel handle
HANDLE NSI_hEvent[defNO_OF_CHANNELS]  = {0};            // FIFO event handle
HANDLE NSI_hMutex= INVALID_HANDLE_VALUE;            // Display synchronization
HANDLE NSI_hWakeUp = INVALID_HANDLE_VALUE;          // WAKE-UP event handle
/* Thread control variables */
DWORD NSI_threadId;
HANDLE NSI_hThread;

#define MAX_CAN_MSG_PTR 8192 //4096 
static STCAN_MSG sTxMsgTab[MAX_CAN_MSG_PTR];
static UINT nbMsgInTab = 0;

/**
 * CCAN_NSI construction
 */
CCAN_NSI::CCAN_NSI()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_NSI object
CCAN_NSI theApp;

/**
 * CCAN_NSI initialization
 */
static HINSTANCE ghLangInst=NULL;

BOOL CCAN_NSI::InitInstance()
{
    // Begin of Multiple Language support
    if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(FALSE);
    }
    // Load resource-only language DLL. It will use the languages
    // detected above, take first available language,
    // or you can specify another language as second parameter to
    // LoadLangResourceDLL. And try that first.
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }
    // End of Multiple Language support

    for(UINT i = 0; i < MAX_CAN_MSG_PTR; i++)
    {
        sTxMsgTab[i].m_ucChannel = 0;
        sTxMsgTab[i].m_unMsgID = 0;
    }

    CWinApp::InitInstance();

    return TRUE;
}

/* canlib32.dll Message Functions */

/**
 * Number of Channels
 */
static UINT sg_nNoOfChannels = 0;
static char sg_omErrStr[MAX_STRING] = {0};

// Count variables
static UCHAR sg_ucNoOfHardware = 0;
const int MAX_DEVICES = 16;

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

UINT64 sg_TimeStampOffset[MAX_DEVICES] = {0};
UINT64 sg_TimeStampInit[MAX_DEVICES] = {0};

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

/**
 * Current state machine
 */
static BYTE sg_byCurrState[MAX_DEVICES] = {CREATE_MAP_TIMESTAMP};

typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)const
    {
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel));
    }
} SACK_MAP;
static HANDLE hEvent = NULL;
static CRITICAL_SECTION sg_DIL_CriticalSection;

static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe
typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;


/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_QueryTickCountBis;
static LARGE_INTEGER sg_lnFrequency;

/**
 * Channel information
 */
struct CChannel
{
    /* NSI channel details */
    int        m_nChannel;
    char       m_strName[MAX_CHAR_LONG];
    DWORD      m_dwHwType;
    int        m_nHwIndex;
    int        m_nHwChannel;
    int        m_nIsOnBus;
    int        m_nDriverMode;
    int        m_nTxAck;
    UINT       m_nLowSpeed;

    /* To store baud rate information */
    USHORT  m_usClock;
    USHORT  m_usSampling;
    UINT    m_unBaudrate;

    /* Bit Timing */
    USHORT  m_usBaudRate; /*BTR0,BTR1*/
    BYTE    m_bCNF1;
    BYTE    m_bCNF2;
    BYTE    m_bCNF3;

    /* Acceptance Filter information for standard and extended envelopes*/
    SACC_FILTER_INFO     m_sFilter[2];

    /** Programmed warning limit of this channel */
    UCHAR m_ucWarningLimit;

    /** Tx Error counter value */
    UCHAR m_ucTxErrorCounter;

    /** Rx Error counter value */
    UCHAR m_ucRxErrorCounter;

    /** Peak Tx Error counter value */
    UCHAR m_ucPeakTxErrorCounter;

    /** Peak Rx Error counter value */
    UCHAR m_ucPeakRxErrorCounter;

    /** Tx Error Handler execution state */
    BOOL m_bTxErrorExecuted;

    /** Rx Error Handler execution state */
    BOOL m_bRxErrorExecuted;

    // Init members with default value
    CChannel()
    {
        m_nIsOnBus      = 0;
        m_nDriverMode   = 0;
        m_nChannel      = -1;
        m_nTxAck        = 0; // Default is TxAck off

        // Baud Rate
        m_usBaudRate = defBAUD_RATE;

        // Programmed warning limit of this channel
        m_ucWarningLimit = defWARNING_LIMIT_INT;

        // Tx Error counter value
        m_ucTxErrorCounter = 0;

        // Rx Error counter value
        m_ucRxErrorCounter = 0;

        // Peak Tx Error counter value
        m_ucPeakTxErrorCounter = 0;

        // Peak Rx Error counter value
        m_ucPeakRxErrorCounter = 0;

        // Tx Error Handler execution state
        m_bTxErrorExecuted = FALSE;

        // Rx Error Handler execution state
        m_bRxErrorExecuted = FALSE;
    }
};

/**
 * Array of channel objects. The size of this array is maximun number
 * of channels the application will support.
 */
static CChannel sg_aodChannels[ defNO_OF_CHANNELS ];
static INT sg_anSelectedItems[CHANNEL_ALLOWED];


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
        hClientHandle = NULL;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }

    }
} SCLIENTBUFMAP;

/*
 * Forward declarations
 */
static void vWriteIntoClientsBuffer(STCANDATA& sCanData);
static UCHAR USB_ucGetErrorCode(LONG lError, BYTE byDir);
static void vCreateTimeModeMapping(HANDLE hDataEvent);
static BOOL bClientIdExist(const DWORD& dwClientId);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(string pcClientName, INT& Index);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static BOOL bRemoveClient(DWORD dwClientId);
BOOL Callback_DILNSI(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, INT /*Length*/);
int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/, PSCONTROLLER_DETAILS pControllerDetails, UINT nCount);
static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails);
int ListHardwareInterfaces(HWND hParent, DWORD , INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount);
//static int nCreateMultipleHardwareNetwork(UINT unDefaultChannelCnt = 0);
static int nCreateSingleHardwareNetwork();
static int nInitHwNetwork(UINT unDefaultChannelCnt = 0);
static int nConnect(BOOL bConnect, BYTE /*hClient*/);
static int nGetNoOfConnectedHardware(void);
static int nSetApplyConfiguration();
static int nSetFilter(UINT unDrvChannel, BOOL bWrite);
static int nSetBaudRate(UINT unDrvChannel);
static int WriteMessageIntoNSIDevice(STCAN_MSG sMessage);

/* TZM specific Global variables */
#define CAN_MAX_ERRSTR 256
string sg_acErrStr = "";
static UINT sg_unClientCnt = 0;
#define MAX_CLIENT_ALLOWED 16
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT m_unDevChannelMap[defNO_OF_CHANNELS] = {(UINT)-1};

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

// state variables
static BOOL sg_bIsConnected = FALSE;
static UCHAR sg_ucControllerMode = defUSB_MODE_ACTIVE;
static CPARAM_THREADPROC sg_sParmRThread;
static int sg_nFRAMES = 1280;//128
const int ENTRIES_IN_GBUF       = 20000;//2000
static STCANDATA sg_asCANMsg;
static STCAN_MSG sg_ReadMsg;
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];

/* CDIL_CAN_NSI class definition */
class CDIL_CAN_NSI : public CBaseDIL_CAN_Controller
{
public:
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
    HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT CAN_GetLastErrorString(string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    //MVN
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    //~MVN
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
};

CDIL_CAN_NSI* sg_pouDIL_CAN_NSI = NULL;
HANDLE g_hDataEvent[defNO_OF_CHANNELS]  = {0};


///------------------------------------------------------------------------------------------------
/// \brief         Returns the CDIL_CAN_NSI object
/// \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_CAN_NSI object
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------------------------------------------------
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == sg_pouDIL_CAN_NSI )
    {
        if ((sg_pouDIL_CAN_NSI = new CDIL_CAN_NSI) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouDIL_CAN_NSI;  /* Doesn't matter even if sg_pouDIL_CAN_NSI is null */

    return hResult;
}

/*************************************************
 *************************************************
 ******                                     ******
 ******      User function definitions      ******
 ******                                     ******
 *************************************************
 *************************************************/
///--------------------------------------------------------------------------------
/// \brief         This function returns the explicit name of the given error code
/// \param[in]     cr, error code
/// \return        Returns the name of the error
/// \authors       Gregory Merchat
/// \date          17.04.2013 Created
///--------------------------------------------------------------------------------
static char* GetCodeString(short cr)
{
    switch( cr )
    {
        case _OK :
            return ": OK.";
        case _MEM_ERR :
            return ": Memory error.";
        case _SEQ_ERR :
            return ": Sequence error.";
        case _PARAM_ERR :
            return ": Parameter error.";
        case _CHIP_ERR :
            return ": Chip error.";
        case _BOARD_ERR :
            return ": Board error.";
        case _ID_OVERFLOW :
            return "_ID_OVERFLOW";
        case _UNKNOWN_ID :
            return "_UNKNOWN_ID";
        case _UNKNOWN_PERIOD :
            return "_UNKNOWN_PERIOD";
        case _PERIODIC_OVERFLOW :
            return "_PERIODIC_OVERFLOW";
        case _EMPTY_BUF :
            return "_EMPTY_BUF";
        case _EMPTY_FIFO :
            return "_EMPTY_FIFO";
        case _FRAME_TYPE_ERR :
            return "_FRAME_TYPE_ERR";
        case _INTERFACE_ERR :
            return "_INTERFACE_ERR";
        case _BUF_OCCUPIED :
            return "_BUF_OCCUPIED";
        case _INVALID_OP :
            return ": Channel invalid or already in use.";
        case _DRV_PARAM_ERR :
            return ": Error acessing driver.";
        case _OPENING_DRV_ERR :
            return " : Error opening driver.";
        case _SLEEP_MODE :
            return "_SLEEP_MODE";
        case _USB_ERR :
            return ": USB error.";
        case _RS232_ERR :
            return ": RS232 error.";
        case _BOARD_TIMEOUT :
            return ": No USB acknowledge.";
        default :
            return "_???";
    }
}

///------------------------------------------------------------------------------------------------------------
/// \brief         This will get the elapse time between the connection time and the time of the first can msg.
/// \return        The elapse time.
/// \authors       Gregory Merchat
/// \date          16.05.2013 Created
///------------------------------------------------------------------------------------------------------------
ULONGLONG GetTimeStamp()
{
    ULONGLONG timeStamp, curSysTime, oldSysTime;
    SYSTEMTIME sysTime = {0};
    GetLocalTime(&sysTime);
    curSysTime = (sysTime.wHour * 3600 + sysTime.wMinute * 60 + sysTime.wSecond) * 10000 + sysTime.wMilliseconds * 10;
    oldSysTime = (sg_CurrSysTime.wHour * 3600 + sg_CurrSysTime.wMinute * 60 + sg_CurrSysTime.wSecond) * 10000 + sg_CurrSysTime.wMilliseconds * 10;
    timeStamp  = curSysTime - oldSysTime;
    return timeStamp;
}

///-------------------------------------------------------------------------------------------------------------
/// \brief         This function returns a formated char string describing the main data in the event structure
///                pointed by "pEvent". "string" must point to a buffer large enough to contain the text.
/// \param[in]     pEvent, event structure
///                unDrvChannel, number of the driver channel.
/// \return        void.
/// \authors       Grégory Merchat
/// \date          18.04.2013 Created
///-------------------------------------------------------------------------------------------------------------
void GetEventString(t_CANevent* pEvent, UINT unDrvChannel)
{
    ULONG64 timeStamp = pEvent->timeStamp;
    LARGE_INTEGER g_QueryTickCount;
    char* pchPCMCIA;
    char* pchPCI;
    pchPCMCIA = strstr(sg_aodChannels[unDrvChannel].m_strName, "CANPCMCIA");
    pchPCI = strstr(sg_aodChannels[unDrvChannel].m_strName, "CANPCI");
    if((pchPCMCIA != NULL) || (pchPCI != NULL))
    {
        if(sg_byCurrState[unDrvChannel] == CREATE_MAP_TIMESTAMP)
        {
            sg_TimeStampInit[unDrvChannel] = GetTimeStamp();
            sg_TimeStampOffset[unDrvChannel] = sg_TimeStampInit[unDrvChannel];
            sg_asCANMsg.m_lTickCount.QuadPart = sg_TimeStampOffset[unDrvChannel];
            sg_byCurrState[unDrvChannel] = CALC_TIMESTAMP_READY;
        }
        else
        {
            g_QueryTickCount.QuadPart = GetTimeStamp();
            sg_asCANMsg.m_lTickCount.QuadPart = sg_TimeStampOffset[unDrvChannel] + (g_QueryTickCount.QuadPart - sg_TimeStampInit[unDrvChannel]);
        }
    }
    else
    {
        if(sg_byCurrState[unDrvChannel] == CREATE_MAP_TIMESTAMP)
        {
            sg_TimeStampInit[unDrvChannel] = timeStamp;
            sg_TimeStampOffset[unDrvChannel] = GetTimeStamp();
            sg_asCANMsg.m_lTickCount.QuadPart = sg_TimeStampOffset[unDrvChannel];
            sg_byCurrState[unDrvChannel] = CALC_TIMESTAMP_READY;
        }
        else
        {
            g_QueryTickCount.QuadPart = timeStamp;
            sg_asCANMsg.m_lTickCount.QuadPart = sg_TimeStampOffset[unDrvChannel] + (g_QueryTickCount.QuadPart - sg_TimeStampInit[unDrvChannel]);
        }
    }
    if(!(pEvent->eventType == _CAN_LOST_MSG))
    {
        switch(pEvent->eventType)
        {
            case _CAN_TX_RX_REMOTE :
            case _CAN_TX_REMOTE :
            case _CAN_TX_AUTO_REMOTE :
            case _CAN_TX_DATA :
                sg_asCANMsg.m_ucDataType = TX_FLAG;
                break;
            case _CAN_RX_DATA_REMOTE :
            case _CAN_RX_REMOTE :
            case _CAN_RX_DATA :
                sg_asCANMsg.m_ucDataType = RX_FLAG;
                break;
            default :
                sg_asCANMsg.m_ucDataType = ERR_FLAG;
                break;
        }
        sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)unDrvChannel+1;
        sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = pEvent->ident;
        sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = (UCHAR)pEvent->identType;
        sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = (UCHAR)pEvent->dlc;
        /*for( i=0; i<pEvent->dlc && i<=_CAN_MAX_DATA; i++ )
        {
            sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[i] = pEvent->data[i];
        }*/
        memcpy(sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData, pEvent->data, pEvent->dlc);
    }
    else
    {
        sg_asCANMsg.m_ucDataType = ERR_FLAG;
        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)unDrvChannel+1;
        sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)unDrvChannel+1;

        // Assign error type in the Error Capture register
        // and the direction of the error
        if (pEvent->eventType == _CAN_TX_DATA)
        {
            sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap = ERROR_DEVICE_BUFF_OVERFLOW;
        }
        else
        {
            sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap = OTHER_ERROR_RX;
        }
        //explaination of error bit
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_nSubError= 0;
    }
    //Write into client buffer.
    vWriteIntoClientsBuffer(sg_asCANMsg);
}

///---------------------------------------------------------------------------------------------------
/// \brief         Read thread procedure
/// \param[in]     pVoid contains the CPARAM_THREADPROC class object
/// \return        0
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------------------------------------------
DWORD WINAPI CanMsgReadThreadProc_CAN_NSI(LPVOID pVoid)
{
    USES_CONVERSION;
    int nStatus = 0;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    /* Validate certain required pointers */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam, (DWORD)-1);

    //get CAN - eventHandles
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        /* Indicates to the CAN interface the HANDLE of the event object used to signal events about the FIFO.
           See the Thread and CreateEvent functions. */
        cr = Ic_ConfigEvent(NSI_hCanal[sg_aodChannels[i].m_nChannel], g_hDataEvent[i], 0);
        if(cr != _OK)
        {
            // Display a message in a new window
            CString omErr;
            omErr.Format(_("Ic_ConfigEvent : NSI %s"), GetCodeString(cr));
            AfxMessageBox(omErr);
        }
    }
    //Apply filters for read handles
    //nSetFilter(false);
    if (g_hDataEvent[0] != NULL)
    {
        pThreadParam->m_hActionEvent = g_hDataEvent[0];
    }
    SetEvent(pThreadParam->m_hActionEvent);
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;

    bool bLoopON = true;
    //int moreDataExist;
    /*static UINT unFlags = 0;
    static DWORD dwTime = 0;
    unsigned char   ucData[8];*/
    t_CANevent NSI_event;

    //New approach{{
    while (bLoopON)
    {
        WaitForMultipleObjects(sg_nNoOfChannels, g_hDataEvent, FALSE, INFINITE);
        for (UINT i = 0; i < sg_nNoOfChannels ; i++)
        {
            //Loop while events are found in FIFO
            while(Ic_GetEvent(NSI_hCanal[sg_aodChannels[i].m_nChannel], &NSI_event) == _OK)
            {
                //count++;
                GetEventString(&NSI_event, i);
            }
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());
    for (UINT i = 0; i < sg_nNoOfChannels+1; i++)
    {
        ResetEvent(g_hDataEvent[i]);
        g_hDataEvent[i] = NULL;
    }
    pThreadParam->m_hActionEvent = NULL;

    return 0;
}

///----------------------------------------------------------------------------------
/// \brief         Writes the message 'sCanData' to the corresponding clients buffer
/// \param[out]    sCanData, is STCANDATA structure
/// \return        void
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    /* Write into the respective client's buffer */
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
        {
            sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sCanData);
        }
    }
}

///---------------------------------------------------------------------------
/// \brief         This will convert the error code from Kvaser driver format
///                to the format that is used by BUSMASTER.
/// \param[in]     lError Error code in Peak USB driver format
/// \param[in]     byDir  Error direction Tx/Rx
/// \return        UCHAR which indicates error code
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------------------
static UCHAR USB_ucGetErrorCode(LONG lError, BYTE byDir)
{
    UCHAR ucReturn = 0;

    // Tx Errors
    if( byDir == 1)
    {
        if (lError & _CAN_LOST_MSG)
        {
            ucReturn = STUFF_ERROR_TX;
        }
        else
        {
            ucReturn = OTHER_ERROR_TX;
        }
    }
    // Rx Errors
    else
    {
        if (lError & _CAN_LOST_MSG)
        {
            ucReturn = STUFF_ERROR_RX;
        }
        else
        {
            ucReturn = OTHER_ERROR_RX;
        }
    }
    // Return the error code
    return ucReturn;
}

///-----------------------------------------------------
/// \brief         Function to create time mode mapping
/// \param[in]     hDataEvent, is HANDLE
/// \return        void
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-----------------------------------------------------
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{
    WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);
}
///---------------------------------------------------------------------------
/// \brief         This function will check if the client ID exists
/// \param[in]     dwClientId, client ID to be checked for existance
/// \return        Returns TRUE if client ID existance is success, else FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------------------
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

///-------------------------------------------------------------
/// \brief         This function will get available client slot
/// \param         void
/// \return        Returns the available client ID
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-------------------------------------------------------------
static DWORD dwGetAvailableClientSlot()
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

///----------------------------------------------------------------------------------------------
/// \brief         This function will check if the client exists and gives back the client index.
/// \param[in]     pcClientName, client name as TCHAR*
/// \param[out]    Index, client index if found
/// \return        TRUE if client name is found, else FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------------------
static BOOL bClientExist(string pcClientName, INT& Index)
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

///-----------------------------------------------------------------------
/// \brief         Function to remove exissting client buffer
/// \param[in]     RootBufferArray, pointer to CBaseCANBufFSE class array
/// \param[out]    unCount, indicates buffer count which will get reduced
/// \param[in]     BufferToRemove, pointer to the buffer to be removed
/// \return        TRUE if removed
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-----------------------------------------------------------------------
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

///----------------------------------------------------------
/// \brief         Function to check if client buffer exists
/// \param[in]     sClientObj, alias to SCLIENTBUFMAP object
/// \param[in]     pBuf, pointer to CBaseCANBufFSE object
/// \return        TRUE if buffer exists, else FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------
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
    return bExist;
}

///---------------------------------------------------------------
/// \brief         Function to retreive error occurred and log it
/// \param[in]     File, pointer to log file
/// \param[in]     Line, indicates line number in log file
/// \return        void
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};

    /* Get the error text for the corresponding error code */
    sg_pIlog->vLogAMessage(A2T(File), Line, A2T(acErrText));

    size_t nStrLen = strlen(acErrText);
    if (nStrLen > CAN_MAX_ERRSTR)
    {
        nStrLen = CAN_MAX_ERRSTR;
    }
    sg_acErrStr = acErrText;
}

///-----------------------------------------------------------------------------
/// \brief         This function will return the client index based on clientID
/// \param[in]     dwClientId, client ID whose client index is needed
/// \param[out]    unClientIndex, client index to be returned
/// \return        Returns TRUE if client ID existance is success, else FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-----------------------------------------------------------------------------
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}

///-------------------------------------------------------------------------
/// \brief         This function will remove the existing client ID
/// \param[in]     dwClientId, client ID to be removed
/// \return        Returns TRUE if client ID removal is success, else FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-------------------------------------------------------------------------
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            /* clear the client first */
            if (sg_asClientToBufMap[unClientIndex].hClientHandle != NULL)
            {
                HRESULT hResult = S_OK;//(*pfCAN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                    sg_asClientToBufMap[unClientIndex].hClientHandle = NULL;
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                    }
                    sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                    bResult = TRUE;
                }
                else
                {
                    vRetrieveAndLog(hResult, __FILE__, __LINE__);
                }
            }
            else
            {
                sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                {
                    sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                }
                sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                bResult = TRUE;

            }
            if (bResult == TRUE)
            {
                if ((unClientIndex + 1) < sg_unClientCnt)
                {
                    sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
                }
                sg_unClientCnt--;
            }
        }
    }
    return bResult;
}

///----------------------------------------------------------------------------------------------
/// \brief         This function will check all hardware connectivity by switching to channel ON.
/// \param[out]    ucaTestResult Array that will hold test result.
///                TRUE if hardware present and FALSE if not connected
/// \param[in]     nChannel, indicates channel ID
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------------------
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) //const
{
    int nReturn = 0;
    unsigned long mode;
    if (nChannel < sg_nNoOfChannels)
    {
        cr = Ic_GetMode(NSI_hCanal[nChannel], &mode);
        if(cr != _OK)
        {
            // Display a message in a new window
            CString omErr;
            omErr.Format(_("NSI - %s %s"), sg_HardwareIntr[nChannel].m_acDescription, GetCodeString(cr));
            AfxMessageBox(omErr);
        }
        else
        {
            if(mode == _CAN_NORMAL)
            {
                ucaTestResult = TRUE;
            }
            else
            {
                ucaTestResult = FALSE;
            }
        }
    }
    return nReturn;
}

///--------------------------------------------------------------------
/// \brief         Callback function for configuration dialog
/// \param[in]     pDatStream, contains SCONTROLLER_DETAILS structure
/// \return        TRUE if CAN_SetConfigData call succeeds, else FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///--------------------------------------------------------------------
BOOL Callback_DILNSI(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, INT /*Length*/)
{
    return (sg_pouDIL_CAN_NSI->CAN_SetConfigData( pDatStream, 0) == S_OK);
}

///-----------------------------------------------------------------
/// \brief         Displays the configuration dialog for controller
/// \return        S_OK for success, S_FALSE for failure
///-----------------------------------------------------------------
int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/, PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

    CChangeRegisters ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
    ouChangeRegister.DoModal();
    nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}

///-------------------------------------------------------------------
/// \brief         Copies the controller config values into channel's
///                controller config structure.
/// \return        TRUE for success, FALSE for failure
///-------------------------------------------------------------------
static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;
    // If successful
    if (pControllerDetails != NULL)
    {
        for( UINT nIndex = 0; nIndex < sg_nNoOfChannels ; nIndex++ )
        {
            char* pcStopStr = NULL;
            CChannel& odChannel = sg_aodChannels[nIndex];

            // Baudrate in BTR0BTR1 format
            odChannel.m_usBaudRate = static_cast <USHORT>(pControllerDetails[nIndex].m_nBTR0BTR1);
            // Baudrate value in decimal
            odChannel.m_unBaudrate = static_cast <UINT>(
                                         _tcstol( pControllerDetails[nIndex].m_omStrBaudrate.c_str(),
                                                  &pcStopStr, defBASE_DEC ));

            // Get Warning Limit
            odChannel.m_ucWarningLimit = static_cast <UCHAR>(
                                             _tcstol( pControllerDetails[nIndex].m_omStrWarningLimit.c_str(),
                                                     &pcStopStr, defBASE_DEC ));

            odChannel.m_nLowSpeed = pControllerDetails[nIndex].m_bLowSpeed;
            strcpy_s(odChannel.m_strName, pControllerDetails[nIndex].m_omHardwareDesc.c_str());

            for ( int i = 0; i < CAN_MSG_IDS ; i++ )
            {
                // Get Acceptance Filter
                if ( pControllerDetails[ nIndex ].m_enmHWFilterType[i] == HW_FILTER_ACCEPT_ALL )
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code3 = 0;

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask3 = 0;
                }
                else if( pControllerDetails[ nIndex ].m_enmHWFilterType[i] == HW_FILTER_REJECT_ALL )
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code3 = 0;

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask1 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask2 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask3 = 0xFF;
                }
                else
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte1[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code1 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte2[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code2 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte3[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code3 = static_cast <UCHAR>(
                            _tcstol(pControllerDetails[ nIndex ].m_omStrAccCodeByte4[i].c_str(),
                                    &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte1[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask1 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte2[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask2 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte3[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask3 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte4[i].c_str(),
                                     &pcStopStr, defBASE_HEX));
                }
                odChannel.m_sFilter[i].m_ucACC_Filter_Type = (UCHAR)pControllerDetails[nIndex].m_bAccFilterMode ;
            }


            // Get Baud Rate
            odChannel.m_usBaudRate = static_cast <USHORT>(pControllerDetails[nIndex].m_nBTR0BTR1 );
        }
        // Get Controller Mode
        // Consider only the first channel mode as controller mode
        sg_ucControllerMode = (UCHAR)pControllerDetails[0].m_ucControllerMode;
        bReturn = TRUE;
    }
    return bReturn;
}

///---------------------------------------------------------------------------
/// \brief        This function will popup hardware selection dialog and gets
///               the user selection of channels.
/// \return       Operation Result. 0 incase of no errors.
///               Failure Error codes otherwise.
///---------------------------------------------------------------------------
int ListHardwareInterfaces(HWND hParent, DWORD , INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, &objMainWnd);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList); // Get the number of the selected device
        return 0;
    }
    else
    {
        return -1;
    }
}

///----------------------------------------------------------------------------
/// \brief         This function will get the hardware selection from the user
///                and will create essential networks.
/// \param         unDefaultChannelCnt
/// \return        returns defERR_OK (always)
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------
static int nCreateMultipleHardwareNetwork(UINT unDefaultChannelCnt = 0)
{
    int nHwCount = sg_ucNoOfHardware;
    //DWORD dwFirmWare[2];
    char chBuffer[512] = "";
    // Display a message in a new window
    CString omErr;
    // Get Hardware Network Map
    for (int nCount = 0; nCount < nHwCount; nCount++)
    {
        cr = Ic_InitDrv((short)nCount, &NSI_hCanal[nCount]);
        if(cr != _OK)
        {
            nCount = nHwCount;
        }
        cr = Ic_GetDeviceInfo(NSI_hCanal[nCount], &NSI_deviceInfo[nCount]);
        cr = Ic_ExitDrv(NSI_hCanal[nCount]);
        if(cr != _OK)
        {
            nCount = nHwCount;
        }
        switch(NSI_deviceInfo[nCount].deviceType)
        {
            case _CANPCISA: // CANPC, CANPCa, CANPCMCIA, CAN104
                strcpy_s(chBuffer, "NSI - ");
                strcat_s(chBuffer, NSI_deviceInfo[nCount].CAN_ISA.cardName);
                sg_HardwareIntr[nCount].m_acDescription = chBuffer;
                sg_HardwareIntr[nCount].m_dwIdInterface = nCount;
                flagLowSpeed = FALSE;
                break;
            case _CANPCI2P: // CANPCI
                strcpy_s(chBuffer, "NSI - ");
                strcat_s(chBuffer, NSI_deviceInfo[nCount].CAN_PCI.cardName);
                sg_HardwareIntr[nCount].m_acDescription = chBuffer;
                sg_HardwareIntr[nCount].m_dwIdInterface = nCount;
                flagLowSpeed = FALSE;
                break;
            case _CANPCUSB: // CAN-USB Interface
                sg_HardwareIntr[nCount].m_dwIdInterface = nCount;
                flagLowSpeed = FALSE;
                break;
            case _OBDUSB: // MUXy, MUXy light
                strcpy_s(chBuffer, NSI_deviceInfo[nCount].CAN_USB.manufacturerName);
                strcat_s(chBuffer, " - ");
                strcat_s(chBuffer, NSI_deviceInfo[nCount].CAN_USB.productName);
                sg_HardwareIntr[nCount].m_acDeviceName = NSI_deviceInfo[nCount].CAN_USB.serialNumber;
                sg_HardwareIntr[nCount].m_acDescription = chBuffer;
                sg_HardwareIntr[nCount].m_dwIdInterface = nCount;
                flagLowSpeed = FALSE;
                break;
            case _USBBOX: // MUXyBox
                strcpy_s(chBuffer, NSI_deviceInfo[nCount].CAN_USB.manufacturerName);
                strcat_s(chBuffer, " - ");
                strcat_s(chBuffer, NSI_cardData[nCount].cardNameString);
                sg_HardwareIntr[nCount].m_acDeviceName = NSI_deviceInfo[nCount].CAN_USB.serialNumber;
                sg_HardwareIntr[nCount].m_acDescription = chBuffer;
                sg_HardwareIntr[nCount].m_dwIdInterface = nCount;
                flagLowSpeed = TRUE;
                break;
            case _USBMUXY2: //MUXy2010 et MUXybox 2
                strcpy_s(chBuffer, NSI_deviceInfo[nCount].CAN_USB.manufacturerName);
                strcat_s(chBuffer, " - ");
                strcat_s(chBuffer, NSI_cardData[nCount].cardNameString);
                sg_HardwareIntr[nCount].m_acDeviceName = NSI_deviceInfo[nCount].CAN_USB.serialNumber;
                sg_HardwareIntr[nCount].m_acDescription = chBuffer;
                sg_HardwareIntr[nCount].m_dwIdInterface = nCount;
                flagLowSpeed = TRUE;
                break;
        }
    }

    /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
    if ( unDefaultChannelCnt && (UINT)nHwCount >= unDefaultChannelCnt )
    {
        for (UINT i = 0; i < unDefaultChannelCnt; i++)
        {
            sg_anSelectedItems[i] = i;
        }
        nHwCount = unDefaultChannelCnt;
    }
    else if ( ListHardwareInterfaces(sg_hOwnerWnd, DRIVER_CAN_NSI, sg_HardwareIntr, sg_anSelectedItems, nHwCount) != 0 )
    {
        return HW_INTERFACE_NO_SEL;
    }
    sg_ucNoOfHardware = (UCHAR)nHwCount; // copy the number of the selected device
    sg_nNoOfChannels = (UINT)nHwCount; // copy the number of the selected device

    //Reorder hardware interface as per the user selection
    for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_aodChannels[nCount].m_nChannel = sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface;
        /*_stprintf(sg_aodChannels[nCount].m_strName , _T("%s, Serial Number - %s"),
                    sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDescription.c_str(),
                    sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDeviceName.c_str());*/
        sprintf_s(sg_aodChannels[nCount].m_strName , _T("%s, Serial Number - %s"),
                  sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDescription.c_str(),
                  sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDeviceName.c_str());
    }
    return defERR_OK;
}

///------------------------------------------------------------------------------------------
/// \brief         This function will create a single network with available single hardware.
/// \param         void
/// \return        returns defERR_OK (always)
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------------------------------------------
static int nCreateSingleHardwareNetwork()
{
    /* Set the number of channels as 1 */
    sg_ucNoOfHardware = (UCHAR)1;
    sg_nNoOfChannels = 1;
    sg_aodChannels[0].m_nChannel = 0;
    char chBuffer[512] = "";

    /* Update channel info */
    cr = Ic_InitDrv((short)0, &NSI_hCanal[0]);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_InitDrv Failed"));
        AfxMessageBox(omErr);
        return 1;
    }
    cr = Ic_GetDeviceInfo(NSI_hCanal[0], &NSI_deviceInfo[0]);
    cr = Ic_ExitDrv(NSI_hCanal[0]);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_ExitDrv Failed"));
        AfxMessageBox(omErr);
        return 1;
    }
    switch(NSI_deviceInfo[0].deviceType)
    {
        case _CANPCISA: // CANPC, CANPCa, CANPCMCIA, CAN104
            flagLowSpeed = FALSE;
            break;
        case _CANPCI2P: // CANPCI
            flagLowSpeed = FALSE;
            break;
        case _CANPCUSB: // CAN-USB Interface
            flagLowSpeed = FALSE;
            break;
        case _OBDUSB: // MUXy, MUXy light
            strcpy_s(chBuffer, NSI_deviceInfo[0].CAN_USB.manufacturerName);
            strcat_s(chBuffer, " - ");
            strcat_s(chBuffer, NSI_deviceInfo[0].CAN_USB.productName);
            sg_HardwareIntr[0].m_acDeviceName = NSI_deviceInfo[0].CAN_USB.serialNumber;
            sg_HardwareIntr[0].m_acDescription = chBuffer;
            sg_HardwareIntr[0].m_dwIdInterface = 0;
            flagLowSpeed = FALSE;
            break;
        case _USBBOX: // MUXyBox
            strcpy(chBuffer, NSI_deviceInfo[0].CAN_USB.manufacturerName);
            strcat(chBuffer, " - ");
            strcat(chBuffer, NSI_deviceInfo[0].CAN_USB.productName);
            sg_HardwareIntr[0].m_acDeviceName = NSI_deviceInfo[0].CAN_USB.serialNumber;
            sg_HardwareIntr[0].m_acDescription = chBuffer;
            sg_HardwareIntr[0].m_dwIdInterface = 0;
            flagLowSpeed = TRUE;
            break;
        case _USBMUXY2: //MUXy2010 et MUXybox 2
            strcpy(chBuffer, NSI_deviceInfo[0].CAN_USB.manufacturerName);
            strcat(chBuffer, " - ");
            strcat(chBuffer, NSI_cardData[0].cardNameString);
            sg_HardwareIntr[0].m_acDeviceName = NSI_deviceInfo[0].CAN_USB.serialNumber;
            sg_HardwareIntr[0].m_acDescription = chBuffer;
            sg_HardwareIntr[0].m_dwIdInterface = 0;
            flagLowSpeed = TRUE;
            break;
    }

    /*_stprintf(sg_aodChannels[0].m_strName , _T("%s, Serial Number - %s"),
                    sg_HardwareIntr[sg_anSelectedItems[0]].m_acDeviceName.c_str(),
                    sg_HardwareIntr[sg_anSelectedItems[0]].m_acDescription.c_str());*/
    sprintf_s(sg_aodChannels[0].m_strName , _T("%s, Serial Number - %s"),
              sg_HardwareIntr[sg_anSelectedItems[0]].m_acDeviceName.c_str(),
              sg_HardwareIntr[sg_anSelectedItems[0]].m_acDescription.c_str());

    return defERR_OK;
}

///---------------------------------------------------------------------------------------------------
/// \brief         This function will find number of hardwares connected.
///                It will create network as per hardware count.
///                This will popup hardware selection dialog in case there are more hardwares present.
/// \param         unDefaultChannelCnt
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------------------------------------------
static int nInitHwNetwork(UINT unDefaultChannelCnt)
{
    int nChannelCount = 0;
    int nResult = NO_HW_INTERFACE;

    /* Select Hardware */
    cr = Ic_EnumCards(&cardCount, NSI_cardData, sizeof(NSI_cardData));
    if( cr != _OK )
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_EnumCards : %s "), GetCodeString(cr));
        AfxMessageBox(omErr);
    }
    nChannelCount = cardCount;
    // Assign the channel count
    sg_ucNoOfHardware = (UCHAR)nChannelCount;

    /* Capture only Driver Not Running event
     * Take action based on number of Hardware Available
     */
    char acNo_Of_Hw[MAX_STRING] = {0};
    //_stprintf(acNo_Of_Hw, _T("Number of NSI hardwares Available: %d"), nChannelCount);
    sprintf_s(acNo_Of_Hw, _T("Number of NSI hardwares Available: %d"), nChannelCount);

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
        //_stprintf(sg_omErrStr, _T("No NSI hardwares Available.\nPlease check if NSI drivers are installed."));
        sprintf_s(sg_omErrStr, _T("No NSI hardwares Available.\nPlease check if NSI drivers are installed."));
        nChannelCount = -1;
    }
    /* Available hardware is lesser then the supported channels */
    else
    {
        // Check whether channel selection dialog is required
        /*if( nChannelCount > 1)
        {
            // Get the selection from the user. This will also
            // create and assign the networks
            nResult = nCreateMultipleHardwareNetwork(unDefaultChannelCnt);
        }
        else
        {
            if(NSI_cardData[0].cardAlreadyOpen == 1)
            {
                nResult = 1;
            }
            else
            {
                // Use available one hardware
                nResult = nCreateSingleHardwareNetwork();
            }
        }*/
        nResult = nCreateMultipleHardwareNetwork(unDefaultChannelCnt);
    }
    return nResult;
}

///---------------------------------------------------------------------------------------
/// \brief         This function will open the CAN canal associate with the parameter.
/// \param         unDrvChannel, number of the driver channel.
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          16.04.2013 Created
///---------------------------------------------------------------------------------------
static int nInitDriver(UINT unDrvChannel)
{
    int nResult = 0;
    cr = Ic_InitDrv((short)unDrvChannel, &NSI_hCanal[unDrvChannel]);
    if( cr != _OK )
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_InitDrv : %s %s"), sg_HardwareIntr[unDrvChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nResult = 1;
    }
    return nResult;
}

///---------------------------------------------------------------------------------------
/// \brief         This function will close the CAN canal associate with the parameter.
/// \param         unDrvChannel, number of the driver channel.
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          16.04.2013 Created
///---------------------------------------------------------------------------------------
static int nExitDriver(UINT unDrvChannel)
{
    int nResult = 0;
    cr = Ic_ExitDrv(NSI_hCanal[unDrvChannel]);
    if( cr != _OK )
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_ExitDrv : %s %s"), sg_HardwareIntr[unDrvChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nResult = 1;
    }
    return nResult;
}

///-----------------------------------------------------------------------------------------
/// \brief         This function will init the bus parameters associate with the parameters.
/// \param         unDrvChannel, number of the driver channel.
///                canBus, bus CAN parameters structure.
///                adressing, type of adressing.
///                padding, lenght of frames.
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          16.04.2013 Created
///-----------------------------------------------------------------------------------------
static int nInitBusParam(UINT unDrvChannel, t_CANbusParams canBus, t_CANaddressing adressing, t_CANpadding padding)
{
    int nResult = 0;
    cr = Ic_InitChip(NSI_hCanal[unDrvChannel], canBus, adressing, padding);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_InitChip : %s %s"), sg_HardwareIntr[unDrvChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nResult = 1;
    }
    return nResult;
}

///---------------------------------------------------------------------------------------
/// \brief         This function will init the Rx mask parameters.
/// \param         unDrvChannel, number of the driver channel.
///                code, id comparator.
///                mask, mask comparator.
///                identType, type of identificator.
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          17.04.2013 Created
///---------------------------------------------------------------------------------------
static int nInitRxMask(UINT unDrvChannel, unsigned long code, unsigned long mask, t_CANidentType identType)
{
    int nResult = 0;
    cr = Ic_SetRxMask(NSI_hCanal[unDrvChannel], code, mask, identType);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_SetRxMask : %s %s"), sg_HardwareIntr[unDrvChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nResult = 1;
    }
    return nResult;
}

///----------------------------------------------------------------------------------------
/// \brief         This function will init the interface associate with the parameters.
/// \param         unDrvChannel, number of the driver channel.
///                mode, desired type of interface
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          16.04.2013 Created
///----------------------------------------------------------------------------------------
static int nInitInterface(UINT unDrvChannel, t_Interface mode)
{
    int nResult = 0;
    cr = Ic_InitInterface(NSI_hCanal[unDrvChannel], mode);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_InitInterface : %s %s"), sg_HardwareIntr[unDrvChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nResult = 1;
    }
    return nResult;
}

///----------------------------------------------------------------------------------------
/// \brief         This function will init the line driver associate with the parameters.
/// \param[in]     unDrvChannel, number of the driver channel.
///                mode, desired type of line driver
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          07.05.2013 Created
///----------------------------------------------------------------------------------------
static int nInitLineDriver(UINT unDrvChannel, short mode)
{
    int nResult = 0;
    cr = Ic_InitLineDrv(NSI_hCanal[unDrvChannel], mode, NSI_hWakeUp, TRUE);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_InitLineDrv : %s %s"), sg_HardwareIntr[unDrvChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nResult = 1;
    }
    return nResult;
}

///------------------------------------------------------------------------
/// \brief         Function to set the channel baudrate configured by user
/// \param[in]     unDrvChannel, number of the driver channel.
/// \return        canOK if succeeded, else respective error code
/// \authors       Grégory Merchat
/// \date          19.04.2013 Created
///------------------------------------------------------------------------
static int nSetBaudRate(UINT unDrvChannel)
{
    int nStatus = 0;

    NSI_canBus.baudpresc = 0;
    NSI_canBus.sjw = 0;
    NSI_canBus.tseg1 = 0;
    NSI_canBus.tseg2 = 0;
    NSI_canBus.sample = 0;

    // Get Current channel reference
    CChannel& odChannel = sg_aodChannels[unDrvChannel];

    BYTE BTR0, BTR1;
    //0x47 14
    BTR0 = odChannel.m_usBaudRate >> 8;
    BTR1 = odChannel.m_usBaudRate & 0xFF;

    NSI_canBus.baudpresc = (BTR0 & NSI_MASK_BRP)+1;
    NSI_canBus.sjw = (BTR0 >> 6)+1;
    NSI_canBus.tseg1 = (BTR1 & NSI_MASK_TSEG1)+1;
    NSI_canBus.tseg2 = ((BTR1 >> 4) & NSI_MASK_TSEG2)+1;
    NSI_canBus.sample = (BTR1 >> 7);

    cr = Ic_InitChip(NSI_hCanal[odChannel.m_nChannel], NSI_canBus, _DC_NORMAL, _DC_NO_PADDING);
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_InitChip : %s %s"), sg_HardwareIntr[odChannel.m_nChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nStatus = -1;
    }
    return nStatus;
}

///----------------------------------------------------------------------
/// \brief         Function to set the channel filter configured by user
/// \param[in]     unDrvChannel, number of the driver channel.
/// \return        canOK if succeeded, else respective error code
/// \authors       Grégory Merhat
/// \date          19.04.2013 Created
///----------------------------------------------------------------------
static int nSetFilter(UINT unDrvChannel, BOOL /*bWrite*/)
{
    int nStatus = 0;

    // Set the client filter
    // Create DWORD Filter
    DWORD dwCode = 0, dwMask = 0;
    // To set no. shifts
    int nShift = sizeof( UCHAR ) * defBITS_IN_BYTE;
    // Get the Filter
    const SACC_FILTER_INFO& sFilter = sg_aodChannels[unDrvChannel].m_sFilter[0];
    // Create Code
    dwCode = ( sFilter.m_ucACC_Code3 << nShift * 3 ) |
             ( sFilter.m_ucACC_Code2 << nShift * 2 ) |
             ( sFilter.m_ucACC_Code1 << nShift ) |
             sFilter.m_ucACC_Code0;
    // Create Mask
    dwMask = ( sFilter.m_ucACC_Mask3 << nShift * 3 ) |
             ( sFilter.m_ucACC_Mask2 << nShift * 2 ) |
             ( sFilter.m_ucACC_Mask1 << nShift ) |
             sFilter.m_ucACC_Mask0;
    if(sFilter.m_ucACC_Filter_Type == _CAN_STD)
    {
        cr = Ic_SetRxMask(NSI_hCanal[sg_aodChannels[unDrvChannel].m_nChannel], dwCode, dwMask, _CAN_STD);
    }
    else
    {
        cr = Ic_SetRxMask(NSI_hCanal[sg_aodChannels[unDrvChannel].m_nChannel], dwCode, dwMask, _CAN_EXT);
    }
    if(cr != _OK)
    {
        // Display a message in a new window
        CString omErr;
        omErr.Format(_("Ic_SetRxMask : %s %s"), sg_HardwareIntr[sg_aodChannels[unDrvChannel].m_nChannel].m_acDescription.c_str(), GetCodeString(cr));
        AfxMessageBox(omErr);
        nStatus = -1;
    }
    return nStatus;
}

///-------------------------------------------------------------------
/// \brief         Function to apply filters and baudrate to channels
/// \param         void
/// \return        defERR_OK if succeeded, else respective error code
/// \authors       Grégory Merhat
/// \date          19.04.2013 Created
///-------------------------------------------------------------------
static int nSetApplyConfiguration()
{
    int nReturn = defERR_OK;
    char* pchMuxyBox2;
    char* pchMuxy2010;
    char* pchMuxyBox;
    char* pchPCMCIA;

    for (UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        // Sequence to follow before setting baudrate
        nReturn = nExitDriver((UINT)sg_aodChannels[unIndex].m_nChannel);
        nReturn = nInitDriver((UINT)sg_aodChannels[unIndex].m_nChannel);
        // Set baudrate
        nReturn = nSetBaudRate(unIndex);
        // Sequence to follow before setting Rx filter
        nReturn = nInitInterface((UINT)sg_aodChannels[unIndex].m_nChannel, _FIFO);
        pchMuxyBox2 = strstr(sg_aodChannels[unIndex].m_strName, "MuxyBox2-CAN CH2");
        pchMuxy2010 = strstr(sg_aodChannels[unIndex].m_strName, "MUXYv2-CAN CH2");
        pchMuxyBox = strstr(sg_aodChannels[unIndex].m_strName, "MUXy box CH");
        pchPCMCIA = strstr(sg_aodChannels[unIndex].m_strName, "CANPCMCIA /LS");
        if(pchMuxyBox2 != NULL)
        {
            if(sg_aodChannels[unIndex].m_nLowSpeed == BST_CHECKED)
            {
                nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_LOW_SPEED);
            }
            else
            {
                nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_HIGH_SPEED);
            }
        }
        if(pchMuxy2010 != NULL)
        {
            if(sg_aodChannels[unIndex].m_nLowSpeed == BST_CHECKED)
            {
                nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_LOW_SPEED);
            }
            else
            {
                nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_HIGH_SPEED);
            }
        }
        if(pchMuxyBox != NULL)
        {
            if(sg_aodChannels[unIndex].m_nLowSpeed == BST_CHECKED)
            {
                nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_LOW_SPEED);
            }
            else
            {
                nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_HIGH_SPEED);
            }
        }
        /*if(pchPCMCIA != NULL)
        {
            nReturn = nInitLineDriver((UINT)sg_aodChannels[unIndex].m_nChannel, _CAN_LOW_SPEED);
        }*/
        // Set Filter
        nReturn = nSetFilter(unIndex, TRUE);
    }
    return nReturn;
}

///-----------------------------------------------------------------------------------
/// \brief         This function will connect the tool with hardware.
/// \param[in]     bConnect TRUE to Connect, FALSE to Disconnect
/// \return        Returns defERR_OK if successful otherwise corresponding Error code.
/// \authors       Gregory Merchat
/// \date          16.04.2013 Created
///-----------------------------------------------------------------------------------
static int nConnect(BOOL bConnect, BYTE /*hClient*/)
{
    int nReturn = defERR_OK;
    unsigned int i;

    if(bConnect)
    {
        InitializeCriticalSection(&sg_CritSectForAckBuf);
        for(i=0; i<sg_nNoOfChannels; i++)
        {
            /* Start CAN controler. Receptions are starting now. The controler is seting the acknowledge bit
               for all correctly formated frame on the CAN bus. */
            cr = Ic_StartChip(NSI_hCanal[sg_aodChannels[i].m_nChannel]);
            if(cr != _OK)
            {
                // Display a message in a new window
                CString omErr;
                omErr.Format(_("Ic_StartChip : NSI %s"), GetCodeString(cr));
                AfxMessageBox(omErr);
                nReturn = -1;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Error Starting Device"));
            }
            sg_byCurrState[i] = CREATE_MAP_TIMESTAMP;
        }
    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForAckBuf);
        for(i=0; i<sg_nNoOfChannels; i++)
        {
            // Stop hardware
            cr = Ic_StopChip(NSI_hCanal[sg_aodChannels[i].m_nChannel]);
            if(cr != _OK)
            {
                // Display a message in a new window
                CString omErr;
                omErr.Format(_("Ic_StopChip : NSI %s"), GetCodeString(cr));
                AfxMessageBox(omErr);
                nReturn = -1;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Error Closing Device"));
            }
            sg_byCurrState[i] = CREATE_MAP_TIMESTAMP;
        }
    }
    return nReturn;
}

///-------------------------------------------------------
/// \brief         Finds the number of hardware connected.
/// \return        Number of channel connected.
/// \authors       Grégory Merchat
/// \date          17.04.2013 Created
///-------------------------------------------------------
static int GetNoOfConnectedHardware(void)
{
    unsigned long nChannelCount = 0;
    // ------------------------------------
    // get number of present channels
    // ------------------------------------
    cr = Ic_EnumCards(&nChannelCount, NSI_cardData, sizeof(NSI_cardData));
    if (cr != _OK )
    {
        strcpy_s(sg_omErrStr, _T("Problem Finding Device!"));
        //_tcscpy(sg_omErrStr, _T("Problem Finding Device!"));
        nChannelCount = (unsigned long)-1;
    }
    /* Return the channel count */
    return (int)nChannelCount;
}

///---------------------------------------------------------------------------------------
/// \brief         This will initiate a CAN message structure to the driver.
/// \param[in]     sMessage Message to Transmit
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          23.04.2013 Created
///---------------------------------------------------------------------------------------
static int WriteMessageIntoNSIDevice(STCAN_MSG sMessage)
{
    int nReturn = 0;
    if ((sMessage.m_ucChannel > 0) && (sMessage.m_ucChannel <= sg_nNoOfChannels))
    {
        unsigned int   nUsedFlags = 0;
        NSI_canObj.ident = (unsigned long)sMessage.m_unMsgID;
        NSI_canObj.identType = _CAN_STD;
        /* if it is an extended frame */
        if (sMessage.m_ucEXTENDED == 1)
        {
            NSI_canObj.identType = _CAN_EXT;
        }
        NSI_canObj.frameType = _CAN_TX_DATA;
        /* in case of remote frame */
        if (sMessage.m_ucRTR == 1)
        {
            NSI_canObj.frameType = _CAN_TX_REMOTE;
        }
        /* Status report request for this message. In FIFO mode, this parameter is required so that
           exchange status report can be found in the FIFO. */
        NSI_canObj.statusRq = _STATUS;
        NSI_canObj.dlc = (unsigned short)sMessage.m_ucDataLen;
        // Display the data bytes
        for( UINT i=0; i<sMessage.m_ucDataLen && i<=_CAN_MAX_DATA; i++ )
        {
            NSI_canObj.data[i] = sMessage.m_ucData[i];
        }
        cr = Ic_InitId(NSI_hCanal[sg_aodChannels[sMessage.m_ucChannel-1].m_nChannel], &NSI_canObj);
        if(cr != _OK)
        {
            // Display a message in a new window
            /*CString omErr;
            omErr.Format(_("Ic_InitId : %s %s"), sg_HardwareIntr[sg_aodChannels[sMessage.m_ucChannel-1].m_nChannel].m_acDescription.c_str(), GetCodeString(cr));
            AfxMessageBox(omErr);*/
            nReturn = 1;
        }
    }
    return nReturn;
}

///---------------------------------------------------------------------------------------
/// \brief         This will send a CAN message to the driver.
/// \param[in]     sMessage Message to Transmit
/// \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
/// \authors       Gregory Merchat
/// \date          23.04.2013 Created
///---------------------------------------------------------------------------------------
static int nTransmitMessage(STCAN_MSG sMessage, DWORD /*dwClientID*/)
{
    STCAN_MSG newMsg = sMessage;
    int nReturn = -1;
    UINT unClientIndex = (UINT)-1;
    static bool flagTxFailed = 0;
    int ret=0;

    unsigned int   nUsedFlags = 1;

    //Transmit message
    cr = Ic_TxMsg(NSI_hCanal[sg_aodChannels[newMsg.m_ucChannel-1].m_nChannel], newMsg.m_unMsgID,
                  newMsg.m_ucDataLen, newMsg.m_ucData);
    if((cr != _OK) && flagTxFailed)
    {
        if (cr == _UNKNOWN_ID)
        {
            ret = WriteMessageIntoNSIDevice(newMsg);
            ret = nTransmitMessage(newMsg, 0);
        }
        else
        {
            flagTxFailed = 0;
            // Display a message in a new window
            /*CString omErr;
            omErr.Format(_("Ic_TxMsg : %s %s %s"),
                sg_HardwareIntr[sg_aodChannels[newMsg.m_ucChannel-1].m_nChannel].m_acDescription.c_str(),
                sg_HardwareIntr[sg_aodChannels[newMsg.m_ucChannel-1].m_nChannel].m_acDeviceName.c_str(),
                GetCodeString(cr));
            AfxMessageBox(omErr);*/
        }
    }
    else
    {
        flagTxFailed = 1;
    }
    return nReturn;
}

/*************************************************
 *************************************************
 ******                                     ******
 ******  End of user function definitions   ******
 ******                                     ******
 *************************************************
 *************************************************/

/*************************************************
 *************************************************
 ******                                     ******
 ******  CDIL_CAN_NSI function definitions  ******
 ******                                     ******
 *************************************************
 *************************************************/

///-----------------------------------------------------
/// \brief         Function to get Controller status
/// \param[out]    StatusData, is s_STATUSMSG structure
/// \return        S_OK (always)
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-----------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
    return S_OK;
}

///---------------------------------------------------------
/// \brief         Sets the application params.
/// \param[in]     hWndOwner, is the main frame HWND value
/// \param[in]     pILog, is pointer to error logger object
/// \return        S_OK (always)
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;
    /* Initialise both the time parameters */
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;
    /* Query Tick Count */
    sg_QueryTickCount.QuadPart = 0;

    CAN_ManageMsgBuf(MSGBUF_CLEAR, NULL, NULL);

    return S_OK;
}

///-------------------------------------------------------------------------------
/// \brief         Registers the buffer pBufObj to the client ClientID
/// \param[in]     bAction, contains one of the values MSGBUF_ADD or MSGBUF_CLEAR
/// \param[in]     ClientID, is the client ID
/// \param[in]     pBufObj, is pointer to CBaseCANBufFSE object
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != NULL)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                //Add msg buffer
                if (pBufObj != NULL)
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
                if (pBufObj != NULL) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = NULL;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
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
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, NULL);
            }
            hResult = S_OK;
        }
    }
    return hResult;
}


///-----------------------------------------------------------------------------------------------
/// \brief         Registers a client to the DIL.
/// \param[in]     bRegister, if TRUE signifies 'Register', FALSE indicates 'Unregister'
/// \param[out]    ClientID, is Client ID assigned, will be used for further client related calls
/// \param[in]     pacClientName, is the client name
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-----------------------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                //Currently store the client information
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    //First slot is reserved to monitor node
                    ClientID = 1;
                    strcpy_s(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    //_tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
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
                    strcpy_s(sg_asClientToBufMap[Index].pacClientName, pacClientName);
                    //_tcscpy(sg_asClientToBufMap[Index].pacClientName, pacClientName);

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
    return hResult;
}

///----------------------------------------------------------------------------------
/// \brief         Returns the controller status.hEvent will be registered
///                and will be set whenever there is change in the controller status.
/// \param[in]     hEvent, is the handle of the event
/// \param[in]     unCntrlStatus, indicates contoller status
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return WARN_DUMMY_API;
}

///---------------------------------------------------------------------------------
/// \brief         Performs intial operations.
///                Initializes filter, queue, controller config with default values.
/// \param         void
/// \return        S_OK if the open driver call successfull otherwise S_FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;
    sg_ReadMsg.m_bCANFD = false;

    /* Register Monitor client */
    DWORD dwClientID = 0;
    if (CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE) == S_OK)
    {
        // ------------------------------------
        // Initialize the CANlib driver libray
        // ------------------------------------
        try
        {
            //canInitializeLibrary(); A REMPLACER
        }
        catch (...)
        {
            //AfxMessageBox("Initialize failed");
        }

        //Initialize the selected channel items array to -1
        for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
        {
            sg_anSelectedItems[i] = -1;
        }

        hResult = S_OK;
    }

    return hResult;
}

///--------------------------------------------------------------------------------
/// \brief         Performs closure operations.
/// \param         void
/// \return        S_OK if the CAN_StopHardware call successfull otherwise S_FALSE
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///--------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    hResult = CAN_StopHardware();
    // Remove all the existing clients
    UINT ClientIndex = 0;
    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);
    }
    // Delete the critical section
    DeleteCriticalSection(&sg_DIL_CriticalSection);
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_DRIVER_SELECTED;
    }
    return hResult;
}

///---------------------------------------------------------------------------
/// \brief         Gets the time mode mapping of the hardware. CurrSysTime
///                will be updated with the system time ref.
///                TimeStamp will be updated with the corresponding timestamp.
/// \param[out]    CurrSysTime, is SYSTEMTIME structure
/// \param[out]    TimeStamp, is UINT64
/// \param[out]    QueryTickCount, is LARGE_INTEGER
/// \return        S_OK for success
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    //TimeStamp   = sg_TimeStamp;
    TimeStamp  = 0;
    if(QueryTickCount != NULL)
    {
        *QueryTickCount = sg_QueryTickCount;
    }
    return S_OK;
}

///---------------------------------------------------------------------
/// \brief         Lists the hardware interface available.
/// \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
/// \param[out]    nCount , is INT contains the selected channel count.
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///---------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_DRIVER_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (( hResult = nInitHwNetwork(nCount)) == 0)
    {
        nCount = sg_nNoOfChannels; // Number of the selected device
        for(int i=0; i<nCount; i++)
        {
            asSelHwInterface[i].m_dwIdInterface = sg_HardwareIntr[sg_aodChannels[i].m_nChannel].m_dwIdInterface;
            asSelHwInterface[i].m_acDeviceName = sg_HardwareIntr[sg_aodChannels[i].m_nChannel].m_acDeviceName;
            asSelHwInterface[i].m_acDescription = sg_HardwareIntr[sg_aodChannels[i].m_nChannel].m_acDescription;
        }
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    }
    else
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Error connecting to driver"));
    }
    return hResult;
}


///--------------------------------------------------------------------
/// \brief         Selects the hardware interface selected by the user.
/// \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
/// \param[out]    nCount , is INT contains the selected channel count.
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///--------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    USES_CONVERSION;
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;
    // Initialize the CAN controler : BitRate (500kbit, sample at 75%)
    NSI_canBus.baudpresc    = 1;    // BRP   = 1
    NSI_canBus.tseg1        = 11;   // TSEG1 = 11
    NSI_canBus.tseg2        = 4;    // TSEG2 = 4
    NSI_canBus.sjw          = 1;    // SJW   = 1
    NSI_canBus.sample       = 0;    // SPL   = 0 (1 sample)
    unsigned int i;
    for(i=0; i<sg_nNoOfChannels; i++)
    {
        if(nInitDriver((UINT)asSelHwInterface[i].m_dwIdInterface) != 0) // Initialisation of the CAN canal
        {
            hResult = S_FALSE;
        }
        if(nInitBusParam((UINT)asSelHwInterface[i].m_dwIdInterface, NSI_canBus, _DC_NORMAL, _DC_NO_PADDING) != 0) // Initialisation of the Bus parameters
        {
            hResult = S_FALSE;
        }
        if(nInitInterface((UINT)asSelHwInterface[i].m_dwIdInterface, _FIFO) != 0) // Initialisation of the Interface
        {
            hResult = S_FALSE;
        }
        if(nInitRxMask((UINT)asSelHwInterface[i].m_dwIdInterface, 0x000, 0x000, _CAN_STD) != 0) // Accept all IDs
        {
            hResult = S_FALSE;
        }
    }
    /* Check for the success */
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    return hResult;
}

///------------------------------------------------------------------------------
/// \brief         Deselects the selected hardware interface.
/// \param         void
/// \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;
    CAN_StopHardware();
    unsigned int i;
    for(i=0; i<sg_nNoOfChannels; i++)
    {
        if(nExitDriver(sg_aodChannels[i].m_nChannel) != 0) // Closure of the CAN canal
        {
            hResult = S_FALSE;
        }
    }
    /* Check for the success */
    sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    return hResult;
}

///-------------------------------------------------------------
/// \brief         Displays the controller configuration dialog.
/// \param[out]    InitData, is SCONTROLLER_DETAILS structure
/// \param[out]    Length , is INT
/// \return        S_OK for success
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(InitData, S_FALSE);

    HRESULT Result = S_FALSE;
    PSCONTROLLER_DETAILS pControllerDetails = (PSCONTROLLER_DETAILS)InitData;
    //First initialize with existing hw description
    for (INT i = 0; i < min(Length, (INT)sg_nNoOfChannels); i++)
    {
        pControllerDetails[i].m_omHardwareDesc  = sg_aodChannels[i].m_strName;
    }
    if (sg_ucNoOfHardware > 0)
    {
        Result = DisplayConfigurationDlg(sg_hOwnerWnd, Callback_DILNSI, pControllerDetails, sg_ucNoOfHardware);
        switch (Result)
        {
            case WARNING_NOTCONFIRMED:
            {
                Result = WARN_INITDAT_NCONFIRM;
            }
            break;
            case INFO_INIT_DATA_CONFIRMED:
            {
                bLoadDataFromContr(pControllerDetails);
                nSetApplyConfiguration();
                for(int i = 0; i < defNO_OF_CHANNELS; i++)
                {
                    sg_ControllerDetails[i] = pControllerDetails[i];
                    InitData[i] = pControllerDetails[i];
                }

                Length = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
                Result = S_OK;
            }
            break;
            case INFO_RETAINED_CONFDATA:
            {
                Result = INFO_INITDAT_RETAINED;
            }
            break;
            case ERR_CONFIRMED_CONFIGURED: // Not to be addressed at present
            case INFO_CONFIRMED_CONFIGURED:// Not to be addressed at present
            default:
            {
                // Do nothing... default return value is S_FALSE.
            }
            break;
        }
    }
    return Result;
}

///------------------------------------------------------------------------------
/// \brief         Sets the controller configuration data supplied by pInitData.
/// \param[in]     pInitDatae, is SCONTROLLER_DETAILS structure
/// \param[in]     Length , is INT
/// \return        S_OK for success
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;

    /* Fill the hardware description details */
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        ((PSCONTROLLER_DETAILS)pInitData)[nCount].m_omHardwareDesc = sg_aodChannels[nCount].m_strName;
    }

    for ( int i =0; i < Length; i++)
    {
        sg_ControllerDetails[i] = pInitData[i];
    }

    return S_OK;
}

///-------------------------------------------------------------------
/// \brief         connects to the channels and initiates read thread.
/// \param         void
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///--------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_OK;

    flagConnect = TRUE;

    //Connect to the channels
    hResult = nConnect(TRUE, NULL);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
        SetEvent(NSI_hEvent[0]);
        vCreateTimeModeMapping(NSI_hEvent[0]);
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    /*t_CANevent NSI_event;
    for (UINT i = 0; i < sg_nNoOfChannels ; i++)
    {
        while(Ic_GetEvent(NSI_hCanal[sg_aodChannels[i].m_nChannel], &NSI_event) != _EMPTY_FIFO) ;
    }*/

    //If everything is ok start the read thread
    if (hResult == S_OK)
    {
        if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_NSI))
        {
            hResult = S_OK;
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Could not start the read thread" ));
        }
    }
    return hResult;
}

///------------------------------------------------------
/// \brief         Stops the controller.
/// \param         void
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;
    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();

    flagConnect = FALSE;

    hResult = nConnect(FALSE, NULL);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        //log the error for closing port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = S_OK;
    }

    return hResult;
}
///------------------------------------------------------
/// \brief         Gets the Tx queue configured.
/// \param[out]    pouFlxTxMsgBuffer, is BYTE*
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return WARN_DUMMY_API;
}

///---------------------------------------------------------------------------
/// \brief         Sends STCAN_MSG structure from the client dwClientID.
/// \param[in]     dwClientID is the client ID
/// \param[in]     sMessage is the application specific CAN message structure
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Gregory Merchat
/// \date          23.04.2013 Created
///---------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    STCAN_MSG sendingMsg = sCanTxMsg;
    HRESULT hResult = S_FALSE;
    if ((sendingMsg.m_ucChannel > 0) && (sendingMsg.m_ucChannel <= sg_nNoOfChannels))
    {
        if (nTransmitMessage(sendingMsg, dwClientID) == defERR_OK)
        {
            hResult = S_OK;
        }
    }
    else
    {
        hResult = ERR_INVALID_CHANNEL;
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return hResult;
}

///------------------------------------------------------
/// \brief         Gets bus config info.
/// \param[out]    BusInfo, is BYTE
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return WARN_DUMMY_API;
}

///-------------------------------------------------------------------
/// \brief         Gets last occured error and puts inside acErrorStr.
/// \param[out]    acErrorStr, is CHAR contains error string
/// \param[in]     nLength, is INT
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetLastErrorString(string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}

///----------------------------------------------------------------------------------
/// \brief         Gets the controller parametes of the channel based on the request.
/// \param[out]    lParam, the value of the controller parameter requested.
/// \param[in]     nChannel, indicates channel ID
/// \param[in]     eContrParam, indicates controller parameter
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {
            case NUMBER_HW:
            {
                lParam = sg_nNoOfChannels;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                if (GetNoOfConnectedHardware() > 0)
                {
                    lParam = sg_nNoOfChannels;
                }
                else
                {
                    hResult = S_FALSE;
                }
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = true;
            }
            break;
            case HW_MODE:
            {
                /*if (nChannel < sg_nNoOfChannels)
                {
                    cr = Ic_StartChip(NSI_hCanal[nChannel]);
                    if(cr != _OK)
                    {
                        break;
                    }
                    omErr.Format(_("CAN_GetControllerParams : Connected"));
                    AfxMessageBox(omErr);
                    cr = Ic_GetChipState(NSI_hCanal[nChannel], &NSI_canErr, &NSI_canEtat);
                    if(cr != _OK)
                    {
                        // Display a message in a new window
                        CString omErr;
                        omErr.Format(_("NSI - %s %s"), sg_HardwareIntr[nChannel].m_acDescription, GetCodeString(cr));
                        AfxMessageBox(omErr);
                    }
                    else
                    {
                        if(NSI_canErr == _CAN_ACTIVE_ERR)
                        {
                            // Display a message in a new window
                            CString omErr;
                            omErr.Format(_("NSI - %s : CAN ACTIVE"), sg_HardwareIntr[nChannel].m_acDescription);
                            AfxMessageBox(omErr);
                            lParam = defCONTROLLER_ACTIVE;
                        }
                        else if (NSI_canErr == _CAN_PASSIVE_ERR)
                        {
                            // Display a message in a new window
                            CString omErr;
                            omErr.Format(_("NSI - %s : CAN PASSIVE"), sg_HardwareIntr[nChannel].m_acDescription);
                            AfxMessageBox(omErr);
                            lParam = defCONTROLLER_PASSIVE;
                        }
                        else
                        {
                            // Display a message in a new window
                            CString omErr;
                            omErr.Format(_("NSI - %s : CAN BUS OFF"), sg_HardwareIntr[nChannel].m_acDescription);
                            AfxMessageBox(omErr);
                            lParam = defCONTROLLER_BUSOFF;
                            hResult = S_FALSE;
                        }
                    }
                    cr = Ic_StopChip(NSI_hCanal[nChannel]);
                    if(cr != _OK)
                    {
                        break;
                    }
                }
                else
                {
                    //unknown
                    lParam = defCONTROLLER_BUSOFF + 1;
                }*/
                if (nChannel < sg_nNoOfChannels)
                {
                    lParam = sg_ucControllerMode;
                    if( sg_ucControllerMode == 0 || sg_ucControllerMode > defMODE_SIMULATE )
                    {
                        lParam = defCONTROLLER_BUSOFF;
                        hResult = S_FALSE;
                    }
                }
                else
                {
                    //Unknown
                    lParam = defCONTROLLER_BUSOFF + 1;
                }
            }
            break;
            case CON_TEST:
            {
                UCHAR ucResult;
                if (nTestHardwareConnection(ucResult, nChannel) == defERR_OK)
                {
                    lParam = (LONG)ucResult;
                }
            }
            break;
            default:
            {
                hResult = S_FALSE;
            }
            break;
        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}

///----------------------------------------------------------------------------------
/// \brief         Sets the controller parametes of the channel based on the request.
/// \param[in]     nValue, indicates hardware mode
/// \param[in]     eContrParam, indicates controller parameter
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///----------------------------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    int hResult = S_OK;
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {
                    for( UINT i = 0; i < sg_nNoOfChannels; i++)
                    {
                        cr = Ic_SetMode(NSI_hCanal[i], _CAN_NETWORK_WAKEUP);
                        if(cr != _OK)
                        {
                            // Display a message in a new window
                            CString omErr;
                            omErr.Format(_("NSI - %s %s"), sg_HardwareIntr[i].m_acDescription, GetCodeString(cr));
                            AfxMessageBox(omErr);
                            hResult = S_FALSE;
                        }
                    }
                }
                break;
                case defMODE_PASSIVE:
                {
                    for( UINT i = 0; i < sg_nNoOfChannels; i++)
                    {
                        cr = Ic_SetMode(NSI_hCanal[i], _CAN_LD_SLEEP);
                        if(cr != _OK)
                        {
                            // Display a message in a new window
                            CString omErr;
                            omErr.Format(_("NSI - %s %s"), sg_HardwareIntr[i].m_acDescription, GetCodeString(cr));
                            AfxMessageBox(omErr);
                            hResult = S_FALSE;
                        }
                    }
                }
                break;
            }
        }
    }
    return hResult;
}


///-----------------------------------------------------------------
/// \brief         Gets the error counter for corresponding channel.
/// \param[out]    sErrorCnt, is SERROR_CNT structure
/// \param[in]     nChannel, indicates channel ID
/// \param[in]     eContrParam, indicates controller parameter
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///-----------------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_aodChannels[nChannel].m_ucRxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucPeakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_aodChannels[nChannel].m_ucPeakRxErrorCounter;
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}


///------------------------------------------------------
/// \brief         Loads vendor's driver library
/// \param         void
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_LoadDriverLibrary(void)
{
    return S_OK;
}

///------------------------------------------------------
/// \brief         Unloads verdor's driver lobrary
/// \param         void
/// \return        S_OK for success, S_FALSE for failure
/// \authors       Arunkumar Karri
/// \date          12.10.2011 Created
///------------------------------------------------------
HRESULT CDIL_CAN_NSI::CAN_UnloadDriverLibrary(void)
{
    return S_OK;
}