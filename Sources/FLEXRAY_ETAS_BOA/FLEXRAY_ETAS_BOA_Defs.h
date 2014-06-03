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

/* Project definitions */
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/FLEXRAY_Datatypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Utility/Utility_Thread.h"

/* Definitions for BOA versions */
#define BOA_VERSION_1_4 0x010400
#define BOA_VERSION_1_5 0x010500
#define BOA_VERSION_2_0 0x020000

/* ETAS BOA includes */
#ifndef BOA_VERSION
#error "No BOA version defined"
#endif
#if BOA_VERSION == BOA_VERSION_1_4
#include "EXTERNAL/BOA 1.4/Include/OCI/ociflx.h"
#include "EXTERNAL/BOA 1.4/Include/CSI/csisfs.h"
#elif BOA_VERSION == BOA_VERSION_1_5
#include "EXTERNAL/BOA 1.5/Include/OCI/ociflx.h"
#include "EXTERNAL/BOA 1.5/Include/CSI/csisfs.h"
#elif BOA_VERSION == BOA_VERSION_2_0
#include "EXTERNAL/BOA_V2/Include/OCI/ociflx.h"
#include "EXTERNAL/BOA_V2/Include/CSI/csisfs.h"
#else
#error "Unknown BOA version defined"
#endif

/* Macro definitions */
#if BOA_VERSION == BOA_VERSION_1_4
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.4"
#define LIB_CSL_NAME    "dll-csiBind_1_4.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_4.dll"
#elif BOA_VERSION == BOA_VERSION_1_5
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.5"
#define LIB_CSL_NAME    "dll-csiBind_1_5.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_5.dll"
#elif BOA_VERSION == BOA_VERSION_2_0
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\2"
#define LIB_CSL_NAME    "dll-csiBind.dll"
#define LIB_OCI_NAME    "dll-ocdProxy.dll"
#else
#error "Unknown BOA version defined"
#endif

/**
 * Declarations of CSI API pointers
 */
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC1)(const char* uriName, CSI_NodeRange range, CSI_Tree* *tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC2)(CSI_Tree* tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC3)(CSI_Tree* tree, const BOA_UuidVersion* uuid, OCI_URIName uriName[], int size, int* count);
typedef OCI_ErrorCode (*PF_OCI_CreateFlexRayControllerVersion)  (const char*  uriLocation,const BOA_Version* apiVersion,OCI_ControllerHandle* controller );
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
    OCI_FLEXRAY_VTable   m_sOCI;
#if BOA_VERSION >= BOA_VERSION_2_0
    PF_OCI_CreateFlexRayControllerVersion createFlexRayController;
#endif
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
    CBaseFLEXBufFSE* m_pClientBuf[MAX_BUFF_ALLOWED];
    std::string m_acClientName;
    UINT m_unBufCount;
    SCLIENTBUFMAP()
    {
        m_dwClientID = 0;
        m_unBufCount = 0;
        m_acClientName = "";

        for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            m_pClientBuf[i] = nullptr;
        }
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
static FLEXRAY_INTERFACE_HW sg_asInterfaceHw[MAX_HW_INTERFACE] = {'\0'};
static int sg_nTotalHwIntf = 0;
// Handle of the XAT library
static HMODULE  sg_hBOALib              = nullptr;
// Handle of the driver library
static HMODULE  sg_hBOAHWLib              = nullptr;

/* Application buffer, logging interface and client id */
static CPARAM_THREADPROC sg_sParmRThread;
static Base_WrapperErrorLogger* sg_pIlog   = nullptr;

// Buffer to store transmission message list
static CFlxTxMsgBuffer sg_ouFlxTxList;
static OCI_FlexRayTxMessage* sg_psBOA_FlxMsgList = nullptr;

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

static std::string sg_acErrStr = "";
static HRESULT sg_hLastError = S_OK;


class FlexrayData
{
public:
    OCI_FlexRayMessage m_OCI_FlexRayMessageArray[OCI_FLEXRAY_MAX_BUFFERCOUNT];
    int m_nValidMessages;
    std::map<SLOT_BASECYCLE, int> m_mapSlotBaseCycle;
};
class BOA_FlexRayData
{
public:
    OCI_FlexRayTxQueueConfiguration m_OCI_NewTxQueueCfg;
    OCI_FlexRayMessage m_OCI_FlexRayMessageArray;
    OCI_QueueHandle m_OCI_NewTxQueueHandle;

};

typedef struct tagCHANNEL
{
    OCI_URIName m_acURI; /*< URI of the Controller */
    OCI_ControllerHandle m_OCI_HwHandle; /*< Controller handle */
    OCI_FlexRayConfigurationEx m_OCI_FlexRayConfig; /*< Controller configuration */
    // Creating OCI_CANFD_Configuration only if BOA_FD_VERSION is defined

    OCI_FlexRayControllerProperties m_OCI_CntrlProp; /*< Controller properties */
    OCI_QueueHandle m_OCI_RxQueueHandle; /*< Controller receive queue handle */
    OCI_FlexRayRxQueueConfiguration m_OCI_RxQueueCfg; /*< Controller receive queue configuration */
    OCI_QueueHandle m_OCI_TxQueueHandle; /*< Controller transmit queue handle */
    OCI_FlexRayTxQueueConfiguration m_OCI_TxQueueCfg; /*< Controller transmit queue configuration */
    OCI_FlexRayRxFilter m_OCI_FrameFilter; /*< Controller frame filter */
    OCI_FlexRayEventFilter m_OCI_EventFilter; /*< Controller event filter */


    std::map<INT, INT> m_SlotBufferMap;

    FlexrayData m_ouFlexrayData;

    UINT m_unMsgConfig;
    UINT m_unCycleTime;
    // OCI_FlexRayErrorFrameFilter m_OCI_ErrorFilter; /*< Controller error filter */
    OCI_InternalErrorEventFilter m_OCI_InternalErrorFilter;
    UCHAR m_ucControllerState; /*< Controller state */
    UCHAR m_ucTxErrorCounter; /*< Controller Tx error counter */
    UCHAR m_ucRxErrorCounter; /*< Controller Rx error counter */
    UCHAR m_ucPeakRxErrorCounter; /*< Controller peak Rx error counter */
    UCHAR m_ucPeakTxErrorCounter; /*< Controller peak Tx error counter */
    OCI_TimerCapabilities m_OCI_TimerCapabilities;
    FLOAT m_fResolution;

    OCI_FlexRayControllerCapabilities m_ControllerCapabilities;
    CRITICAL_SECTION m_ouCriticalSection;

    //Thread
    CPARAM_THREADPROC m_ouDataTransmitThread;
    std::map<SLOT_BASECYCLE, BOA_FlexRayData> m_ouBoaFlexRayData;
    int nValid;

    tagCHANNEL()
    {
        //OCI Transfer Data
        //memset(&m_OCI_FlexRayMessageArray, 0, OCI_FLEXRAY_MAX_BUFFERCOUNT * sizeof(OCI_FlexRayMessage));
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

