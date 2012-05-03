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
 * \file      CAN_ETAS_BOA/CAN_ETAS_BOA.cpp
 * \brief     Source file for BOA DIL functions
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for BOA DIL functions
 * Defines the initialization routines for the DLL.
 */

/* C includes */
#include <search.h>             /* For qsort */

/* C++ includes */
#include <string>

/* Project includes */
#include "CAN_ETAS_BOA_stdafx.h"
#include "CAN_ETAS_BOA.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "EXTERNAL_INCLUDE/OCI/ocican.h"
#include "EXTERNAL_INCLUDE/CSI/csisfs.h"
#include "Include/CanUsbDefs.h"
#include "Include/Can_Error_Defs.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "HardwareListing.h"
#include "ChangeRegisters_CAN_ETAS_BOA.h"

#define USAGE_EXPORT
#include "CAN_ETAS_BOA_Extern.h"

using namespace std;

BEGIN_MESSAGE_MAP(CCAN_ETAS_BOA, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_ETAS_BOA construction
 */
CCAN_ETAS_BOA::CCAN_ETAS_BOA()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_ETAS_BOA object
CCAN_ETAS_BOA theApp;


/**
 * CCAN_ETAS_BOA initialization
 */
BOOL CCAN_ETAS_BOA::InitInstance()
{
    CWinApp::InitInstance();
    return TRUE;
}

const BYTE FILTER_ADD = 0x01;
const BYTE FILTER_REMOVE = 0x02;
const BYTE QUEUE_ADD = 0x01;
const BYTE QUEUE_DESTROY = 0x02;

/**
 * Channel information
 */
typedef struct tagCHANNEL
{
    OCI_URIName m_acURI; /*< URI of the Controller */
    OCI_ControllerHandle m_OCI_HwHandle; /*< Controller handle */
    OCI_CANConfiguration m_OCI_CANConfig; /*< Controller configuration */
    OCI_CANControllerProperties m_OCI_CntrlProp; /*< Controller properties */
    OCI_QueueHandle m_OCI_RxQueueHandle; /*< Controller receive queue handle */
    OCI_CANRxQueueConfiguration m_OCI_RxQueueCfg; /*< Controller receive queue configuration */
    OCI_QueueHandle m_OCI_TxQueueHandle; /*< Controller transmit queue handle */
    OCI_CANTxQueueConfiguration m_OCI_TxQueueCfg; /*< Controller transmit queue configuration */
    OCI_CANRxFilter m_OCI_FrameFilter; /*< Controller frame filter */
    OCI_CANEventFilter m_OCI_EventFilter; /*< Controller event filter */
    OCI_CANErrorFrameFilter m_OCI_ErrorFilter; /*< Controller error filter */
    OCI_InternalErrorEventFilter m_OCI_InternalErrorFilter;
    UCHAR m_ucControllerState; /*< Controller state */
    UCHAR m_ucTxErrorCounter; /*< Controller Tx error counter */
    UCHAR m_ucRxErrorCounter; /*< Controller Rx error counter */
    UCHAR m_ucPeakRxErrorCounter; /*< Controller peak Rx error counter */
    UCHAR m_ucPeakTxErrorCounter; /*< Controller peak Tx error counter */
    OCI_TimerCapabilities m_OCI_TimerCapabilities;
    FLOAT m_fResolution;
} SCHANNEL;

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
    CBaseCANBufFSE* m_pClientBuf[MAX_BUFF_ALLOWED];
    string m_acClientName;
    UINT m_unBufCount;
    SCLIENTBUFMAP()
    {
        m_dwClientID = 0;
        m_unBufCount = 0;
        m_acClientName = "";

        for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            m_pClientBuf[i] = NULL;
        }
    }
};

/**
 * Array of clients
 */
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

const INT MAX_MAP_SIZE = 3000;

typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)
    {
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel));
    }
} SACK_MAP;

typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;
static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe

/**
 * Channel instances
 */
static SCHANNEL sg_asChannel[defNO_OF_CHANNELS];

/**
 * Number of current channel selected
 */
static UINT sg_nNoOfChannels = 0;

static BOOL sg_bIsDriverRunning = FALSE;

/**
 * Controller configuration details
 */
static SCONTROLLER_DETAILS sg_asControllerDets[defNO_OF_CHANNELS];

static HWND sg_hOwnerWnd = NULL;

const BYTE CREATE_MAP_TIMESTAMP = 0x1;

const BYTE CALC_TIMESTAMP_READY = 0x2;

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

static HANDLE sg_hEvent = NULL;

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
static HMODULE sg_hLibCSI = NULL;
static HMODULE sg_hLibOCI = NULL;

/**
 * Declarations of CSI API pointers
 */
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC1)(const char* uriName, CSI_NodeRange range, CSI_Tree* *tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC2)(CSI_Tree* tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC3)(CSI_Tree* tree, const BOA_UuidVersion* uuid, OCI_URIName uriName[], int size, int* count);

/**
 * Macro definitions
 */
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.4"
#define LIB_CSL_NAME    "dll-csiBind_1_4.dll"
#define LIB_OCI_NAME    "dll-ocdProxy_1_4.dll"

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
    OCI_CAN_VTable   m_sOCI;
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

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

/* CDIL_CAN_ETAS_BOA class definition */
class CDIL_CAN_ETAS_BOA : public CBaseDIL_CAN_Controller
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
void vBlinkHw(INTERFACE_HW s_HardwareIntr)
{
    OCI_ControllerHandle ouOCI_HwHandle;
    BOA_ResultCode err =  (*(sBOA_PTRS.m_sOCI.createCANController))(s_HardwareIntr.m_acNameInterface.c_str(),
                          &(ouOCI_HwHandle));

    if (err == OCI_SUCCESS)
    {
        SCHANNEL s_asChannel;
        strcpy_s(s_asChannel.m_acURI, s_HardwareIntr.m_acNameInterface.c_str());
        s_asChannel.m_OCI_RxQueueCfg.onFrame.userData = (void*)ouOCI_HwHandle;
        s_asChannel.m_OCI_RxQueueCfg.onEvent.userData = (void*)ouOCI_HwHandle;
        //configure the controller first
        BOA_ResultCode ErrorCode = (*(sBOA_PTRS.m_sOCI.openCANController))(ouOCI_HwHandle,
                                   &(s_asChannel.m_OCI_CANConfig),
                                   &(s_asChannel.m_OCI_CntrlProp));

        //HRESULT hResult = S_OK;
        if (ErrorCode == OCI_SUCCESS)
        {
            Sleep(500);

            if ((*(sBOA_PTRS.m_sOCI.closeCANController))(ouOCI_HwHandle) == OCI_SUCCESS)
            {
                (*(sBOA_PTRS.m_sOCI.destroyCANController))(ouOCI_HwHandle) == OCI_SUCCESS;
            }
        }
    }
}
static CDIL_CAN_ETAS_BOA* sg_pouDIL_CAN_ETAS_BOA = NULL;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;

    if ( NULL == sg_pouDIL_CAN_ETAS_BOA )
    {
        if ((sg_pouDIL_CAN_ETAS_BOA = new CDIL_CAN_ETAS_BOA) == NULL)
        {
            hResult = S_FALSE;
        }
    }

    *ppvInterface = (void*) sg_pouDIL_CAN_ETAS_BOA;  /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */
    return hResult;
}

