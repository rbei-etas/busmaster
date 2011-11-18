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
// CAN_Vector_XL.cpp : Defines the initialization routines for the DLL.
//

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
XLSETGLOBALTIMESYNC			   xlSetGlobalTimeSync = NULL;

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

typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)const
    {        
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel)); 
    }
}SACK_MAP;

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

typedef struct tagDATINDSTR
{
    BOOL    m_bIsConnected;
    HANDLE  m_hHandle;
    BOOL    m_bToContinue;
} sDatIndStr;

static sDatIndStr s_DatIndThread;

#define MAX_BUFF_ALLOWED 16
#define RECEIVE_EVENT_SIZE 1                // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      4096             // internal driver queue size in CAN events

/**
 * Client and Client Buffer map
 */
typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    TCHAR pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = NULL;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (TCHAR) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }

    }
} SCLIENTBUFMAP;

/* Local variables */
static XLCLOSEDRIVER           xlDllCloseDriver = NULL;
static XLOPENDRIVER            xlDllOpenDriver = NULL;

/* Forward declarations*/
static int nInitHwNetwork();
static BOOL bRemoveClient(DWORD dwClientId);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(TCHAR* pcClientName, INT& Index);
static BOOL bClientIdExist(const DWORD& dwClientId);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line);
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static int nConnect(BOOL bConnect, BYTE /*hClient*/);
static int nGetNoOfConnectedHardware(void);
BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID);
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
static SCONTROLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];


// TZM specific Global variables
#define CAN_MAX_ERRSTR 256
static char sg_acErrStr[CAN_MAX_ERRSTR] = {'\0'};
static UINT sg_unClientCnt = 0;
#define MAX_CLIENT_ALLOWED 16
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT m_unDevChannelMap[defNO_OF_CHANNELS] = {-1};

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
	HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
	HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
	HRESULT CAN_GetBoardInfo(s_BOARDINFO& BoardInfo);
	HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
	HRESULT CAN_GetVersionInfo(VERSIONINFO& sVerInfo);
	HRESULT CAN_GetLastErrorString(CHAR* acErrorStr, int nLength);
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

static CDIL_CAN_VectorXL* sg_pouDIL_CAN_VectorXL = NULL;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{	
	HRESULT hResult = S_OK;
	if ( NULL == sg_pouDIL_CAN_VectorXL )
	{
		if ((sg_pouDIL_CAN_VectorXL = new CDIL_CAN_VectorXL) == NULL)
		{
			hResult = S_FALSE;
		}
	}
	*ppvInterface = (void *) sg_pouDIL_CAN_VectorXL; /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */

	return hResult;
}

/**
 * Number of Channels
 */
static UINT m_nNoOfChannels = 0;

static TCHAR m_omErrStr[MAX_STRING] = {0};

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

/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;

/**
 * Channel information
 */
struct CChannel
{
    /* To store baud rate information */
    USHORT  m_usClock;
    USHORT  m_usSampling;
    UINT    m_unBaudrate;

    /* Bit Timing */
    USHORT  m_usBaudRate; /*BTR0,BTR1*/
    BYTE    m_bCNF1;
    BYTE    m_bCNF2;
    BYTE    m_bCNF3;

    /**
     * To store controller state
     * 0 - Active State
     * 1 - Passive State
     * 2 - Bus Off State
     * -1 - For invalid value
     */
    UCHAR m_ucControllerState;

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
        // Baud Rate
        m_usBaudRate = defBAUD_RATE;

        // Controller state
        m_ucControllerState = defMODE_ACTIVE;

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
static CChannel m_aodChannels[ defNO_OF_CHANNELS ];

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_VectorXL::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger *pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;

    /* Initialise both the time parameters */
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;

    /* Query Tick Count */
    sg_QueryTickCount.QuadPart = 0;

    /* INITIALISE_ARRAY(sg_acErrStr); */
    memset(sg_acErrStr, 0, sizeof(sg_acErrStr));
    CAN_ManageMsgBuf(MSGBUF_CLEAR, NULL, NULL);

