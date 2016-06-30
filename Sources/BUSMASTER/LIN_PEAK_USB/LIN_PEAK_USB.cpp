/******************************************************************************
  Project       :  BUSMASTER
  FileName      :  LIN_PEAK_USB.cpp
  Description   :  Source file for Peak USB DIL functions
  Author(s)     :  Saravanan
  Date Created  :  10.05.2014
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "LIN_PEAK_USB_stdafx.h"
#include "LIN_PEAK_USB.h"
#include "Error.h"
//#include "Include/basedefs.h"

//#include "DataTypes/MsgBufAll_DataTypes.h"
//#include "DataTypes/DIL_Datatypes.h"
#include "EXTERNAL/PLinApi.h"
#include "LIN_PEAK_USB_Channel.h"
#include "LIN_PEAK_USB_Network.h"
#include "Utility/Utility_Thread.h"
//#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface\CommanDIL_LIN.h"
#include "DIL_Interface\HardwareListing.h"
//#include "ChangeRegisters.h"
#include <LINDriverDefines.h>
// CLIN_PEAK_USB

BEGIN_MESSAGE_MAP(CLIN_PEAK_USB, CWinApp)
END_MESSAGE_MAP()

const int LIN_VER_1_3 = 13;
const int LIN_VER_2_0 = 20;
const int LIN_VER_2_1 = 21;

/**
 * CLIN_PEAK_USB construction
 */
CLIN_PEAK_USB::CLIN_PEAK_USB()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CLIN_PEAK_USB object
CLIN_PEAK_USB theApp;

/**
 * CLIN_PEAK_USB initialization
 */
BOOL CLIN_PEAK_USB::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

// First define the callback function type
typedef BOOL (*DILCALLBACK)(BYTE Argument, PSCONTROLLER_DETAILS pDatStream, int Length);

/* Headers files required for std::list operations*/
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#define USAGE_EXPORT
#include "LIN_PEAK_USB_Extern.h"

#define MAX_BUFF_ALLOWED 16
#define MONITOR_NODE_NAME _("LIN_MONITOR") // Kss to check
#define MAX_LINHW 8
#define MAX_HLINNET 16

#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3
#define defUSB_PARAM_BUFFER_SIZE               1024

typedef tagClientBufMap sClientBufMap;
typedef sClientBufMap SCLIENTBUFMAP;
typedef sClientBufMap* PSCLIENTBUFMAP;

static DWORD  sg_dwClientID = 0;

typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;

/* Harware Network List structure */
struct tagHwNetList
{
    HLINHW m_hHw;
    HWND m_hNet; // kss to remove

    tagHwNetList ()
    {
        m_hHw = 0;
        m_hNet = 0;
    }
};

typedef tagHwNetList sHwNetList;
typedef sHwNetList SHWNETLIST;
typedef SHWNETLIST* PSHWNETLIST;

const BYTE CREATE_MAP_TIMESTAMP = 0x1;
const BYTE CALC_TIMESTAMP_READY = 0x2;
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

static SYSTEMTIME sg_CurrSysTime;
static UINT64 sg_TimeStampRef = 0;
static LARGE_INTEGER sg_QueryTickCount;
static HWND sg_hOwnerWnd = NULL;

static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe


static CPARAM_THREADPROC sg_sParmRThread;
static STATUSMSG sg_sCurrStatus;
static CNetwork* sg_podActiveNetwork;
static CNetwork sg_odSimulationNetwork;
static HLINHW  sg_hHardware[ 16 ]; // kss to check -- assuming 16 slave nodes maximum
static INTERFACE_HW sg_IntrHw[defNO_OF_CHANNELS];
SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];

// Network for actual hardware
static CNetwork sg_odHardwareNetwork;
// Create time struct. Use 0 to transmit the message with out any delay
static unsigned __int64 sg_sTime;
// static global variables ends

const int SIZE_WORD     = sizeof(WORD);
const int SIZE_CHAR     = sizeof(TCHAR);

// TZM specific Global variables
#define LIN_MAX_ERRSTR 256
#define MAX_CLIENT_ALLOWED 16
string sg_acErrStr = "";
static UINT sg_unClientCnt = 0;
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT sg_unCntrlrInitialised = 0;
static HMODULE sg_hLinUsbLib = NULL;
static HANDLE sg_hHandle = NULL;
static HANDLE sg_hCntrlStateChangeEvent = NULL;

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

// Current buffer size
HLINCLIENT sg_hClient = 0; //Global client used for sending and reading

// State machine
enum
{
    STATE_DRIVER_SELECTED    = 0x0,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;
// State variables
static BOOL sg_bIsConnected = FALSE;
static BOOL sg_bIsDriverRunning = FALSE;
static UCHAR sg_ucControllerMode = defUSB_MODE_ACTIVE;

// Count variables
static UCHAR sg_ucNoOfHardware = 0;

/* Error Definitions */
#define LIN_USB_OK 0
#define LIN_QRCV_EMPTY 0x20
#define defPROC1Err 1
#define defPROC2Err 2
#define defPROC3Err 4
#define defPROC4Err 8
#define defPROC5Err 16
#define defPROC6Err 32
#define defPROC7Err 64
#define defPROC8Err 128
#define defPROC9Err 256
#define defPROC10Err 512
#define defPROC11Err 1024
#define defPROC12Err 2048
#define defPROC13Err 4096
#define defPROC14Err 8192
#define defPROC15Err (8192 * 2)
#define defPROC16Err (8192 * 4)
#define defPROC17Err (8192 * 8)
#define defPROC18Err (8192 * 16)
#define defLoadDriverErr (defPROC18Err * 2)

#define MAX_CHAR_SHORT 128
#define MAX_CHAR_LONG  512
#define LIN_USBMSGTYPE_DATA 2 // kss to check
/* Function Pointers for API */
//typedef DWORD (_stdcall *PAPIPROC1)(TLINHardwareParam);// LIN_SetDeviceName // kss to check -- use sethardwareparam with device name as parameter
typedef DWORD (_stdcall* PAPIPROC1)(HLINHW*, WORD, int*);                           // LIN_GetAvailableHardware
typedef DWORD (_stdcall* PAPIPROC2)(LPCSTR, DWORD, HLINCLIENT*);                    // LIN_RegisterClient
typedef DWORD (_stdcall* PAPIPROC3)(HLINCLIENT, HLINHW);                            // LIN_ResetHardware
typedef DWORD (_stdcall* PAPIPROC4)(HLINCLIENT);                                    // LIN_ResetClient
typedef DWORD (_stdcall* PAPIPROC5)(HLINCLIENT ,HLINHW, TLINMsg*);                  // LIN_Write
typedef DWORD (_stdcall* PAPIPROC6)(HLINCLIENT, TLINRcvMsg*);                       // LIN_Read
typedef DWORD (_stdcall* PAPIPROC7)(HLINCLIENT, TLINRcvMsg*, int, int*);            // LIN_Read_Multi
typedef DWORD (_stdcall* PAPIPROC8)(HLINCLIENT, HLINHW);                            // LIN_ConnectClient
typedef DWORD (_stdcall* PAPIPROC9)(HLINCLIENT, HLINHW, unsigned __int64);          // LIN_SetClientFilter
typedef DWORD (_stdcall* PAPIPROC10)(HLINCLIENT, HLINHW);                           // LIN_DisconnectClient
typedef DWORD (_stdcall* PAPIPROC11)(HLINCLIENT);                                   // LIN_RemoveClient
typedef DWORD (_stdcall* PAPIPROC12)(HLINCLIENT, HLINHW, TLINHardwareParam, void*, WORD);   // LIN_SetHardwareParam
typedef DWORD (_stdcall* PAPIPROC13)(HLINHW, TLINHardwareParam, void*, WORD);               // LIN_GetHardwareParam
//typedef DWORD (_stdcall *PAPIPROC14)(HCANNET, WORD, DWORD);//CAN_SetNetParam // kss to check
//typedef DWORD (_stdcall *PAPIPROC15)(HCANNET, WORD, void*, WORD);//CAN_GetNetParam // kss to check
typedef DWORD (_stdcall* PAPIPROC16)(HLINCLIENT, TLINClientParam, DWORD);           // LIN_SetClientParam
typedef DWORD (_stdcall* PAPIPROC17)(LPSTR, WORD);                                  // LIN_GetVersionInfo
typedef DWORD (_stdcall* PAPIPROC18)(TLINError, BYTE, LPSTR, WORD);                 // LIN_GetErrorText
//typedef DWORD (_stdcall *PAPIPROC19)(LPCSTR, DWORD hWnd, HLINCLIENT*);            // CAN_RegisterClient // kss to check. already added above instead of regiaterhardware
typedef DWORD (_stdcall* PAPIPROC20)(HLINCLIENT, HLINHW, TLINHardwareMode, WORD);   // LIN_InitializeHardware
typedef DWORD (_stdcall* PAPIPROC21)(HLINCLIENT, HLINHW, BYTE, BYTE);               // LIN_RegisterFrameId
typedef DWORD (_stdcall* PAPIPROC22)(HLINCLIENT, HLINHW, TLINFrameEntry*);          // LIN_SetFrameEntry
typedef DWORD (_stdcall* PAPIPROC23)(HLINCLIENT, HLINHW, BYTE, BYTE, BYTE, BYTE*);  // LIN_UpdateByteArray
typedef DWORD (_stdcall* PAPIPROC24)(TLINMsg*);                                     // LIN_CalculateChecksum
typedef DWORD (_stdcall* PAPIPROC25)(HLINCLIENT , HLINHW );
typedef DWORD (_stdcall* PAPIPROC26)(BYTE*);

static PAPIPROC1 pfLIN_GetAvailableHw = NULL;
static PAPIPROC2 pfLIN_RegisterNet = NULL; // kss to change the pfLIN_RegisterNet to pfLIN_RegisterClient as the former is removed
static PAPIPROC3 pfLIN_ResetHardware = NULL;
static PAPIPROC4 pfLIN_ResetClient = NULL;
static PAPIPROC5 pfLIN_Write = NULL;
static PAPIPROC6 pfLIN_Read = NULL;
static PAPIPROC7 pfLIN_ReadMulti = NULL;
static PAPIPROC8 pfLIN_ConnectToNet = NULL;
static PAPIPROC9 pfLIN_SetClientFilter = NULL;
static PAPIPROC10 pfLIN_DisconnectFromNet = NULL;
static PAPIPROC11 pfLIN_RemoveClient = NULL;
static PAPIPROC12 pfLIN_SetHwParam = NULL;
static PAPIPROC13 pfLIN_GetHwParam = NULL;
//static PAPIPROC14 pfCAN_SetNetParam = NULL; // kss to check
//static PAPIPROC15 pfCAN_GetNetParam = NULL; // kss to check
static PAPIPROC16 pfLIN_SetClientParam = NULL;
static PAPIPROC17 pfLIN_VersionInfo = NULL;
static PAPIPROC18 pfLIN_GetErrText = NULL;
//static PAPIPROC19 pfCAN_RegisterClient = NULL; // kss to check. this is a replica of PAPIPROC2
static PAPIPROC20 pfLIN_InitializeHardware = NULL;
static PAPIPROC21 pfLIN_RegisterFrameId = NULL;
static PAPIPROC22 pfLIN_SetFrameEntry = NULL;
static PAPIPROC23 pfLIN_UpdateByteArray = NULL;
static PAPIPROC24 pfLIN_CalcChecksum = NULL;
static PAPIPROC25 pfLIN_ResetHardwareConfig = NULL;
static PAPIPROC26 pfLIN_GetPID = NULL;
/* CDIL_LIN_PeakUSB class definition */
class CDIL_LIN_PeakUSB : public CCommanDIL_LIN
{
public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT LIN_PerformInitOperations(void);
    HRESULT PerformClosureOperations(void);
    HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT LIN_DeselectHwInterface(void);

