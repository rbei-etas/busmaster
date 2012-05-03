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
 * \file      CAN_Vector_XL.cpp
 * \brief     Source file for Vector XL DIL functions
 * \author    Tobias Lorenz, Arunkumar Karri
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for Vector XL DIL functions
 */

/* C++ includes */
#include <string>

/* Project includes */
#include "CAN_Vector_XL_stdafx.h"
#include "CAN_Vector_XL.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/CAN_Error_Defs.h"
#include "Include/CanUsbDefs.h"
#include "Include/Struct_CAN.h"
#include "Utility/Utility_Thread.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "HardwareListing.h"

#define DYNAMIC_XLDRIVER_DLL
#include "EXTERNAL_INCLUDE/vxlapi.h"

#define USAGE_EXPORT
#include "CAN_Vector_XL_Extern.h"

using namespace std;

// CCAN_Vector_XL

BEGIN_MESSAGE_MAP(CCAN_Vector_XL, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_Vector_XL construction
 */
CCAN_Vector_XL::CCAN_Vector_XL()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_Vector_XL object
CCAN_Vector_XL theApp;


/**
 * CCAN_Vector_XL initialization
 */
BOOL CCAN_Vector_XL::InitInstance()
{
    CWinApp::InitInstance();
    return TRUE;
}

/* function pointers */
XLGETAPPLCONFIG                xlGetApplConfig = NULL;
XLSETAPPLCONFIG                xlSetApplConfig = NULL;
XLGETDRIVERCONFIG              xlGetDriverConfig = NULL;
XLGETCHANNELINDEX              xlGetChannelIndex = NULL;
XLGETCHANNELMASK               xlGetChannelMask = NULL;
XLOPENPORT                     xlOpenPort = NULL;
XLSETTIMERRATE                 xlSetTimerRate = NULL;
XLRESETCLOCK                   xlResetClock = NULL;
XLSETNOTIFICATION              xlSetNotification = NULL;
XLFLUSHRECEIVEQUEUE            xlFlushReceiveQueue = NULL;
XLGETRECEIVEQUEUELEVEL         xlGetReceiveQueueLevel = NULL;
XLACTIVATECHANNEL              xlActivateChannel = NULL;
XLRECEIVE                      xlReceive = NULL;
XLGETEVENTSTRING               xlGetEventString = NULL;
XLGETERRORSTRING               xlGetErrorString = NULL;
XLGETSYNCTIME                  xlGetSyncTime = NULL;
XLGENERATESYNCPULSE            xlGenerateSyncPulse = NULL;
XLPOPUPHWCONFIG                xlPopupHwConfig = NULL;
XLDEACTIVATECHANNEL            xlDeactivateChannel = NULL;
XLCLOSEPORT                    xlClosePort = NULL;
XLSETTIMERBASEDNOTIFY          xlSetTimerBasedNotify = NULL;
XLSETTIMERRATEANDCHANNEL       xlSetTimerRateAndChannel = NULL;
XLGETLICENSEINFO               xlGetLicenseInfo = NULL;
XLSETGLOBALTIMESYNC            xlSetGlobalTimeSync = NULL;

/* CAN specific functions */
XLCANSETCHANNELOUTPUT          xlCanSetChannelOutput = NULL;
XLCANSETCHANNELMODE            xlCanSetChannelMode = NULL;
XLCANSETRECEIVEMODE            xlCanSetReceiveMode = NULL;
XLCANSETCHANNELTRANSCEIVER     xlCanSetChannelTransceiver = NULL;
XLCANSETCHANNELPARAMS          xlCanSetChannelParams = NULL;
XLCANSETCHANNELPARAMSC200      xlCanSetChannelParamsC200 = NULL;
XLCANSETCHANNELBITRATE         xlCanSetChannelBitrate = NULL;
XLCANSETCHANNELACCEPTANCE      xlCanSetChannelAcceptance = NULL;
XLCANADDACCEPTANCERANGE        xlCanAddAcceptanceRange = NULL;
XLCANREMOVEACCEPTANCERANGE     xlCanRemoveAcceptanceRange = NULL;
XLCANRESETACCEPTANCE           xlCanResetAcceptance = NULL;
XLCANREQUESTCHIPSTATE          xlCanRequestChipState = NULL;
XLCANFLUSHTRANSMITQUEUE        xlCanFlushTransmitQueue = NULL;
XLCANTRANSMIT                  xlCanTransmit = NULL;

static  CRITICAL_SECTION sg_CritSectForWrite;       // To make it thread safe

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

#define MAX_BUFF_ALLOWED 16
#define RECEIVE_EVENT_SIZE 1                // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      4096             // internal driver queue size in CAN events

/**
 * Client and Client Buffer map
 */
class SCLIENTBUFMAP
{
public:
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    string pacClientName;
    UINT unBufCount;
    SCLIENTBUFMAP()
    {
        dwClientID = 0;
        hClientHandle = NULL;
        unBufCount = 0;
        pacClientName = "";

        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }
    }
};

/* Local variables */
static XLCLOSEDRIVER           xlDllCloseDriver = NULL;
static XLOPENDRIVER            xlDllOpenDriver = NULL;

/* Forward declarations*/
static int nInitHwNetwork();
static BOOL bRemoveClient(DWORD dwClientId);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(char* pcClientName, INT& Index);
static BOOL bClientIdExist(const DWORD& dwClientId);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line);
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static int nConnect(BOOL bConnect);
static int nGetNoOfConnectedHardware(void);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED],
                                UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static int nDisconnectFromDriver();

// state variables
static BOOL sg_bIsConnected = FALSE;
static UCHAR sg_ucControllerMode = defUSB_MODE_ACTIVE;
static CPARAM_THREADPROC sg_sParmRThread;
static int sg_nFRAMES = 128;
const int ENTRIES_IN_GBUF       = 2000;
static STCANDATA sg_asCANMsg;
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];