/**
 * Error logger
 */
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

/**
 * Declarations of ProcessCanData. This is a call back function
 * which will be called by BOA framework whenever there is a msg
 */
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessage* msg);

/**
 * Declarations of ProcessEvents. This is a call back function
 * which will be called by BOA framework whenever there are internal events
 */
static void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessage* msg);

/**
 * CallBack function used by the qsort Function
**/

INT nCallBackStrCompareFn( const void* str1, const void* str2)
{
    return( strcmp((char*)str1,(char*)str2) );
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;

    for (UINT i = 0; i < sClientObj.m_unBufCount; i++)
    {
        if (pBuf == sClientObj.m_pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.m_unBufCount; //break the loop
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
            if (i < (unCount - 1)) //If not the last bufffer
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }

            unCount--;
        }
    }

    return bReturn;
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
BOOL bGetBOAInstallationPath(char* pcPath, INT& nSize)
{
    USES_CONVERSION;
    BOOL bResult = FALSE;
    LONG lError = 0;
    HKEY sKey;
    BYTE acGCCPath[1024];
    DWORD dwSize = sizeof(BYTE[1024]) ;
    ULONG ulType = REG_SZ;
    // Get the installation path for BOA 1.4
    lError = RegOpenKeyEx( HKEY_LOCAL_MACHINE, BOA_REGISTRY_LOCATION, 0, KEY_READ, &sKey);

    // If the registry key open successfully, get the value in "path"
    // sub key
    if(lError==ERROR_SUCCESS)
    {
        lError = RegQueryValueEx(sKey,"path",0, &ulType, acGCCPath,&dwSize);
        nSize = (INT)dwSize;
        strcpy(pcPath, A2T((char*)acGCCPath));
        RegCloseKey(sKey);
        bResult = TRUE;
    }

    return bResult;
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
HRESULT GetCSI_API_Pointers(HMODULE hLibCSI)
{
    HRESULT hResult = S_OK;

    if (hLibCSI != NULL)
    {
        if ((sBOA_PTRS.m_sCSI.createProtocolTree        = (PROC1)GetProcAddress(hLibCSI, "CSI_CreateProtocolTree")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.destroyProtocolTree  = (PROC2)GetProcAddress(hLibCSI, "CSI_DestroyProtocolTree")) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.getUriForUuid        = (PROC3)GetProcAddress(hLibCSI, "CSI_GetUriForUuid")) == NULL)
        {
            hResult = S_FALSE;
        }
    }

    return hResult;
}

/**
 * Gets the OCI API function pointer from the ocdproxy.dll
 */