    HRESULT SetConfigData(IBMNetWorkGetService* ouConfig);
    HRESULT StartHardware(void);
    HRESULT PreStartHardware(void);
    HRESULT StopHardware(void);
    HRESULT LIN_ResetHardware(void);
    HRESULT LIN_GetCurrStatus(STATUSMSG& StatusData);
    HRESULT LIN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT LIN_Send(STLIN_MSG& sLinTxMsg);

    HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg)
    {
        return S_OK;
    };
    HRESULT LIN_ResetSlaveRespData(void)
    {
        return S_OK;
    };
    HRESULT LIN_DisableSlave(STLIN_MSG& sMessage);
    HRESULT LIN_GetBusConfigInfo(BYTE* BusInfo);
    //HRESULT LIN_GetVersionInfo(VERSIONINFO& sVerInfo); //kss to check
    HRESULT LIN_GetLastErrorString(string& acErrorStr);
    HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_GetConfiguration( SCONTROLLER_DETAILS_LIN[], INT& nSize );
    HRESULT LIN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
    {
        return S_OK;
    }
    HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT LIN_SetAppParams(HWND hWndOwner);
    //HRESULT LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj); //kss to check
    //HRESULT LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName); //kss to check
    HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT LIN_LoadDriverLibrary(void);
    HRESULT LIN_UnloadDriverLibrary(void);

    static DWORD WINAPI LINMsgReadThreadProc_LIN_Vector_XL(LPVOID pVoid);
    void ProcessLINMsg();
};

static CDIL_LIN_PeakUSB* sg_pouDIL_LIN_PeakUSB = NULL;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_LIN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == sg_pouDIL_LIN_PeakUSB )
    {
        if ((sg_pouDIL_LIN_PeakUSB = new CDIL_LIN_PeakUSB) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouDIL_LIN_PeakUSB;

    return hResult;
}

/* Function pointers ends*/

/* HELPER FUNCTIONS START */

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

static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            sg_asClientToBufMap[unClientIndex].dwClientID = 0;
            memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (TCHAR) * MAX_PATH);
            for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
            }
            sg_asClientToBufMap[unClientIndex].unBufCount = 0;
            //Shift the last element to this position
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

void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

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
        ClientID = (*iResult).m_unClientID;
        sg_asAckMapBuf.erase(iResult);
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}
/* Static Global common functions starts*/


/* Function to initialize all global data variables */

// Function create and set read indication event
static int SetReadIndicationEvent(HANDLE hReadEvent)
{
    int nReturn = -1;
    if( hReadEvent != NULL )
    {
        nReturn = (*pfLIN_SetClientParam)(sg_hClient, clpOnReceiveEventHandle,
                                          (DWORD) hReadEvent);
    }
    if( hReadEvent != NULL )
    {
        sg_hHandle = hReadEvent;
        nReturn = S_OK;
    }
    return nReturn;
}

/* Function to retreive error occurred and log it */
static void vRetrieveAndLog(DWORD dwErrorCode, char* File, int Line)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};
    // Get the error text for the corresponding error code
    if ((*pfLIN_GetErrText)(dwErrorCode, 1, acErrText, MAX_PATH) == LIN_USB_OK)
    {

        size_t nStrLen = strlen(acErrText);
        if (nStrLen > LIN_MAX_ERRSTR)
        {
            nStrLen = LIN_MAX_ERRSTR;
        }
        sg_acErrStr = acErrText;
    }
}
static int nSetHardwareMode(UCHAR ucDeviceMode)
{
    int nReturn = -1;
    return nReturn;
}

/* Function to create time mode mapping */
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{

    WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    //Query performance counter
    QueryPerformanceCounter(&sg_QueryTickCount);
}

static BOOL bLoadDataFromContr(IBMNetWorkGetService*  asDeviceConfig)
{
    BOOL bReturn = FALSE;
    string strLinVer = "";

    if ( asDeviceConfig != nullptr )
    {
        int nChannels = 0;
        ICluster* pCluster;
        ChannelSettings ouChannelSettings;
        asDeviceConfig->GetChannelCount(LIN, nChannels);
        for( int nIndex = 0; nIndex < nChannels; nIndex++ )
        {
            CChannel& odChannel = sg_odHardwareNetwork.m_aodChannels[ nIndex ];

            asDeviceConfig->GetDBService(LIN, nIndex, 0, &pCluster);

            asDeviceConfig->GetChannelSettings(LIN, nIndex, &ouChannelSettings);
            // Baudrate in BTR0BTR1 format
            odChannel.m_usBaudRate = static_cast <USHORT>(ouChannelSettings.m_ouLINSettings.m_nBaudRate);
            odChannel.m_unBaudrate = static_cast <UINT>(ouChannelSettings.m_ouLINSettings.m_nBaudRate);

            // Set LIN mode
            odChannel.m_unLINMode = modSlave;
            if (ouChannelSettings.m_ouLINSettings.m_bIsMasterMode)
            {
                odChannel.m_unLINMode = modMaster;
            }

            odChannel.m_unLINVersion = LIN_VER_2_0;
            strLinVer = ouChannelSettings.m_ouLINSettings.m_strProtocolVersion;
            if (strLinVer == "LIN 1.1" || strLinVer == "LIN 1.2"|| strLinVer == "LIN 1.3")
            {
                odChannel.m_unLINVersion = LIN_VER_1_3;
            }
        }

        bReturn = TRUE;
    }
    return bReturn;
}