// Global variables
static string sg_acErrStr;
static UINT sg_unClientCnt = 0;
#define MAX_CLIENT_ALLOWED 16
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT sg_unDevChannelMap[defNO_OF_CHANNELS] = {(UINT)-1};

static HINSTANCE               hxlDll;

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

/////////////////////////////////////////////////////////////////////////////
// globals

char            g_AppName[XL_MAX_LENGTH+1]  = "BUSMASTER";            //!< Application name which is displayed in VHWconf
XLportHandle    g_xlPortHandle[MAX_CLIENT_ALLOWED]
    = {XL_INVALID_PORTHANDLE};               //!< Global porthandles
XLdriverConfig  g_xlDrvConfig;                                        //!< Contains the actual hardware configuration
XLaccess        g_xlChannelMask             = 0;                      //!< Global channelmask (includes all founded channels)
XLaccess        g_xlPermissionMask          = 0;                      //!< Global permissionmask (includes all founded channels)
unsigned int    g_BaudRate                  = 500000;                 //!< Default baudrate 500KBauds
int             g_silent                    = 0;                      //!< flag to visualize the message events (on/off)
unsigned int    g_TimerRate                 = 0;                      //!< Global timerrate (to toggel)
XLhandle        g_hDataEvent[MAX_CLIENT_ALLOWED]  = {0};
////////////////////////////////////////////////////////////////////////////

/* CDIL_CAN_VectorXL class definition */
class CDIL_CAN_VectorXL : public CBaseDIL_CAN_Controller
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
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
};

CDIL_CAN_VectorXL* g_pouDIL_CAN_VectorXL = NULL;

/**
* \brief         Returns the CDIL_CAN_VectorXL object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_CAN_VectorXL object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;

    if ( NULL == g_pouDIL_CAN_VectorXL )
    {
        if ((g_pouDIL_CAN_VectorXL = new CDIL_CAN_VectorXL) == NULL)
        {
            hResult = S_FALSE;
        }
    }

    *ppvInterface = (void*) g_pouDIL_CAN_VectorXL;  /* Doesn't matter even if g_pouDIL_CAN_VectorXL is null */
    return hResult;
}

/**
 * Number of Channels
 */
static UINT sg_nNoOfChannels = 0;

static char sg_omErrStr[MAX_STRING] = {0};

// Count variables
static UCHAR sg_ucNoOfHardware = 0;

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
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;

/**
 * Channel information
 */
struct CChannel
{
    /**
     * Pointer to corresponding XLchannelConfig
     */
    XLchannelConfig* m_pXLChannelInfo;

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
        // Pointer to corresponding XLchannelConfig
        m_pXLChannelInfo = NULL;
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
* \brief         Sets the application params.
* \param[in]     hWndOwner, is the main frame HWND value
* \param[in]     pILog, is pointer to error logger object
* \return        S_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;
    /* Initialise both the time parameters */
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;
    /* Query Tick Count */
    sg_QueryTickCount.QuadPart = 0;
    sg_acErrStr = "";
    CAN_ManageMsgBuf(MSGBUF_CLEAR, NULL, NULL);
    return S_OK;
}


/**
* \brief         Unloads verdor's driver lobrary
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_UnloadDriverLibrary(void)
{
    if (hxlDll != NULL)
    {
        FreeLibrary(hxlDll);
        hxlDll = NULL;
    }

    return S_OK;
}

/**
* \brief         Registers the buffer pBufObj to the client ClientID
* \param[in]     bAction, contains one of the values MSGBUF_ADD or MSGBUF_CLEAR
* \param[in]     ClientID, is the client ID
* \param[in]     pBufObj, is pointer to CBaseCANBufFSE object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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
                /* Add msg buffer */
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
                /* clear msg buffer */
                if (pBufObj != NULL)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
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
            /* clear msg buffer */
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, NULL);
            }

            hResult = S_OK;
        }
    }

    return hResult;
}

/**
* \brief         Registers a client to the DIL.
* \param[in]     bRegister, if TRUE signifies 'Register', FALSE indicates 'Unregister'
* \param[out]    ClientID, is Client ID assigned, will be used for further client related calls
* \param[in]     pacClientName, is the client name
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
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
                /* Currently store the client information */
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
                    ClientID = 1;
                    sg_asClientToBufMap[0].pacClientName = pacClientName;
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
                    sg_asClientToBufMap[Index].pacClientName = pacClientName;
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

