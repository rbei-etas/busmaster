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
 * \file      CAN_Kvaser_CAN.cpp
 * \brief     Source file for Kvaser CAN DIL functions
 * \author    Tobias Lorenz, Arunkumar Karri
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for Kvaser CAN DIL functions
 */
// CAN_Kvaser_CAN.cpp : Defines the initialization routines for the DLL.
//

#include "CAN_Kvaser_CAN_stdafx.h"
#include "CAN_Kvaser_CAN.h"
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

#define DYNAMIC_XLDRIVER_DLL
#include "EXTERNAL_INCLUDE/canlib.h"

#define USAGE_EXPORT
#include "CAN_Kvaser_CAN_Extern.h"

// CCAN_Kvaser_CAN

BEGIN_MESSAGE_MAP(CCAN_Kvaser_CAN, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_Kvaser_CAN construction
 */
CCAN_Kvaser_CAN::CCAN_Kvaser_CAN()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_Kvaser_CAN object
CCAN_Kvaser_CAN theApp;


/**
 * CCAN_Kvaser_CAN initialization
 */
BOOL CCAN_Kvaser_CAN::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

#define MAX_BUFF_ALLOWED 16

/* canlib32.dll Message Functions */



/**
 * Number of Channels
 */
static UINT sg_nNoOfChannels = 0;

static TCHAR sg_omErrStr[MAX_STRING] = {0};

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

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

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

static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe
typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;


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
	/* Kvaser channel details */
	int        m_nChannel;
	TCHAR      m_strName[MAX_CHAR_LONG];
	DWORD      m_dwHwType;
	canHandle  m_hnd;
	int        m_nHwIndex;
	int        m_nHwChannel;
	int        m_nIsOnBus;
	int        m_nDriverMode;
	int        m_nTxAck;

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
		m_nDriverMode   = canDRIVER_NORMAL;
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

/* TZM specific Global variables */
#define CAN_MAX_ERRSTR 256
static char sg_acErrStr[CAN_MAX_ERRSTR] = {'\0'};
static UINT sg_unClientCnt = 0;
#define MAX_CLIENT_ALLOWED 16
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT m_unDevChannelMap[defNO_OF_CHANNELS] = {(UINT)-1};

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

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
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], 
								UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static int nDisconnectFromDriver();
static int nSetApplyConfiguration();

// state variables
static BOOL sg_bIsConnected = FALSE;
static UCHAR sg_ucControllerMode = defUSB_MODE_ACTIVE;
static CPARAM_THREADPROC sg_sParmRThread;
static int sg_nFRAMES = 128;
const int ENTRIES_IN_GBUF       = 2000;
static STCANDATA sg_asCANMsg;
static STCAN_MSG sg_ReadMsg;
static SCONTROLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];

/* CDIL_CAN_Kvaser class definition */
class CDIL_CAN_Kvaser : public CBaseDIL_CAN_Controller
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

CDIL_CAN_Kvaser* sg_pouDIL_CAN_Kvaser = NULL;
HANDLE g_hDataEvent[defNO_OF_CHANNELS]  = {0};


/* Using different channel handles to read from channels.This is because the channel handles
   are not thread safe */
canHandle sg_arrReadHandles[CHANNEL_ALLOWED];

/**
* \brief         Returns the CDIL_CAN_Kvaser object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_CAN_Kvaser object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{	
	HRESULT hResult = S_OK;
	if ( NULL == sg_pouDIL_CAN_Kvaser )
	{
		if ((sg_pouDIL_CAN_Kvaser = new CDIL_CAN_Kvaser) == NULL)
		{
			hResult = S_FALSE;
		}
	}
	*ppvInterface = (void *) sg_pouDIL_CAN_Kvaser; /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */

	return hResult;
}

/* CDIL_CAN_Kvaser function definitions */

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK (always)
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
	StatusData.wControllerStatus = NORMAL_ACTIVE;
    return S_OK;
}