static int nGetCalcChecksum(int nChannelIndex, int nId )
{
    int nChecksum = cstEnhanced;

    if ((nChannelIndex >= 0) && (nChannelIndex < sg_odHardwareNetwork.m_nNoOfChannels))
    {
        if ( sg_odHardwareNetwork.m_aodChannels[nChannelIndex ].m_unLINVersion <= LIN_VER_1_3 || ( nId >= 60 && nId <= 63 ) )
        {
            nChecksum = cstClassic;
        }
    }

    return nChecksum;
}

/* Processing of the received packets from bus */
void CDIL_LIN_PeakUSB::ProcessLINMsg()
{
    int nReturn = -1;

    // Array of lin Messages supported by USB driver
    TLINRcvMsg sMsgArray[ 125 ];
    TLINRcvMsg sLinMsg;
    int nActualRead = 124;
    // Read message from the driver
    nReturn = (*pfLIN_ReadMulti)( sg_hClient, sMsgArray, 124, &nActualRead);
    /*nReturn = (*pfLIN_Read)(sg_hClient, &sLinMsg);*/

    TLINMsg sMessage;
    // To extract ID from PID. Considering the first 2 bits as parity bits
    // and received as MSB, 0011 1111 is used as a mask to retrieve ID
    unsigned int unMask = 0x3F;

    for (int nIndex = 0; nIndex < nActualRead; nIndex++)
    {
        if (nReturn == errOK || nReturn == errRcvQueueEmpty)
        {
            sMessage.FrameId = sMsgArray[nIndex].FrameId;
            sMessage.FrameId &= unMask;
            sMessage.Length = sMsgArray[nIndex].Length;
            sMessage.Direction = sMsgArray[nIndex].Direction;
            sMessage.ChecksumType = sMsgArray[nIndex].ChecksumType;
            memcpy (sMessage.Data, sMsgArray[nIndex].Data, 8);
            sMessage.Checksum = sMsgArray[nIndex].Checksum;

            // Get the time stamping of the current message
            UINT64 sTimeValue = sMsgArray[nIndex].TimeStamp/100;
            STLINDATA sLinData;

            //if( sMsgArray[nIndex].Type == mstStandard )
            {
                // Convert the time value
                sLinData.m_uDataInfo.m_sLINMsg.m_ulTimeStamp = sMsgArray[nIndex].TimeStamp; // kss to check. if this is fine, then quadpart might not be required
                sLinData.m_lTickCount.QuadPart = ((LONGLONG)sMsgArray[nIndex].TimeStamp / 100);

                if (sg_byCurrState == CREATE_MAP_TIMESTAMP)
                {
                    //sg_TimeStampRef = sLinData.m_lTickCount.QuadPart; // kss to checkl.. directly assigned the time stamp value
                    sg_TimeStampRef = sTimeValue;
                    //query performance counter
                    sg_byCurrState  = CALC_TIMESTAMP_READY;
                }

                UINT unChannel = 1; // Default set as channel 1 as only one channel is allowed
                sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel = unChannel;
                sLinData.m_uDataInfo.m_sErrInfo.m_ucChannel = unChannel;
                sLinData.m_uDataInfo.m_sLINMsg.m_ulTimeStamp = sLinData.m_lTickCount.QuadPart;
                sLinData.m_uDataInfo.m_sErrInfo.m_ulTimeStamp = sLinData.m_lTickCount.QuadPart;

                // Find Rx/Tx Type
                if (sMessage.Direction == dirPublisher)
                {
                    sLinData.m_ucDataType = TX_FLAG;
                }
                else if (sMessage.Direction == dirSubscriber ||
                         sMessage.Direction == dirSubscriberAutoLength)
                {
                    sLinData.m_ucDataType = RX_FLAG;
                }

                // Check for proper message
                if( sMsgArray[nIndex].Type == mstStandard && sMsgArray[nIndex].ErrorFlags == 0)
                {
                    // Copy data bytes
                    memcpy( sLinData.m_uDataInfo.m_sLINMsg.m_ucData,
                            sMessage.Data, sMessage.Length );

                    // Copy DLC Value
                    sLinData.m_uDataInfo.m_sLINMsg.m_ucDataLen = sMessage.Length;

                    // Copy Message ID
                    sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID = sMessage.FrameId;

                    // Assign Channel Number
                    sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel = (UCHAR)unChannel;



                    // Update Checksum
                    sLinData.m_uDataInfo.m_sLINMsg.m_ucChksum = sMessage.Checksum;

                    // Update Checksum type
                    sLinData.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = 0; // classic
                    if (sMessage.ChecksumType == cstEnhanced)
                    {
                        sLinData.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = 1;
                    }

                    // Update LIN message type
                    sLinData.m_eLinMsgType = LIN_MSG;
                }
                else
                {
                    //Handle Error counter Update here
                    // Channel starts from 1 where array index starts from
                    // 0. So sub 1 from channel id
                    CChannel& odChannel = sg_odHardwareNetwork.m_aodChannels[ unChannel - 1];

                    // Check whether it is error message
                    //if( sMsgArray[nIndex].Type != mstStandard )
                    sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_SLEEP;
                    {
                        if (sMsgArray[nIndex].Type == mstBusSleep )
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_SET;
                            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_SLEEP;
                        }
                        else if (sMsgArray[nIndex].Type == mstBusWakeUp )
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_WAKEUP;
                            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_WAKEUP;
                        }

                        sLinData.m_eLinMsgType = LIN_EVENT;



                        // Assign the channel number
                        sLinData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)unChannel;
                        sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID = sLinData.m_uDataInfo.m_sErrInfo.m_ucId = sMsgArray[nIndex].FrameId & unMask;
                        sLinData.m_uDataInfo.m_sErrInfo.m_ucCrc = sMsgArray[nIndex].Checksum;
                        sLinData.m_uDataInfo.m_sErrInfo.m_eCheckSumType = CHECKSUM_CLASSIC;
                        if (sMsgArray[nIndex].ChecksumType == cstEnhanced)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eCheckSumType = CHECKSU_ENHANCED;
                        }

                        if (sMsgArray[nIndex].Direction == dirSubscriber || sMsgArray[nIndex].Direction == dirSubscriberAutoLength)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount++;
                            sLinData.m_uDataInfo.m_sErrInfo.m_ucDir = RX_FLAG;
                        }
                        if (sMsgArray[nIndex].Direction == dirPublisher)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount++;
                            sLinData.m_uDataInfo.m_sErrInfo.m_ucDir = TX_FLAG;
                        }

                        if (sMsgArray[nIndex].ErrorFlags & MSG_ERR_INCONSISTENT_SYNC)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRSYNC;
                        }
                        if (sMsgArray[nIndex].ErrorFlags & MSG_ERR_SLAVE_NOT_RESPONDING)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRNOANS;
                        }
                        if (sMsgArray[nIndex].ErrorFlags & MSG_ERR_TIMEOUT)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRNOANS;
                        }
                        if (sMsgArray[nIndex].ErrorFlags & MSG_ERR_CHECKSUM)
                        {
                            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRCRC;
                        }
                    }
                }
                vWriteIntoClientsBuffer(sLinData);
            }
            //else
            //{
            //}
        }
        else
        {
            vRetrieveAndLog(nReturn, __FILE__, __LINE__);
            break;
        }
    }
}
/* Read thread procedure */