/**
* \brief         Returns the controller status.hEvent will be registered
*                and will be set whenever there is change in the controller status.
* \param[in]     hEvent, is the handle of the event
* \param[in]    unCntrlStatus, indicates contoller status
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return S_OK;
}

/**
* \brief         Loads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_LoadDriverLibrary(void)
{
    HRESULT hResult = S_OK;

    if (hxlDll != NULL)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("vxlapi.dll already loaded"));
        hResult = DLL_ALREADY_LOADED;
    }

    if (hResult == S_OK)
    {
        hxlDll = LoadLibrary("vxlapi.dll");

        if (hxlDll == NULL)
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("vxlapi.dll loading failed"));
            hResult = ERR_LOAD_DRIVER;
        }
        else
        {
            xlDllOpenDriver           = (XLOPENDRIVER)                GetProcAddress(hxlDll,"xlOpenDriver");
            xlDllCloseDriver          = (XLCLOSEDRIVER)               GetProcAddress(hxlDll,"xlCloseDriver");
            /* bus independed functions */
            xlGetApplConfig            = (XLGETAPPLCONFIG)            GetProcAddress(hxlDll,"xlGetApplConfig");
            xlSetApplConfig            = (XLSETAPPLCONFIG)            GetProcAddress(hxlDll,"xlSetApplConfig");
            xlGetDriverConfig          = (XLGETDRIVERCONFIG)          GetProcAddress(hxlDll,"xlGetDriverConfig");
            xlGetChannelIndex          = (XLGETCHANNELINDEX)          GetProcAddress(hxlDll,"xlGetChannelIndex");
            xlGetChannelMask           = (XLGETCHANNELMASK)           GetProcAddress(hxlDll,"xlGetChannelMask");
            xlOpenPort                 = (XLOPENPORT)                 GetProcAddress(hxlDll,"xlOpenPort");
            xlSetTimerRate             = (XLSETTIMERRATE)             GetProcAddress(hxlDll,"xlSetTimerRate");
            xlResetClock               = (XLRESETCLOCK)               GetProcAddress(hxlDll,"xlResetClock");
            xlSetNotification          = (XLSETNOTIFICATION)          GetProcAddress(hxlDll,"xlSetNotification");
            xlFlushReceiveQueue        = (XLFLUSHRECEIVEQUEUE)        GetProcAddress(hxlDll,"xlFlushReceiveQueue");
            xlGetReceiveQueueLevel     = (XLGETRECEIVEQUEUELEVEL)     GetProcAddress(hxlDll,"xlGetReceiveQueueLevel");
            xlActivateChannel          = (XLACTIVATECHANNEL)          GetProcAddress(hxlDll,"xlActivateChannel");
            xlReceive                  = (XLRECEIVE)                  GetProcAddress(hxlDll,"xlReceive");
            xlGetEventString           = (XLGETEVENTSTRING)           GetProcAddress(hxlDll,"xlGetEventString");
            xlGetErrorString           = (XLGETERRORSTRING)           GetProcAddress(hxlDll,"xlGetErrorString");
            xlGenerateSyncPulse        = (XLGENERATESYNCPULSE)        GetProcAddress(hxlDll,"xlGenerateSyncPulse");
            xlGetSyncTime              = (XLGETSYNCTIME)              GetProcAddress(hxlDll,"xlGetSyncTime");
            xlPopupHwConfig            = (XLPOPUPHWCONFIG)            GetProcAddress(hxlDll,"xlPopupHwConfig");
            xlDeactivateChannel        = (XLDEACTIVATECHANNEL)        GetProcAddress(hxlDll,"xlDeactivateChannel");
            xlClosePort                = (XLCLOSEPORT)                GetProcAddress(hxlDll,"xlClosePort");
            xlSetTimerBasedNotify      = (XLSETTIMERBASEDNOTIFY)      GetProcAddress(hxlDll,"xlSetTimerBasedNotify");
            xlSetTimerRateAndChannel   = (XLSETTIMERRATEANDCHANNEL)   GetProcAddress(hxlDll, "xlSetTimerRateAndChannel");
            xlGetLicenseInfo           = (XLGETLICENSEINFO)           GetProcAddress(hxlDll, "xlGetLicenseInfo");
            xlSetGlobalTimeSync        = (XLSETGLOBALTIMESYNC)        GetProcAddress(hxlDll, "xlSetGlobalTimeSync");
            /* CAN specific functions */
            xlCanSetChannelOutput      = (XLCANSETCHANNELOUTPUT)      GetProcAddress(hxlDll,"xlCanSetChannelOutput");
            xlCanSetChannelMode        = (XLCANSETCHANNELMODE)        GetProcAddress(hxlDll,"xlCanSetChannelMode");
            xlCanSetReceiveMode        = (XLCANSETRECEIVEMODE)        GetProcAddress(hxlDll,"xlCanSetReceiveMode");
            xlCanSetChannelTransceiver = (XLCANSETCHANNELTRANSCEIVER) GetProcAddress(hxlDll,"xlCanSetChannelTransceiver");
            xlCanSetChannelParams      = (XLCANSETCHANNELPARAMS)      GetProcAddress(hxlDll,"xlCanSetChannelParams");
            xlCanSetChannelParamsC200  = (XLCANSETCHANNELPARAMSC200)  GetProcAddress(hxlDll,"xlCanSetChannelParamsC200");
            xlCanSetChannelBitrate     = (XLCANSETCHANNELBITRATE)     GetProcAddress(hxlDll,"xlCanSetChannelBitrate");
            xlCanSetChannelAcceptance  = (XLCANSETCHANNELACCEPTANCE)  GetProcAddress(hxlDll,"xlCanSetChannelAcceptance");
            xlCanAddAcceptanceRange    = (XLCANADDACCEPTANCERANGE)    GetProcAddress(hxlDll,"xlCanAddAcceptanceRange");
            xlCanRemoveAcceptanceRange = (XLCANREMOVEACCEPTANCERANGE) GetProcAddress(hxlDll,"xlCanRemoveAcceptanceRange");
            xlCanResetAcceptance       = (XLCANRESETACCEPTANCE)       GetProcAddress(hxlDll,"xlCanResetAcceptance");
            xlCanRequestChipState      = (XLCANREQUESTCHIPSTATE)      GetProcAddress(hxlDll,"xlCanRequestChipState");
            xlCanFlushTransmitQueue    = (XLCANFLUSHTRANSMITQUEUE)    GetProcAddress(hxlDll,"xlCanFlushTransmitQueue");
            xlCanTransmit              = (XLCANTRANSMIT)              GetProcAddress(hxlDll,"xlCanTransmit");

            /* check for error */
            if (!xlDllOpenDriver || !xlDllCloseDriver ||
                    !xlGetApplConfig || !xlSetApplConfig ||
                    !xlGetDriverConfig || !xlGetChannelIndex ||
                    !xlGetChannelMask || !xlOpenPort ||
                    !xlSetTimerRate || !xlResetClock ||
                    !xlSetNotification || !xlFlushReceiveQueue ||
                    !xlGetReceiveQueueLevel || !xlActivateChannel ||
                    !xlReceive || !xlGetEventString ||
                    !xlGetErrorString || !xlGenerateSyncPulse ||
                    !xlGetSyncTime || !xlPopupHwConfig ||
                    !xlDeactivateChannel || !xlClosePort ||
                    !xlSetTimerBasedNotify || !xlSetTimerRateAndChannel ||
                    !xlGetLicenseInfo || !xlCanSetChannelOutput ||
                    !xlCanSetChannelMode || !xlCanSetReceiveMode ||
                    !xlCanSetChannelTransceiver || !xlCanSetChannelParams ||
                    !xlCanSetChannelParamsC200 || !xlCanSetChannelBitrate ||
                    !xlCanSetChannelAcceptance || !xlCanAddAcceptanceRange ||
                    !xlCanRemoveAcceptanceRange || !xlCanResetAcceptance ||
                    !xlCanRequestChipState || !xlCanFlushTransmitQueue ||
                    !xlCanTransmit || !xlSetGlobalTimeSync)
            {
                FreeLibrary(hxlDll);
                sg_pIlog->vLogAMessage(A2T(__FILE__),
                                       __LINE__, _T("Getting Process address of the APIs failed"));
                hResult = ERR_LOAD_DRIVER;
            }
        }
    }

    return hResult;
}