	return S_OK;
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_VectorXL::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != NULL)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP &sClientObj = sg_asClientToBufMap[unClientIndex];
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls  
 */
HRESULT CDIL_CAN_VectorXL::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName)
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

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the controller status. hEvent will be registered
 * and will be set whenever there is change in the controller
 * status.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
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
			xlCanResetAcceptance	   = (XLCANRESETACCEPTANCE)       GetProcAddress(hxlDll,"xlCanResetAcceptance");
			xlCanRequestChipState      = (XLCANREQUESTCHIPSTATE)      GetProcAddress(hxlDll,"xlCanRequestChipState");
			xlCanFlushTransmitQueue	   = (XLCANFLUSHTRANSMITQUEUE)    GetProcAddress(hxlDll,"xlCanFlushTransmitQueue");
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs intial operations.
 * Initializes filter, queue, controller config
 * with default values.
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

	return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
static BOOL bLoadDataFromContr(PSCONTROLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;

    /* If successful */
    if (pControllerDetails != NULL)
    {
        for( UINT nIndex = 0; nIndex < m_nNoOfChannels; nIndex++ )
        {
            TCHAR* pcStopStr = NULL;
            CChannel& odChannel = m_aodChannels[ nIndex ];

            /* Get Warning Limit */
            odChannel.m_ucWarningLimit = static_cast <UCHAR>(
                                             _tcstol( pControllerDetails[ nIndex ].m_omStrWarningLimit,
                                                     &pcStopStr, defBASE_DEC ));

            /* Get Baud Rate */
            odChannel.m_usBaudRate = static_cast <USHORT>(
                                         pControllerDetails[ nIndex ].m_nBTR0BTR1 );
            odChannel.m_unBaudrate = _tstoi( pControllerDetails[ nIndex ].m_omStrBaudrate );
            odChannel.m_ucControllerState = pControllerDetails[ nIndex ].m_ucControllerMode;
        }

        /* Get Controller Mode */
        // Consider only the first channel mode as controller mode
        //sg_ucControllerMode = pControllerDetails[ 0 ].m_ucControllerMode;

        bReturn = TRUE;
    }
    return bReturn;    
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs closure operations.
 */
HRESULT CDIL_CAN_VectorXL::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

	//Stop the read thread
    sg_sParmRThread.bTerminateThread();

	nDisconnectFromDriver();
	// ------------------------------------
	// Close the driver
	// ------------------------------------
	xlDllCloseDriver();    
	
    UINT ClientIndex = 0;
    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);
    }	
    hResult = CAN_DeselectHwInterface();

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_DRIVER_SELECTED;
    }

	return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the time mode mapping of the hardware. CurrSysTime
 * will be updated with the system time ref.
 * TimeStamp will be updated with the corresponding timestamp.
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Lists the hardware interface available. sSelHwInterface
 * will contain the user selected hw interface.
 */
HRESULT CDIL_CAN_VectorXL::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (nInitHwNetwork() == 0)
    {
        nCount = m_nNoOfChannels;
        for (UINT i = 0; i < m_nNoOfChannels; i++)
        {
            asSelHwInterface[i].m_dwIdInterface = i;
			unsigned int serialNumber = m_aodChannels[i].m_pXLChannelInfo->serialNumber;
            _stprintf(asSelHwInterface[i].m_acDescription, _T("%d"), serialNumber);
			hResult = S_OK;
            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
        }
    }
    else
    {
        hResult = NO_HW_INTERFACE;
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Error connecting to driver"));
    }
    return hResult;
}

/**
 * \brief Selects the hardware interface selected by the user.
 * \return S_OK for success, S_FALSE for failure
 *
 * Function to List Hardware interfaces connect to the system and requests to the
 * user to select.
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Deselects the selected hardware interface.
 */
HRESULT CDIL_CAN_VectorXL::CAN_DeselectHwInterface(void)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    CAN_ResetHardware();

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

