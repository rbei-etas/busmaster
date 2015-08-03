/**
 * \file      LIN_ISOLAR_EVE_VLIN_Defines.h
 * \brief     Contains all the definitions and Function pointers
 * \author    a4@uincore.net
 * \copyright Copyright (c) 2015, UinCore. All rights reserved.
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
#include "linlib.h"
#include "canlib.h"

#define LIN_VERSION_1_3                      0x01 //!< LIN version 1.3
#define LIN_VERSION_2_0                      0x02 //!< LIN version 2.0
#define LIN_VERSION_2_1                      0x03 //!< LIN version 2.1

/**
 * Client and Client Buffer map
 */


/* Project definitions */
//static UINT sg_unClientCnt = 0;
//static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

/* Receive Thread object */
static CPARAM_THREADPROC sg_sParmRThread;

static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe
typedef std::list<SACK_MAP> CACK_MAP_LIST;
static STLINDATA sg_asLINMsg;

static HWND sg_hOwnerWnd = nullptr;
static Base_WrapperErrorLogger* sg_pIlog   = nullptr;
std::string sg_acErrStr = "";

/* Forward declarations*/
static BOOL bRemoveClient(DWORD dwClientId);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(std::string pcClientName, INT& Index);
static BOOL bClientIdExist(const DWORD& dwClientId);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line);
//static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseLINBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseLINBufFSE* RootBufferArray[MAX_BUFF_ALLOWED],
                                UINT& unCount, CBaseLINBufFSE* BufferToRemove);

/**
 * Channel information
 */
struct CChannel
{
    /* Kvaser channel details */
    int        m_nChannel;
    char      m_strName[MAX_CHAR_LONG];
    DWORD      m_dwHwType;
    LinHandle  m_hnd;
    int        m_nHwIndex;
    int        m_nHwChannel;
    int        m_nIsOnBus;
    int        m_nDriverMode;
    int        m_nTxAck;

    /* To store baud rate information */
    USHORT  m_usClock;
    USHORT  m_usSampling;
    UINT    m_unBaudrate;
	    /* LIN Mode */
    UINT  m_unLINMode;

    /* LIN Version */
    UINT  m_unLINVersion;
	std::string m_strLinVersion;

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
        m_nDriverMode   = LIN_SLAVE;
        m_nChannel      = -1;
        m_hnd           = -1;
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
 * Number of Channels
 */
static UINT sg_nNoOfChannels = 0;

static char sg_omErrStr[256] = {0};

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

char            g_AppName[321]  = "XDiag";            //!< Application name which is displayed in VHWconf
unsigned int    g_BaudRate                  = 19200;                  //!< Default baudrate 19.2KBauds
int             g_silent                    = 0;                      //!< flag to visualize the message events (on/off)
unsigned int    g_TimerRate                 = 0;                      //!< Global timerrate (to toggel)
HANDLE          g_hDataEvent                = 0;
////////////////////////////////////////////////////////////////////////////

static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];
static UINT sg_unDevChannelMap[defNO_OF_CHANNELS] = {(UINT)-1};
static BOOL sg_bIsConnected = FALSE;