/**
* \brief         Performs intial operations.
*                Initializes filter, queue, controller config with default values.
* \param         void
* \return        S_OK if the open driver call successfull otherwise S_FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;
    /* Register Monitor client */
    DWORD dwClientID = 0;
    CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE);

    // ------------------------------------
    // open the driver
    // ------------------------------------
    if (xlDllOpenDriver() == XL_SUCCESS)
    {
        hResult = S_OK;
    }

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return hResult;
}

/**
* \brief         Performs closure operations.
* \param         void
* \return        S_OK if the CAN_StopHardware call successfull otherwise S_FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;
    hResult = CAN_StopHardware();
    // ------------------------------------
    // Close the driver
    // ------------------------------------
    xlDllCloseDriver();
    UINT ClientIndex = 0;

    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);
    }

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_DRIVER_SELECTED;
    }

    return hResult;
}

/**
* \brief         Gets the time mode mapping of the hardware. CurrSysTime
*                will be updated with the system time ref.
*                TimeStamp will be updated with the corresponding timestamp.
* \param[out]    CurrSysTime, is SYSTEMTIME structure
* \param[out]    TimeStamp, is UINT64
* \param[out]    QueryTickCount, is LARGE_INTEGER
* \return        S_OK for success
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStamp;

    if(QueryTickCount != NULL)
    {
        *QueryTickCount = sg_QueryTickCount;
    }

    return S_OK;
}

/**
* \brief         Lists the hardware interface available.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (nInitHwNetwork() == 0)
    {
        nCount = sg_nNoOfChannels;

        for (UINT i = 0; i < sg_nNoOfChannels; i++)
        {
            char buf[512];
            asSelHwInterface[i].m_dwIdInterface = i;
            unsigned int serialNumber = sg_aodChannels[i].m_pXLChannelInfo->serialNumber;
            sprintf_s(buf, _T("%d"), serialNumber);
            asSelHwInterface[i].m_acDescription = buf;
            sprintf_s(sg_ControllerDetails[i].m_omHardwareDesc, _T("Vector - %s SN - %d Channel Index - %d"),
                      sg_aodChannels[i].m_pXLChannelInfo->name,
                      serialNumber,
                      sg_aodChannels[i].m_pXLChannelInfo->channelIndex);
            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
        }

        hResult = S_OK;
    }
    else
    {
        hResult = NO_HW_INTERFACE;
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Error connecting to driver"));
    }

    return hResult;
}

/**
* \brief         Selects the hardware interface selected by the user.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{
    USES_CONVERSION;
    VALIDATE_POINTER_RETURN_VAL(hxlDll, S_FALSE);
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);
    /* Check for the success */
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    return S_OK;
}

/**
* \brief         Deselects the selected hardware interface.
* \param         void
* \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_DeselectHwInterface(void)
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
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_DisplayConfigDlg(PCHAR& InitData, INT& /*Length*/)
{
    xlPopupHwConfig(NULL, INFINITE);
    //Get back the baud rate from controller
    SCONTROLLER_DETAILS* pCntrlDetails = (SCONTROLLER_DETAILS*)InitData;
    xlGetDriverConfig(&g_xlDrvConfig);

    for ( UINT i = 0 ; i < sg_nNoOfChannels ; i++ )
    {
        sprintf_s(pCntrlDetails[i].m_omStrBaudrate, ("%0.3f"), float(
                      g_xlDrvConfig.channel[sg_aodChannels[i].m_pXLChannelInfo->channelIndex].
                      busParams.data.can.bitRate / 1000.000 ));
    }

    return S_OK;
}

/**
* \brief         Sets the controller configuration data supplied by ConfigFile.
* \param[in]     ConfigFile, is SCONTROLLER_DETAILS structure
* \param[in]     Length , is INT
* \return        S_OK for success
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SetConfigData(PCHAR ConfigFile, INT Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;

    /* Fill the hardware description details */
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        strcpy_s(((PSCONTROLLER_DETAILS)ConfigFile)[nCount].m_omHardwareDesc,
                 sg_ControllerDetails[nCount].m_omHardwareDesc);
    }

    memcpy((void*)sg_ControllerDetails, (void*)ConfigFile, Length);
    return S_OK;
}

/**
* \brief         Writes the message 'sCanData' to the corresponding clients buffer
* \param[out]    sCanData, is STCANDATA structure
* \param[out]    unClientIndex , is UINT containing client ID
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vWriteIntoClientsBuffer(STCANDATA& sCanData, UINT unClientIndex)
{
    /* Write into the respective client's buffer */
    for (UINT j = 0; j < sg_asClientToBufMap[unClientIndex].unBufCount; j++)
    {
        sg_asClientToBufMap[unClientIndex].pClientBuf[j]->WriteIntoBuffer(&sCanData);
    }
}