DWORD WINAPI CanMsgReadThreadProc_LIN_Usb(LPVOID pVoid)
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    // Validate certain required pointers
    VALIDATE_POINTER_RETURN_VAL(pThreadParam, (DWORD)-1);

    // Assign thread action to CREATE_TIME_MAP
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
    // Create the notification event
    pThreadParam->m_hActionEvent = CreateEvent(NULL, false, false, NULL);

    // Get the handle to the controller and validate it
    VALIDATE_POINTER_RETURN_VAL(pThreadParam->m_hActionEvent, (DWORD)-1);

    // Set the event to LIN_USB driver for wakeup and frame arrival notification
    SetReadIndicationEvent(pThreadParam->m_hActionEvent);

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                sg_pouDIL_LIN_PeakUSB->ProcessLINMsg(); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                sg_pouDIL_LIN_PeakUSB->ProcessLINMsg();
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

    // Close the notification event for startup
    CloseHandle(pThreadParam->m_hActionEvent);
    pThreadParam->m_hActionEvent = NULL;

    return 0;
}
/* creates a map between harware and network*/
static int nCreateHwNetMap( HLINHW hDevices[],
                            int nSize,
                            SHWNETLIST sHwNetList[] )
{
    int nReturn = errOK;
    // Initialise association with hardware handles
    for( int nIndex = 0; nIndex < nSize; nIndex++)
    {
        sHwNetList[ nIndex ].m_hHw = hDevices[ nIndex ];
        sHwNetList[ nIndex ].m_hNet = 0;
    }
    // Buffer to get Network/Hardware Param
    char cBuff[ defUSB_PARAM_BUFFER_SIZE ] = {0};

    // Iterate thro list of devices
    int nDeviceNumber = 0;
    for( int nIndex = 0; nIndex < nSize; nIndex++)
    {
        // Get the Hardware Handle
        nReturn = pfLIN_GetHwParam( hDevices[nIndex], hwpDeviceNumber, &nDeviceNumber, sizeof(nDeviceNumber));
        if( nReturn == errOK )
        {
            // Update the Hw detail in Hw-Net map
            for( int nHwIndex = 0; nHwIndex < nSize; nHwIndex++ )
            {
                if( cBuff[0] == hDevices[nHwIndex] )
                {
                    // Update the net Handle value
                    //sHwNetList[ nHwIndex ].m_hNet = (HCANNET)(nIndex + 1);
                }
            }
        }
        // IF USB driver is not loaded properly
        else if( nReturn != errOK )
        {
            nReturn = -1;
            nIndex = MAX_HLINNET;
        }
    }
    return nReturn;
}
/* Create single hardware network when there is one hardware*/
static int nCreateSingleHardwareNetwork()
{
    USES_CONVERSION;
    int nReturn = -1;
    // Get Hardware Network Map
    SHWNETLIST sHwNetList[ 1 ];
    // Create Hardware - Network association map
    nCreateHwNetMap( sg_hHardware, 1, sHwNetList );
    sg_ControllerDetails[0].m_omHardwareDesc =  sg_IntrHw[0].m_acDescription;
    // Set the number of channels
    sg_odHardwareNetwork.m_nNoOfChannels = 1;
    // Assign hardware handle
    sg_odHardwareNetwork.m_aodChannels[ 0 ].m_hHardwareHandle = sHwNetList[ 0 ].m_hHw;
    // Assign Net Handle
    return nReturn;
}

/*******************************************************************************
 Function Name  : nGetFreeNetHandle
 Input(s)       : -
 Output         : int - Exisiting Network Count
 Functionality  : This is USB Specific function. This will find first available
                  network handle. If nothing found this will return 0.
 Member of      :
 Author(s)      : Raja N
 Date Created   : 17.09.2004
 Modifications  :
*******************************************************************************/
static int nGetFreeNetHandle()
{
    int nErr;
    UCHAR ucIndex;
    int nNetHandle = 0;

    return nNetHandle;
}
static INT nCreateNetwork(HLINHW hHw, INT nChannelIndex)
{
    INT nReturn = 0;
    HLINHW hCurrentHw = 0;
    //HLINNET hCurrentNet = 0;

    // Get Current Channel Reference
    CChannel& orCurrentChannel = sg_odHardwareNetwork.m_aodChannels [ nChannelIndex ];
    // Check for existing network
    hCurrentHw = hHw;

    // Create network if the net handle is 0

    // Assign information to the channel
    orCurrentChannel.m_hHardwareHandle = hCurrentHw;
    //orCurrentChannel.m_hNetworkHandle = hCurrentNet;
    return nReturn;
}

void vBlinkHw(INTERFACE_HW s_HardwareIntr)
{
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.
* \param[in]     psInterfaces, is INTERFACE_HW structue
* \param[out]    pnSelList, contains channels selected array
* \param[out]    nCount, contains selected channel count
* \return        returns 0 if success, else -1
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
int ListHardwareInterfaces(HWND hParent,INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, LIN, LIN_CHANNEL_ALLOWED, &objMainWnd);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList);
        return 0;
    }
    else
    {
        return -1;
    }
}

/* Create multiple hardware network when there are more than one hardware*/
static int nCreateMultipleHardwareNetwork(UINT unDefaultChannelCnt = 0)
{
    int nReturn = S_OK;
    INT nHwCount = sg_ucNoOfHardware;

    /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
    if ( unDefaultChannelCnt && nHwCount >= unDefaultChannelCnt )
    {
        for (UINT i = 0; i < unDefaultChannelCnt; i++)
        {
            sg_anSelectedItems[i] = i;
        }
        nHwCount = unDefaultChannelCnt;
    }
    /* Show channel selection dialog */
    else if ( ListHardwareInterfaces(sg_hOwnerWnd, sg_IntrHw, sg_anSelectedItems, nHwCount) != 0 )
    {
        return HW_INTERFACE_NO_SEL;
    }

    sg_ucNoOfHardware = nHwCount;

    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_hHardware[nCount] = (HLINHW)sg_IntrHw[sg_anSelectedItems[nCount]].m_dwIdInterface;
        sg_ControllerDetails[nCount].m_omHardwareDesc = sg_IntrHw[sg_anSelectedItems[nCount]].m_acDescription;
    }
    // Associate channels selected by the user to
    // Channels in the network
    SHWNETLIST asHwNetList[defNO_OF_CHANNELS];
    memset(asHwNetList, 0, sizeof (SHWNETLIST) * defNO_OF_CHANNELS);
    //create Hw net map
    nCreateHwNetMap(sg_hHardware, sg_ucNoOfHardware, asHwNetList);

    sg_odHardwareNetwork.m_nNoOfChannels = 0;
    // Check for Upper limit of number of channels
    if( sg_ucNoOfHardware > defNO_OF_CHANNELS )
    {
        sg_ucNoOfHardware = defNO_OF_CHANNELS;
    }
    for (UINT i = 0; i < sg_ucNoOfHardware; i++)
    {
        nCreateNetwork(asHwNetList[i].m_hHw, i);
        sg_odHardwareNetwork.m_nNoOfChannels++;
    }

    return nReturn;
}