HRESULT GetOCI_API_Pointers(HMODULE hLibOCI)
{
    HRESULT hResult = S_OK;

    if (hLibOCI != NULL)
    {
        if ((sBOA_PTRS.m_sOCI.createCANController = (PF_OCI_CreateCANController)
                GetProcAddress(hLibOCI, _T("OCI_CreateCANController"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.destroyCANController = (PF_OCI_DestroyCANController)
                GetProcAddress(hLibOCI, _T("OCI_DestroyCANController"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.openCANController = (PF_OCI_OpenCANController)
                GetProcAddress(hLibOCI, _T("OCI_OpenCANController"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.closeCANController = (PF_OCI_CloseCANController)
                GetProcAddress(hLibOCI, _T("OCI_CloseCANController"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.getCANConfiguration = (PF_OCI_GetCANConfiguration)
                GetProcAddress(hLibOCI, _T("OCI_GetCANConfiguration"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.getCANControllerProperties = (PF_OCI_GetCANControllerProperties)
                GetProcAddress(hLibOCI, _T("OCI_GetCANControllerProperties"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.setCANControllerProperties = (PF_OCI_SetCANControllerProperties)
                GetProcAddress(hLibOCI, _T("OCI_SetCANControllerProperties"))) == NULL)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.canioVTable.getCANControllerCapabilities = (PF_OCI_GetCANControllerCapabilities)
                GetProcAddress(hLibOCI, _T("OCI_GetCANControllerCapabilities"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.getCANControllerStatus = (PF_OCI_GetCANControllerStatus)
                  GetProcAddress(hLibOCI, _T("OCI_GetCANControllerStatus"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.createCANTxQueue = (PF_OCI_CreateCANTxQueue)
                  GetProcAddress(hLibOCI, _T("OCI_CreateCANTxQueue"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.writeCANData = (PF_OCI_WriteCANData)
                  GetProcAddress(hLibOCI, _T("OCI_WriteCANData"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.destroyCANTxQueue = (PF_OCI_DestroyCANTxQueue)
                  GetProcAddress(hLibOCI, _T("OCI_DestroyCANTxQueue"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.createCANRxQueue = (PF_OCI_CreateCANRxQueue)
                  GetProcAddress(hLibOCI, _T("OCI_CreateCANRxQueue"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.readCANData = (PF_OCI_ReadCANData)
                  GetProcAddress(hLibOCI, _T("OCI_ReadCANData"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.destroyCANRxQueue = (PF_OCI_DestroyCANRxQueue)
                  GetProcAddress(hLibOCI, _T("OCI_DestroyCANRxQueue"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANFrameFilter = (PF_OCI_AddCANFrameFilter)
                  GetProcAddress(hLibOCI, _T("OCI_AddCANFrameFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANFrameFilter = (PF_OCI_RemoveCANFrameFilter)
                  GetProcAddress(hLibOCI, _T("OCI_RemoveCANFrameFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANBusEventFilter = (PF_OCI_AddCANBusEventFilter)
                  GetProcAddress(hLibOCI, _T("OCI_AddCANBusEventFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANBusEventFilter = (PF_OCI_RemoveCANBusEventFilter)
                  GetProcAddress(hLibOCI, _T("OCI_RemoveCANBusEventFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANErrorFrameFilter = (PF_OCI_AddCANErrorFrameFilter)
                  GetProcAddress(hLibOCI, _T("OCI_AddCANErrorFrameFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter = (PF_OCI_RemoveCANErrorFrameFilter)
                  GetProcAddress(hLibOCI, _T("OCI_RemoveCANErrorFrameFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, _T("OCI_GetTimerStatus"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, _T("OCI_GetTimerStatus"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities = (PF_OCI_GetTimerCapabilities)
                  GetProcAddress(hLibOCI, _T("OCI_GetTimerCapabilities"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerValue = (PF_OCI_GetTimerValue)
                  GetProcAddress(hLibOCI, _T("OCI_GetTimerValue"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.addTimerEventFilter = (PF_OCI_AddTimerEventFilter)
                  GetProcAddress(hLibOCI, _T("OCI_AddTimerEventFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.removeTimerEventFilter = (PF_OCI_RemoveTimerEventFilter)
                  GetProcAddress(hLibOCI, _T("OCI_RemoveTimerEventFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.getError = (PF_OCI_GetError)
                  GetProcAddress(hLibOCI, _T("OCI_GetError"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter = (PF_OCI_AddInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, _T("OCI_AddInternalErrorEventFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter = (PF_OCI_RemoveInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, _T("OCI_RemoveInternalErrorEventFilter"))) == NULL)
        {
            hResult = S_FALSE;
        }
    }

    return hResult;
}

/**
 * Search for all connected Hardware, that supports the OCI
 * CAN interface and deliver the URI location of the hardware.
 */
BOA_ResultCode OCI_FindCANController(OCI_URIName uriName[], INT nSize, INT* nFound)
{
    OCI_ErrorCode ec;
    /* Container for search results */
    CSI_Tree* sfsTree = NULL;
    /* Specify that we want to search for nodes which implement v1.1.0.0 of OCI_CAN. */
    static const BOA_UuidVersion ociCanUuid = { UUID_OCICAN, {1,1,0,0} };
    /* Specify that we want to search for any kind of node, not just hardware nodes */
    const CSI_NodeRange nodeRange = {CSI_NODE_MIN, CSI_NODE_MAX};
    /* Search for all connected hardware and latch the result for further processing */
    ec = (*(sBOA_PTRS.m_sCSI.createProtocolTree))("", nodeRange, &sfsTree);

    if (ec == OCI_SUCCESS)
    {
        /* Find the URIs for all nodes which implement v1.1.0.0 of OCI_CAN. */
        ec = (*(sBOA_PTRS.m_sCSI.getUriForUuid))(sfsTree, &ociCanUuid, uriName, nSize, nFound);

        if (ec == OCI_SUCCESS)
        {
            ec = (*(sBOA_PTRS.m_sCSI.destroyProtocolTree))(sfsTree);
        }
    }

    return ec;
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
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
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
static BOOL bClientExist(char* pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName, sg_asClientToBufMap[i].m_acClientName.c_str()))
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
            sg_asClientToBufMap[unClientIndex].m_dwClientID = 0;
            sg_asClientToBufMap[unClientIndex].m_acClientName = "";

            for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].m_pClientBuf[i] = NULL;
            }

            sg_asClientToBufMap[unClientIndex].m_unBufCount = 0;

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
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientId)
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
static DWORD dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;

    for (INT i = 0; i < MAX_CLIENT_ALLOWED; i++)
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

/**
 * Pushes an entry into the list at the last position
 */
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
        ClientID = (*iResult).m_ClientID;
        sg_asAckMapBuf.erase(iResult);
    }

    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}

/**
 * \param[in] nChannel Channel information
 *
 * Initialize default values for controller configuration
 */
void vInitializeControllerConfig(UINT nChannel)
{
    sg_asChannel[nChannel].m_OCI_CANConfig.baudrate = 500000;
    sg_asChannel[nChannel].m_OCI_CANConfig.samplePoint = 70;
    sg_asChannel[nChannel].m_OCI_CANConfig.samplesPerBit = OCI_CAN_THREE_SAMPLES_PER_BIT;
    sg_asChannel[nChannel].m_OCI_CANConfig.BTL_Cycles = 10;
    sg_asChannel[nChannel].m_OCI_CANConfig.SJW = 4;
    sg_asChannel[nChannel].m_OCI_CANConfig.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
    sg_asChannel[nChannel].m_OCI_CANConfig.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
    sg_asChannel[nChannel].m_OCI_CANConfig.selfReceptionMode = OCI_SELF_RECEPTION_ON;
    //Set controller property to SUSPENDED
    sg_asChannel[nChannel].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;
}

/**
 * \param[in] nChannel Channel information
 *
 * Initialize default values for queue confgiuration
 */
void vInitializeQueueConfig(UINT nChannel)
{
    /* configure Rx Queue*/
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.function = ProcessCanData;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.function = ProcessEvents;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.selfReceptionMode = OCI_SELF_RECEPTION_ON;
    /* configure Tx Queue*/
    sg_asChannel[nChannel].m_OCI_TxQueueCfg.reserved = 0;
}

/**
 * \param[in] nChannel Channel information
 *
 * Initialize default values for filter confgiuration
 */
void vInitializeFilterConfig(UINT nChannel)
{
    /* configure frame filter*/
    sg_asChannel[nChannel].m_OCI_FrameFilter.frameIDMask = 0;
    sg_asChannel[nChannel].m_OCI_FrameFilter.frameIDValue = 0;
    sg_asChannel[nChannel].m_OCI_FrameFilter.tag = 0;
    /* configure frame filter*/
    sg_asChannel[nChannel].m_OCI_EventFilter.destination = 0;
    sg_asChannel[nChannel].m_OCI_EventFilter.eventCode = 0;
    sg_asChannel[nChannel].m_OCI_EventFilter.tag = 0;
    /* configure Error filter */
    sg_asChannel[nChannel].m_OCI_ErrorFilter.destination = 0;
    sg_asChannel[nChannel].m_OCI_ErrorFilter.errorFrame = 0;
    sg_asChannel[nChannel].m_OCI_ErrorFilter.tag = 0;
    /* configure internal error filter */
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.eventCode = OCI_INTERNAL_GENERAL_ERROR;
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.tag = 0;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Adds or removes the HW filter to/from the channel.
 */
HRESULT ManageFilters(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode ErrCode = OCI_FAILURE;

    if (byCode == FILTER_ADD)
    {
        // Add Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANFrameFilter))
                  (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                   &(sg_asChannel[nChannel].m_OCI_FrameFilter), 1);

        if (ErrCode == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not add frame filter"));
        }

        // Add Event filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANBusEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not add Event filter"));
            }
        }

        // Add Error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANErrorFrameFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_ErrorFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not add Error filter"));
            }
        }

        // Add internal error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not add Error filter"));
            }
        }
    }
    else if (byCode == FILTER_REMOVE)
    {
        // Remove Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANFrameFilter))
                  (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                   &(sg_asChannel[nChannel].m_OCI_FrameFilter), 1);

        if (ErrCode == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not remove frame filter"));
        }

        // Remove Event filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANBusEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not remove Event filter"));
            }
        }

        // Remove Error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_ErrorFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not remove Error filter"));
            }
        }

        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not add Error filter"));
            }
        }
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Adds or removes the queue to/from the channel.
 */
HRESULT ManageQueue(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode Err = OCI_ERR_FLAG_ERROR;

    if (byCode == QUEUE_ADD)
    {
        //Create CAN Rx queue
        Err = (*(sBOA_PTRS.m_sOCI.canioVTable.createCANRxQueue))
              (sg_asChannel[nChannel].m_OCI_HwHandle,
               &(sg_asChannel[nChannel].m_OCI_RxQueueCfg),
               &(sg_asChannel[nChannel].m_OCI_RxQueueHandle));

        if (Err == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not create Rx Queue"));
        }

        //Create CAN Tx queue
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.canioVTable.createCANTxQueue))
                  (sg_asChannel[nChannel].m_OCI_HwHandle,
                   &(sg_asChannel[nChannel].m_OCI_TxQueueCfg),
                   &(sg_asChannel[nChannel].m_OCI_TxQueueHandle));

            if (Err != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not create TX Queue"));
            }
        }
    }
    else if (byCode == QUEUE_DESTROY)
    {
        //Destroy CAN Rx queue
        Err = (*(sBOA_PTRS.m_sOCI.canioVTable.destroyCANRxQueue))(sg_asChannel[nChannel].m_OCI_RxQueueHandle);

        if (Err == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not create Rx Queue"));
        }

        //Create CAN Tx queue
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.canioVTable.destroyCANTxQueue))(sg_asChannel[nChannel].m_OCI_TxQueueHandle);

            if (Err != OCI_SUCCESS)
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not create TX Queue"));
            }
        }
    }

    return hResult;
}

/**
 * copies from OCI_CANRxMessage struct into STCANDATA struct
 */
void vCopyOCI_CAN_RX_2_DATA(const OCI_CANRxMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->dlc;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) == OCI_CAN_MSG_FLAG_EXTENDED)? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_REMOTE_FRAME) == OCI_CAN_MSG_FLAG_REMOTE_FRAME)? 1 : 0;
    DestMsg->m_ucDataType   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) == OCI_CAN_MSG_FLAG_SELFRECEPTION)? TX_FLAG : RX_FLAG;
    UINT Channel = DestMsg->m_uDataInfo.m_sCANMsg.m_ucChannel;

    if (Channel > defNO_OF_CHANNELS)
    {
        Channel = 1;//Take appropriate action
    }

    DestMsg->m_lTickCount.QuadPart = (LONGLONG)(SrcMsg->timeStamp * sg_asChannel[Channel - 1].m_fResolution);
    memcpy(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData, SrcMsg->data, sizeof(UCHAR) * 8);
}

/**
 * \return Returns channel number
 *
 * Gets the channel with handle OCI_ControllerHandle
 */
UINT nGetChannel(const OCI_ControllerHandle hHandle)
{
    UINT i;

    for (i = 0; i < sg_nNoOfChannels; i++)
    {
        if (sg_asChannel[i].m_OCI_HwHandle == hHandle)
        {
            break;
        }
    }

    return (i+1);
}

/**
 * Create time mod mapping
 */
void vCreateTimeModeMapping(HANDLE hEvent)
{
    WaitForSingleObject(hEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    //Query Tick Count
    QueryPerformanceCounter(&sg_QueryTickCount);
}

/**
 * This function writes the message to the corresponding clients buffer
 */
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    //Write into the client's buffer and Increment message Count
    if (sCanData.m_ucDataType == TX_FLAG)
    {
        static SACK_MAP sAckMap;
        UINT ClientId = 0;
        static UINT Index = (UINT)-1;
        sAckMap.m_Channel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
        sAckMap.m_MsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;

        if (bRemoveMapEntry(sAckMap, ClientId))
        {
            BOOL bClientExists = bGetClientObj(ClientId, Index);

            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                //Tx for monitor nodes and sender node
                if ((i == CAN_MONITOR_NODE_INDEX)  || (bClientExists && (i == Index)))
                {
                    for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
                    {
                        sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sCanData);
                    }
                }
                else
                {
                    //Send the other nodes as Rx.
                    for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
                    {
                        static STCANDATA sTempCanData;
                        sTempCanData = sCanData;
                        sTempCanData.m_ucDataType = RX_FLAG;
                        sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sTempCanData);
                    }
                }
            }
        }
    }
    else // provide it to everybody
    {
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
            {
                sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sCanData);
            }
        }
    }
}

/**
 * Processes Rx msg and writes into regiastered clients buffer.
 */
void vProcessRxMsg(void* userData, struct OCI_CANMessage* msg)
{
    /* First calculate timestamp for first message*/
    EnterCriticalSection(&sg_DIL_CriticalSection);
    static STCANDATA sCanData;
    static OCI_ControllerHandle hHandle;
    hHandle = (OCI_ControllerHandle)userData;
    sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)nGetChannel(hHandle);
    vCopyOCI_CAN_RX_2_DATA(&(msg->data.rxMessage), &sCanData);

    if (sg_byCurrState == CREATE_MAP_TIMESTAMP)
    {
        //sg_TimeStamp = sCanData.m_lTickCount.QuadPart;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);
        sg_byCurrState = CALC_TIMESTAMP_READY;
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
    }

    //Write the msg into registered client's buffer
    vWriteIntoClientsBuffer(sCanData);
    LeaveCriticalSection(&sg_DIL_CriticalSection);
}

/**
 * Processes Tx msg.
 */
void vProcessTxMsg(void* /*userData*/, OCI_CANMessage* /*msg*/)
{
}

/**
 * Copies OCI_CANErrorFrameMessage struct into STCANDATA.
 */
void vCopyOCI_CAN_ERR_2_DATA(const OCI_CANErrorFrameMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->dlc;
    DestMsg->m_lTickCount.QuadPart = SrcMsg->timeStamp;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) == OCI_CAN_MSG_FLAG_EXTENDED)? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_REMOTE_FRAME) == OCI_CAN_MSG_FLAG_REMOTE_FRAME)? 1 : 0;
    DestMsg->m_ucDataType = ERR_FLAG;
    BOOL bIsTx = (SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) == OCI_CAN_MSG_FLAG_SELFRECEPTION;

    if (bIsTx)
    {
        DestMsg->m_uDataInfo.m_sErrInfo.m_ucTxErrCount += 1;
    }
    else
    {
        DestMsg->m_uDataInfo.m_sErrInfo.m_ucRxErrCount += 1;
    }

    switch (SrcMsg->type)
    {
        case OCI_CAN_ERR_TYPE_BITSTUFF:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = STUFF_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = STUFF_ERROR_RX;
            }
        }
        break;

        case OCI_CAN_ERR_TYPE_FORMAT:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = FORM_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = FORM_ERROR_RX;
            }
        }
        break;

        case OCI_CAN_ERR_TYPE_BIT:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = BIT_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = BIT_ERROR_RX;
            }
        }
        break;

        case OCI_CAN_ERR_TYPE_OTHER:
        {
            if (bIsTx)
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = OTHER_ERROR_TX;
            }
            else
            {
                DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = OTHER_ERROR_RX;
            }
        }
        break;

        case OCI_CAN_ERR_TYPE_OVERLOAD:
        case OCI_CAN_ERR_TYPE_CRC:
        case OCI_CAN_ERR_TYPE_BIT_DOMINANT_BUT_RECSV:
        case OCI_CAN_ERR_TYPE_ACK:
        case OCI_CAN_ERR_TYPE_BIT_RECSV_BUT_DOMINANT:
        default:
        {
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_UNKNOWN;
        }
        break;
    }
}

/**
 * Updates Tx & Rx error counter of the nChannel
 */
void vUpdateErrorCounter(UCHAR ucTxError, UCHAR ucRxError, UINT nChannel)
{
    // Update Tx Error counter and peak Tx Error Counter
    sg_asChannel[nChannel].m_ucTxErrorCounter = sg_asChannel[nChannel].m_ucTxErrorCounter + ucTxError;

    // Update Peak Value
    if( ucTxError > sg_asChannel[nChannel].m_ucPeakTxErrorCounter )
    {
        sg_asChannel[nChannel].m_ucPeakTxErrorCounter = ucTxError;
    }

    // Update Rx Error counter and peak Rx Error Counter
    sg_asChannel[nChannel].m_ucRxErrorCounter = sg_asChannel[nChannel].m_ucRxErrorCounter + ucRxError;

    // Update Peak Value
    if( ucRxError > sg_asChannel[nChannel].m_ucPeakRxErrorCounter )
    {
        sg_asChannel[nChannel].m_ucPeakRxErrorCounter = ucRxError;
    }
}

/**
 * Processes error msg and writes into registered clients buffer.
 */
void vProcessErrMsg(void* userData, struct OCI_CANMessage* msg)
{
    static STCANDATA sCanData;
    vCopyOCI_CAN_ERR_2_DATA(&(msg->data.errorFrameMessage), &sCanData);
    int32* pUserData = (int32*)userData;
    sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)nGetChannel(*pUserData);
    sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
    // Update channel error counter
    vUpdateErrorCounter(sCanData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount,
                        sCanData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount,
                        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel);

    //Write the msg into registered client's buffer
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; j++)
        {
            sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sCanData);
        }
    }
}

/**
 * Callback function called by the BOA framework
 * when there is presence of msg in the bus.
 */
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessage* msg)
{
    switch (msg->type)
    {
        case OCI_CAN_RX_MESSAGE:
            vProcessRxMsg(userData, msg);
            break;

        case OCI_CAN_TX_MESSAGE:
            vProcessTxMsg(userData, msg);
            break;

        case OCI_CAN_ERROR_FRAME:
            vProcessErrMsg(userData, msg);
            break;
    }
}

/**
 * processes bus event.
 */
static void vProcessBusEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
{
}

/**
 * processes internal event.
 */
void vProcessInternalErrEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
{
}

/**
 * process queue event.
 */
void vProcessQueueEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
{
}

/**
 * process tmer event.
 */
void vProcessTimerEvent(void* /*userData*/, struct OCI_CANMessage* msg)
{
    switch (msg->data.timerEventMessage.eventCode)
    {
        case OCI_TIMER_EVENT_SYNC_LOCK:
            SetEvent(sg_hEvent);
            vCreateTimeModeMapping(sg_hEvent);
            break;

        case OCI_TIMER_EVENT_SYNC_LOSS:
            //Nothing at this moment
            break;
    }
}

/**
 * Callback function called by the BOA framework
 * when there is internal bus event.
 */
void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessage* msg)
{
    switch (msg->type)
    {
        case OCI_CAN_BUS_EVENT:
            vProcessBusEvent(userData, msg);
            break;

        case OCI_CAN_INTERNAL_ERROR_EVENT:
            vProcessInternalErrEvent(userData, msg);
            break;

        case OCI_CAN_QUEUE_EVENT:
            vProcessQueueEvent(userData, msg);
            break;

        case OCI_CAN_TIMER_EVENT:
            vProcessTimerEvent(userData, msg);
            break;
    }
}

/**
 * \return Returns S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;
    return S_OK;
}

/**
 * \return Returns S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_UnloadDriverLibrary(void)
{
    /* Unload OCI library */
    if (sg_hLibOCI != NULL)
    {
        FreeLibrary(sg_hLibOCI);
    }

    /* Unload CSI library */
    if (sg_hLibCSI != NULL)
    {
        FreeLibrary(sg_hLibCSI);
    }

    /* Invalidate all API pointers */
    memset(&sBOA_PTRS, 0, sizeof (SBOA_POINTER_TABLE));
    return S_OK;
}

/**
 * \return Returns S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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
                if (pBufObj != NULL)
                {
                    if (sClientObj.m_unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.m_pClientBuf[sClientObj.m_unBufCount++] = pBufObj;
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
                //clear msg buffer
                if (pBufObj != NULL) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.m_pClientBuf, sClientObj.m_unBufCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.m_unBufCount; i++)
                    {
                        sClientObj.m_pClientBuf[i] = NULL;
                    }

                    sClientObj.m_unBufCount = 0;
                }

                hResult = S_OK;
            }
            else
            {
                ////ASSERT(FALSE);
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
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].m_dwClientID, NULL);
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
HRESULT CDIL_CAN_ETAS_BOA::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
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
                    sg_asClientToBufMap[0].m_acClientName = pacClientName;
                    sg_asClientToBufMap[0].m_dwClientID = ClientID;
                    sg_asClientToBufMap[0].m_unBufCount = 0;
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
                    sg_asClientToBufMap[Index].m_acClientName = pacClientName;
                    sg_asClientToBufMap[Index].m_dwClientID = ClientID;
                    sg_asClientToBufMap[Index].m_unBufCount = 0;
                }

                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].m_dwClientID;
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
HRESULT CDIL_CAN_ETAS_BOA::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& unCntrlStatus)
{
    unCntrlStatus = defCONTROLLER_ACTIVE; //Temporary solution. TODO
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_LoadDriverLibrary(void)
{
    HRESULT hResult = S_FALSE;
    /* Get BOA installation path from the registery */
    char acPath[MAX_PATH] = {'\0'};
    INT nSize = 0;
    bGetBOAInstallationPath(acPath, nSize);
    /* Load cslproxy.dll library */
    char acLIB_CSL[MAX_PATH] = {'\0'};
    sprintf_s(acLIB_CSL, sizeof(acLIB_CSL), "%s\\%s", acPath, LIB_CSL_NAME);
    /* LoadLibraryEx instead of LoadLibrary seems to be necessary under Windows 7 when the library is not in DLL search path (system32) */
    sg_hLibCSI = LoadLibraryEx(acLIB_CSL, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (sg_hLibCSI != NULL)
    {
        hResult = GetCSI_API_Pointers(sg_hLibCSI);

        /* Load the OCI library to use CAN controller */
        if (hResult == S_OK)
        {
            char acLIB_OCI[MAX_PATH] = {'\0'};
            sprintf_s(acLIB_OCI, sizeof(acLIB_OCI), "%s\\%s", acPath, LIB_OCI_NAME);
            sg_hLibOCI = LoadLibraryEx(acLIB_OCI, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

            if (sg_hLibOCI != NULL)
            {
                hResult = GetOCI_API_Pointers(sg_hLibOCI);

                if (hResult != S_OK)
                {
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not get OCI function pointers"));
                }
                else
                {
                    sg_bIsDriverRunning = TRUE;
                }
            }
            else
            {
                hResult = S_FALSE;
                sprintf_s(acPath, sizeof(acPath), _T("%s failed to load"), acLIB_OCI);
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acPath);
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, "Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support");
            }
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not get CSI function pointers"));
        }
    }
    else
    {
        sprintf_s(acPath, sizeof(acPath), _T("%s failed to load"), acLIB_CSL);
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acPath);
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, "Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support");
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs intial operations.
 * Initializes filter, queue, controller config with default values.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_PerformInitOperations(void)
{
    /* Create critical section for ensuring thread
       safeness of read message function */
    InitializeCriticalSection(&sg_DIL_CriticalSection);
    /* Create an event for timestamp calculations*/
    sg_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    /*Initial Channel info with default values.
      1. Controller configuration
      2. Queue configuration
      3. Filter configuration*/

    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
    {
        /* Controller configuration default values*/
        vInitializeControllerConfig(i);
        /* Queue configuration default values*/
        vInitializeQueueConfig(i);
        /* Filter configuration default values*/
        vInitializeFilterConfig(i);
    }

    //Register monitor client
    DWORD dwClient = 0;
    CAN_RegisterClient(TRUE, dwClient, CAN_MONITOR_NODE);

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return S_OK;
}