/**
* \brief         Sets the application params.
* \param[in]     hWndOwner, is the main frame HWND value
* \param[in]     pILog, is pointer to error logger object
* \return        S_OK (always)
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
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
* \brief         Registers the buffer pBufObj to the client ClientID
* \param[in]     bAction, contains one of the values MSGBUF_ADD or MSGBUF_CLEAR
* \param[in]     ClientID, is the client ID
* \param[in]     pBufObj, is pointer to CBaseCANBufFSE object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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
* \brief         Registers a client to the DIL.
* \param[in]     bRegister, if TRUE signifies 'Register', FALSE indicates 'Unregister'
* \param[out]    ClientID, is Client ID assigned, will be used for further client related calls  
* \param[in]     pacClientName, is the client name
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName)
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
                    strcpy_s(sg_asClientToBufMap[0].pacClientName, pacClientName);
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
                    strcpy_s(sg_asClientToBufMap[Index].pacClientName, pacClientName);

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
*				 and will be set whenever there is change in the controller status.
* \param[in]     hEvent, is the handle of the event
* \param[in]    unCntrlStatus, indicates contoller status
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return WARN_DUMMY_API;
}

/**
* \brief         Performs intial operations.
*			     Initializes filter, queue, controller config with default values.                
* \param         void
* \return        S_OK if the open driver call successfull otherwise S_FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_PerformInitOperations(void)
{
	HRESULT hResult = S_FALSE;

    /* Register Monitor client */
    DWORD dwClientID = 0;
    CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE);

	// ------------------------------------
	// Initialize the CANlib driver libray
	// ------------------------------------		
	canInitializeLibrary();

	//Initialize the selected channel items array to -1
	for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
	{
		sg_anSelectedItems[i] = -1;
	}

	hResult = S_OK;

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
    // If successful
    if (pControllerDetails != NULL)
    {
        for( UINT nIndex = 0; nIndex < sg_nNoOfChannels ; nIndex++ )
        {
            TCHAR* pcStopStr = NULL;
            CChannel& odChannel = sg_aodChannels[ nIndex ];

            // Baudrate in BTR0BTR1 format          
            odChannel.m_usBaudRate = static_cast <USHORT>(pControllerDetails[ nIndex ].m_nBTR0BTR1);
            // Baudrate value in decimal
            odChannel.m_unBaudrate = static_cast <UINT>(
                    _tcstol( pControllerDetails[ nIndex ].m_omStrBaudrate,
                    &pcStopStr, defBASE_DEC ));
                   
            // Get Warning Limit
            odChannel.m_ucWarningLimit = static_cast <UCHAR>(
                    _tcstol( pControllerDetails[ nIndex ].m_omStrWarningLimit,
                    &pcStopStr, defBASE_DEC ));

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
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte1[i],
							&pcStopStr, defBASE_HEX ));

					odChannel.m_sFilter[i].m_ucACC_Code1 = static_cast <UCHAR>(
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte2[i],
							&pcStopStr, defBASE_HEX ));

					odChannel.m_sFilter[i].m_ucACC_Code2 = static_cast <UCHAR>(
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte3[i],
							&pcStopStr, defBASE_HEX ));

					odChannel.m_sFilter[i].m_ucACC_Code3 = static_cast <UCHAR>(
							_tcstol(pControllerDetails[ nIndex ].m_omStrAccCodeByte4[i],
							&pcStopStr, defBASE_HEX));

					odChannel.m_sFilter[i].m_ucACC_Mask0 = static_cast <UCHAR>(
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte1[i],
							&pcStopStr, defBASE_HEX));

					odChannel.m_sFilter[i].m_ucACC_Mask1 = static_cast <UCHAR>(
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte2[i],
							&pcStopStr, defBASE_HEX));

					odChannel.m_sFilter[i].m_ucACC_Mask2 = static_cast <UCHAR>(
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte3[i],
							&pcStopStr, defBASE_HEX));

					odChannel.m_sFilter[i].m_ucACC_Mask3 = static_cast <UCHAR>(
							_tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte4[i],
							&pcStopStr, defBASE_HEX));        	            
				}
				odChannel.m_sFilter[i].m_ucACC_Filter_Type = (UCHAR)i ;
			}


            // Get Baud Rate
            odChannel.m_usBaudRate = static_cast <USHORT>(
                    pControllerDetails[ nIndex ].m_nBTR0BTR1 );
        }
        // Get Controller Mode
        // Consider only the first channel mode as controller mode
        sg_ucControllerMode = pControllerDetails[ 0 ].m_ucControllerMode;
        
        bReturn = TRUE;
    }
    return bReturn;    
}