/* Get the number of devices and there handles; */
static int nGetNoOfConnectedHardware(int& nHardwareCount)
{
    USES_CONVERSION;

    int nReturn = -1;
    // Iterate through list of hardware handles
    // Declare buffer for parameter passing
    int nHwType = 0;
    HLINHW hHw[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    WORD wBuffSize = 8;
    int nNumDevices = 0;
    // Get the list of hardware (each channel is a  hardware)
    (*pfLIN_GetAvailableHw)(hHw, wBuffSize, &nNumDevices);
    HLINHW ucHwID = NULL;

    // Loop through list
    for(int nIndex = 0;  nIndex < nNumDevices; nIndex++ )
    {
        ucHwID = hHw[nIndex];
        //Get the hardware type
        nReturn = pfLIN_GetHwParam( ucHwID, hwpType, &nHwType, 0);
        // If successful
        if ( nReturn == errOK )
        {
            // Update the handle in the array
            TCHAR acTempStr[256] = {'\0'};
            char cDeviceName[LIN_MAX_NAME_LENGTH] = "";
            TLINVersion sFirmwareVer;
            // Get firmware version. String is in xx.yy format where xx = major version and yy = minor version
            (*pfLIN_GetHwParam)( ucHwID, hwpFirmwareVersion, &sFirmwareVer,
                                 sizeof(TLINVersion) );
            _stprintf(acTempStr, "%d.%d", sFirmwareVer.Major, sFirmwareVer.Minor);

            //Update global INTERFACE_HW structure
            sg_IntrHw[sg_ucNoOfHardware].m_dwIdInterface = ucHwID;

            // Get device name
            (*pfLIN_GetHwParam)( ucHwID, hwpName, cDeviceName,
                                 LIN_MAX_NAME_LENGTH );
            sg_IntrHw[sg_ucNoOfHardware].m_acDeviceName = acTempStr;
            sg_IntrHw[sg_ucNoOfHardware].m_acAdditionalInfo = cDeviceName;

            //Get Device Number
            int nDeviceID = 0;
            (*pfLIN_GetHwParam)( ucHwID, hwpDeviceNumber, &nDeviceID, sizeof(nDeviceID) );
            char chTemp[256];

            sprintf_s(chTemp,sizeof(chTemp), "PLIN-USB Device ID - %Xh", nDeviceID);
            sg_IntrHw[sg_ucNoOfHardware].m_acDescription = chTemp;

            sg_hHardware [ sg_ucNoOfHardware++ ] = ucHwID;
        }
        else
        {
            // Set the number of devices to zero
            sg_ucNoOfHardware = 0;
            // Break the loop
            nIndex = MAX_LINHW;
        }
    }
    // Assign the hardware count to the parameter
    nHardwareCount = sg_ucNoOfHardware;

    // Return the operation result
    return nReturn;
}
/* Initialization of hardware network */
static int nInitHwNetwork(UINT unDefaultChannelCnt = 0)
{
    int nDevices = 0;
    int nReturn = NO_HW_INTERFACE;

    //Initiailize to 0
    sg_ucNoOfHardware = 0;

    // Select Hardware
    // This function will be called only for USB mode
    nReturn = nGetNoOfConnectedHardware( nDevices );
    // Capture only Driver Not Running event
    if( nReturn == errOK )
    {
        // Assign the device count
        sg_ucNoOfHardware = (UCHAR)nDevices;
        // Proceed only if it is successful
        if( nReturn == S_OK )
        {
            if (nDevices == 0)
            {
                nReturn = NO_HW_INTERFACE;
            }
            else if( nDevices > 1 )
            {
                // Get the selection from the user. This will also
                // create and assign the networks
                nReturn = nCreateMultipleHardwareNetwork(unDefaultChannelCnt);
            }
            else
            {
                // Use available one hardware
                nReturn = nCreateSingleHardwareNetwork();
            }
        }
    }
    return nReturn;
}
// create client and get the client handle
int nCreateClient(TCHAR* omStrClient, ULONG hMainFrame, HLINCLIENT& hClient)
{
    USES_CONVERSION;
    int nReturn = -1;
    return nReturn;
}

// Function connect driver
static int nConnectToDriver(UINT unDefaultChannelCnt = 0)
{
    int nReturn = -1;

    //if( sg_bIsDriverRunning == TRUE )
    {
        // Select Hardware or Simulation Network
        nReturn = nInitHwNetwork(unDefaultChannelCnt);
        /* check if everything is ok */
        if ( nReturn != S_OK )
        {
            return nReturn;
        }

        for( UINT unIndex = 0; unIndex < sg_odHardwareNetwork.m_nNoOfChannels;
                unIndex++ )
        {
            //nReturn = (*pfLIN_ResetHardware)( sg_hClient,
            //    sg_odHardwareNetwork.m_aodChannels[ unIndex ].m_hHardwareHandle);
            if( nReturn == S_OK )
            {
                // Reset the channel associated with the hardware
                sg_odHardwareNetwork.m_aodChannels[ unIndex ].vResetChannel();
            }
            else
            {
                vRetrieveAndLog(nReturn, __FILE__, __LINE__);
                // Break the loop
                unIndex = sg_odHardwareNetwork.m_nNoOfChannels;
            }
        }
    }
    return nReturn;
}

static BOOL bGetDriverStatus()
{
    USES_CONVERSION;
    BOOL bReturn = FALSE;

    return bReturn;
}

static int nSetBaudRate( )
{
    int nReturn = -1;
    return nReturn;
}

static int nSetWarningLimit()
{
    int nReturn = S_OK;
    return nReturn;
}

/* apply controller mode and baud rate */
static int nSetApplyConfiguration()
{
    int nReturn = S_OK;
    return nReturn;
}


/* Test Hardware */
static int nTestHardwareConnection(HLINHW hHardwareHandle)
{
    int nReturn = -1;
    return nReturn;
}
/* connect to the bus */
static int nConnect(BOOL bConnect)
{
    VALIDATE_POINTER_RETURN_VAL(sg_podActiveNetwork, S_FALSE);
    int nReturn = S_OK;
    return nReturn;
}

/* Function to set API function pointers */
HRESULT GetPEAK_USB_APIFuncPtrs(void)
{
    LRESULT lResult = 0;
    if (sg_hLinUsbLib != NULL)
    {
        //Check1
        pfLIN_GetAvailableHw = (PAPIPROC1)GetProcAddress(sg_hLinUsbLib, "LIN_GetAvailableHardware");
        if (NULL == pfLIN_GetAvailableHw)
        {
            lResult = defPROC1Err;
        }
        //Check2
        pfLIN_RegisterNet  = (PAPIPROC2)GetProcAddress(sg_hLinUsbLib, "LIN_RegisterClient");
        if (NULL == pfLIN_RegisterNet)
        {
            lResult = lResult | defPROC2Err;
        }
        //Check3
        pfLIN_ResetHardware = (PAPIPROC3)GetProcAddress(sg_hLinUsbLib, "LIN_ResetHardware");
        if (NULL == pfLIN_ResetHardware)
        {
            lResult = lResult | defPROC3Err;
        }
        //Check4
        pfLIN_ResetClient = (PAPIPROC4)GetProcAddress(sg_hLinUsbLib, "LIN_ResetClient");
        if (NULL == pfLIN_ResetClient)
        {
            lResult = lResult | defPROC4Err;
        }
        //Check5
        pfLIN_Write = (PAPIPROC5)GetProcAddress(sg_hLinUsbLib, "LIN_Write");
        if (NULL == pfLIN_Write)
        {
            lResult = lResult | defPROC5Err;
        }
        //Check6
        pfLIN_Read  = (PAPIPROC6) GetProcAddress(sg_hLinUsbLib, "LIN_Read");
        if (NULL == pfLIN_Read)
        {
            lResult = lResult | defPROC6Err;
        }
        //Check7
        pfLIN_ReadMulti = (PAPIPROC7)GetProcAddress(sg_hLinUsbLib, "LIN_ReadMulti");
        if (NULL == pfLIN_ReadMulti)
        {
            lResult = lResult | defPROC7Err;
        }
        //Check8
        pfLIN_ConnectToNet  = (PAPIPROC8)GetProcAddress(sg_hLinUsbLib, "LIN_ConnectClient");
        if (NULL == pfLIN_ConnectToNet)
        {
            lResult = lResult | defPROC8Err;
        }
        //Check9
        pfLIN_SetClientFilter  = (PAPIPROC9)GetProcAddress(sg_hLinUsbLib, "LIN_SetClientFilter");
        if (NULL == pfLIN_SetClientFilter)
        {
            lResult = lResult | defPROC9Err;
        }
        //Check10
        pfLIN_DisconnectFromNet  = (PAPIPROC10)GetProcAddress(sg_hLinUsbLib, "LIN_DisconnectClient");
        if (NULL == pfLIN_DisconnectFromNet)
        {
            lResult = lResult | defPROC10Err;
        }
        //Check11
        pfLIN_RemoveClient  = (PAPIPROC11)GetProcAddress(sg_hLinUsbLib, "LIN_RemoveClient");
        if (NULL == pfLIN_RemoveClient)
        {
            lResult = lResult | defPROC11Err;
        }
        //Check12
        pfLIN_SetHwParam  = (PAPIPROC12)GetProcAddress(sg_hLinUsbLib, "LIN_SetHardwareParam");
        if (NULL == pfLIN_SetHwParam)
        {
            lResult = lResult | defPROC12Err;
        }
        //Check13
        pfLIN_GetHwParam = (PAPIPROC13)GetProcAddress(sg_hLinUsbLib, "LIN_GetHardwareParam");
        if (NULL == pfLIN_GetHwParam)
        {
            lResult = lResult | defPROC13Err;
        }
        ////Check14
        //pfLIN_SetNetParam = (PAPIPROC14)GetProcAddress(sg_hLinUsbLib, "LIN_SetNetParam");
        //if (NULL == pfLIN_SetNetParam)
        //{
        //    lResult = lResult | defPROC14Err;
        //}
        ////Check15
        //pfLIN_GetNetParam = (PAPIPROC15)GetProcAddress(sg_hLinUsbLib, "LIN_GetNetParam");
        //if (NULL == pfLIN_GetNetParam)
        //{
        //    lResult = lResult | defPROC15Err;
        //}
        //Check16
        pfLIN_SetClientParam  = (PAPIPROC16)GetProcAddress(sg_hLinUsbLib, "LIN_SetClientParam");
        if (NULL == pfLIN_SetClientParam)
        {
            lResult = lResult | defPROC16Err;
        }
        //Check17
        pfLIN_VersionInfo = (PAPIPROC17) GetProcAddress(sg_hLinUsbLib, "LIN_GetVersionInfo");
        if (NULL == pfLIN_VersionInfo)
        {
            lResult = lResult | defPROC17Err;
        }
        //Check18
        pfLIN_GetErrText = (PAPIPROC18) GetProcAddress(sg_hLinUsbLib, "LIN_GetErrorText");
        if (NULL == pfLIN_GetErrText)
        {
            lResult = lResult | defPROC17Err;
        }
        ////Check19
        //pfLIN_RegisterClient = (PAPIPROC19) GetProcAddress(sg_hLinUsbLib, "LIN_RegisterClient");
        //if (NULL == pfLIN_RegisterClient)
        //{
        //    lResult = lResult | defPROC17Err;
        //}

        //Check20
        pfLIN_InitializeHardware = (PAPIPROC20) GetProcAddress(sg_hLinUsbLib, "LIN_InitializeHardware");
        if (NULL == pfLIN_InitializeHardware)
        {
            lResult = lResult | defPROC17Err;
        }

        //Check21
        pfLIN_RegisterFrameId = (PAPIPROC21) GetProcAddress(sg_hLinUsbLib, "LIN_RegisterFrameId");
        if (NULL == pfLIN_RegisterFrameId)
        {
            lResult = lResult | defPROC17Err;
        }

        pfLIN_SetFrameEntry = (PAPIPROC22) GetProcAddress(sg_hLinUsbLib, "LIN_SetFrameEntry");
        if (NULL == pfLIN_SetFrameEntry)
        {
            lResult = lResult | defPROC17Err;
        }

        pfLIN_UpdateByteArray = (PAPIPROC23) GetProcAddress(sg_hLinUsbLib, "LIN_UpdateByteArray");
        if (NULL == pfLIN_UpdateByteArray)
        {
            lResult = lResult | defPROC17Err;
        }

        pfLIN_CalcChecksum = (PAPIPROC24) GetProcAddress(sg_hLinUsbLib, "LIN_CalculateChecksum");
        if (NULL == pfLIN_CalcChecksum)
        {
            lResult = lResult | defPROC17Err;
        }


        pfLIN_ResetHardwareConfig = (PAPIPROC25) GetProcAddress(sg_hLinUsbLib, "LIN_ResetHardwareConfig");
        if (NULL == pfLIN_ResetHardwareConfig)
        {
            lResult = lResult | defPROC17Err;
        }

        pfLIN_GetPID = (PAPIPROC26) GetProcAddress(sg_hLinUsbLib, "LIN_GetPID");
        if (NULL == pfLIN_GetPID)
        {
            lResult = lResult | defPROC17Err;
        }


        //check for error
        if (lResult != 0)
        {
            FreeLibrary(sg_hLinUsbLib);
        }
    }
    else
    {
        lResult = defLoadDriverErr;
    }
    return (HRESULT) lResult;
}

/* HELPER FUNCTIONS END */

/* GENERAL FUNCTION SET STARTS */
/* Perform initialization operations specific to TZM */
HRESULT CDIL_LIN_PeakUSB::LIN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;
    //Register Monitor client
    //LIN_RegisterClient(TRUE, sg_dwClientID, LIN_MONITOR_NODE);
    sg_podActiveNetwork = &sg_odHardwareNetwork;

    // Register to the hardware
    if (NULL != pfLIN_RegisterNet)
    {
        if (sg_hClient == 0)
        {
            if (pfLIN_RegisterNet("BUSMASTER LIN Client", 0, &sg_hClient) == ERROR_SUCCESS)
            {
                hResult = S_OK;
            }
        }
    }

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return hResult;
}