/**
* \brief         Function to create time mode mapping
* \param[in]     hDataEvent, is HANDLE
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{
    WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);
}

/**
* \brief         This will convert the error code from vector driver format
*                to the format that is used by BUSMASTER.
* \param[in]     lError Error code in Peak USB driver format
* \param[in]     byDir  Error direction Tx/Rx
* \return        UCHAR which indicates error code
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static UCHAR USB_ucGetErrorCode(LONG lError, BYTE byDir)
{
    UCHAR ucReturn = 0;

    // Tx Errors
    if( byDir == 1)
    {
        if (lError & XL_CAN_MSG_FLAG_ERROR_FRAME)
        {
            ucReturn = BIT_ERROR_TX;
        }

        if (lError & XL_CAN_MSG_FLAG_NERR )
        {
            ucReturn = FORM_ERROR_TX;
        }
        else
        {
            ucReturn = OTHER_ERROR_TX;
        }
    }
    // Rx Errors
    else
    {
        if (lError & XL_CAN_MSG_FLAG_ERROR_FRAME)
        {
            ucReturn = BIT_ERROR_RX;
        }

        if (lError & XL_CAN_MSG_FLAG_NERR)
        {
            ucReturn = FORM_ERROR_RX;
        }
        else
        {
            ucReturn = OTHER_ERROR_RX;
        }
    }

    // Return the error code
    return ucReturn;
}

/**
* \brief         This will classify the messages, which can be one of Rx, Tx or
*                Error messages. In case of Err messages this identifies under
*                what broader category (Rx / Tx) does this occur.
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[out]    sCanData Application specific data format
* \return        TRUE (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BYTE bClassifyMsgType(XLevent& xlEvent, STCANDATA& sCanData)
{
    sCanData.m_lTickCount.QuadPart = (LONGLONG)xlEvent.timeStamp / 100000;

    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {
        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        UINT64 unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;

        //Time difference should be +ve value
        if(sCanData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStamp  = (LONGLONG)(sCanData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            sg_TimeStamp  = (LONGLONG)(unConnectionTime - sCanData.m_lTickCount.QuadPart);
        }

        sg_byCurrState = CALC_TIMESTAMP_READY;
    }

    if ( !(xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_ERROR_FRAME) &&
            !(xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_OVERRUN)     &&
            !(xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_NERR)         )
    {
        ///* Check for RTR Message */
        if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_REMOTE_FRAME)
        {
            sCanData.m_ucDataType = RX_FLAG;
            sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = TRUE;
        }
        else
        {
            sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = FALSE;
        }

        if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_TX_COMPLETED)
        {
            sCanData.m_ucDataType = TX_FLAG;
        }
        else
        {
            sCanData.m_ucDataType = RX_FLAG;
        }

        /* Copy data length */
        sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = (UCHAR)xlEvent.tagData.msg.dlc;
        /* Copy the message data */
        memcpy(sCanData.m_uDataInfo.m_sCANMsg.m_ucData,
               xlEvent.tagData.msg.data, xlEvent.tagData.msg.dlc);
        /* Copy the message ID */
        sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID = (UINT)xlEvent.tagData.msg.id;
        /* Check for extended message indication */
        sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED =
            (xlEvent.tagData.msg.id & XL_CAN_EXT_MSG_ID) ? TRUE : FALSE;

        if ( sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED )
        {
            sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID ^= XL_CAN_EXT_MSG_ID;    // make it std
        }

        /* Set channel ID */
        sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
    }
    else
    {
        sCanData.m_ucDataType = ERR_FLAG;
        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
        sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
        // Assign error type in the Error Capture register
        // and the direction of the error
        BOOL bIsTxMsg = FALSE;

        if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_TX_COMPLETED)
        {
            bIsTxMsg = TRUE;
        }

        sCanData.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap =
            USB_ucGetErrorCode(xlEvent.tagData.msg.flags, (BYTE) bIsTxMsg);
        //explaination of error bit
        sCanData.m_uDataInfo.m_sErrInfo.m_nSubError= 0;
    }

    return TRUE;
}

/**
* \brief         Processing of the received packets from bus
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[in]     unClientIndex is the client index
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void ProcessCANMsg(XLevent& xlEvent, UINT unClientIndex)
{
    bClassifyMsgType(xlEvent, sg_asCANMsg);
    vWriteIntoClientsBuffer(sg_asCANMsg, unClientIndex);
}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
DWORD WINAPI CanMsgReadThreadProc_CAN_Vector_XL(LPVOID pVoid)
{
    USES_CONVERSION;
    XLstatus xlStatus = XL_SUCCESS;
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    /* Validate certain required pointers */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam, (DWORD)-1);
    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    /* Set the event to CAN_Vector_XL driver for wakeup and frame arrival notification */
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        xlStatus = xlSetNotification (g_xlPortHandle[i], &g_hDataEvent[i], 1);
    }

    /* Set the thread action event to first notification event handle */
    if (g_hDataEvent[0] != NULL)
    {
        pThreadParam->m_hActionEvent = g_hDataEvent[0];
    }

    /* Get the handle to the controller and validate it */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam->m_hActionEvent, (DWORD)-1);
    DWORD dwResult = 0;
    unsigned int msgsrx;
    XLevent  xlEvent;
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForMultipleObjects(sg_unClientCnt, g_hDataEvent, FALSE, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                bool bMoreDataExist;

                do
                {
                    bMoreDataExist = false;

                    /* Read all the available CAN messages using 'xlReceive'
                       across all the port handles */
                    for (UINT i = 0; i < sg_unClientCnt; i++)
                    {
                        msgsrx = RECEIVE_EVENT_SIZE;
                        xlStatus = xlReceive(g_xlPortHandle[i], &msgsrx, &xlEvent);

                        switch (xlStatus)
                        {
                            case XL_SUCCESS:
                            {
                                ProcessCANMsg(xlEvent, i);
                                bMoreDataExist = true;
                            }
                            break;

                            default:
                                break;
                        }
                    }
                }
                while (bMoreDataExist);
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            case CREATE_TIME_MAP:
            {
                vCreateTimeModeMapping(pThreadParam->m_hActionEvent);
                SetEvent(pThreadParam->m_hActionEvent);
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
            }
            break;

            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());

    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        ResetEvent(g_hDataEvent[i]);
        g_hDataEvent[i] = NULL;
    }

    pThreadParam->m_hActionEvent = NULL;
    return 0;
}