BOOL Callback_DILTZM(BYTE /*Argument*/, PBYTE pDatStream, int /*Length*/)
{
	return (sg_pouDIL_CAN_VectorXL->CAN_SetConfigData((CHAR *) pDatStream, 0) == S_OK);
}

/**
 * \return defERR_OK if successful otherwise corresponding Error code.
 *
 * This function will Get the baud rate of the controller channels
 * 
 */
static int nGetBaudRate()
{
    int nReturn = 0;
	XLaccess xlChanMaskTx = 0;
	XLstatus xlStatus;

	xlStatus = xlGetDriverConfig(&g_xlDrvConfig);

	if ( xlStatus == XL_SUCCESS )
	{
		
	}

    return nReturn;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Displays the controller configuration dialog.
 * Fields are initialized with values supplied by InitData.
 * InitData will be updated with the user selected values.
 */
HRESULT CDIL_CAN_VectorXL::CAN_DisplayConfigDlg(PCHAR& InitData, INT& Length)
{
	xlPopupHwConfig(NULL, INFINITE);

	//Get back the baud rate from controller
	SCONTROLER_DETAILS* pCntrlDetails = (SCONTROLER_DETAILS*)InitData;		
	XLstatus xlStatus;

	xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
	for ( int i = 0 ; i < m_nNoOfChannels ; i++ )
	{
		sprintf(pCntrlDetails[i].m_omStrBaudrate, ("%0.3f"), float(
					g_xlDrvConfig.channel[m_aodChannels[i].m_pXLChannelInfo->channelIndex].
													busParams.data.can.bitRate / 1000.000 ));				
	}

    return S_OK;
}

/**
 * S_OK for success, S_FALSE for failure
 *
 * Sets the controller configuration data supplied by InitData.
 */
HRESULT CDIL_CAN_VectorXL::CAN_SetConfigData(PCHAR ConfigFile, INT Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;

    memcpy((void*)sg_ControllerDetails, (void*)ConfigFile, Length);

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

/**
 * Function to create time mode mapping
 */
static void vCreateTimeModeMapping()
{    
    GetLocalTime(&sg_CurrSysTime);

    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);	
}

/**
 * \param[in] lError Error code in Peak USB driver format
 * \param[in] byDir  Error direction Tx/Rx
 * \return Error code in BUSMASTER application format
 *
 * This will convert the error code from Perk USB driver format
 * to the format that is used by BUSMASTER.
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
 * \param[in]  XLevent& xlEvent message polled from the bus in XLevent format
 * \param[out] sCanData Application specific data format 
 * \return TRUE (always)
 *
 * This will classify the messages, which can be one of Rx, Tx or
 * Error messages. In case of Err messages this identifies under
 * what broader category (Rx / Tx) does this occur.
 */
static BYTE bClassifyMsgType(XLevent& xlEvent, STCANDATA& sCanData)
{	
    static LONGLONG QuadPartRef = 0;
    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {		
		vCreateTimeModeMapping();
		QuadPartRef = (LONGLONG)xlEvent.timeStamp / 100000;
		sg_byCurrState = CALC_TIMESTAMP_READY;
    }

	sCanData.m_lTickCount.QuadPart = (LONGLONG)(xlEvent.timeStamp / 100000);
	sg_TimeStamp = sCanData.m_lTickCount.QuadPart =
                           (sCanData.m_lTickCount.QuadPart - QuadPartRef);
	
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
		sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = xlEvent.tagData.msg.dlc;

        /* Copy the message data */
        memcpy(sCanData.m_uDataInfo.m_sCANMsg.m_ucData,
				xlEvent.tagData.msg.data, xlEvent.tagData.msg.dlc);

        /* Copy the message ID */
        sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID = (UINT)xlEvent.tagData.msg.id;

        /* Check for extended message indication */
        sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED =
            (xlEvent.tagData.msg.id & XL_CAN_EXT_MSG_ID) ? TRUE : FALSE;

		if ( sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED )
			sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID ^= XL_CAN_EXT_MSG_ID; // make it std		
		
		/* Set channel ID */
		sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)m_unDevChannelMap[xlEvent.chanIndex] + 1;
    }
	else
	{
        sCanData.m_ucDataType = ERR_FLAG;
        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)m_unDevChannelMap[xlEvent.chanIndex] + 1;
		sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)m_unDevChannelMap[xlEvent.chanIndex] + 1;

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
 * Processing of the received packets from bus
 */