/* Perform closure operations specific to TZM */
HRESULT CDIL_LIN_PeakUSB::PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    UINT ClientIndex = 0;
    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);
    }
    StopHardware(); //return value not important

    /*hResult = LIN_DeselectHwInterface();
    sg_ucNoOfHardware = 0;*/

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_DRIVER_SELECTED;
    }

    return hResult;
}
/* Rrtreive time mode mapping */
HRESULT CDIL_LIN_PeakUSB::LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStampRef;

    QueryTickCount = sg_QueryTickCount;

    return S_OK;
}

static void USB_omStrGetDriverInfo(TCHAR* pcDriverInfo)
{
    USES_CONVERSION;
    // Get Driver information
    CHAR pacDriverInfo[256];
    (*pfLIN_VersionInfo)(pacDriverInfo, sizeof(pacDriverInfo));
    _tcscpy(pcDriverInfo, A2T(pacDriverInfo));
}

/* Function to List Hardware interfaces connect to the system and requests to the
   user to select*/
HRESULT CDIL_LIN_PeakUSB::LIN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    //if (bGetDriverStatus())
    {
        char chTemp[512];
        strcpy_s(chTemp, sizeof(chTemp), asSelHwInterface[0].m_acDescription.c_str());
        USB_omStrGetDriverInfo(chTemp);
        asSelHwInterface[0].m_acDescription = chTemp;

        if ( ( hResult = nConnectToDriver(nCount) ) == LIN_USB_OK)
        {
            for (UINT i = 0; i < sg_ucNoOfHardware; i++)
            {
                asSelHwInterface[i].m_dwIdInterface = sg_hHardware[i];
            }
            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
            nCount = sg_odHardwareNetwork.m_nNoOfChannels;
            hResult = S_OK;
        }
    }
    //else
    //{
    //    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Error in getting driver status"));
    //}
    return hResult;
}

//Reset hardware or software
static int nResetHardware(BOOL bHardwareReset)
{
    int nReturn = -1;

    // Hardware Reset
    if( bHardwareReset == TRUE )
    {
        // Note this will affect all clients connected in the network
        // Reset All hardware connected
        for( UINT unIndex = 0; unIndex < sg_odHardwareNetwork.m_nNoOfChannels;
                unIndex++ )
        {
            nReturn = (*pfLIN_ResetHardware)( sg_hClient,
                                              sg_odHardwareNetwork.m_aodChannels[ unIndex ].m_hHardwareHandle );
            if( nReturn == S_OK )
            {
                // Reset the channel associated with the hardware
                sg_odHardwareNetwork.m_aodChannels[ unIndex ].vResetChannel();
            }
            else
            {
                vRetrieveAndLog(nReturn, __FILE__, __LINE__);
                // Break the loop
                unIndex = sg_odHardwareNetwork.m_nNoOfChannels;
            }
        }
    }
    else
    {
        // Reset the client
        nReturn = (*pfLIN_ResetClient)(sg_hClient);
        // Check for failure
        if( nReturn != S_OK )
        {
            // Display connection error
            vRetrieveAndLog(nReturn, __FILE__, __LINE__);
        }
    }
    return nReturn;
}
/* Function to deselect the chosen hardware interface */
HRESULT CDIL_LIN_PeakUSB::LIN_DeselectHwInterface(void)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    // First remove All the connected Clients

    LIN_ResetHardware();
    if ( pfLIN_RemoveClient != NULL )
    {
        if ((*pfLIN_RemoveClient)(sg_hClient) != LIN_USB_OK)
        {
            hResult = S_FALSE;
        }
    }
    sg_hClient = 0;

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

/* Function to select hardware interface chosen by the user */
HRESULT CDIL_LIN_PeakUSB::LIN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;

    VALIDATE_POINTER_RETURN_VAL(sg_hLinUsbLib, S_FALSE);
    HRESULT hResult = S_FALSE;

    //if (bGetDriverStatus())
    {
        // register the client into the driver
        //if (nCreateClient(LIN_MONITOR_NODE, NULL, sg_hClient) == S_OK)
        {
            //Mark an entry in DIL_CAN also
            DWORD ClientId = 0;
            LIN_RegisterClient(TRUE, ClientId, LIN_MONITOR_NODE);
            for (UINT i = 0; i < sg_ucNoOfHardware; i++)
            {
                sg_hHardware[i] = (HLINHW)asSelHwInterface[i].m_dwIdInterface;

            }
            /* Create Hw Net map*/
            SHWNETLIST sHwNetList[defNO_OF_CHANNELS];
            /* this function maps existing net to the hw*/
            nCreateHwNetMap(sg_hHardware, sg_ucNoOfHardware, sHwNetList);

            for (UINT i = 0; i < sg_ucNoOfHardware; i++)
            {
                /* this function creates the network for a hw if not exists*/
                nCreateNetwork(sHwNetList[i].m_hHw, i);

            }
            sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
            hResult = S_OK;
        }
    }
    /* else
     {
         sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Driver is not running..."));
     }*/
    return hResult;
}