/**
* \brief         connects to the channels and initiates read thread.
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_OK;
    //Connect to the network
    hResult = nConnect(TRUE);

    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    //If everything is ok start the read thread
    if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_Vector_XL))
    {
        hResult = S_OK;
    }
    else
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Could not start the read thread" ));
    }

    return hResult;
}

/**
* \brief         This will close the connection with the driver. This will be
*                called before deleting HI layer. This will be called during
*                application close.
* \param         void
* \return        Operation Result. 0 incase of no errors. Failure Error codes(-1) otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nDisconnectFromDriver()
{
    int nReturn = 0;

    for ( UINT i = 0; i< sg_unClientCnt; i++ )
    {
        if (g_xlPortHandle[i] != XL_INVALID_PORTHANDLE)
        {
            xlDeactivateChannel( g_xlPortHandle[i], g_xlChannelMask );
            xlClosePort(g_xlPortHandle[i]);
            g_xlPortHandle[i] = XL_INVALID_PORTHANDLE;
        }
        else
        {
            nReturn = -1;
        }
    }

    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    return nReturn;
}

/**
* \brief         Function to map device channels with BUSMASTER channel order
* \param         void
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vMapDeviceChannelIndex()
{
    //Reset previous channel ID assignment if any
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        sg_unDevChannelMap[sg_aodChannels[i].m_pXLChannelInfo->channelIndex] = i;
    }
}

/**
* \brief         This function will connect the tool with hardware. This will
*                establish the data link between the application and hardware.
* \param[in]     bConnect TRUE to Connect, FALSE to Disconnect
* \return        Returns defERR_OK if successful otherwise corresponding Error code.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nConnect(BOOL bConnect)
{
    int nReturn = -1;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
        XLstatus xlStatus;

        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            // ------------------------------------
            // open ONE port PER each node including all channels
            // ------------------------------------
            xlStatus = xlOpenPort(&g_xlPortHandle[i], g_AppName, g_xlChannelMask, &g_xlPermissionMask,
                                  RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
        }

        if (XL_SUCCESS == xlStatus)
        {
            //Calculate connected Timestamp
            QueryPerformanceCounter(&sg_QueryTickCount);
            // Get frequency of the performance counter
            QueryPerformanceFrequency(&sg_lnFrequency);

            // Convert it to time stamp with the granularity of hundreds of microsecond
            //if (sg_QueryTickCount.QuadPart * 10000 > sg_QueryTickCount.QuadPart)
            if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
            {
                sg_TimeStamp = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
            }
            else
            {
                sg_TimeStamp = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
            }

            /* Transit into 'CREATE TIME MAP' state */
            sg_byCurrState = CREATE_MAP_TIMESTAMP;
            vMapDeviceChannelIndex();
            sg_bIsConnected = bConnect;

            // ------------------------------------
            // go with all selected channels on bus
            // ------------------------------------
            for ( UINT i = 0; i< sg_unClientCnt; i++ )
            {
                xlStatus = xlActivateChannel(g_xlPortHandle[i], g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
            }

            if(xlStatus == XL_SUCCESS)
            {
                nReturn = defERR_OK;
            }
        }
    }
    else if (sg_bIsConnected && !bConnect) // Connected & to be disconnected
    {
        sg_bIsConnected = bConnect;
        Sleep(0); // Let other threads run for once
        nReturn = nDisconnectFromDriver();
    }
    else
    {
        nReturn = defERR_OK;
    }

    if ( sg_bIsConnected )
    {
        InitializeCriticalSection(&sg_CritSectForWrite);
    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForWrite);
    }

    return nReturn;
}

/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;
    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();
    hResult = nConnect(FALSE);

    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return hResult;
}

/**
* \brief         Resets the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_ResetHardware(void)
{
    HRESULT hResult = S_FALSE;
    /* Stop the hardware if connected */
    hResult = CAN_StopHardware(); // return value not necessary
    return hResult;
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
    return S_OK;
}

/**
* \brief         This will send a CAN message to the driver. In case of USB
*                this will write the message in to the driver buffer and will
*                return. In case if parallel port mode this will write the
*                message and will wait for the ACK event from the driver. If
*                the event fired this will return 0. Otherwise this will return
*                wait time out error. In parallel port it is a blocking call
*                and in case of failure condition this will take 2 seconds.
* \param[in]     sMessage Message to Transmit
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nWriteMessage(STCAN_MSG sMessage, DWORD dwClientID)
{
    int nReturn = -1;
    UINT unClientIndex = (UINT)-1;

    if ((sMessage.m_ucChannel > 0) &&
            (sMessage.m_ucChannel <= sg_nNoOfChannels))
    {
        static XLevent       xlEvent;
        XLstatus             xlStatus = 0;
        unsigned int         messageCount = 1;
        memset(&xlEvent, 0, sizeof(xlEvent));
        xlEvent.tag                 = XL_TRANSMIT_MSG;
        XLaccess xlChanMaskTx = 0;

        /* if it is an extended frame */
        if (sMessage.m_ucEXTENDED == 1)
        {
            sMessage.m_unMsgID ^= XL_CAN_EXT_MSG_ID; // toggle ext/std
        }

        /* in case of remote frame */
        if (sMessage.m_ucRTR == 1)
        {
            xlEvent.tagData.msg.flags   = XL_CAN_MSG_FLAG_REMOTE_FRAME;
        }
        else
        {
            xlEvent.tagData.msg.flags   = 0;
        }

        xlEvent.tagData.msg.id      = sMessage.m_unMsgID;
        xlEvent.tagData.msg.dlc     = (unsigned short)sMessage.m_ucDataLen;
        memcpy(&xlEvent.tagData.msg.data, &sMessage.m_ucData, sMessage.m_ucDataLen);
        //Get channel mask
        xlChanMaskTx = sg_aodChannels[sMessage.m_ucChannel - 1].m_pXLChannelInfo->channelMask;

        //Transmit message
        if ( bGetClientObj(dwClientID, unClientIndex) )
        {
            xlStatus = xlCanTransmit(g_xlPortHandle[unClientIndex], xlChanMaskTx, &messageCount, &xlEvent);
        }

        //set result
        nReturn = xlStatus;
    }

    return nReturn;
}

