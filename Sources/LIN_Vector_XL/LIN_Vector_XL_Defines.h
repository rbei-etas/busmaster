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
 * \file      LIN_ISOLAR_EVE_VLIN_Defines.h
 * \brief     Contains all the definitions and Function pointers
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains all the definitions and Function pointers
 */

/* Vendor header files */

/* Project headers definitions */
#include "DataTypes/DIL_Datatypes.h"
#include "DIL_Interface\CommanDIL_LIN.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Utility/Utility_Thread.h"
#define DYNAMIC_XLDRIVER_DLL
#include "EXTERNAL/vxlapi.h"



/**
 * Client and Client Buffer map
 */


/* Project definitions */
//static UINT sg_unClientCnt = 0;
//static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

/* Receive Thread object */
static CPARAM_THREADPROC sg_sParmRThread;

/* For maintaining Acknwoledgement map entries */
/*typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)const
    {
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel));
    }
} SACK_MAP;*/

static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe
typedef std::list<SACK_MAP> CACK_MAP_LIST;
//static CACK_MAP_LIST sg_asAckMapBuf;
static STLINDATA sg_asLINMsg;

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;
string sg_acErrStr = "";

/* Forward declarations*/
static BOOL bRemoveClient(DWORD dwClientId);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(string pcClientName, INT& Index);
static BOOL bClientIdExist(const DWORD& dwClientId);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line);
//static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseLINBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseLINBufFSE* RootBufferArray[MAX_BUFF_ALLOWED],
                                UINT& unCount, CBaseLINBufFSE* BufferToRemove);


/* VxlApi.dll handle */
static HINSTANCE               hxlDll;

/* Vector XL function pointers */
XLCLOSEDRIVER                  xlDllCloseDriver = NULL;
XLOPENDRIVER                   xlDllOpenDriver = NULL;
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

/* LIN specific functions */
XLLINSETCHANNELPARAMS           xlLinSetChannelParams = NULL;
XLLINSETDLC                     xlLinSetDLC = NULL;
XLLINSETSLAVE                   xlLinSetSlave = NULL;
XLLINSENDREQUEST                xlLinSendRequest = NULL;
XLLINSETSLEEPMODE               xlLinSetSleepMode = NULL;
XLLINWAKEUP                     xlLinWakeUp = NULL;
XLLINSETCHECKSUM                xlLinSetChecksum = NULL;
XLLINSWITCHSLAVE                xlLinSwitchSlave = NULL;

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
    USHORT  m_usBaudRate;

    /* LIN Mode */
    UINT  m_unLINMode;

    /* LIN Version */
    UINT  m_unLINVersion;

    /* Acceptance Filter information for standard and extended envelopes*/
    SACC_FILTER_INFO     m_sFilter[2];

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


    map<int, int> m_nMapIdChecksumType;

    // Init members with default value
    CChannel()
    {
        // Baud Rate
        m_usBaudRate = 20;  //TODO::

        // LIN Mode
        m_unLINMode = XL_LIN_SLAVE;

        // LIN Version
        m_unLINVersion = XL_LIN_VERSION_1_3;

        // Controller state
        m_ucControllerState = defMODE_ACTIVE;

        // Pointer to corresponding XLchannelConfig
        m_pXLChannelInfo = NULL;

        m_nMapIdChecksumType.clear();
    }
};

/**
 * Array of channel objects. The size of this array is maximun number
 * of channels the application will support.
 */
static CChannel sg_aodChannels[ defNO_OF_CHANNELS ];

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

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

/* To make it thread safe */
static  CRITICAL_SECTION sg_CritSectForWrite;

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

/////////////////////////////////////////////////////////////////////////////
// globals

char            g_AppName[XL_MAX_LENGTH+1]  = "BUSMASTER";            //!< Application name which is displayed in VHWconf
XLportHandle    g_xlPortHandle[MAX_CLIENT_ALLOWED]
    = {XL_INVALID_PORTHANDLE};                                        //!< Global porthandles
XLdriverConfig  g_xlDrvConfig;                                        //!< Contains the actual hardware configuration
XLaccess        g_xlChannelMask             = 0;                      //!< Global channelmask (includes all founded channels)
XLaccess        g_xlPermissionMask          = 0;                      //!< Global permissionmask (includes all founded channels)
unsigned int    g_BaudRate                  = 19200;                  //!< Default baudrate 19.2KBauds
int             g_silent                    = 0;                      //!< flag to visualize the message events (on/off)
unsigned int    g_TimerRate                 = 0;                      //!< Global timerrate (to toggel)
XLhandle        g_hDataEvent[MAX_CLIENT_ALLOWED]  = {0};
////////////////////////////////////////////////////////////////////////////

static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];
static UINT sg_unDevChannelMap[defNO_OF_CHANNELS] = {(UINT)-1};
static BOOL sg_bIsConnected = FALSE;