/* Function to set controller configuration*/
HRESULT CDIL_LIN_PeakUSB::SetConfigData(IBMNetWorkGetService* ouConfig)
{
    HRESULT hResult = S_FALSE;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    bLoadDataFromContr(ouConfig);

    return hResult;
}

BOOL Callback_DILTZM(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, int /*Length*/)
{
    return S_OK;
    /*return (sg_pouDIL_LIN_PeakUSB->LIN_SetConfigData( pDatStream, 0) == S_OK);*/
}

int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/,
                            PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

    //CChangeRegisters ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
    //   ouChangeRegister.DoModal();
    //   nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}


/* Function to start monitoring the bus */
HRESULT CDIL_LIN_PeakUSB::StartHardware(void)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
    //Restart the read thread
    sg_sParmRThread.bTerminateThread();
    sg_sParmRThread.m_pBuffer = (LPVOID) sg_hHardware[0];

    if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_LIN_Usb))
    {
        hResult = S_OK;
    }
    //If everything is ok connect to the network
    if (hResult == S_OK)
    {
        hResult = nConnect(TRUE);
        if (hResult == LIN_USB_OK)
        {
            hResult = S_OK;
            sg_bCurrState = STATE_CONNECTED;
            HANDLE hActionEvent = NULL;
            SetEvent(hActionEvent);
            vCreateTimeModeMapping(hActionEvent);
        }
        else
        {
            //log the error for open port failure
            vRetrieveAndLog(hResult, __FILE__, __LINE__);
            hResult = S_FALSE;
        }
    }

    if ( S_OK == hResult )
    {
        sg_bCurrState = STATE_CONNECTED;
        sg_byCurrState = CREATE_MAP_TIMESTAMP;
    }

    return hResult;
}

HRESULT CDIL_LIN_PeakUSB::PreStartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;

    DWORD dwRet = errOK;
    sg_odHardwareNetwork.vResetChannelData();

    // Connect to the hardware
    dwRet = pfLIN_ConnectToNet(sg_hClient, sg_hHardware[0]);

    // Initialize the hardware
    UINT unHwMode = sg_odHardwareNetwork.m_aodChannels[0].m_unLINMode;
    dwRet = pfLIN_InitializeHardware(sg_hClient, sg_hHardware[0], unHwMode, sg_odHardwareNetwork.m_aodChannels[0].m_unBaudrate);

    // Register to receive all frames
    dwRet = pfLIN_RegisterFrameId(sg_hClient, sg_hHardware[0], 0, 63);

    // Set the filter to receive all frames
    dwRet = pfLIN_SetClientFilter(sg_hClient, sg_hHardware[0], 0xFFFFFFFFFFFFFFFF);

    // Register to receive on status frames
    dwRet = pfLIN_SetClientParam(sg_hClient, clpReceiveStatusFrames, 1);

    // Set scehdule     - LIN_SetSchedule
    // Delete schedule  - LIN_DeleteSchedule
    // Startschedule    - LIN_StartSchedule
    // Suspend scedule  - LIN_SuspendSchedule
    // Resume scedule   - LIN_ResumeSchedule

    return hResult;
}

/* Function to stop monitoring the bus */
HRESULT CDIL_LIN_PeakUSB::StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    hResult = nConnect(FALSE);

    // Disconnect client
    //pfLIN_DisconnectFromNet(sg_hClient, sg_odHardwareNetwork.m_aodChannels[0].m_hHardwareHandle);
    //pfLIN_RemoveClient(sg_hClient);
    pfLIN_ResetHardware(sg_hClient, sg_odHardwareNetwork.m_aodChannels[0].m_hHardwareHandle);
    pfLIN_ResetHardwareConfig(sg_hClient, sg_odHardwareNetwork.m_aodChannels[0].m_hHardwareHandle);
    //// Remove Client kss to add this while changeing the hardware / deselecting
    //pfLIN_RemoveClient(sg_hClient);

    hResult = pfLIN_InitializeHardware(sg_hClient, sg_hHardware[0], modNone, sg_odHardwareNetwork.m_aodChannels[0].m_unBaudrate);
    if (hResult == LIN_USB_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = S_FALSE;
    }
    return hResult;
}
static BOOL bClientExist(TCHAR* pcClientName, UINT& Index)
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


/* Function to reset the hardware, fcClose resets all the buffer */
HRESULT CDIL_LIN_PeakUSB::LIN_ResetHardware(void)
{
    HRESULT hResult = S_FALSE;
    // Stop the hardware if connected
    StopHardware(); //return value not necessary
    if (sg_sParmRThread.bTerminateThread())
    {
        if (nResetHardware(TRUE) == LIN_USB_OK)
        {
            hResult = S_OK;
        }
    }
    return hResult;
}

/* Function to get Controller status */
HRESULT CDIL_LIN_PeakUSB::LIN_GetCurrStatus(STATUSMSG& StatusData)
{
    if (sg_ucControllerMode == defUSB_MODE_ACTIVE)
    {
        StatusData.wControllerStatus = NORMAL_ACTIVE;
    }
    else if (sg_ucControllerMode == defUSB_MODE_PASSIVE)
    {
        StatusData.wControllerStatus = NORMAL_PASSIVE;
    }
    else if (sg_ucControllerMode == defUSB_MODE_SIMULATE)
    {
        StatusData.wControllerStatus = NOT_DEFINED;
    }
    return S_OK;
}

/* Function to get Tx Msg Buffers configured from chi file */
HRESULT CDIL_LIN_PeakUSB::LIN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}

/* Function to Send CAN Message to Transmit buffer. This is called only after checking the controller in active mode */
HRESULT CDIL_LIN_PeakUSB::LIN_Send(STLIN_MSG& sLinTxMsg)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    DWORD dwRet = errOK;

    CChannel& odChannel = sg_odHardwareNetwork.m_aodChannels[ sLinTxMsg.m_ucChannel - 1 ];
    // Set LIN checsum type
    odChannel.m_linChecksumType = nGetCalcChecksum(sLinTxMsg.m_ucChannel - 1, sLinTxMsg.m_ucMsgID);

    if ( sLinTxMsg.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        // Set the frame entries to hardware
        TLINFrameEntry linFrame;
        linFrame.ChecksumType = odChannel.m_linChecksumType;

        linFrame.Direction = dirPublisher; // Rx
        if (sLinTxMsg.m_ucMsgTyp == 1)
        {
            linFrame.Direction = dirPublisher; // Tx
        }

        linFrame.Flags = FRAME_FLAG_RESPONSE_ENABLE; // Slave Enable Publisher Response
        linFrame.FrameId = sLinTxMsg.m_ucMsgID;
        memcpy(linFrame.InitialData, sLinTxMsg.m_ucData, sizeof(linFrame.InitialData));
        linFrame.Length = sLinTxMsg.m_ucDataLen;

        stIdData ouData;
        ouData.m_nLength = sLinTxMsg.m_ucDataLen;
        memcpy(ouData.ucData, linFrame.InitialData, sizeof(linFrame.InitialData));
        odChannel.m_IdSlaveData[linFrame.FrameId] = ouData;

        // Set the frame that needs to be transmitted
        dwRet = pfLIN_SetFrameEntry(sg_hClient, sg_hHardware[0], &linFrame);
        if (dwRet == errOK)
        {
            hResult = S_OK;
            // Set the byte value to the hardware
            dwRet = pfLIN_UpdateByteArray(sg_hClient, sg_hHardware[0], linFrame.FrameId,
                                          (BYTE)0, linFrame.Length, linFrame.InitialData);
            if (dwRet != errOK)
            {
                hResult = S_FALSE;
            }
        }
        else
        {
            hResult = S_FALSE;
        }
    }
    else if ( sg_bCurrState == STATE_CONNECTED)
    {
        if (odChannel.m_unLINMode == modMaster)
        {

            TLINMsg msg;

            //1. Message Id
            msg.FrameId = sLinTxMsg.m_ucMsgID;
            pfLIN_GetPID(&msg.FrameId);


            //2. Msg Dir and Checksum type Checksum value
            map<int, stIdData>::iterator itrData = odChannel.m_IdSlaveData.find(sLinTxMsg.m_ucMsgID);

            if ( itrData != odChannel.m_IdSlaveData.end() )     //Both Master and Slave Task
            {
                msg.Direction = dirPublisher;
                msg.ChecksumType = odChannel.m_linChecksumType;
                memcpy(msg.Data, itrData->second.ucData, sizeof(itrData->second.ucData));
                dwRet = pfLIN_CalcChecksum(&msg);
            }
            else                                                //Master Task Only
            {
                msg.Direction = dirSubscriberAutoLength;
                msg.ChecksumType = cstAuto;
            }

            //3. Messaage Length
            msg.Length = sLinTxMsg.m_ucDataLen;

            // Try to send the LIN frame message with LIN_Write.
            dwRet = pfLIN_Write(sg_hClient, sg_hHardware[0], &msg);
            hResult = S_OK;
            if (dwRet != errOK)
            {
                hResult = S_FALSE;
            }

        }
        else
        {
            hResult = S_FALSE;
        }
    }

    return hResult;
}