/**
 * \return TRUE for success, FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;

    // If successful
    if (pControllerDetails != NULL)
    {
        char* pcStopStr = NULL;

        for( INT i = 0; i < defNO_OF_CHANNELS; i++ )
        {
            sg_asChannel[i].m_OCI_CANConfig.baudrate =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrBaudrate,
                                            &pcStopStr, defBASE_DEC ));
            sg_asChannel[i].m_OCI_CANConfig.baudrate
                = (sg_asChannel[i].m_OCI_CANConfig.baudrate * 1000);
            sg_asChannel[i].m_OCI_CANConfig.samplesPerBit =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSampling,
                                            &pcStopStr, defBASE_DEC ));
            sg_asChannel[i].m_OCI_CANConfig.samplePoint =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSamplePercentage,
                                            &pcStopStr, defBASE_DEC ));
            sg_asChannel[i].m_OCI_CANConfig.SJW =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSjw,
                                            &pcStopStr, defBASE_DEC ));
            sg_asChannel[i].m_OCI_CANConfig.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
            sg_asChannel[i].m_OCI_CANConfig.selfReceptionMode = OCI_SELF_RECEPTION_ON;
            sg_asChannel[i].m_OCI_CANConfig.BTL_Cycles = 10;
            sg_asChannel[i].m_OCI_CANConfig.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
        }

        bReturn = TRUE;
    }

    return bReturn;
}

/**
 * Copies from STCAN_MSG structure into OCI_CANTxMessage message structure
 */