static void ProcessCANMsg(XLevent& xlEvent, UINT unClientIndex)
{
    int nSize = sg_nFRAMES;

	bClassifyMsgType(xlEvent, sg_asCANMsg);

	vWriteIntoClientsBuffer(sg_asCANMsg, unClientIndex);        
}

/**
 * Read thread procedure
 */
DWORD WINAPI CanMsgReadThreadProc_CAN_Vector_XL(LPVOID pVoid)
{
    USES_CONVERSION;
	XLstatus xlStatus = XL_SUCCESS;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;

    /* Validate certain required pointers */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam, (DWORD)-1);
    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    /* Set the event to CAN_ICS_neoVI driver for wakeup and frame arrival notification */

	for (UINT i = 0; i < sg_unClientCnt; i++)
		XLstatus xlStatus = xlSetNotification (g_xlPortHandle[i], &g_hDataEvent[i], 1);	

    if (g_hDataEvent[0] != NULL)
    {
        s_DatIndThread.m_hHandle = g_hDataEvent[0];
        pThreadParam->m_hActionEvent = g_hDataEvent[0];
    }

    /* Get the handle to the controller and validate it */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam->m_hActionEvent, (DWORD)-1);

    DWORD dwResult = 0;
	unsigned int    msgsrx = RECEIVE_EVENT_SIZE;	
	XLevent  xlEvent; 

    while (s_DatIndThread.m_bToContinue)
    {
        if (s_DatIndThread.m_bIsConnected)
        {
			for (UINT i = 0; i < sg_unClientCnt; i++)
			{
				WaitForSingleObject(g_hDataEvent[i],10);			

				xlStatus = XL_SUCCESS;
				while (!xlStatus) 
				{		      
					msgsrx = RECEIVE_EVENT_SIZE;
					xlStatus = xlReceive(g_xlPortHandle[i], &msgsrx, &xlEvent);   
					//if ( xlStatus!=XL_ERR_QUEUE_IS_EMPTY ) 
					if ( xlStatus == XL_SUCCESS )
					{
						ProcessCANMsg(xlEvent, i);
					}							    
				}
			}
        }
        else
        {
            WaitForSingleObject(pThreadParam->m_hActionEvent, 500);
            switch (pThreadParam->m_unActionCode)
            {
                case EXIT_THREAD:
                {
                    s_DatIndThread.m_bToContinue = FALSE;
                }
                break;
            }
        }
    }
	SetEvent(pThreadParam->hGetExitNotifyEvent());

    /* Close the notification event for startup */
    //CloseHandle(pThreadParam->m_hActionEvent);
	for (UINT i = 0; i < sg_unClientCnt; i++)
	{		
		ResetEvent(g_hDataEvent[i]);
		g_hDataEvent[i] = NULL;
	}
	//ResetEvent(pThreadParam->m_hActionEvent);
    pThreadParam->m_hActionEvent = NULL;

    return 0;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Starts the controller.
 */
HRESULT CDIL_CAN_VectorXL::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
    //Restart the read thread
    sg_sParmRThread.bTerminateThread();
    sg_sParmRThread.m_pBuffer = (LPVOID) &s_DatIndThread;
    s_DatIndThread.m_bToContinue = TRUE;	

    if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_Vector_XL))
    {
        hResult = S_OK;
    }
    else
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Could not start the read thread" ));
    }
    //If everything is ok connect to the network
    if (hResult == S_OK)
    {
        for (UINT ClientIndex = 0; ClientIndex < sg_unClientCnt; ClientIndex++)
        {
            BYTE hClient = sg_asClientToBufMap[ClientIndex].hClientHandle;
            hResult = nConnect(TRUE, hClient);
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
        }
    }
    return hResult;    
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This will close the connection with the driver. This will be
 * called before deleting HI layer. This will be called during
 * application close.
 */