/**
* \brief         Performs closure operations.
* \param         void
* \return        S_OK if the CAN_StopHardware call successfull otherwise S_FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

	hResult = CAN_StopHardware();	
	
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStamp;	
	//TimeStamp = 0;	
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_ListHwInterfaces(INTERFACE_HW_LIST& /*asSelHwInterface*/, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (nInitHwNetwork() == 0)
    {
        nCount = sg_nNoOfChannels;
		hResult = S_OK;
		sg_bCurrState = STATE_HW_INTERFACE_LISTED;
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{   
	USES_CONVERSION;    

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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_DeselectHwInterface(void)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

	hResult = CAN_ResetHardware();

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

/**
* \brief         Callback function for configuration dialog
* \param[in]     pDatStream, contains SCONTROLER_DETAILS structure
* \return        TRUE if CAN_SetConfigData call succeeds, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
BOOL Callback_DILTZM(BYTE /*Argument*/, PBYTE pDatStream, int /*Length*/)
{
	return (sg_pouDIL_CAN_Kvaser->CAN_SetConfigData((CHAR *) pDatStream, 0) == S_OK);
}

/**
* \brief         Helper function to display configuration dialog
* \param[in]     pControllerDetails, is SCONTROLER_DETAILS structure
* \param[in]     nCount , is the channel count
* \return        returns configuration confirmation status
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/, 
                            PSCONTROLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

	CChangeRegisters ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
    ouChangeRegister.DoModal();
    nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}

/**
* \brief         Displays the controller configuration dialog.
* \param[out]    InitData, is SCONTROLER_DETAILS structure
* \param[out]    Length , is INT
* \return        S_OK for success
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_DisplayConfigDlg(PCHAR& InitData, INT& Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(InitData, S_FALSE);

    HRESULT Result = S_FALSE;
    
    PSCONTROLER_DETAILS pControllerDetails = (PSCONTROLER_DETAILS)InitData;
    //First initialize with existing hw description
    for (INT i = 0; i < min(Length, (INT)sg_nNoOfChannels); i++)
    {   
		sprintf_s(pControllerDetails[i].m_omHardwareDesc, _T("%s"), sg_aodChannels[i].m_strName);
    }
    if (sg_ucNoOfHardware > 0)
    {
        int nResult = DisplayConfigurationDlg(sg_hOwnerWnd, Callback_DILTZM,
            pControllerDetails, sg_ucNoOfHardware);
        switch (nResult)
        {
            case WARNING_NOTCONFIRMED:
            {
                Result = WARN_INITDAT_NCONFIRM;
            }
            break;
            case INFO_INIT_DATA_CONFIRMED:
            {
                bLoadDataFromContr(pControllerDetails);
                memcpy(sg_ControllerDetails, pControllerDetails, sizeof (SCONTROLER_DETAILS) * defNO_OF_CHANNELS);                
                memcpy(InitData, (void*)sg_ControllerDetails, sizeof (SCONTROLER_DETAILS) * defNO_OF_CHANNELS);
                Length = sizeof(SCONTROLER_DETAILS) * defNO_OF_CHANNELS;
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
    else
    {
        Result = S_OK;
    }

    return Result;    
}

/**
* \brief         Function to set the channel baud rate configured by user
* \param         void
* \return        canOK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nSetBaudRate( )
{
	canStatus nStatus = canOK;

    /* Set baud rate to all available hardware */
    for ( UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        // Get Current channel reference
        CChannel& odChannel = sg_aodChannels[ unIndex ];
		if( odChannel.m_hnd >= 0 )
        {
			BYTE BTR0, BTR1;
			//0x47 14
			BTR0 = odChannel.m_usBaudRate >> 8;
			BTR1 = odChannel.m_usBaudRate & 0xFF;
            // Set the baud rate
			nStatus = canSetBusParamsC200( odChannel.m_hnd,				//Handle of the channel
										   BTR0,						//BTR0
										   BTR1);						//BTR1
        }
        else
        {
            vRetrieveAndLog(nStatus, __FILE__, __LINE__);
            // Invalid Hardware Handle. Could be a simulation network.
            // Wrong call            
        }
        // Check for failure
		if( nStatus != canOK )
        {
            // break the loop
            unIndex = sg_nNoOfChannels;
        }
    }
    return nStatus;
}

/**
* \brief         Function to set the channel baud rate configured by user
* \param         void
* \return        canOK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nSetFilter(BOOL bWrite)
{
	canStatus nStatus = canOK;
	canHandle objHandle;
        
    // Set the client filter
	for ( UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        // Create DWORD Filter
        DWORD dwCode = 0, dwMask = 0;
        // To set no. shifts
        int nShift = sizeof( UCHAR ) * defBITS_IN_BYTE;
        // Get the Filter

		if ( sg_aodChannels[unIndex].m_hnd < 0) 
		{
			nStatus = canERR_NOTINITIALIZED;
			break;
		}
		for ( UINT i = 0 ; i < CAN_MSG_IDS ; i++ )
		{			
			const SACC_FILTER_INFO& sFilter = sg_aodChannels[ unIndex ].m_sFilter[i];
	        
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

			//Set handle based on the variable 'bWrite'
			if ( bWrite )
			{
				//Write handle
				objHandle = sg_aodChannels[unIndex].m_hnd;
			}
			else
			{
				//Read handle
				objHandle = sg_arrReadHandles[unIndex];
			}

			// Set the client filter
			nStatus = canSetAcceptanceFilter(objHandle, dwCode, dwMask, sFilter.m_ucACC_Filter_Type);
		}
		if( nStatus != canOK )
		{
			vRetrieveAndLog(nStatus, __FILE__, __LINE__);	           
			// Stop the loop as there is an error
			unIndex = sg_nNoOfChannels;
		}

    }

    return nStatus;
}

/**
* \brief         Function to apply filters and baud rate to channels
* \param         void
* \return        defERR_OK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nSetApplyConfiguration()
{
    int nReturn = defERR_OK;

    if( nReturn == defERR_OK )
    {
        // Set Hardware Mode
        //nReturn = nSetHardwareMode ( sg_ucControllerMode );
    }
    // Set baud rate only for hardware network
    if( nReturn == defERR_OK &&
        sg_ucControllerMode != defUSB_MODE_SIMULATE )
    {
        // Set Baud Rate
        nReturn = nSetBaudRate ();
    }

    if( nReturn == defERR_OK )
    {
        // Set Filter
        nReturn = nSetFilter (TRUE);
    }
	// Set Self Reception option 
	if ( nReturn == defERR_OK )
	{
		for ( UINT i = 0; i < sg_nNoOfChannels; i++ )
		{
			int txAck; 
			if ( sg_ControllerDetails[i].m_bSelfReception )
				txAck = 1; // Turn on txAcks			
			else
				txAck = 0; // Turn off txAcks			

			canIoCtl(sg_aodChannels[i].m_hnd, canIOCTL_SET_TXACK, &txAck, 4);
		}
	}

    return nReturn;
}

/**
* \brief         Sets the controller configuration data supplied by ConfigFile.
* \param[in]     ConfigFile, is SCONTROLER_DETAILS structure
* \param[in]     Length , is INT
* \return        S_OK for success
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_SetConfigData(PCHAR ConfigFile, INT Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;     

	/* Fill the hardware description details */
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
	{		
		strcpy_s(((PSCONTROLER_DETAILS)ConfigFile)[nCount].m_omHardwareDesc, 
				sg_aodChannels[nCount].m_strName);		
	}

    memcpy((void*)sg_ControllerDetails, (void*)ConfigFile, Length);

    return S_OK;
}