/**
* \brief         Sends STCAN_MSG structure from the client dwClientID.
* \param[in]     dwClientID is the client ID
* \param[in]     sMessage is the application specific CAN message structure
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_FALSE;

    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= sg_nNoOfChannels)
        {
            EnterCriticalSection(&sg_CritSectForWrite); // Lock the buffer

            if (nWriteMessage(sMessage, dwClientID) == defERR_OK)
            {
                hResult = S_OK;
            }

            LeaveCriticalSection(&sg_CritSectForWrite); // Unlock the buffer
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

    return hResult;
}

/**
 * \brief         Gets last occured error and puts inside acErrorStr.
 * \param[out]    acErrorStr contains error string
 * \return        S_OK for success, S_FALSE for failure
 *
 * Gets last occured error and puts inside acErrorStr.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetLastErrorString(string& acErrorStr)
{
    acErrorStr = sg_acErrStr;
    return S_OK;
}

/**
* \brief         This function will check all hardware connectivity by activating selected channels.
* \param[out]    ucaTestResult Array that will hold test result.
                 TRUE if hardware present and false if not connected
* \param[in]     nChannel, indicates channel ID
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) //const
{
    int nReturn = 0;

    if (nChannel < sg_nNoOfChannels)
    {
        XLstatus xlStatus;
        // ------------------------------------
        // go with all selected channels on bus
        // ------------------------------------
        xlStatus = xlActivateChannel(g_xlPortHandle[0], g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);

        if ( xlStatus == XL_SUCCESS )
        {
            ucaTestResult = TRUE;
        }
        else
        {
            sg_bIsConnected = FALSE;
            ucaTestResult = FALSE;
        }
    }

    return nReturn;
}

/**
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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
                if (nGetNoOfConnectedHardware() > 0)
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

/**
* \brief         Gets the error counter for corresponding channel.
* \param[out]    sErrorCnt, is SERROR_CNT structure
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
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

/* HELPER FUNCTIONS START */

/**
* \brief         Finds the number of hardware connected.
* \param         void
* \return        defERR_OK if successful otherwise corresponding Error code.
*                0, Query successful, but no device found
*                > 0, Number of devices found
*                < 0, query for devices unsuccessful
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nGetNoOfConnectedHardware(void)
{
    int nResult = 0;
    XLstatus xlStatus = XL_SUCCESS;

    // ------------------------------------
    // get the hardware configuration
    // ------------------------------------
    if ( XL_SUCCESS == xlStatus )
    {
        xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
    }

    if (XL_SUCCESS == xlStatus)
    {
        // ------------------------------------
        // select the wanted channels
        // ------------------------------------
        g_xlChannelMask = 0;

        for (UINT i=0; i < g_xlDrvConfig.channelCount; i++)
        {
            // we take all hardware we found and
            // check that we have only CAN cabs/piggy's
            // at the moment there is no VN8910 XLAPI support!
            if ( /*(g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
              &&*/ (g_xlDrvConfig.channel[i].hwType != XL_HWTYPE_VN8900) )
            {
                if ( (g_xlDrvConfig.channel[i].hwType == XL_HWTYPE_CANCASEXL) &&
                        !(g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) )
                {
                    continue;
                }

                nResult++;
            }
        }

        if (!nResult)
        {
            strcpy_s(sg_omErrStr, _T("No available channels found! (e.g. no CANcabs...)\r\n"));
            xlStatus = XL_ERROR;
        }
    }
    else
    {
        strcpy_s(sg_omErrStr, _T("Problem Finding Device!"));
        nResult = -1;
    }

    /* Return the operation result */
    return nResult;
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.
* \param[in]     psInterfaces, is INTERFACE_HW structue
* \param[out]    pnSelList, contains channels selected array
* \param[out]    nCount, contains selected channel count
* \return        returns 0 (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
int ListHardwareInterfaces(HWND /*hParent*/, DWORD /*dwDriver*/, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, NULL);
    HwList.DoModal();
    nCount = HwList.nGetSelectedList(pnSelList);
    return 0;
}