void vCopy_2_OCI_CAN_Data(OCI_CANTxMessage& DestMsg, const STCAN_MSG& SrcMsg)
{
    DestMsg.res     = 0;
    DestMsg.frameID = SrcMsg.m_unMsgID;
    DestMsg.dlc     = SrcMsg.m_ucDataLen;
    DestMsg.flags   = SrcMsg.m_ucEXTENDED ? OCI_CAN_MSG_FLAG_EXTENDED : 0;
    DestMsg.flags   |= SrcMsg.m_ucRTR ? OCI_CAN_MSG_FLAG_REMOTE_FRAME : 0;
    memcpy(DestMsg.data, SrcMsg.m_ucData, sizeof(UCHAR) * 8);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs closure operations.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;
    CAN_StopHardware();
    //deselect hw interface
    hResult = CAN_DeselectHwInterface();
    /* close the existing handle */
    CloseHandle(sg_hEvent);
    sg_hEvent = NULL;
    /* Remove all the existing clients */
    UINT ClientIndex = 0;

    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].m_dwClientID);
    }

    /* Delete the critical section */
    DeleteCriticalSection(&sg_DIL_CriticalSection);

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
HRESULT CDIL_CAN_ETAS_BOA::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp   = sg_TimeStamp;

    if(QueryTickCount != NULL)
    {
        *QueryTickCount = sg_QueryTickCount;
    }

    return S_OK;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will popup hardware selection dialog and gets the user selection of channels.
 *
 */