/**
* \brief         Function to store the Tx message entry into sg_asAckMapBuf list.
* \param[in]     RefObj, is SACK_MAP entry
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
	EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);	
	LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

/**
* \brief         Function to retreive the Tx message entry from sg_asAckMapBuf list and delete.
*                Also returns the respective client ID.
* \param[in]     RefObj, is SACK_MAP entry to find and delete
* \param[out]    ClientID, is client ID of the RefObj found
* \return        TRUE if RefObj is found, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{   
	EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator  iResult = 
        std::find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );  			
    //if ((*iResult).m_ClientID > 0)
	if (iResult != sg_asAckMapBuf.end())
    {
        bResult = TRUE;
        ClientID = (*iResult).m_ClientID;
        sg_asAckMapBuf.erase(iResult);
    }
	LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}

/**
* \brief         Writes the message 'sCanData' to the corresponding clients buffer
* \param[out]    sCanData, is STCANDATA structure
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    //Write into the client's buffer and Increment message Count    
    static SACK_MAP sAckMap;
    UINT ClientId = 0;
    static UINT Index = (UINT)-1;
    sAckMap.m_Channel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
    sAckMap.m_MsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;

	//Check if it is an acknowledgement message
    if (bRemoveMapEntry(sAckMap, ClientId))
    {
        BOOL bClientExists = bGetClientObj(ClientId, Index);
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            //Tx for monitor nodes and sender node
            if ((i == CAN_MONITOR_NODE_INDEX)  || (bClientExists && (i == Index)))
            {
				sCanData.m_ucDataType = TX_FLAG;
                for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
                {
                    sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sCanData);
                }
            }
            else
            {
                //Send the other nodes as Rx.
                for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
                {
                    static STCANDATA sTempCanData;
                    sTempCanData = sCanData;
                    sTempCanData.m_ucDataType = RX_FLAG;
                    sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sTempCanData);
                }
            }
        }
    }
    else // provide it to everybody
    {
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
            {
                sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sCanData);
            }
        }
    }
}

/**
* \brief         This will convert the error code from Kvaser driver format
*                to the format that is used by BUSMASTER.
* \param[in]     lError Error code in Peak USB driver format
* \param[in]     byDir  Error direction Tx/Rx
* \return        UCHAR which indicates error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static UCHAR USB_ucGetErrorCode(LONG lError, BYTE byDir)
{
    UCHAR ucReturn = 0;

    // Tx Errors
    if( byDir == 1)
    {
        if (lError & canMSG_ERROR_FRAME)
        {
            ucReturn = BIT_ERROR_TX;
        }
        if (lError & canMSG_NERR )
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
        if (lError & canMSG_ERROR_FRAME)
        {
            ucReturn = BIT_ERROR_RX;
        }
        if (lError & canMSG_NERR)
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
* \brief         Function to create time mode mapping
* \param[in]     hDataEvent, is HANDLE
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{   
	WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);	
}

/**
* \brief         Processing of the received packets from bus
* \param[in]     nChannelIndex, is the channel ID
* \param[in]     nFlags, is the message flags
* \param[in]     dwTime, is the message time stamp
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static void ProcessCANMsg(int nChannelIndex, UINT& nFlags, DWORD& dwTime)
{    
    static LONGLONG QuadPartRef = 0;
    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {				
		QuadPartRef = (LONGLONG)dwTime *10;
		sg_byCurrState = CALC_TIMESTAMP_READY;
        
        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);	
        UINT64 unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;
		//Time difference should be +ve value
		if((dwTime * 10) >= unConnectionTime) 
	        sg_TimeStamp  = (LONGLONG)((dwTime * 10) - unConnectionTime);
		else
	        sg_TimeStamp  = (LONGLONG)(unConnectionTime - (dwTime * 10));

    }

	sg_asCANMsg.m_lTickCount.QuadPart = (LONGLONG)(dwTime * 10);
	/*sg_asCANMsg.m_lTickCount.QuadPart =
                           _abs64(sg_asCANMsg.m_lTickCount.QuadPart - QuadPartRef);*/

	if ( !(nFlags & canMSG_ERROR_FRAME) &&
		 !(nFlags & canMSG_NERR) &&
		 !(nFlags & canMSGERR_SW_OVERRUN) &&
		 !(nFlags & canMSGERR_HW_OVERRUN) &&
		 !(nFlags & canMSGERR_STUFF) &&
		 !(nFlags & canMSGERR_FORM) &&
		 !(nFlags & canMSGERR_CRC) &&
		 !(nFlags & canMSGERR_BIT0) &&
		 !(nFlags & canMSGERR_BIT1) &&
		 !(nFlags & canMSGERR_OVERRUN) &&
		 !(nFlags & canMSGERR_BIT) &&
		 !(nFlags & canMSGERR_BUSERR) )	
    {
        /* Check for RTR Message */
		if (nFlags & canMSG_RTR)
        {
            sg_asCANMsg.m_ucDataType = RX_FLAG;
            sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = TRUE;
        }
        else
        {
            sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = FALSE;
        }

		/* Check for Tx/Rx */
        if (nFlags & canMSG_TXACK)
        {
            sg_asCANMsg.m_ucDataType = TX_FLAG;
        }
        else
        {
            sg_asCANMsg.m_ucDataType = RX_FLAG;
        }
        /* Check for extended message indication */
        sg_ReadMsg.m_ucEXTENDED =
            (nFlags & canMSG_EXT) ? TRUE : FALSE;

		/* Set channel ID */
		sg_ReadMsg.m_ucChannel = (UCHAR)nChannelIndex + 1;


        /* Copy STCAN_MSG */
		sg_asCANMsg.m_uDataInfo.m_sCANMsg = sg_ReadMsg; 

	}
	else
	{
        sg_asCANMsg.m_ucDataType = ERR_FLAG;
        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)nChannelIndex + 1;
		sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucChannel =  (UCHAR)nChannelIndex + 1;

        // Assign error type in the Error Capture register
        // and the direction of the error
        BOOL bIsTxMsg = FALSE;
        if (nFlags & canMSG_TXACK)
        {
            bIsTxMsg = TRUE;
        }
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap = USB_ucGetErrorCode(nFlags, (BYTE) bIsTxMsg);
        //explaination of error bit
        sg_asCANMsg.m_uDataInfo.m_sErrInfo.m_nSubError= 0;
	}



	//Write into client buffer.
	vWriteIntoClientsBuffer(sg_asCANMsg);        
}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        0
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
DWORD WINAPI CanMsgReadThreadProc_CAN_Kvaser_CAN(LPVOID pVoid)
{
    USES_CONVERSION;
	canStatus nStatus = canOK;	

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;

    /* Validate certain required pointers */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam, (DWORD)-1);
    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
	

	//get CAN - eventHandles
	for (UINT i = 0; i < sg_nNoOfChannels; i++)
	{
		sg_arrReadHandles[i] = canOpenChannel(sg_aodChannels[i].m_nChannel, canOPEN_ACCEPT_VIRTUAL);
		nStatus = canBusOn(sg_arrReadHandles[i]);

		HANDLE tmp;
		nStatus = canIoCtl(sg_arrReadHandles[i],
							canIOCTL_GET_EVENTHANDLE,
							&tmp,
							sizeof(tmp)); 
		if ( nStatus == canOK )
			g_hDataEvent[i] = tmp;
	}
	//Apply filters for read handles
	nSetFilter(false);

    if (g_hDataEvent[0] != NULL)
    {
        pThreadParam->m_hActionEvent = g_hDataEvent[0];
    }

	bool bLoopON = true;
	int moreDataExist;				
	static UINT unFlags = 0;
	static DWORD dwTime = 0;
	unsigned char   ucData[8];
	
	//New approach{{
	while (bLoopON)
    {
        WaitForMultipleObjects(sg_nNoOfChannels, g_hDataEvent, FALSE, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
				do 
				{
					moreDataExist = 0;
					for (UINT i = 0; i < sg_nNoOfChannels ; i++) 
					{
						//Read CAN Message from channel
						nStatus = canRead(sg_arrReadHandles[i], (long*)&sg_ReadMsg.m_unMsgID, 
										&ucData[0], (unsigned int*)&sg_ReadMsg.m_ucDataLen, 
										&unFlags, &dwTime);
						switch (nStatus) 
						{
							case canOK:
								memcpy(sg_ReadMsg.m_ucData, ucData, (unsigned int)sg_ReadMsg.m_ucDataLen);
								ProcessCANMsg(i, unFlags, dwTime);
								moreDataExist = 1;
							break;

							case canERR_NOMSG:
								// No more data on this handle
							break;

							default:								
							break;
						}
					}   
				} while (moreDataExist);
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
				SetEvent(pThreadParam->m_hActionEvent);
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
	for ( UINT i =0 ; i < sg_nNoOfChannels; i++ )
	{
		//Get Off the bus
		nStatus = canBusOff(sg_arrReadHandles[i]);	
		//Close the channel connection
		nStatus = canClose(sg_arrReadHandles[i]);
		sg_arrReadHandles[i] = canERR_NOTINITIALIZED;
	}					
	for (UINT i = 0; i < sg_nNoOfChannels+1; i++)
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;

	//Connect to the channels
    hResult = nConnect(TRUE, NULL);
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
    if (hResult == S_OK)
    {
		if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_Kvaser_CAN))
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

/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

	//Terminate the read thread
	sg_sParmRThread.bTerminateThread();	

    hResult = nConnect(FALSE, NULL);
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_ResetHardware(void)
{
	HRESULT hResult = S_OK;

    /* Stop the hardware if connected */
    CAN_StopHardware(); // return value not necessary

    return hResult;
    
}

/**
* \brief         This will send a CAN message to the driver.
* \param[in]     sMessage Message to Transmit
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nWriteMessage(STCAN_MSG sMessage, DWORD /*dwClientID*/)
{
    int nReturn = -1;	
	UINT unClientIndex = (UINT)-1;

    /* Return when in disconnected state */
    //if (!sg_bIsConnected) return nReturn;

    if ((sMessage.m_ucChannel > 0) &&
            (sMessage.m_ucChannel <= sg_nNoOfChannels))
    {        
		canStatus    nStatus;
		unsigned int   nUsedFlags = 0;		
		
		/* if it is an extended frame */
        if (sMessage.m_ucEXTENDED == 1)
        {            
			 nUsedFlags^= canMSG_EXT; // toggle ext/std			
        }				
		/* in case of remote frame */
        if (sMessage.m_ucRTR == 1)
        {            
			nUsedFlags^= canMSG_RTR; 			
        }

		//Transmit message		
		nStatus = canWrite(sg_aodChannels[sMessage.m_ucChannel-1].m_hnd, sMessage.m_unMsgID,
			sMessage.m_ucData, (unsigned short)sMessage.m_ucDataLen, nUsedFlags);

		//set result		        
		nReturn = nStatus;        
    }

    return nReturn;
}