/**
* \brief         This function will get the hardware selection from the user
*                and will create essential networks.
* \param         void
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nCreateMultipleHardwareNetwork()
{
    int nHwCount = sg_ucNoOfHardware;
    int nChannels = 0;

    // Get Hardware Network Map
    for (unsigned int nCount = 0; nCount < g_xlDrvConfig.channelCount; nCount++)
    {
        // we take all hardware we found and
        // check that we have only CAN cabs/piggy's
        // at the moment there is no VN8910 XLAPI support!
        if ( /*(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
              && */(g_xlDrvConfig.channel[nCount].hwType != XL_HWTYPE_VN8900) )
        {
            if ( (g_xlDrvConfig.channel[nCount].hwType == XL_HWTYPE_CANCASEXL) &&
                    !(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) )
            {
                continue;
            }

            sg_HardwareIntr[nChannels].m_dwIdInterface = nCount;
            sg_HardwareIntr[nChannels].m_dwVendor = g_xlDrvConfig.channel[nCount].serialNumber;
            /*_stprintf(acTempStr, _T("SN: %d, Port ID: %d"), sg_HardwareIntr[nChannels].m_dwVendor,
                                                                    sg_HardwareIntr[nChannels].m_dwIdInterface);*/
            sg_HardwareIntr[nChannels].m_acDescription = g_xlDrvConfig.channel[nCount].name;
            nChannels++;
        }
    }

    nHwCount = nChannels;   //Reassign hardware count according to final list of channels supported.
    ListHardwareInterfaces(sg_hOwnerWnd, DRIVER_CAN_VECTOR_XL, sg_HardwareIntr, sg_anSelectedItems, nHwCount);
    sg_ucNoOfHardware = (UCHAR)nHwCount;
    sg_nNoOfChannels = (UINT)nHwCount;
    g_xlChannelMask = 0;

    //Reorder hardware interface as per the user selection
    for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_aodChannels[nCount].m_pXLChannelInfo  = &g_xlDrvConfig.channel[sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface];
        g_xlChannelMask |= sg_aodChannels[nCount].m_pXLChannelInfo->channelMask;
    }

    g_xlPermissionMask = g_xlChannelMask;
    return defERR_OK;
}

/**
* \brief         This function will create a single network with available single hardware.
* \param         void
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nCreateSingleHardwareNetwork()
{
    g_xlChannelMask = 0;
    // Set the number of channels as 1
    sg_ucNoOfHardware = (UCHAR)1;
    sg_nNoOfChannels = 1;

    for (UINT i=0; i < g_xlDrvConfig.channelCount; i++)
    {
        // we take all hardware we found and
        // check that we have only CAN cabs/piggy's
        // at the moment there is no VN8910 XLAPI support!
        if ( (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
                && (g_xlDrvConfig.channel[i].hwType != XL_HWTYPE_VN8900) )
        {
            sg_aodChannels[0].m_pXLChannelInfo  = &g_xlDrvConfig.channel[i];
            g_xlChannelMask |= sg_aodChannels[0].m_pXLChannelInfo->channelMask;
            break;
        }
    }

    g_xlPermissionMask = g_xlChannelMask;
    return defERR_OK;
}

/**
* \brief         This function will find number of hardwares connected.
*                It will create network as per hardware count.
*                This will popup hardware selection dialog in case there are more hardwares present.
* \param         void
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nInitHwNetwork()
{
    int nChannelCount = 0;
    int nResult = -1;
    /* Select Hardware */
    nChannelCount = nGetNoOfConnectedHardware();
    // Assign the channel count
    sg_ucNoOfHardware = (UCHAR)nChannelCount;
    /* Capture only Driver Not Running event
     * Take action based on number of Hardware Available
     */
    char acNo_Of_Hw[MAX_STRING] = {0};
    sprintf_s(acNo_Of_Hw, _T("Number of Vector hardwares Available: %d"), nChannelCount);

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
        MessageBox(NULL,sg_omErrStr, NULL, MB_OK | MB_ICONERROR);
        nChannelCount = -1;
    }
    /* Available hardware is lesser then the supported channels */
    else
    {
        // Check whether channel selection dialog is required
        if( nChannelCount > 1)
        {
            // Get the selection from the user. This will also
            // create and assign the networks
            nResult = nCreateMultipleHardwareNetwork();
        }
        else
        {
            // Use available one hardware
            nResult = nCreateSingleHardwareNetwork();
        }
    }

    return nResult;
}

/**
* \brief         This function will check if the client exists and gives back the client index.
* \param[in]     pcClientName, client name as char*
* \param[out]    Index, client index if found
* \return        TRUE if client name is found, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bClientExist(char* pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName, sg_asClientToBufMap[i].pacClientName.c_str()))
        {
            Index = i;
            return TRUE;
        }
    }

    return FALSE;
}

/**
* \brief         This function will get available client slot
* \param         void
* \return        Returns the available client ID
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
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

/**
* \brief         This function will remove the existing client ID
* \param[in]     dwClientId, client ID to be removed
* \return        Returns TRUE if client ID removal is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
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
                    sg_asClientToBufMap[unClientIndex].pacClientName = "";

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
                sg_asClientToBufMap[unClientIndex].pacClientName = "";

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

/**
* \brief         This function will check if the client ID exists
* \param[in]     dwClientId, client ID to be checked for existance
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
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
* \brief         This function will return the client index based on clientID
* \param[in]     dwClientId, client ID whose client index is needed
* \param[out]    unClientIndex, client index to be returned
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
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
* \brief         Function to retreive error occurred and log it
* \param[in]     File, pointer to log file
* \param[in]     Line, indicates line number in log file
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line)
{
    USES_CONVERSION;
    char acErrText[MAX_PATH] = {'\0'};
    /* Get the error text for the corresponding error code */
    sg_pIlog->vLogAMessage(A2T(File), Line, A2T(acErrText));
    sg_acErrStr = acErrText;
}

/**
* \brief         Function to check if client buffer exists
* \param[in]     sClientObj, alias to SCLIENTBUFMAP object
* \param[in]     pBuf, pointer to CBaseCANBufFSE object
* \return        TRUE if buffer exists, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;

    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; // break the loop
        }
    }

    return bExist;
}

/**
* \brief         Function to remove exissting client buffer
* \param[in]     RootBufferArray, pointer to CBaseCANBufFSE class array
* \param[out]    unCount, indicates buffer count which will get reduced
* \param[in]     BufferToRemove, pointer to the buffer to be removed
* \return        TRUE if removed
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
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