int ListHardwareInterfaces(HWND /*hParent*/, DWORD /*dwDriver*/, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, NULL, vBlinkHw);
    HwList.DoModal();
    nCount = HwList.nGetSelectedList(pnSelList);
    return 0;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Lists the hardware interface available. sSelHwInterface
 * will contain the user selected hw interface.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_DRIVER_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    char acURI[defNO_OF_CHANNELS][MAX_STRING]= {0};
    INT nFound = 0;

    if (OCI_FindCANController(acURI, defNO_OF_CHANNELS, &nFound) == OCI_SUCCESS)
    {
        nCount = nFound;
        qsort((void*)acURI, nCount, sizeof(acURI[0]), nCallBackStrCompareFn);

        if (nCount > 0)//Success only if there exists alteast one hw
        {
            INTERFACE_HW* psHWInterface = new INTERFACE_HW[nCount];

            for (INT i = 0; i < nCount; i++)
            {
                psHWInterface[i].m_dwIdInterface = 0;
                psHWInterface[i].m_dwVendor = 0;
                psHWInterface[i].m_acDeviceName = "";
                psHWInterface[i].m_acNameInterface = A2T(acURI[i]);
                psHWInterface[i].m_acDescription = A2T(acURI[i]);
            }

            if (nCount > 1)// List hw interface if there are more than one hw
            {
                ListHardwareInterfaces(NULL, DRIVER_CAN_ETAS_BOA, psHWInterface, sg_anSelectedItems, nCount);
                /* return value is not necessary as the OUT parameter sg_anSelectedItems is unaltered
                   if user has not selected anything */
            }

            //set the current number of channels
            sg_nNoOfChannels = min(nCount, defNO_OF_CHANNELS);

            for (UINT nList = 0; nList < sg_nNoOfChannels; nList++)
            {
                asSelHwInterface[nList].m_acNameInterface = psHWInterface[sg_anSelectedItems[nList]].m_acNameInterface;
                asSelHwInterface[nList].m_acDescription = psHWInterface[sg_anSelectedItems[nList]].m_acDescription;
                asSelHwInterface[nList].m_dwIdInterface = 100 + nList; // Give a dummy number
            }

            //Delete the array
            delete[] psHWInterface;
            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
            hResult = S_OK;
        }
        else
        {
            hResult = NO_HW_INTERFACE;
        }
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Selects the hardware interface selected by the user.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_OK;

    //First select only supported number of HW interfaces
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        strcpy_s(sg_asChannel[i].m_acURI, T2A((LPTSTR) (asSelHwInterface[i].m_acNameInterface.c_str())));
    }

    // Create the controller instance.
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        BOA_ResultCode err =  (*(sBOA_PTRS.m_sOCI.createCANController))(sg_asChannel[i].m_acURI,
                              &(sg_asChannel[i].m_OCI_HwHandle));

        if (err == OCI_SUCCESS)
        {
            /* Assign to userdata of QueueCfg. This will be useful to differentiate
            between the controller */
            sg_asChannel[i].m_OCI_RxQueueCfg.onFrame.userData = (void*)sg_asChannel[i].m_OCI_HwHandle;
            sg_asChannel[i].m_OCI_RxQueueCfg.onEvent.userData = (void*)sg_asChannel[i].m_OCI_HwHandle;
            //configure the controller first
            BOA_ResultCode ErrorCode = (*(sBOA_PTRS.m_sOCI.openCANController))(sg_asChannel[i].m_OCI_HwHandle,
                                       &(sg_asChannel[i].m_OCI_CANConfig),
                                       &(sg_asChannel[i].m_OCI_CntrlProp));

            if (ErrorCode == OCI_SUCCESS)
            {
                if (ManageQueue(QUEUE_ADD, i) == S_OK)
                {
                    if (ManageFilters(FILTER_ADD, i) == S_OK)
                    {
                        hResult = (*(sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities))(sg_asChannel[i].m_OCI_HwHandle,
                                  &(sg_asChannel[i].m_OCI_TimerCapabilities));

                        if (hResult == S_OK)
                        {
                            sg_asChannel[i].m_fResolution = (float)10000 /
                                                            (float)(sg_asChannel[i].m_OCI_TimerCapabilities.tickFrequency);
                        }
                        else
                        {
                            hResult = ERR_LOAD_HW_INTERFACE;
                            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not get timer resolution"));
                        }
                    }
                    else
                    {
                        hResult = ERR_LOAD_HW_INTERFACE;
                        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not add filters"));
                    }
                }
                else
                {
                    hResult = ERR_LOAD_HW_INTERFACE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could create Rx Queue"));
                }
            }
            else
            {
                hResult = ERR_LOAD_HW_INTERFACE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not open controller"));
            }
        }
        else
        {
            hResult = ERR_LOAD_HW_INTERFACE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not create controller"));
        }
    }

    //Check for the success
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Deselects the selected hardware interface.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;

    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if ((hResult = ManageFilters(FILTER_REMOVE, i)) == S_OK)
        {
            if ((hResult = ManageQueue(QUEUE_DESTROY, i)) == S_OK)
            {
                if ((*(sBOA_PTRS.m_sOCI.closeCANController))(sg_asChannel[i].m_OCI_HwHandle) == OCI_SUCCESS)
                {
                    if ((*(sBOA_PTRS.m_sOCI.destroyCANController))(sg_asChannel[i].m_OCI_HwHandle) == OCI_SUCCESS)
                    {
                        hResult |= S_OK;
                    }
                    else
                    {
                        hResult = S_FALSE;
                        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not destroy controller"));
                    }
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not close controller"));
                }
            }
            else
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not destroy the queue"));
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not remove the filter"));
        }
    }

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    }

    return hResult;
}