/**
* \brief         Sends STCAN_MSG structure from the client dwClientID.
* \param[in]     dwClientID is the client ID
* \param[in]     sMessage is the application specific CAN message structure
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    static SACK_MAP sAckMap;
    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sCanTxMsg.m_ucChannel <= sg_nNoOfChannels)
        {
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_MsgID    = sCanTxMsg.m_unMsgID;
            sAckMap.m_Channel  = sCanTxMsg.m_ucChannel;

            /* Mark an entry in Map. This is helpful to identify
               which client has been sent this message in later stage */
            vMarkEntryIntoMap(sAckMap);			
            if (nWriteMessage(sCanTxMsg, dwClientID) == defERR_OK)
            {
                hResult = S_OK;
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

    return hResult;    
}

/**
 * \brief         Gets last occured error and puts inside acErrorStr.
 * \param[out]    acErrorStr contains error string
 * \return        S_OK for success, S_FALSE for failure
 */
HRESULT CDIL_CAN_Kvaser::CAN_GetLastErrorString(string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}

/**
* \brief         This function will check all hardware connectivity by switching to channel ON.
* \param[out]    ucaTestResult Array that will hold test result. 
				 TRUE if hardware present and false if not connected
* \param[in]     nChannel, indicates channel ID
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) //const
{    
    int nReturn = 0;    
	canStatus nStatus;
    if (nChannel < sg_nNoOfChannels)
    {
		nStatus = canBusOn(sg_aodChannels[nChannel].m_hnd);		
		if ( nStatus < 0 ) 
        {            
            sg_bIsConnected = FALSE;
            ucaTestResult = FALSE;
        }
        else
        {
			ucaTestResult = TRUE;
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
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

/**
* \brief         Loads vendor's driver library
* \param		 void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_LoadDriverLibrary(void)
{
    return S_OK;
}

/**
* \brief         Unloads verdor's driver lobrary
* \param		 void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
HRESULT CDIL_CAN_Kvaser::CAN_UnloadDriverLibrary(void)
{
    return S_OK;
}

/* Helper Function definitions */