HRESULT CDIL_LIN_PeakUSB::LIN_DisableSlave(STLIN_MSG& sMessage)
{
    HRESULT hResult = S_FALSE;

    return 0;
}

HRESULT CDIL_LIN_PeakUSB::LIN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return S_OK;
}

/* Function to retreive error string of last occurred error*/
HRESULT CDIL_LIN_PeakUSB::LIN_GetLastErrorString(string& acErrorStr)
{
    acErrorStr = sg_acErrStr;
    return S_OK;
}

/* Set application parameters specific to LIN_USB */
HRESULT CDIL_LIN_PeakUSB::LIN_SetAppParams(HWND hWndOwner)
{
    sg_hOwnerWnd = hWndOwner;
    // Initialise both the time parameters
    //INITIALISE_DATA(sg_CurrSysTime);
    memset(&sg_CurrSysTime, 0, sizeof(sg_CurrSysTime));
    sg_TimeStampRef = 0x0;
    //Initialise Query Tick Count;
    sg_QueryTickCount.QuadPart = 0;
    //INITIALISE_DATA(sg_sCurrStatus);
    memset(&sg_sCurrStatus, 0, sizeof(sg_sCurrStatus));
    //INITIALISE_ARRAY(sg_acErrStr);
    sg_acErrStr = "";
    LIN_ManageMsgBuf(MSGBUF_CLEAR, NULL, NULL);

    return S_OK;
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
            i = MAX_CLIENT_ALLOWED; //break the loop
        }
    }
    return nClientId;
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    return bExist;
}

static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
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
    return bReturn;
}

/* Function to load driver PLinApi.dll */
HRESULT CDIL_LIN_PeakUSB::LIN_LoadDriverLibrary(void)
{
    USES_CONVERSION;

    HRESULT hResult = S_OK;

    if (sg_hLinUsbLib != NULL)
    {

        hResult = DLL_ALREADY_LOADED;
    }

    if (S_OK == hResult)
    {
        sg_hLinUsbLib = LoadLibrary("PLinApi.dll");
        if (sg_hLinUsbLib == NULL)
        {

            hResult = S_FALSE;
        }
        else
        {
            int nResult = GetPEAK_USB_APIFuncPtrs();
            if (nResult != 0)
            {
                // Log list of the function pointers non-retrievable
                // TO DO: specific information on failure in getting function pointer

                hResult = S_FALSE;
            }
        }
    }

    return hResult;
}

/* Function to Unload Driver library */
HRESULT CDIL_LIN_PeakUSB::LIN_UnloadDriverLibrary(void)
{
    // Don't bother about the success & hence the result
    LIN_DeselectHwInterface();

    // Store the Boardinfo to global variable

    if (sg_hLinUsbLib != NULL)
    {
        FreeLibrary(sg_hLinUsbLib);
        sg_hLinUsbLib = NULL;
    }
    return S_OK;
}

/* END OF FILTER IN/OUT FUNCTION */
HRESULT CDIL_LIN_PeakUSB::LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    if (hEvent != NULL)
    {
        sg_hCntrlStateChangeEvent = hEvent;
    }
    unCntrlStatus = static_cast<UINT>(sg_ucControllerMode);
    return S_OK;
}

HRESULT CDIL_LIN_PeakUSB::LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {

            case NUMBER_HW:
            {
                lParam = sg_ucNoOfHardware;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                int nConHwCnt = 0;
                if (nGetNoOfConnectedHardware(nConHwCnt) == S_OK)
                {
                    lParam = nConHwCnt;
                }
                else
                {
                    hResult = S_FALSE;
                }
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = sg_bIsDriverRunning;
            }
            break;
            case HW_MODE:
            {
                if (nChannel < sg_ucNoOfHardware)
                {
                    lParam = defCONTROLLER_ACTIVE;
                }
                else
                {
                    lParam = 0;
                }
            }
            break;
            case CON_TEST:
            {
                LONG lResult = 0;
                if (nChannel <= sg_podActiveNetwork->m_nNoOfChannels)
                {
                    if (nTestHardwareConnection(sg_hHardware[nChannel - 1]) == S_OK)
                    {
                        lResult = (LONG)1;
                    }
                    lParam = lResult;
                }
                else
                {
                    hResult = ERR_INVALID_CHANNEL;
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

HRESULT CDIL_LIN_PeakUSB::LIN_GetConfiguration( SCONTROLLER_DETAILS_LIN psControllerConfig[], INT& nSize )
{

    for ( int i = 0; i < sg_ucNoOfHardware; i++ )
    {
        psControllerConfig[i].m_BaudRate = sg_odHardwareNetwork.m_aodChannels[i].m_unBaudrate;
        psControllerConfig[i].m_strProtocolVersion = "LIN 1.3";

        if (sg_odHardwareNetwork.m_aodChannels[i].m_unLINVersion == LIN_VER_2_0)
        {
            psControllerConfig[i].m_strProtocolVersion = "LIN 2.0";
        }
        else if (sg_odHardwareNetwork.m_aodChannels[i].m_unLINVersion == LIN_VER_2_1)
        {
            psControllerConfig[i].m_strProtocolVersion = "LIN 2.1";
        }

        psControllerConfig[i].m_strHwUri = sg_ControllerDetails[i].m_omHardwareDesc;
    }
    nSize = sg_ucNoOfHardware;
    return S_OK;
}

/*******************************************************************************
 Function Name  : nGetErrorCounter
 Input(s)       : sErrorCount - Error Counter Structure
 Output         : int - Operation Result. 0 incase of no errors. Failure Error
                  codes otherwise.
 Functionality  : This function will return the error counter values. In case of
                  USB this is not supported.
 Member of      : -
 Author(s)      : Raja N
 Date Created   : 02.09.2004
 Modifications  : Raja N on 9.3.2005
                  Modifications to support multi channel. USB driver supports
                  indication whenever there is a change in the error counter
                  value. But parallel port mode this is done in polling. So
                  update the error counter whenever it is read from the driver.
*******************************************************************************/
static int nGetErrorCounter( UINT unChannel,
                             SERROR_CNT& sErrorCount)
{
    int nReturn = -1;

    // Check for the valid channel index
    if( unChannel < sg_podActiveNetwork->m_nNoOfChannels )
    {
        // Get the channel reference
        CChannel& odChannel = sg_podActiveNetwork->m_aodChannels[ unChannel ];
        // Assign the error counter value
        /*sErrorCount.m_ucRxErrCount = odChannel.m_ucRxErrorCounter;
        sErrorCount.m_ucTxErrCount = odChannel.m_ucTxErrorCounter;*/
        nReturn = S_OK;
    }
    else
    {
        // Invalid channel ID. Help debugging
        nReturn = ERR_INVALID_CHANNEL;
    }
    return nReturn;
}

HRESULT CDIL_LIN_PeakUSB::LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM /*eContrParam*/)
{
    HRESULT hResult = S_FALSE;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_podActiveNetwork->m_nNoOfChannels)
        {
            if (nGetErrorCounter(nChannel, sErrorCnt) == S_OK)
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
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}