static int nDisconnectFromDriver()
{
    int nReturn = 0;
	XLstatus xlStatus;

	for ( UINT i = 0; i< sg_unClientCnt; i++ )
	{
		if (g_xlPortHandle[i] != XL_INVALID_PORTHANDLE) 
		{
			xlStatus = xlDeactivateChannel( g_xlPortHandle[i], g_xlChannelMask ); 
			xlStatus = xlClosePort(g_xlPortHandle[i]);		
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
 * \return void
 *
 * Function to map device channels with BUSMASTER channel order
 */
static void vMapDeviceChannelIndex()
{
	//Reset previous channel ID assignment if any
	for (UINT i = 0; i < m_nNoOfChannels; i++)	
	{
		m_unDevChannelMap[m_aodChannels[i].m_pXLChannelInfo->channelIndex] = i;
	}
}

/**
 * \param[in] bConnect TRUE to Connect, FALSE to Disconnect
 * \return Returns defERR_OK if successful otherwise corresponding Error code.
 *
 * This function will connect the tool with hardware. This will
 * establish the data link between the application and hardware.
 * Parallel Port Mode: Controller will be disconnected.
 * USB: Client will be disconnected from the network
 */
static int nConnect(BOOL bConnect, BYTE /*hClient*/)
{
    int nReturn = -1;
	XLstatus xlStatus;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
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
			/* Transit into 'CREATE TIME MAP' state */
            sg_byCurrState = CREATE_MAP_TIMESTAMP;				
			vMapDeviceChannelIndex();
			sg_bIsConnected = bConnect;			
			s_DatIndThread.m_bIsConnected = sg_bIsConnected;					
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
        s_DatIndThread.m_bIsConnected = sg_bIsConnected;		
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Stops the controller.
 */
HRESULT CDIL_CAN_VectorXL::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    for (UINT ClientIndex = 0; ClientIndex < sg_unClientCnt; ClientIndex++)
    {
        BYTE hClient = sg_asClientToBufMap[ClientIndex].hClientHandle;

        hResult = nConnect(FALSE, hClient);
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
            ClientIndex = sg_unClientCnt; // break the loop
        }
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Resets the controller.
 */
HRESULT CDIL_CAN_VectorXL::CAN_ResetHardware(void)
{
	HRESULT hResult = S_FALSE;

    /* Stop the hardware if connected */
    CAN_StopHardware(); // return value not necessary
    /*if (sg_sParmRThread.bTerminateThread())
    {
        hResult = S_OK;
    }*/

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Function to get Controller status
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
	StatusData.wControllerStatus = NORMAL_ACTIVE;

	return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the Tx queue configured.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}

/**
 * \param[in] sMessage Message to Transmit
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This will send a CAN message to the driver. In case of USB
 * this will write the message in to the driver buffer and will
 * return. In case if parallel port mode this will write the
 * message and will wait for the ACK event from the driver. If
 * the event fired this will return 0. Otherwise this will return
 * wait time out error. In parallel port it is a blocking call
 * and in case of failure condition this will take 2 seconds.
 */
static int nWriteMessage(STCAN_MSG sMessage, DWORD dwClientID)
{
    int nReturn = -1;
	XLaccess xlChanMaskTx = 0;
	UINT unClientIndex = -1;

    /* Return when in disconnected state */
    //if (!sg_bIsConnected) return nReturn;

    if ((sMessage.m_ucChannel > 0) &&
            (sMessage.m_ucChannel <= m_nNoOfChannels))
    {        
		static XLevent       xlEvent;
		XLstatus             xlStatus;
		unsigned int         messageCount = 1;

		memset(&xlEvent, 0, sizeof(xlEvent));

		xlEvent.tag                 = XL_TRANSMIT_MSG;
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
		xlChanMaskTx = m_aodChannels[sMessage.m_ucChannel - 1].m_pXLChannelInfo->channelMask;		
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Sends STCAN_MSG structure from the client dwClientID.   
 */
HRESULT CDIL_CAN_VectorXL::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= m_nNoOfChannels)
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets board info.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetBoardInfo(s_BOARDINFO& /*BoardInfo*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets bus config info.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets driver version info.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetVersionInfo(VERSIONINFO& /*sVerInfo*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets last occured error and puts inside acErrorStr.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetLastErrorString(CHAR* acErrorStr, INT nLength)
{
    int nCharToCopy = (int) (strlen(sg_acErrStr));
    if (nCharToCopy > nLength)
    {
        nCharToCopy = nLength;
    }
    strncpy(acErrorStr, sg_acErrStr, nCharToCopy);

    return S_OK;    
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Applies FilterType(PASS/STOP) filter for corresponding
 * channel. Frame ids are supplied by punMsgIds.
 */
HRESULT CDIL_CAN_VectorXL::CAN_FilterFrames(FILTER_TYPE /*FilterType*/, TYPE_CHANNEL /*Channel*/, UINT* /*punMsgIds*/, UINT /*nLength*/)
{
    return S_OK;
}

/**
 * \param[out] ucaTestResult Array that will hold test result. TRUE if hardware present and false if not connected
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will check all hardware connectivity by activating selected channels.
 */
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) //const
{    
    int nReturn = 0;    
	XLstatus xlStatus;
    if (nChannel < m_nNoOfChannels)
    {
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the controller param eContrParam of the channel.
 * Value stored in lParam.
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
                lParam = m_nNoOfChannels;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                if (nGetNoOfConnectedHardware() > 0)
                {
                    lParam = m_nNoOfChannels;
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
                if (nChannel < m_nNoOfChannels)
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
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the error counter for corresponding channel.
 */
HRESULT CDIL_CAN_VectorXL::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= m_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = m_aodChannels[nChannel].m_ucTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = m_aodChannels[nChannel].m_ucRxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = m_aodChannels[nChannel].m_ucPeakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = m_aodChannels[nChannel].m_ucPeakRxErrorCounter;
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
 * \return
 *   Returns defERR_OK if successful otherwise corresponding Error code.
 *   0, Query successful, but no device found
 *   > 0, Number of devices found
 *   < 0, query for devices unsuccessful
 *
 * Finds the number of hardware connected.
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
				nResult++;
			}
		}
		if (!nResult) 
		{			
			_tcscpy(m_omErrStr, _T("No available channels found! (e.g. no CANcabs...)\r\n"));
			xlStatus = XL_ERROR;			
		}
	}
	else
	{
        _tcscpy(m_omErrStr, _T("Problem Finding Device!"));
        nResult = -1;
	}	
    /* Return the operation result */
    return nResult;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will popup hardware selection dialog and gets the user selection of channels.
 *
 */
int ListHardwareInterfaces(HWND hParent, DWORD dwDriver, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{    	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CHardwareListing HwList(psInterfaces, nCount, NULL);
    HwList.DoModal();
    nCount = HwList.nGetSelectedList(pnSelList);  
	
    return 0;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will get the hardware selection from the user
 * and will create essential networks.
 */
static int nCreateMultipleHardwareNetwork()
{
	//TCHAR acTempStr[256] = {_T('\0')};
	INT anSelectedItems[defNO_OF_CHANNELS] = {0};
	int nHwCount = sg_ucNoOfHardware;
	int nChannels = 0;
    // Get Hardware Network Map
	for (int nCount = 0; nCount < nHwCount; nCount++)
	{
		// we take all hardware we found and
		// check that we have only CAN cabs/piggy's
		// at the moment there is no VN8910 XLAPI support!		
		if ( /*(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
			  && */(g_xlDrvConfig.channel[nCount].hwType != XL_HWTYPE_VN8900) ) 
		{ 
			sg_HardwareIntr[nChannels].m_dwIdInterface = nCount;
			sg_HardwareIntr[nChannels].m_dwVendor = g_xlDrvConfig.channel[nCount].serialNumber;		
			/*_stprintf(acTempStr, _T("SN: %d, Port ID: %d"), sg_HardwareIntr[nChannels].m_dwVendor, 
																	sg_HardwareIntr[nChannels].m_dwIdInterface);*/
			_tcscpy(sg_HardwareIntr[nChannels].m_acDescription, g_xlDrvConfig.channel[nCount].name);			
			nChannels++;
		}
	}
	nHwCount = nChannels;	//Reassign hardware count according to final list of channels supported.
	ListHardwareInterfaces(sg_hOwnerWnd, DRIVER_CAN_VECTOR_XL, sg_HardwareIntr, anSelectedItems, nHwCount);

    sg_ucNoOfHardware = (UCHAR)nHwCount;
	m_nNoOfChannels = (UINT)nHwCount;
	g_xlChannelMask = 0;
	//Reorder hardware interface as per the user selection
	for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
	{
		m_aodChannels[nCount].m_pXLChannelInfo  = &g_xlDrvConfig.channel[sg_HardwareIntr[anSelectedItems[nCount]].m_dwIdInterface];		
		g_xlChannelMask |= m_aodChannels[nCount].m_pXLChannelInfo->channelMask;
	}
	g_xlPermissionMask = g_xlChannelMask;

	s_DatIndThread.m_bToContinue = TRUE;
	s_DatIndThread.m_bIsConnected = FALSE;	

	return defERR_OK;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will create a single network with available single hardware.
 */
static int nCreateSingleHardwareNetwork()
{    
	g_xlChannelMask = 0;

    // Set the number of channels as 1
	sg_ucNoOfHardware = (UCHAR)1;
	m_nNoOfChannels = 1;		        

	for (UINT i=0; i < g_xlDrvConfig.channelCount; i++) 
	{      
		// we take all hardware we found and
		// check that we have only CAN cabs/piggy's
		// at the moment there is no VN8910 XLAPI support!
		if ( (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
		  && (g_xlDrvConfig.channel[i].hwType != XL_HWTYPE_VN8900) ) 
		{ 	        				
			m_aodChannels[0].m_pXLChannelInfo  = &g_xlDrvConfig.channel[i];		
			g_xlChannelMask |= m_aodChannels[0].m_pXLChannelInfo->channelMask;	
			break;
		}
	}

	g_xlPermissionMask = g_xlChannelMask;

    s_DatIndThread.m_bToContinue = TRUE;
    s_DatIndThread.m_bIsConnected = FALSE;  
    
    return defERR_OK;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will create find number of hardware connected.
 * It will create network as per hardware count.
 * This will popup hardware selection dialog in case there are more
 * hardware present.
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
    TCHAR acNo_Of_Hw[MAX_STRING] = {0};
    _stprintf(acNo_Of_Hw, _T("Number of Vector hardwares Available: %d"), nChannelCount);

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
		MessageBox(NULL,m_omErrStr, NULL, MB_OK | MB_ICONERROR);
		nChannelCount = -1;
    }
    /* Available hardware is lesser then the supported channels */
    else
    {
        s_DatIndThread.m_bToContinue = TRUE;
        s_DatIndThread.m_bIsConnected = FALSE;

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

static BOOL bClientExist(TCHAR* pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName, sg_asClientToBufMap[i].pacClientName))
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

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
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (TCHAR) * MAX_PATH);
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
                memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (TCHAR) * MAX_PATH);
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
 * Function to retreive error occurred and log it
 */
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
    strncpy(sg_acErrStr, acErrText, nStrLen);
}

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