/**
* \brief         This will close the connection with the driver. This will be
*				 called before deleting HI layer. This will be called during
*			     application close.
* \param         void
* \return        Operation Result. 0 incase of no errors. Failure Error codes(-1) otherwise.
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nDisconnectFromDriver()
{
    int nReturn = 0;
	canStatus nStatus;

	for ( UINT i = 0; i< sg_nNoOfChannels; i++ )
	{
		int txAckOff = 0; // Turn off txAcks
		nStatus = canIoCtl(sg_aodChannels[i].m_hnd, canIOCTL_SET_TXACK, &txAckOff, 4);

		//switch off the channel
		nStatus = canBusOff(sg_aodChannels[i].m_hnd);
		sg_aodChannels[i].m_nIsOnBus = 0;
		//Close the channel connection
		nStatus = canClose(sg_aodChannels[i].m_hnd);
		if (nStatus < 0) 
		{
			nReturn = -1;
		}
	}
	sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return nReturn;
}

/**
* \brief         This function will connect the tool with hardware. This will
*				 establish the data link between the application and hardware.
* \param[in]     bConnect TRUE to Connect, FALSE to Disconnect
* \return        Returns defERR_OK if successful otherwise corresponding Error code.
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nConnect(BOOL bConnect, BYTE /*hClient*/)
{
    int nReturn = -1;
	canStatus nStatus;
	canHandle  hnd;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
		for( UINT i = 0; i < sg_nNoOfChannels; i++)
		{			
			//open CAN channel
			hnd = canOpenChannel(sg_aodChannels[i].m_nChannel, canOPEN_ACCEPT_VIRTUAL);

			if (hnd >= 0) 
			{		
				sg_aodChannels[i].m_hnd = hnd;			  
				canIoCtl(sg_aodChannels[i].m_hnd, canIOCTL_FLUSH_TX_BUFFER, NULL, NULL);
				//go on bus (every channel)
				nStatus = canBusOn(sg_aodChannels[i].m_hnd);

				if (nStatus < 0) 
				{		
					break;
				} 
				else 
				{
					sg_aodChannels[i].m_nIsOnBus = 1;
				}
			}
			else
			{
				break;
			}
		}
		if(nStatus == canOK) 
		{
			/* Transit into 'CREATE TIME MAP' state */
			sg_byCurrState = CREATE_MAP_TIMESTAMP;							
			sg_bIsConnected = bConnect;						
			nReturn = defERR_OK;		
            // Update configuration to restore the settings
			bLoadDataFromContr(sg_ControllerDetails);
            nSetApplyConfiguration();
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
		InitializeCriticalSection(&sg_CritSectForAckBuf);

        QueryPerformanceCounter(&sg_QueryTickCount);	
	    // Get frequency of the performance counter
        QueryPerformanceFrequency(&sg_lnFrequency);
        // Convert it to time stamp with the granularity of hundreds of microsecond
        if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
        {
            sg_TimeStamp = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
        }
        else
        {
            sg_TimeStamp = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
        }	
	}
	else
	{
		DeleteCriticalSection(&sg_CritSectForAckBuf);
	}

    return nReturn;
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.
* \param[in]	 psInterfaces, is INTERFACE_HW structue
* \param[out]	 pnSelList, contains channels selected array
* \param[out]	 nCount, contains selected channel count
* \return        returns 0 (always)
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
*				 and will create essential networks.
* \param		 void
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nCreateMultipleHardwareNetwork()
{
	int nHwCount = sg_ucNoOfHardware;	
	TCHAR acVendor[MAX_CHAR_LONG];
	DWORD dwFirmWare[2];

    // Get Hardware Network Map
	for (int nCount = 0; nCount < nHwCount; nCount++)
	{
		sg_HardwareIntr[nCount].m_dwIdInterface = nCount;		

	    canGetChannelData(nCount, canCHANNELDATA_CARD_SERIAL_NO,
							 acVendor, sizeof(acVendor));
		sscanf_s( acVendor, "%ld", &sg_HardwareIntr[nCount].m_dwVendor );		

	    canGetChannelData(nCount, canCHANNELDATA_CHANNEL_NAME,
                  sg_HardwareIntr[nCount].m_acDescription,
                  sizeof(sg_HardwareIntr[nCount].m_acDescription));	

		//Get Firmware info
		canGetChannelData(nCount, canCHANNELDATA_CARD_FIRMWARE_REV, dwFirmWare, sizeof(dwFirmWare));

		sprintf_s(sg_HardwareIntr[nCount].m_acDeviceName,"0x%08lx 0x%08lx", dwFirmWare[0], dwFirmWare[1]);
	}	
	ListHardwareInterfaces(sg_hOwnerWnd, DRIVER_CAN_KVASER_CAN, sg_HardwareIntr, sg_anSelectedItems, nHwCount);

    sg_ucNoOfHardware = (UCHAR)nHwCount;
	sg_nNoOfChannels = (UINT)nHwCount;
	
	//Reorder hardware interface as per the user selection
	for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
	{		
		sg_aodChannels[nCount].m_nChannel = sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface;				
		sprintf_s(sg_aodChannels[nCount].m_strName , _T("Kvaser - %s, Serial Number- %ld, Firmware- %s"),
									sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDescription,
									sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwVendor,
									sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDeviceName);		
	}

	return defERR_OK;
}

/**
* \brief         This function will create a single network with available single hardware.
* \param		 void
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nCreateSingleHardwareNetwork()
{    
    /* Set the number of channels as 1 */
	sg_ucNoOfHardware = (UCHAR)1;
	sg_nNoOfChannels = 1;		        

	sg_aodChannels[0].m_nChannel = 0;

	/* Update channel info */

	TCHAR acVendor[MAX_CHAR_LONG];
	DWORD dwFirmWare[2];

	canGetChannelData(0, canCHANNELDATA_CARD_SERIAL_NO,
						 acVendor, sizeof(acVendor));
	sscanf_s( acVendor, "%ld", &sg_HardwareIntr[0].m_dwVendor );		

    canGetChannelData(0, canCHANNELDATA_CHANNEL_NAME,
              sg_HardwareIntr[0].m_acDescription,
              sizeof(sg_HardwareIntr[0].m_acDescription));	

	/* Get Firmware info */
	canGetChannelData(0, canCHANNELDATA_CARD_FIRMWARE_REV, dwFirmWare, sizeof(dwFirmWare));

	sprintf_s(sg_HardwareIntr[0].m_acDeviceName,"0x%08lx 0x%08lx", dwFirmWare[0], dwFirmWare[1]);

	sprintf_s(sg_aodChannels[0].m_strName, _T("%s, Serial Number: %ld, Firmware: %s"),
								sg_HardwareIntr[0].m_acDescription,
								sg_HardwareIntr[0].m_dwVendor,
								sg_HardwareIntr[0].m_acDeviceName);	
    
    return defERR_OK;
}