/**
 * \return TRUE for success, FALSE for failure
 *
 * Call back function called from ConfigDialogDIL
 */
BOOL Callback_DILBOA(BYTE /*Argument*/, PBYTE pDatStream, INT /*Length*/)
{
    return (sg_pouDIL_CAN_ETAS_BOA->CAN_SetConfigData((CHAR*) pDatStream, 0) == S_OK);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Displays the configuration dialog for controller
 */
int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/,
                            PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;
    CChangeRegisters_CAN_ETAS_BOA ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
    ouChangeRegister.DoModal();
    nResult = ouChangeRegister.nGetInitStatus();
    return nResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Displays the controller configuration dialog.
 * Fields are initialized with values supplied by InitData.
 * InitData will be updated with the user selected values.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_DisplayConfigDlg(PCHAR& InitData, INT& Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(InitData, WARN_INITDAT_NCONFIRM);
    USES_CONVERSION;
    INT Result = WARN_INITDAT_NCONFIRM;
    PSCONTROLLER_DETAILS psContrlDets = (PSCONTROLLER_DETAILS)InitData;

    //First initialize with existing hw description
    for (INT i = 0; i < min(Length, (INT)sg_nNoOfChannels); i++)
    {
        strcpy_s(psContrlDets[i].m_omHardwareDesc, T2A(sg_asChannel[i].m_acURI));
    }

    if (sg_nNoOfChannels > 0)
    {
        Result = DisplayConfigurationDlg(sg_hOwnerWnd, Callback_DILBOA,
                                         psContrlDets, sg_nNoOfChannels);

        switch (Result)
        {
            case WARNING_NOTCONFIRMED:
            {
                Result = WARN_INITDAT_NCONFIRM;
            }
            break;

            case INFO_INIT_DATA_CONFIRMED:
            {
                Length = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
                Result = CAN_SetConfigData(InitData, Length);

                if (Result == S_OK)
                {
                    Result = INFO_INITDAT_CONFIRM_CONFIG;
                }
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

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the controller configuration data supplied by InitData.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_SetConfigData(PCHAR pInitData, INT /*Length*/)
{
    HRESULT hResult = WARNING_NOTCONFIRMED;
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(pInitData, hResult);
    BOA_ResultCode ErrCode = OCI_FAILURE;
    PSCONTROLLER_DETAILS pControllerDetails = (PSCONTROLLER_DETAILS)pInitData;
    bLoadDataFromContr(pControllerDetails);

    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        //First remove all the filters
        ManageFilters(FILTER_REMOVE, i);
        //First remove all the Rx Tx queues
        ManageQueue(QUEUE_DESTROY, i);
        // if controller is open, close the controller. Do not bother about return value
        ErrCode = (*sBOA_PTRS.m_sOCI.closeCANController)(sg_asChannel[i].m_OCI_HwHandle);
        //Now load the controller config and open the controller
        ErrCode = (*sBOA_PTRS.m_sOCI.openCANController)(sg_asChannel[i].m_OCI_HwHandle,
                  &(sg_asChannel[i].m_OCI_CANConfig),
                  &(sg_asChannel[i].m_OCI_CntrlProp));
        /* Fill the hardware description details */
        strcpy_s(((PSCONTROLLER_DETAILS)pInitData)[i].m_omHardwareDesc,
                 sg_asChannel[i].m_acURI);

        if (ErrCode == OCI_SUCCESS)
        {
            // Rx Tx queue
            if (ManageQueue(QUEUE_ADD, i) == S_OK)
            {
                if (ManageFilters(FILTER_ADD, i) == S_OK)
                {
                    hResult |= S_OK;
                }
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not configure the controller"));
        }
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Starts the controller.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;

    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if ((*(sBOA_PTRS.m_sOCI.getCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                &(sg_asChannel[i].m_OCI_CntrlProp))
                == OCI_SUCCESS)
        {
            if ((sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_SUSPENDED)
                    || (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING))
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_RUNNING;

                if ((*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                        &(sg_asChannel[i].m_OCI_CntrlProp))
                        == OCI_SUCCESS)
                {
                    hResult |= S_OK;
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not start the controller in running mode"));
                }
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not get controller mode"));
        }
    }

    //Check for the success
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_CONNECTED;
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

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Stops the controller.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_OK;

    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if ((*(sBOA_PTRS.m_sOCI.getCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                &(sg_asChannel[i].m_OCI_CntrlProp))
                == OCI_SUCCESS)
        {
            if (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING)
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;

                if ((*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                        &(sg_asChannel[i].m_OCI_CntrlProp))
                        == OCI_SUCCESS)
                {
                    hResult |= S_OK;
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not stop the controller in suspended mode"));
                }
            }
            else if (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_SUSPENDED)
            {
                hResult |= S_OK;
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not get controller mode"));
        }
    }

    //Check for the success
    if (hResult == S_OK)
    {
        sg_byCurrState = CREATE_MAP_TIMESTAMP;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
        DeleteCriticalSection(&sg_CritSectForAckBuf);
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Function to get Controller status
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Resets the controller.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_ResetHardware(void)
{
    return WARN_DUMMY_API;
}

/**
 * Sends STCAN_MSG structure from the client dwClientID.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_FALSE;

    if (bClientIdExist(dwClientID))
    {
        if (sCanTxMsg.m_ucChannel <= sg_nNoOfChannels)
        {
            static OCI_CANMessage sOciCanMsg;
            static OCI_CANTxMessage sOciTxCanMsg;
            static SACK_MAP sAckMap;
            vCopy_2_OCI_CAN_Data(sOciTxCanMsg, sCanTxMsg);
            sOciCanMsg.type = OCI_CAN_TX_MESSAGE;
            sOciCanMsg.reserved = 0;
            uint32 nRemaining = 0;
            memcpy(&(sOciCanMsg.data.txMessage), &(sOciTxCanMsg), sizeof(OCI_CANTxMessage));
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_Channel  = sCanTxMsg.m_ucChannel;
            sAckMap.m_MsgID    = sOciCanMsg.data.txMessage.frameID;
            vMarkEntryIntoMap(sAckMap);
            BOA_ResultCode ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.writeCANData))
                                     (sg_asChannel[sCanTxMsg.m_ucChannel - 1].m_OCI_TxQueueHandle, OCI_NO_TIME, &sOciCanMsg, 1, &nRemaining);

            if (ErrCode == OCI_SUCCESS)
            {
                hResult = S_OK;
            }
            else
            {
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("could not write can data into bus"));
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
 * Gets last occured error and puts inside acErrorStr.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_GetLastErrorString(string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}

/**
 * Gets the controller param eContrParam of the channel.
 * Value stored in lParam.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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
                lParam = sg_nNoOfChannels;
            }
            break;

            case DRIVER_STATUS:
            {
                lParam = sg_bIsDriverRunning;
            }
            break;

            case HW_MODE:
            {
                if (nChannel < sg_nNoOfChannels)
                {
                    lParam = defMODE_ACTIVE;
                }
                else
                {
                    //unknown
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
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }

    return hResult;
}

/**
 * Gets the error counter for corresponding channel.
 */
HRESULT CDIL_CAN_ETAS_BOA::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;

    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_asChannel[nChannel].m_ucTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_asChannel[nChannel].m_ucRxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_asChannel[nChannel].m_ucPeakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_asChannel[nChannel].m_ucPeakRxErrorCounter;
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
