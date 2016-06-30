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
 * \file      ContrConfigPeakUsbDefs.h
 * \brief     Macro definitions for controller configuration.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Macro definitions for controller configuration.
 */

#pragma once

/* Project definitions */
/* Vendor header files */
//#include "EXTERNAL/UCIFLX.H"
//#include "EXTERNAL/UCIBRD.H"
//#include "EXTERNAL/Ucierr.h"
//#include "EXTERNAL/XatXXReg.h"

#include "EXTERNAL/BOA 1.5\CSI/csisfs.h"
#include "EXTERNAL/BOA 1.5\OCI/ocilin.h"
#include "LINDriverDefines.h"
#include <map>
#include <vector>
#include "Utility/Utility_Thread.h"



#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.5"
#define LIB_CSL_NAME    "dll-csiBind_1_5.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_5.dll"

/**
 * Declarations of CSI API pointers
 */
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC1)(const char* uriName, CSI_NodeRange range, CSI_Tree* *tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC2)(CSI_Tree* tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC3)(CSI_Tree* tree, const BOA_UuidVersion* uuid, OCI_URIName uriName[], int size, int* count);
typedef LONG SLOT_BASECYCLE;
/**
 * CSI pointers table
 */
typedef struct tagCSI_VTABLE
{
    PROC1 createProtocolTree;
    PROC2 destroyProtocolTree;
    PROC3 getUriForUuid;

} CSI_VTABLE;

/**
 * Complete pointers of OCI, OCI CAN, CSI
 */
typedef struct tagBOA_POINTER_TABLE
{
    CSI_VTABLE       m_sCSI;
    OCI_LIN_VTable   m_sOCI;
} SBOA_POINTER_TABLE;

static SBOA_POINTER_TABLE sBOA_PTRS;


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

/**
 * Client and Client Buffer map
 */
#define MAX_BUFF_ALLOWED 16
#define MAX_CLIENT_ALLOWED 16
static UINT sg_unClientCnt = 0;

class SCLIENTBUFMAP
{
public:
    DWORD m_dwClientID;
    std::string m_acClientName;
    UINT m_unBufCount;
    SCLIENTBUFMAP()
    {
        m_dwClientID = 0;
        m_unBufCount = 0;
        m_acClientName = "";
    }
};

/**
 * Array of clients
 */
static std::vector<SCLIENTBUFMAP> sg_asClientToBufMap(MAX_CLIENT_ALLOWED);

static CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe

//
///************************** Static variables declaration area **************************/
//
/* For hardware interface list */
#define MAX_HW_INTERFACE 4
#define MAX_LIN_BUFFER_COUNT 64
static INTERFACE_HW sg_asInterfaceHw[MAX_HW_INTERFACE] = {'\0'};
static int sg_nTotalHwIntf = 0;
// Handle of the XAT library
static HMODULE  sg_hBOALib              = nullptr;
// Handle of the driver library
static HMODULE  sg_hBOAHWLib              = nullptr;

/* Application buffer, logging interface and client id */
static CPARAM_THREADPROC sg_sParmRThread;


// Buffer to store transmission message list
//static CFlxTxMsgBuffer sg_ouFlxTxList;
static OCI_LINTxMessage* sg_psBOA_FlxMsgList = nullptr;

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

// Buffer for the driver operation related error messages
static std::string sg_acErrStr = "";
static HRESULT sg_hLastError = S_OK;

static STLINDATA sg_asLINMsg;

class LINData
{
public:
    OCI_LINMessage m_OCI_LINMessageArray[MAX_LIN_BUFFER_COUNT];
    int m_nValidMessages;
};
class BOA_LINData
{
public:
    STLIN_MSG m_ouLinData;
    OCI_QueueHandle m_OCI_TxQueueHandle;
    UINT m_ouFrameType;
};

static BOOL m_bLINConnected = FALSE;

/* To make it thread safe */
static  CRITICAL_SECTION sg_CritSectForWrite;

typedef struct tagCHANNEL
{
    OCI_URIName m_acURI; /*< URI of the Controller */
    std::string m_strLinVersion;
    OCI_ControllerHandle m_OCI_HwHandle; /*< Controller handle */
    OCI_LINConfiguration m_OCI_LinConfig; /*< Controller configuration */

    // Creating OCI_CANFD_Configuration only if BOA_FD_VERSION is defined

    OCI_LINControllerProperties m_OCI_CntrlProp; /*< Controller properties */
    OCI_QueueHandle m_OCI_RxQueueHandle; /*< Controller receive queue handle */
    OCI_LINRxQueueConfiguration m_OCI_RxQueueCfg; /*< Controller receive queue configuration */
    OCI_QueueHandle m_OCI_TxQueueHandle; /*< Controller transmit queue handle */
    OCI_LINTxQueueConfiguration m_OCI_TxQueueCfg; /*< Controller transmit queue configuration */
    OCI_LINRxFilter m_OCI_FrameFilter; /*< Controller frame filter */
    OCI_LINEventFilter m_OCI_EventFilter; /*< Controller event filter */



    std::map<INT, INT> m_SlotBufferMap;

    UINT m_unMsgConfig;
    UINT m_unCycleTime;
    OCI_InternalErrorEventFilter m_OCI_InternalErrorFilter;
    UCHAR m_ucControllerState; /*< Controller state */
    UCHAR m_ucTxErrorCounter; /*< Controller Tx error counter */
    UCHAR m_ucRxErrorCounter; /*< Controller Rx error counter */
    UCHAR m_ucPeakRxErrorCounter; /*< Controller peak Rx error counter */
    UCHAR m_ucPeakTxErrorCounter; /*< Controller peak Tx error counter */
    OCI_TimerCapabilities m_OCI_TimerCapabilities;
    FLOAT m_fResolution;

    OCI_LINControllerCapabilities m_ControllerCapabilities;
    CRITICAL_SECTION m_ouCriticalSection;
    OCI_QueueHandle m_OCI_Master_Queue;

    //Thread
    CPARAM_THREADPROC m_ouDataTransmitThread;
    std::map<UINT, BOA_LINData> m_ouBoaLINData;

    int nValid;



    tagCHANNEL()
    {
        //OCI Transfer Data

        m_unMsgConfig = 0;
        m_unCycleTime = -1;
        //Thread Initialisation
        InitializeCriticalSection(&m_ouCriticalSection);
    }
    ~tagCHANNEL()
    {
        DeleteCriticalSection(&m_ouCriticalSection);
    }
} SCHANNEL;

/**
 * Channel instances
 */
static std::vector<SCHANNEL> sg_asChannel(defNO_OF_CHANNELS);

/**
 * Number of current channel selected
 */
static UINT sg_nNoOfChannels = 0;

static BOOL sg_bIsDriverRunning = FALSE;

static HWND sg_hOwnerWnd = nullptr;

const BYTE CREATE_MAP_TIMESTAMP = 0x1;
const BYTE CALC_TIMESTAMP_READY = 0x2;

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

static HANDLE sg_hEvent = nullptr;

static CRITICAL_SECTION sg_DIL_CriticalSection;

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

/**
 * Timer variables
 */
static SYSTEMTIME sg_CurrSysTime;
static UINT64 sg_TimeStamp = 0;
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;

/**
 * Required libraries
 */
static HMODULE sg_hLibCSI = nullptr;
static HMODULE sg_hLibOCI = nullptr;

static BOOL sg_bIsConnected = FALSE;