/**
* \brief         Finds the number of hardware connected.
* \param		 void
* \return        defERR_OK if successful otherwise corresponding Error code.
*			     0, Query successful, but no device found
*				 > 0, Number of devices found
*				 < 0, query for devices unsuccessful
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nGetNoOfConnectedHardware(void)
{
    int nChannelCount = 0;
	canStatus nStatus;

	// ------------------------------------
	// get number of present channels
	// ------------------------------------
	nStatus = canGetNumberOfChannels((int*)&nChannelCount);

	if (nStatus != canOK ) 
	{
        strcpy_s(sg_omErrStr, _T("Problem Finding Device!"));
        nChannelCount = -1;
	}	
    /* Return the channel count */
    return nChannelCount;
}

/**
* \brief         This function will find number of hardwares connected.
*				 It will create network as per hardware count.
*				 This will popup hardware selection dialog in case there are more hardwares present.
* \param		 void
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
    sprintf_s(acNo_Of_Hw, _T("Number of Kvaser hardwares Available: %d"), nChannelCount);

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
	sprintf_s(sg_omErrStr, _T("No Kvaser hardwares Available.\nPlease check if Kvaser drivers are installed."));
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
* \brief         This function will remove the existing client ID
* \param[in]	 dwClientId, client ID to be removed
* \return        Returns TRUE if client ID removal is success, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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

/**
* \brief         This function will check if the client ID exists
* \param[in]	 dwClientId, client ID to be checked for existance
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
* \param[in]	 dwClientId, client ID whose client index is needed
* \param[out]	 unClientIndex, client index to be returned
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
* \param[in]	 File, pointer to log file
* \param[in]	 Line, indicates line number in log file
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
    strncpy_s(sg_acErrStr, acErrText, nStrLen);
}

/**
* \brief         Function to check if client buffer exists
* \param[in]	 sClientObj, alias to SCLIENTBUFMAP object
* \param[in]	 pBuf, pointer to CBaseCANBufFSE object
* \return        TRUE if buffer exists, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
* \param[in]	 RootBufferArray, pointer to CBaseCANBufFSE class array
* \param[out]	 unCount, indicates buffer count which will get reduced
* \param[in]	 BufferToRemove, pointer to the buffer to be removed
* \return        TRUE if removed
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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

/**
* \brief         This function will check if the client exists and gives back the client index.
* \param[in]     pcClientName, client name as TCHAR*
* \param[out]    Index, client index if found
* \return        TRUE if client name is found, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
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

/**
* \brief         This function will get available client slot
* \param	     void
* \return        Returns the available client ID
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
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
