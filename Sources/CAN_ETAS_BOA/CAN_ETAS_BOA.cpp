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
 * @brief Source file for BOA DIL functions
 * @author Pradeep Kadoor, Tobias Lorenz
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for BOA DIL functions
 * Defines the initialization routines for the DLL.
 */

/* C includes */
#include <search.h>             /* For qsort */

/* C++ includes */
#include <list>
#include <string>
#include <vector>

/* Project includes */
#include "CAN_ETAS_BOA_stdafx.h"
#include "CAN_ETAS_BOA.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "Include/Can_Error_Defs.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "HardwareListing.h"
#include "ChangeRegisters_CAN_ETAS_BOA.h"
#include "Application/MultiLanguage.h"
#include "Utility/MultiLanguageSupport.h"

#define USAGE_EXPORT
#include "CAN_ETAS_BOA_Extern.h"

/* ETAS BOA includes */
#ifndef BOA_VERSION
#error "No BOA version defined"
#endif
#if BOA_VERSION == BOA_VERSION_1_4
#include "EXTERNAL/BOA 1.4/Include/OCI/ocican.h"
#include "EXTERNAL/BOA 1.4/Include/CSI/csisfs.h"
#elif BOA_VERSION == BOA_VERSION_1_5
#include "EXTERNAL/BOA 1.5/Include/OCI/ocican.h"
#include "EXTERNAL/BOA 1.5/Include/CSI/csisfs.h"
#elif BOA_VERSION >= BOA_VERSION_2_0
#include "EXTERNAL/BOA_V2/Include/OCI/ocican.h"
#include "EXTERNAL/BOA_V2/Include/CSI/csisfs.h"
#else
#error "Unknown BOA version defined"
#endif

BEGIN_MESSAGE_MAP(CCAN_ETAS_BOA, CWinApp)
END_MESSAGE_MAP()

CCAN_ETAS_BOA::CCAN_ETAS_BOA()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/** The one and only CCAN_ETAS_BOA object */
static CCAN_ETAS_BOA theApp;

/**
 * CCAN_ETAS_BOA initialization
 */
static HINSTANCE ghLangInst = nullptr;

BOOL CCAN_ETAS_BOA::InitInstance()
{
    /* Begin of Multiple Language support */
    if ( CMultiLanguage::m_nLocales <= 0 ) // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage(); // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH]; // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(false);
    }

    /*
     * Load resource-only language DLL. It will use the languages
     * detected above, take first available language,
     * or you can specify another language as second parameter to
     * LoadLangResourceDLL. And try that first.
     */
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }

    CWinApp::InitInstance();

    return TRUE;
}

static const BYTE FILTER_ADD = 0x01;
static const BYTE FILTER_REMOVE = 0x02;
static const BYTE QUEUE_ADD = 0x01;
static const BYTE QUEUE_DESTROY = 0x02;

/**
 * Channel information
 */
typedef struct tagCHANNEL
{
    /** URI of the Controller */
    OCI_URIName m_acURI;

    /** Controller handle */
    OCI_ControllerHandle m_OCI_HwHandle;

    /** Controller configuration */
    OCI_CANConfiguration m_OCI_CANConfig;

    /** Controller properties */
    OCI_CANControllerProperties m_OCI_CntrlProp;

    /** Controller receive queue handle */
    OCI_QueueHandle m_OCI_RxQueueHandle;

    /** Controller receive queue configuration */
    OCI_CANRxQueueConfiguration m_OCI_RxQueueCfg;

    /** Controller transmit queue handle */
    OCI_QueueHandle m_OCI_TxQueueHandle;

    /** Controller transmit queue configuration */
    OCI_CANTxQueueConfiguration m_OCI_TxQueueCfg;

    /** Controller frame filter */
    OCI_CANRxFilter m_OCI_FrameFilter;

    /** Controller event filter */
    OCI_CANEventFilter m_OCI_EventFilter;

    /** Controller error filter */
    OCI_CANErrorFrameFilter m_OCI_ErrorFilter;

    OCI_InternalErrorEventFilter m_OCI_InternalErrorFilter;

    /** Controller state */
    UCHAR m_ucControllerState;

    /** Controller Tx error counter */
    UCHAR m_ucTxErrorCounter;

    /** Controller Rx error counter */
    UCHAR m_ucRxErrorCounter;

    /** Controller peak Rx error counter */
    UCHAR m_ucPeakRxErrorCounter;

    /** Controller peak Tx error counter */
    UCHAR m_ucPeakTxErrorCounter;

    OCI_TimerCapabilities m_OCI_TimerCapabilities;

    FLOAT m_fResolution;

    /** controller capability of CAN_FD */
    bool m_bSupportCANFD;
} SCHANNEL;

/* Client and Client Buffer map */
#define MAX_BUFF_ALLOWED 16
#define MAX_CLIENT_ALLOWED 16
static UINT sg_unClientCnt = 0;

class SCLIENTBUFMAP
{
public:
    DWORD m_dwClientID;
    CBaseCANBufFSE* m_pClientBuf[MAX_BUFF_ALLOWED];
    std::string m_acClientName;
    UINT m_unBufCount;

    SCLIENTBUFMAP() :
        m_dwClientID(0),
        m_unBufCount(0),
        m_acClientName("")
    {
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

static CRITICAL_SECTION sg_CritSectForAckBuf; // To make it thread safe

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

static const BYTE CREATE_MAP_TIMESTAMP = 0x1;
static const BYTE CALC_TIMESTAMP_READY = 0x2;

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

static HANDLE sg_hEvent = nullptr;

static CRITICAL_SECTION sg_DIL_CriticalSection;

static INT sg_anSelectedItems[CHANNEL_ALLOWED];
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];

/* Timer variables */
static SYSTEMTIME sg_CurrSysTime;
static LONGLONG sg_TimeStamp = 0;
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;

/* Required libraries */
static HMODULE sg_hLibCSI = nullptr;
static HMODULE sg_hLibOCI = nullptr;

/* Declarations of CSI API pointers */
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC1)(const char* uriName, CSI_NodeRange range, CSI_Tree* *tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC2)(CSI_Tree* tree);
typedef CSI_DECLSPEC OCI_ErrorCode (*PROC3)(CSI_Tree* tree, const BOA_UuidVersion* uuid, OCI_URIName uriName[], int size, int* count);

typedef OCI_ErrorCode (*PF_OCI_CreateCANControllerVersion)(const char* uriLocation, const BOA_Version* apiVersion, OCI_ControllerHandle* controller);

/* Macro definitions */
#if BOA_VERSION == BOA_VERSION_1_4
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.4"
#define LIB_CSL_NAME "dll-csiBind_1_4.dll"
#define LIB_OCI_NAME "dll-ocdProxy_1_4.dll"
#elif BOA_VERSION == BOA_VERSION_1_5
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\1.5"
#define LIB_CSL_NAME "dll-csiBind_1_5.dll"
#define LIB_OCI_NAME "dll-ocdProxy_1_5.dll"
#elif BOA_VERSION >= BOA_VERSION_2_0
#define BOA_REGISTRY_LOCATION "SOFTWARE\\ETAS\\BOA\\2"
#define LIB_CSL_NAME "dll-csiBind.dll"
#define LIB_OCI_NAME "dll-ocdProxy.dll"
#else
#error "Unknown BOA version defined"
#endif

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
#if BOA_VERSION >= BOA_VERSION_2_0
    PF_OCI_CreateCANControllerVersion createCANController;
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

static BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

/* CDIL_CAN_ETAS_BOA class definition */
class CDIL_CAN_ETAS_BOA : public CBaseDIL_CAN_Controller
{
public:
    /* overloaded functions */
    HRESULT CAN_PerformInitOperations(void);
    HRESULT CAN_PerformClosureOperations(void);
    HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT CAN_DeselectHwInterface(void);
    HRESULT CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_GetCurrStatus(s_STATUSMSG& StatusData);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);
};

static CDIL_CAN_ETAS_BOA* sg_pouDIL_CAN_ETAS_BOA = nullptr;

USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if (sg_pouDIL_CAN_ETAS_BOA == nullptr)
    {
        if ((sg_pouDIL_CAN_ETAS_BOA = new CDIL_CAN_ETAS_BOA) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouDIL_CAN_ETAS_BOA; // Doesn't matter even if it is nullptr

    return hResult;
}

/**
 * Error logger
 */
static Base_WrapperErrorLogger* sg_pIlog = nullptr;

/**
 * Declarations of ProcessCanData. This is a call back function
 * which will be called by BOA framework whenever there is a msg
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessageEx* msg);
#else
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessage* msg);
#endif

/**
 * Declarations of ProcessEvents. This is a call back function
 * which will be called by BOA framework whenever there are internal events
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessageEx* msg);
#else
static void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessage* msg);
#endif

/**
 * CallBack function used by the qsort Function
 */
static INT nCallBackStrCompareFn( const void* str1, const void* str2)
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
            i = sClientObj.m_unBufCount; // break the loop
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
            if (i < (unCount - 1)) // If not the last bufffer
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
static BOOL bGetBOAInstallationPath(std::string& pcPath)
{
    USES_CONVERSION;

    BOOL bResult = FALSE;
    LONG lError = 0;
    HKEY sKey;

    /* Get the installation path for BOA */
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, BOA_REGISTRY_LOCATION, 0, KEY_READ, &sKey);

    /* If the registry key open successfully, get the value in "path" sub key */
    if(lError==ERROR_SUCCESS)
    {
        ULONG ulType = REG_SZ;
        BYTE acGCCPath[1024];
        DWORD dwSize = sizeof(acGCCPath);
        lError = RegQueryValueEx(sKey, "path", 0, &ulType, acGCCPath, &dwSize);
        pcPath = A2T((char*)acGCCPath);
        RegCloseKey(sKey);
        bResult = TRUE;
    }
    return bResult;
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
static HRESULT GetCSI_API_Pointers(HMODULE hLibCSI)
{
    HRESULT hResult = S_OK;
    if (hLibCSI != nullptr)
    {
        if ((sBOA_PTRS.m_sCSI.createProtocolTree        = (PROC1)GetProcAddress(hLibCSI, "CSI_CreateProtocolTree")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.destroyProtocolTree  = (PROC2)GetProcAddress(hLibCSI, "CSI_DestroyProtocolTree")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.getUriForUuid        = (PROC3)GetProcAddress(hLibCSI, "CSI_GetUriForUuid")) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    return hResult;
}

/**
 * Gets the OCI API function pointer from the ocdproxy.dll
 */
static HRESULT GetOCI_API_Pointers(HMODULE hLibOCI)
{
    HRESULT hResult = S_OK;
    if (hLibOCI != nullptr)
    {
#if BOA_VERSION >= BOA_VERSION_2_0
        if ((sBOA_PTRS.createCANController  = (PF_OCI_CreateCANControllerVersion)
                                              GetProcAddress(hLibOCI, "OCI_CreateCANControllerVersion")) == nullptr)
        {
            hResult = S_FALSE;
        }
#else
        if ((sBOA_PTRS.m_sOCI.createCANController = (PF_OCI_CreateCANController)
                GetProcAddress(hLibOCI, "OCI_CreateCANController")) == nullptr)
        {
            hResult = S_FALSE;
        }
#endif
        else if ((sBOA_PTRS.m_sOCI.destroyCANController = (PF_OCI_DestroyCANController)
                  GetProcAddress(hLibOCI, "OCI_DestroyCANController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.openCANController = (PF_OCI_OpenCANController)
                  GetProcAddress(hLibOCI, "OCI_OpenCANController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.closeCANController = (PF_OCI_CloseCANController)
                  GetProcAddress(hLibOCI, "OCI_CloseCANController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.getCANConfiguration = (PF_OCI_GetCANConfiguration)
                  GetProcAddress(hLibOCI, "OCI_GetCANConfiguration")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.getCANControllerProperties = (PF_OCI_GetCANControllerProperties)
                  GetProcAddress(hLibOCI, "OCI_GetCANControllerProperties")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.setCANControllerProperties = (PF_OCI_SetCANControllerProperties)
                  GetProcAddress(hLibOCI, "OCI_SetCANControllerProperties")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.getCANControllerCapabilities = (PF_OCI_GetCANControllerCapabilities)
                  GetProcAddress(hLibOCI, "OCI_GetCANControllerCapabilities")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.getCANControllerStatus = (PF_OCI_GetCANControllerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetCANControllerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.createCANTxQueue = (PF_OCI_CreateCANTxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateCANTxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
#if BOA_VERSION >= BOA_VERSION_2_0
        else if ((sBOA_PTRS.m_sOCI.writeCANDataEx = (PF_OCI_WriteCANDataEx)
                  GetProcAddress(hLibOCI, "OCI_WriteCANDataEx")) == nullptr)
        {
            hResult = S_FALSE;
        }
#endif
        else if ((sBOA_PTRS.m_sOCI.canioVTable.writeCANData = (PF_OCI_WriteCANData)
                  GetProcAddress(hLibOCI, "OCI_WriteCANData")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.destroyCANTxQueue = (PF_OCI_DestroyCANTxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyCANTxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.createCANRxQueue = (PF_OCI_CreateCANRxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateCANRxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.readCANData = (PF_OCI_ReadCANData)
                  GetProcAddress(hLibOCI, "OCI_ReadCANData")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.destroyCANRxQueue = (PF_OCI_DestroyCANRxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyCANRxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANFrameFilter = (PF_OCI_AddCANFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_AddCANFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANFrameFilter = (PF_OCI_RemoveCANFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANBusEventFilter = (PF_OCI_AddCANBusEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddCANBusEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANBusEventFilter = (PF_OCI_RemoveCANBusEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANBusEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.addCANErrorFrameFilter = (PF_OCI_AddCANErrorFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_AddCANErrorFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter = (PF_OCI_RemoveCANErrorFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANErrorFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetTimerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetTimerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities = (PF_OCI_GetTimerCapabilities)
                  GetProcAddress(hLibOCI, "OCI_GetTimerCapabilities")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerValue = (PF_OCI_GetTimerValue)
                  GetProcAddress(hLibOCI, "OCI_GetTimerValue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.addTimerEventFilter = (PF_OCI_AddTimerEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddTimerEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.removeTimerEventFilter = (PF_OCI_RemoveTimerEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveTimerEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.getError = (PF_OCI_GetError)
                  GetProcAddress(hLibOCI, "OCI_GetError")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter = (PF_OCI_AddInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddInternalErrorEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter = (PF_OCI_RemoveInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveInternalErrorEventFilter")) == nullptr)
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
static void findCanNodes(CSI_Tree* sfsTree, std::string uriPrefix, OCI_URIName uriNames[], uint32 size, uint32* position)
{
    /* Uncomment the next line to get a view of the items in the tree */
    // printf( "uriPrefix is %s; node is %s\n", uriPrefix, sfsTree->item.uriNames );

    /* Basic error checking */
    if(!sfsTree || !uriNames || uriPrefix.empty() || !position)
    {
        return;
    }

    /*
     * Does the current tree node have the URI name which begins with "CAN:"?
     * (Each node which represents a CAN port always has a URI name of the form "CAN:n".
     */
    if(strncmp(sfsTree->item.uriName, "CAN:", 4) == 0)
    {
        if (*position < size)
        {
            std::string newUriName;
            newUriName.append(uriPrefix.c_str());
            newUriName.append("/");
            newUriName.append(sfsTree->item.uriName);
            strncpy_s(uriNames[*position], sizeof(OCI_URIName), newUriName.c_str(), newUriName.length());
            (*position)++;
        }
        else
        {
            return;
        }
    }

    /* If the current tree node has a child, recurse into it */
    if (sfsTree->child)
    {
        std::string newUriPrefix;
        newUriPrefix.append(uriPrefix);
        newUriPrefix.append("/");
        newUriPrefix.append(sfsTree->item.uriName);
        findCanNodes(sfsTree->child, newUriPrefix, uriNames, size, position);
    }

    /* If the current tree node has a sibling, recurse into it */
    if (sfsTree->sibling)
    {
        findCanNodes(sfsTree->sibling, uriPrefix, uriNames, size, position);
    }
}

static BOA_ResultCode OCI_FindCANController(OCI_URIName uriName[], INT nSize, INT* nFound)
{
    if (!nFound)
    {
        return S_FALSE;
    }

    /* Container for search results */
    CSI_Tree* sfsTree = nullptr;

    /* Specify that we want to search for physical hardware nodes */
    const CSI_NodeRange nodeRange = {CSI_NODE_MIN_PHYSICAL_NODE , CSI_NODE_MAX_PHYSICAL_NODE};

    /* The prefix of the URI of the root of the device tree */
    OCI_URIName uriPrefix = "ETAS:/";

    /* Search for all connected hardware and latch the result for further processing */
    OCI_ErrorCode ec;
    ec = sBOA_PTRS.m_sCSI.createProtocolTree("", nodeRange, &sfsTree);
    if (OCI_FAILED(ec))
    {
        return ec;
    }

    /* Search the tree and fill array with the results */
    *nFound = 0;
    uint32 unFound = 0;
    findCanNodes(sfsTree, uriPrefix, uriName, nSize, &unFound);
    *nFound = unFound;

    /* Clean up the protocol tree. */
    ec = sBOA_PTRS.m_sCSI.destroyProtocolTree(sfsTree);
    return ec;
}

/**
 * unClientIndex will have index to client array which has clientId dwClientID.
 *
 * @return Returns true if found else false.
 */
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;

    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; // break the loop
            bResult = TRUE;
            break;
        }
    }
    return bResult;
}

/**
 * Checks for the existance of the client with the name pcClientName.
 *
 * @return TRUE if client exists else FALSE
 */
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (pcClientName == sg_asClientToBufMap[i].m_acClientName)
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Removes the client with client id dwClientId.
 *
 * @return TRUE if client removed else FALSE
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
                sg_asClientToBufMap[unClientIndex].m_pClientBuf[i] = nullptr;
            }
            sg_asClientToBufMap[unClientIndex].m_unBufCount = 0;
            if ((unClientIndex + 1) < sg_unClientCnt)
            {
                sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
            }
            --sg_unClientCnt;
            bResult = TRUE;
        }
    }
    return bResult;
}

/**
 * Searches for the client with the id dwClientId.
 *
 * @return TRUE if client exists else FALSE
 */
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;

    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
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
            i = MAX_CLIENT_ALLOWED; // break the loop
        }
    }
    return nClientId;
}

/**
 * Pushes an entry into the list at the last position
 */
static void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    sg_asAckMapBuf.push_back(RefObj);
}

static BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator iResult =
        find(sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj);
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
 * Initialize default values for controller configuration
 *
 * @param[in] nChannel Channel information
 */
static void vInitializeControllerConfig(UINT nChannel)
{
    /* Set CAN default values */
    sg_asChannel[nChannel].m_OCI_CANConfig.baudrate = 500000;
    sg_asChannel[nChannel].m_OCI_CANConfig.samplePoint = 80;
    sg_asChannel[nChannel].m_OCI_CANConfig.samplesPerBit = OCI_CAN_THREE_SAMPLES_PER_BIT;
    sg_asChannel[nChannel].m_OCI_CANConfig.BTL_Cycles = 80;
    sg_asChannel[nChannel].m_OCI_CANConfig.SJW = 16;
    sg_asChannel[nChannel].m_OCI_CANConfig.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
    sg_asChannel[nChannel].m_OCI_CANConfig.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
    sg_asChannel[nChannel].m_OCI_CANConfig.selfReceptionMode = OCI_SELF_RECEPTION_ON;

    /* Set controller property to SUSPENDED */
    sg_asChannel[nChannel].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;

#if BOA_VERSION >= BOA_VERSION_2_0
    /* Set CAN FD default values */
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDEnabled = true;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.dataBitRate                  = 2000000;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.dataSamplePoint              = 70;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.dataBTL_Cycles               = 10;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.dataSJW                      = 3;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.txDelayCompensationControl   = OCI_CANFDTX_DELAY_COMPENSATION_OFF;
    //sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.txSecondarySamplePointOffset = 0;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.canFdTxConfig                = OCI_CANFDTX_USE_CANFD_FRAMES_ONLY;
    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.canFdRxConfig.canRxMode      = OCI_CAN_RXMODE_CAN_FRAMES_IGNORED;
    //
    //sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.canFdRxConfig.canRxMode      = OCI_CAN_RXMODE_CAN_FRAMES_USING_CAN_MESSAGE;

    sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.canFdRxConfig.canFdRxMode    = OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE;

    //sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.canFdRxConfig.canFdRxMode    = OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE_PADDING;

    // sg_asChannel[nChannel].m_OCI_CANConfig.canFDConfig.canFdTxConfig                = OCI_CANFDTX_USE_CAN_AND_CANFD_FRAMES;
    sg_asChannel[nChannel].m_OCI_CANConfig.busParticipationMode = OCI_BUSMODE_ACTIVE;
#endif
}

/**
 * Initialize default values for queue confgiuration
 *
 * @param[in] nChannel Channel information
 */
static void vInitializeQueueConfig(UINT nChannel)
{
    /* configure Rx Queue*/
#if BOA_VERSION >= BOA_VERSION_2_0
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.functionEx = ProcessCanData;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.functionEx = ProcessEvents;
#else
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.function = ProcessCanData;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.function = ProcessEvents;
#endif
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.selfReceptionMode = OCI_SELF_RECEPTION_ON;

    /* configure Tx Queue*/
    sg_asChannel[nChannel].m_OCI_TxQueueCfg.reserved = 0;
}

/**
 * Initialize default values for filter confgiuration
 *
 * @param[in] nChannel Channel information
 */
static void vInitializeFilterConfig(UINT nChannel)
{
    /* configure frame filter*/
    sg_asChannel[nChannel].m_OCI_FrameFilter.frameIDMask = 0;
    sg_asChannel[nChannel].m_OCI_FrameFilter.frameIDValue = 0;
    sg_asChannel[nChannel].m_OCI_FrameFilter.tag = 0;

    /* configure event filter*/
    sg_asChannel[nChannel].m_OCI_EventFilter.destination =
        OCI_EVENT_DESTINATION_CALLBACK;
    sg_asChannel[nChannel].m_OCI_EventFilter.eventCode =
        OCI_CAN_BUS_EVENT_STATE_ACTIVE |
        OCI_CAN_BUS_EVENT_STATE_PASSIVE |
        OCI_CAN_BUS_EVENT_STATE_ERRLIMIT |
        OCI_CAN_BUS_EVENT_STATE_BUSOFF |
        OCI_CAN_BUS_EVENT_FAULT_TOLERANT_SINGLE_WIRE ;
    sg_asChannel[nChannel].m_OCI_EventFilter.tag = 0;

    /* configure error filter */
    sg_asChannel[nChannel].m_OCI_ErrorFilter.destination =
        OCI_EVENT_DESTINATION_CALLBACK;
    sg_asChannel[nChannel].m_OCI_ErrorFilter.errorFrame =
        OCI_CAN_ERR_TYPE_BITSTUFF |
        OCI_CAN_ERR_TYPE_FORMAT |
        OCI_CAN_ERR_TYPE_ACK |
        OCI_CAN_ERR_TYPE_BIT |
        OCI_CAN_ERR_TYPE_BIT_RECSV_BUT_DOMINANT |
        OCI_CAN_ERR_TYPE_BIT_DOMINANT_BUT_RECSV |
        OCI_CAN_ERR_TYPE_CRC |
        OCI_CAN_ERR_TYPE_OVERLOAD |
        OCI_CAN_ERR_TYPE_OTHER;
    sg_asChannel[nChannel].m_OCI_ErrorFilter.tag = 0;

    /* configure internal error filter */
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.eventCode =
        OCI_INTERNAL_GENERAL_ERROR;
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.tag = 0;
}

/**
 * Adds or removes the HW filter to/from the channel.
 *
 * @return S_OK for success, S_FALSE for failure
 */
static HRESULT ManageFilters(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode ErrCode = OCI_FAILURE;
    if (byCode == FILTER_ADD)
    {
        /* Add frame filter */
        ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANFrameFilter))
                  (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                   &(sg_asChannel[nChannel].m_OCI_FrameFilter), 1);
        if (BOA_SUCCEEDED(ErrCode))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add frame filter"));
        }

        /* Add bus event filter */
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANBusEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);

            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Event filter"));
            }
        }

        /* Add error frame filter */
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.addCANErrorFrameFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_ErrorFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
            }
        }

        /* Add internal error filter */
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
            }
        }

    }
    else if (byCode == FILTER_REMOVE)
    {
        /* Remove frame filter */
        ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANFrameFilter))
                  (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                   &(sg_asChannel[nChannel].m_OCI_FrameFilter), 1);
        if (BOA_SUCCEEDED(ErrCode))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove frame filter"));
        }

        /* Remove bus event filter */
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANBusEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove Event filter"));
            }
        }

        /* Remove error frame filter */
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_ErrorFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove Error filter"));
            }
        }

        /* Remove internal error event filter */
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);
            if (BOA_FAILED(ErrCode))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
            }
        }
    }
    return hResult;
}

/**
 * Adds or removes the queue to/from the channel.
 *
 * @return S_OK for success, S_FALSE for failure
 */
static HRESULT ManageQueue(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode Err = OCI_ERR_FLAG_ERROR;
    if (byCode == QUEUE_ADD)
    {
        /* Create CAN Rx queue */
        Err = (*(sBOA_PTRS.m_sOCI.canioVTable.createCANRxQueue))
              (sg_asChannel[nChannel].m_OCI_HwHandle,
               &(sg_asChannel[nChannel].m_OCI_RxQueueCfg),
               &(sg_asChannel[nChannel].m_OCI_RxQueueHandle));
        if (BOA_SUCCEEDED(Err))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create Rx Queue"));
        }

        /* Create CAN Tx queue */
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.canioVTable.createCANTxQueue))
                  (sg_asChannel[nChannel].m_OCI_HwHandle,
                   &(sg_asChannel[nChannel].m_OCI_TxQueueCfg),
                   &(sg_asChannel[nChannel].m_OCI_TxQueueHandle));
            if (BOA_FAILED(Err))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
            }
        }
    }
    else if (byCode == QUEUE_DESTROY)
    {
        /* Destroy CAN Rx queue */
        Err = (*(sBOA_PTRS.m_sOCI.canioVTable.destroyCANRxQueue))(sg_asChannel[nChannel].m_OCI_RxQueueHandle);
        if (BOA_SUCCEEDED(Err))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create Rx Queue"));
        }

        /* Create CAN Tx queue */
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.canioVTable.destroyCANTxQueue))(sg_asChannel[nChannel].m_OCI_TxQueueHandle);
            if (BOA_FAILED(Err))
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
            }
        }
    }
    return hResult;
}

/**
 * copies from OCI_CANRxMessage struct into STCANDATA struct
 */
static void vCopyOCI_CAN_RX_2_DATA(const OCI_CANRxMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = min(SrcMsg->dlc, 8);
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = (SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) ? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR = (SrcMsg->flags & OCI_CAN_MSG_FLAG_REMOTE_FRAME) ? 1 : 0;
    DestMsg->m_ucDataType = (SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) ? TX_FLAG : RX_FLAG;

    UINT Channel = DestMsg->m_uDataInfo.m_sCANMsg.m_ucChannel;
    if (Channel > defNO_OF_CHANNELS)
    {
        Channel = 1; // Take appropriate action
    }

    DestMsg->m_lTickCount.QuadPart = (LONGLONG)(SrcMsg->timeStamp * sg_asChannel[Channel - 1].m_fResolution);
    memcpy(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData, SrcMsg->data, sizeof(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData));
}

/**
 * copies from OCI_CANRxMessage struct into STCANDATA struct
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vCopyOCI_CAN_FD_RX_2_DATA(const OCI_CANFDRxMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->size;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) == OCI_CAN_MSG_FLAG_EXTENDED) ? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR = 0;
    DestMsg->m_ucDataType   = ((SrcMsg->flags & OCI_CAN_MSG_FLAG_SELFRECEPTION) == OCI_CAN_MSG_FLAG_SELFRECEPTION) ? TX_FLAG : RX_FLAG;

    UINT Channel = DestMsg->m_uDataInfo.m_sCANMsg.m_ucChannel;
    if (Channel > defNO_OF_CHANNELS)
    {
        Channel = 1; // Take appropriate action
    }
    DestMsg->m_lTickCount.QuadPart = (LONGLONG)(SrcMsg->timeStamp * sg_asChannel[Channel - 1].m_fResolution);
    memcpy(DestMsg->m_uDataInfo.m_sCANMsg.m_ucData, SrcMsg->data, SrcMsg->size);
}
#endif

/**
 * Gets the channel with handle OCI_ControllerHandle
 *
 * @return Returns channel number
 */
static UINT nGetChannel(const OCI_ControllerHandle hHandle)
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
static void vCreateTimeModeMapping(HANDLE hEvent)
{
    WaitForSingleObject(hEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /* Query Tick Count */
    QueryPerformanceCounter(&sg_QueryTickCount);
}

/**
 * This function writes the message to the corresponding clients buffer
 */
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    /* Write into the client's buffer and Increment message Count */
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
                /* Tx for monitor nodes and sender node */
                if (bClientExists && (i == Index))
                {
                    for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; ++j)
                    {
                        sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sCanData);
                    }
                }
                else
                {
                    /* Send the other nodes as Rx. */
                    for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; ++j)
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
        for (UINT i = 0; i < sg_unClientCnt; ++i)
        {
            for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; ++j)
            {
                sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sCanData);
            }
        }
    }
}

/**
 * Processes Rx msg and writes into regiastered clients buffer.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessRxMsg(void* userData, struct OCI_CANMessageEx* msg)
#else
static void vProcessRxMsg(void* userData, struct OCI_CANMessage* msg)
#endif
{
    /* First calculate timestamp for first message*/
    EnterCriticalSection(&sg_DIL_CriticalSection);

    static STCANDATA sCanData;
    static OCI_ControllerHandle hHandle;

    hHandle = (OCI_ControllerHandle)userData;
    sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)nGetChannel(hHandle);

    if ( msg->type == OCI_CAN_RX_MESSAGE )
    {
        // sCanData.m_uDataInfo.m_sCANMsg.m_bCANFDMsg = false;
        vCopyOCI_CAN_RX_2_DATA(&(msg->data.rxMessage), &sCanData);
        sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    }
#if BOA_VERSION >= BOA_VERSION_2_0
    /* Check if its a CAN FD message */
    else if ( msg->type ==  OCI_CANFDRX_MESSAGE )
    {
        // sCanData.m_uDataInfo.m_sCANMsg.m_bCANFDMsg = true;
        vCopyOCI_CAN_FD_RX_2_DATA(&(msg->data.canFDRxMessage), &sCanData);
        sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD = true;
    }
#endif

    if (sg_byCurrState == CREATE_MAP_TIMESTAMP)
    {
        // sg_TimeStamp = sCanData.m_lTickCount.QuadPart;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);
        sg_byCurrState = CALC_TIMESTAMP_READY;


        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        LONGLONG unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;

        /* Time difference should be +ve value */
        if(sCanData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStamp  = sCanData.m_lTickCount.QuadPart - unConnectionTime;
        }
        else
        {
            sg_TimeStamp  = unConnectionTime - sCanData.m_lTickCount.QuadPart;
        }
    }

    /* Write the msg into registered client's buffer */
    vWriteIntoClientsBuffer(sCanData);

    LeaveCriticalSection(&sg_DIL_CriticalSection);
}

/**
 * Processes Tx msg.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessTxMsg(void* /*userData*/, OCI_CANMessageEx* /*msg*/)
#else
static void vProcessTxMsg(void* /*userData*/, OCI_CANMessage* /*msg*/)
#endif
{
}

/**
 * Copies OCI_CANErrorFrameMessage struct into STCANDATA.
 */
static void vCopyOCI_CAN_ERR_2_DATA(const OCI_CANErrorFrameMessage* SrcMsg, STCANDATA* DestMsg)
{
    DestMsg->m_uDataInfo.m_sCANMsg.m_unMsgID = SrcMsg->frameID;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucDataLen = SrcMsg->dlc;
    DestMsg->m_lTickCount.QuadPart = SrcMsg->timeStamp;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = (SrcMsg->flags & OCI_CAN_MSG_FLAG_EXTENDED) ? 1 : 0;
    DestMsg->m_uDataInfo.m_sCANMsg.m_ucRTR = (SrcMsg->flags & OCI_CAN_MSG_FLAG_REMOTE_FRAME) ? 1 : 0;
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
            /* stuff error */
        case OCI_CAN_ERR_TYPE_BITSTUFF:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = bIsTx ? STUFF_ERROR_TX : STUFF_ERROR_RX;
            break;

            /* form error */
        case OCI_CAN_ERR_TYPE_FORMAT:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = bIsTx ? FORM_ERROR_TX : FORM_ERROR_RX;
            break;

            /* bit error */
        case OCI_CAN_ERR_TYPE_BIT:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = bIsTx ? BIT_ERROR_TX : BIT_ERROR_RX;
            break;

            /* other error */
        case OCI_CAN_ERR_TYPE_OTHER:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = bIsTx ? OTHER_ERROR_TX : OTHER_ERROR_RX;
            break;

            /* crc error */
        case OCI_CAN_ERR_TYPE_CRC:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = bIsTx ? CRC_ERROR_TX : CRC_ERROR_RX;
            break;

            /* acknowledgement error */
        case OCI_CAN_ERR_TYPE_ACK:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = bIsTx ? ACK_ERROR_TX : ACK_ERROR_RX;
            break;

            /* other error categories currently just map to ERROR_UNKNOWN */
        case OCI_CAN_ERR_TYPE_OVERLOAD:
        case OCI_CAN_ERR_TYPE_BIT_DOMINANT_BUT_RECSV:
        case OCI_CAN_ERR_TYPE_BIT_RECSV_BUT_DOMINANT:
        default:
            DestMsg->m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_UNKNOWN;
            break;
    }
}

/**
 * Updates Tx and Rx error counter of the nChannel
 */
static void vUpdateErrorCounter(UCHAR ucTxError, UCHAR ucRxError, UINT nChannel)
{
    /* Update Tx Error counter and peak Tx Error Counter */
    sg_asChannel[nChannel].m_ucTxErrorCounter = sg_asChannel[nChannel].m_ucTxErrorCounter + ucTxError;

    /* Update Peak Value */
    sg_asChannel[nChannel].m_ucPeakTxErrorCounter = max(sg_asChannel[nChannel].m_ucPeakTxErrorCounter, ucTxError);

    /* Update Rx Error counter and peak Rx Error Counter */
    sg_asChannel[nChannel].m_ucRxErrorCounter = sg_asChannel[nChannel].m_ucRxErrorCounter + ucRxError;

    /* Update Peak Value */
    sg_asChannel[nChannel].m_ucPeakRxErrorCounter = max(sg_asChannel[nChannel].m_ucPeakRxErrorCounter, ucRxError);
}

/**
 * Processes error msg and writes into registered clients buffer.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessErrMsg(void* userData, struct OCI_CANMessageEx* msg)
#else
static void vProcessErrMsg(void* userData, struct OCI_CANMessage* msg)
#endif
{
    static STCANDATA sCanData;
    vCopyOCI_CAN_ERR_2_DATA(&(msg->data.errorFrameMessage), &sCanData);
    sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = 0;
    sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)nGetChannel((OCI_ControllerHandle)userData);
    sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;

    /*  Update channel error counter */
    vUpdateErrorCounter(sCanData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount,
                        sCanData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount,
                        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel);

    /* Write the msg into registered client's buffer */
    for (UINT i = 0; i < sg_unClientCnt; ++i)
    {
        for (UINT j = 0; j < sg_asClientToBufMap[i].m_unBufCount; ++j)
        {
            sg_asClientToBufMap[i].m_pClientBuf[j]->WriteIntoBuffer(&sCanData);
        }
    }
}

/**
 * Callback function called by the BOA framework
 * when there is presence of msg in the bus.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessageEx* msg)
#else
static void (OCI_CALLBACK ProcessCanData)(void* userData, struct OCI_CANMessage* msg)
#endif
{
    switch (msg->type)
    {
        case OCI_CAN_RX_MESSAGE:
#if BOA_VERSION >= BOA_VERSION_2_0
        case OCI_CANFDRX_MESSAGE:
#endif
            vProcessRxMsg(userData, msg);
            break;
        case OCI_CAN_TX_MESSAGE:
#if BOA_VERSION >= BOA_VERSION_2_0
        case OCI_CANFDTX_MESSAGE:
#endif
            vProcessTxMsg(userData, msg);
            break;

    }
}

/**
 * processes bus event.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessBusEvent(void* /*userData*/, struct OCI_CANMessageEx* /*msg*/)
#else
static void vProcessBusEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
#endif
{

}

/**
 * processes internal event.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessInternalErrEvent(void* /*userData*/, struct OCI_CANMessageEx* /*msg*/)
#else
static void vProcessInternalErrEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
#endif
{

}

/**
 * process queue event.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessQueueEvent(void* /*userData*/, struct OCI_CANMessageEx* /*msg*/)
#else
static void vProcessQueueEvent(void* /*userData*/, struct OCI_CANMessage* /*msg*/)
#endif
{

}

/**
 * process tmer event.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void vProcessTimerEvent(void* /*userData*/, struct OCI_CANMessageEx* msg)
#else
static void vProcessTimerEvent(void* /*userData*/, struct OCI_CANMessage* msg)
#endif
{
    switch (msg->data.timerEventMessage.eventCode)
    {
        case OCI_TIMER_EVENT_SYNC_LOCK:
            SetEvent(sg_hEvent);
            vCreateTimeModeMapping(sg_hEvent);
            break;

        case OCI_TIMER_EVENT_SYNC_LOSS:
            // Nothing at this moment
            break;
    }
}

/**
 * Callback function called by the BOA framework
 * when there is internal bus event.
 */
#if BOA_VERSION >= BOA_VERSION_2_0
static void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessageEx* msg)
#else
static void (OCI_CALLBACK ProcessEvents)(void* userData, struct OCI_CANMessage* msg)
#endif
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

        case OCI_CAN_ERROR_FRAME:
            vProcessErrMsg(userData, msg);
            break;
    }
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;
    return S_OK;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_UnloadDriverLibrary(void)
{
    /* Unload OCI library */
    if (sg_hLibOCI != nullptr)
    {
        FreeLibrary(sg_hLibOCI);
    }

    /* Unload CSI library */
    if (sg_hLibCSI != nullptr)
    {
        FreeLibrary(sg_hLibCSI);
    }

    /* Invalidate all API pointers */
    memset(&sBOA_PTRS, 0, sizeof(SBOA_POINTER_TABLE));
    return S_OK;
}

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
                /* Add msg buffer */
                if (pBufObj != nullptr)
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
                /* clear msg buffer */
                if (pBufObj != nullptr) // Remove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.m_pClientBuf, sClientObj.m_unBufCount, pBufObj);
                }
                else // Remove all
                {
                    for (UINT i = 0; i < sClientObj.m_unBufCount; i++)
                    {
                        sClientObj.m_pClientBuf[i] = nullptr;
                    }
                    sClientObj.m_unBufCount = 0;
                }
                hResult = S_OK;
            }
            else
            {
                // ASSERT(false);
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
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].m_dwClientID, nullptr);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}

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
                /* Currently store the client information */
                if (strcmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
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

HRESULT CDIL_CAN_ETAS_BOA::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& unCntrlStatus)
{
    unCntrlStatus = defCONTROLLER_ACTIVE; /* Temporary solution. TODO */
    return S_OK;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_LoadDriverLibrary(void)
{
    HRESULT hResult = S_FALSE;
    std::string acPath;

    /* Get BOA installation path from the registery */
    bGetBOAInstallationPath(acPath);

    /* Load cslproxy.dll library */
    std::string acLIB_CSL = "";
    acLIB_CSL.append(acPath);
    acLIB_CSL.append("\\");
    acLIB_CSL.append(LIB_CSL_NAME);

    /* LoadLibraryEx instead of LoadLibrary seems to be necessary under Windows 7 when the library is not in DLL search path (system32) */
    sg_hLibCSI = LoadLibraryEx(acLIB_CSL.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (sg_hLibCSI != nullptr)
    {
        hResult = GetCSI_API_Pointers(sg_hLibCSI);

        /* Load the OCI library to use CAN controller */
        if (hResult == S_OK)
        {
            std::string acLIB_OCI;
            acLIB_OCI.append(acPath);
            acLIB_OCI.append("\\");
            acLIB_OCI.append(LIB_OCI_NAME);
            sg_hLibOCI = LoadLibraryEx(acLIB_OCI.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

            if (sg_hLibOCI != nullptr)
            {
                hResult = GetOCI_API_Pointers(sg_hLibOCI);
                if (hResult != S_OK)
                {
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get OCI function pointers"));
                }
                else
                {
                    sg_bIsDriverRunning = TRUE;
                }
            }
            else
            {
                hResult = S_FALSE;
                std::string acErr;
                acErr.append(acLIB_OCI);
                acErr.append(" failed to load");
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acErr);
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
            }
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get CSI function pointers"));
        }
    }
    else
    {
        std::string acErr;
        acErr.append(acLIB_CSL);
        acErr.append(_(" failed to load"));
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acErr);
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
    }
    return hResult;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_PerformInitOperations(void)
{
    /*
     * Create critical section for ensuring thread
     * safeness of read message function
     */
    InitializeCriticalSection(&sg_DIL_CriticalSection);

    /* Create an event for timestamp calculations */
    sg_hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    /*
     * Initial Channel info with default values.
     * 1. Controller configuration
     * 2. Queue configuration
     * 3. Filter configuration
     */
    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
    {
        /* Controller configuration default values */
        vInitializeControllerConfig(i);

        /* Queue configuration default values */
        vInitializeQueueConfig(i);

        /* Filter configuration default values */
        vInitializeFilterConfig(i);
    }

    /* Register monitor client */
    DWORD dwClient = 0;
    CAN_RegisterClient(TRUE, dwClient, CAN_MONITOR_NODE);

    /* Initialize the selected channel items array to -1 */
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return S_OK;
}

unsigned int GetSelectedChannelIndex(unsigned int unIndex)
{
    int nSelChannel = -1;
    for(int nChannel = 0; nChannel < defNO_OF_CHANNELS; nChannel++)
    {
        if((sg_HardwareList.m_omHardwareChannel[nChannel] == sg_SelectedChannels.m_omHardwareChannel[unIndex])
                && sg_HardwareList.m_omHardwareChannel[nChannel] != "")
        {
            nSelChannel = nChannel;
            break;
        }
    }
    return nSelChannel;
}
HRESULT CDIL_CAN_ETAS_BOA::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS ouControllerDetails,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount)
{
    HRESULT hReturn = S_OK;

    PSCONTROLLER_DETAILS psContrlDets = (PSCONTROLLER_DETAILS)ouControllerDetails;
    sg_SelectedChannels.m_nChannelCount = 0;
    sg_nNoOfChannels = 0;
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }
    for (int nChannel = 0; nChannel < defNO_OF_CHANNELS/*(sg_ControllerDetails[nChannel].m_omHardwareDesc != "") && (sg_ControllerDetails[nChannel].m_omHardwareDesc != "Simulation")*/ ; nChannel++)
    {
        if((psContrlDets[nChannel].m_omHardwareDesc != "") && (psContrlDets[nChannel].m_omHardwareDesc != "Simulation"))
        {
            sg_SelectedChannels.m_omHardwareChannel[nChannel] = psContrlDets[nChannel].m_omHardwareDesc;
            sg_ControllerDetails[nChannel].m_omHardwareDesc = psContrlDets[nChannel].m_omHardwareDesc;
            sg_SelectedChannels.m_nChannelCount += 1;
            sg_nNoOfChannels++;
        }
    }
    INTERFACE_HW_LIST psHWInterface;
    bool bIsChannelSelected = false;
    for (UINT nList = 0; nList < sg_SelectedChannels.m_nChannelCount; nList++)
    {
        sg_anSelectedItems[nList] = GetSelectedChannelIndex(nList);

        if(sg_anSelectedItems[nList] != -1)
        {
            psHWInterface[nList].m_dwIdInterface = 0;
            psHWInterface[nList].m_dwVendor = 0;
            psHWInterface[nList].m_acDeviceName = "";
            psHWInterface[nList].m_acNameInterface = sg_SelectedChannels.m_omHardwareChannel[nList];
            psHWInterface[nList].m_acDescription = sg_SelectedChannels.m_omHardwareChannel[nList];

            bIsChannelSelected = true;
        }
    }

    if(true == bIsHardwareListed && false == bIsChannelSelected)
    {
        sg_SelectedChannels.m_nChannelCount = 1;
        sg_anSelectedItems[0] = 0;

        psHWInterface[0].m_dwIdInterface = 0;
        psHWInterface[0].m_dwVendor = 0;
        psHWInterface[0].m_acDeviceName = "";
        psHWInterface[0].m_acNameInterface = sg_HardwareList.m_omHardwareChannel[0];
        psHWInterface[0].m_acDescription = sg_HardwareList.m_omHardwareChannel[0];
        hReturn = S_FALSE;
    }

    if(sg_SelectedChannels.m_nChannelCount > 0)
    {
        sg_nNoOfChannels = min(sg_SelectedChannels.m_nChannelCount, defNO_OF_CHANNELS);
        sg_bCurrState = STATE_HW_INTERFACE_LISTED;
        CAN_SelectHwInterface(psHWInterface, sg_SelectedChannels.m_nChannelCount);
    }

    return hReturn;
}
/**
 * Copies the controller config values into channel's
 * controller config structure.
 *
 * @return TRUE for success, FALSE for failure
 */
static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;

    /* If successful */
    if (pControllerDetails != nullptr)
    {
        char* pcStopStr = nullptr;
        for( INT i = 0; i < defNO_OF_CHANNELS; i++ )
        {
            sg_asChannel[i].m_OCI_CANConfig.baudrate =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrBaudrate.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].m_OCI_CANConfig.samplesPerBit =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSampling.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].m_OCI_CANConfig.samplePoint =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSamplePercentage.c_str(),
                                            &pcStopStr, 10));
            sg_asChannel[i].m_OCI_CANConfig.SJW =
                static_cast <UINT>(_tcstol( pControllerDetails[ i ].m_omStrSjw.c_str(),
                                            &pcStopStr, 10));
            strcpy_s(sg_asChannel[i].m_acURI, pControllerDetails[i].m_omHardwareDesc.c_str());
            sg_asChannel[i].m_OCI_CANConfig.syncEdge = OCI_CAN_SINGLE_SYNC_EDGE;
            if( FALSE == pControllerDetails [ i ].m_bSelfReception )
            {
                sg_asChannel[i].m_OCI_CANConfig.selfReceptionMode = OCI_SELF_RECEPTION_OFF;
            }
            else
            {
                sg_asChannel[i].m_OCI_CANConfig.selfReceptionMode = OCI_SELF_RECEPTION_ON;
            }
            sg_asChannel[i].m_OCI_CANConfig.BTL_Cycles = 10;
            sg_asChannel[i].m_OCI_CANConfig.physicalMedia = OCI_CAN_MEDIA_HIGH_SPEED;
            sg_asChannel[i].m_OCI_CANConfig.BTL_Cycles               = pControllerDetails[i].m_unBTL_Cycles;

#if BOA_VERSION >= BOA_VERSION_2_0
            /* Set CAN FD parameters */
            sg_asChannel[i].m_OCI_CANConfig.canFDEnabled = true;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.dataBitRate                  = pControllerDetails[i].m_unDataBitRate;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.dataSamplePoint              = pControllerDetails[i].m_unDataSamplePoint;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.dataBTL_Cycles               = pControllerDetails[i].m_unDataBTL_Cycles;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.dataSJW                      = pControllerDetails[i].m_unDataSJW;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.txDelayCompensationControl   = (OCI_CANFDTxDelayCompensation)
                    pControllerDetails[i].m_bTxDelayCompensationControl;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.txSecondarySamplePointOffset = pControllerDetails[i].m_unTxSecondarySamplePointOffset;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.canFdRxConfig.canRxMode      = (OCI_CANRxMode)
                    pControllerDetails[i].m_bytCanRxMode;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.canFdRxConfig.canFdRxMode    = (OCI_CANFDRxMode)
                    pControllerDetails[i].m_bytCanFdRxMode;
            sg_asChannel[i].m_OCI_CANConfig.canFDConfig.canFdTxConfig                = (OCI_CANFDTxConfig)
                    pControllerDetails[i].m_bytCanFdTxConfig;
            sg_asChannel[i].m_OCI_CANConfig.busParticipationMode = OCI_BUSMODE_ACTIVE;
#endif
        }
        bReturn = TRUE;
    }
    return bReturn;
}

/**
 * Copies from STCAN_MSG structure into OCI_CANTxMessage message structure
 */
static void vCopy_2_OCI_CAN_Data(OCI_CANTxMessage& DestMsg, const STCAN_MSG& SrcMsg)
{
    DestMsg.res     = 0;
    DestMsg.frameID = SrcMsg.m_unMsgID;
    DestMsg.dlc     = SrcMsg.m_ucDataLen;
    DestMsg.flags   = SrcMsg.m_ucEXTENDED ? OCI_CAN_MSG_FLAG_EXTENDED : 0;
    DestMsg.flags   |= SrcMsg.m_ucRTR ? OCI_CAN_MSG_FLAG_REMOTE_FRAME : 0;
    memcpy(DestMsg.data, SrcMsg.m_ucData, sizeof(UCHAR) * 8);
}


#if BOA_VERSION >= BOA_VERSION_2_0
static BOOL vCopy_2_OCI_CANFD_Data(OCI_CANFDTxMessage& DestMsg, const STCAN_MSG& SrcMsg)
{
    DestMsg.res = 0;
    DestMsg.frameID = SrcMsg.m_unMsgID;
    DestMsg.size = SrcMsg.m_ucDataLen;
    if (!(
                (SrcMsg.m_ucDataLen <= 8) ||
                (SrcMsg.m_ucDataLen == 12) ||
                (SrcMsg.m_ucDataLen == 16) ||
                (SrcMsg.m_ucDataLen == 20) ||
                (SrcMsg.m_ucDataLen == 24) ||
                (SrcMsg.m_ucDataLen == 32) ||
                (SrcMsg.m_ucDataLen == 48) ||
                (SrcMsg.m_ucDataLen == 64)))
    {
        //AfxMessageBox("Unsupported Data Length. It should be 0..8, 12, 16, 20, 24, 32, 48 or 64");
        return FALSE;
    }
    DestMsg.flags = SrcMsg.m_ucEXTENDED ? OCI_CAN_MSG_FLAG_EXTENDED : 0;
    DestMsg.flags |= SrcMsg.m_ucRTR ? OCI_CAN_MSG_FLAG_REMOTE_FRAME : 0;
    memcpy(DestMsg.data, SrcMsg.m_ucData, sizeof(UCHAR) * SrcMsg.m_ucDataLen);
    return TRUE;
}
#endif

HRESULT CDIL_CAN_ETAS_BOA::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    CAN_StopHardware();

    /* deselect hw interface */
    hResult = CAN_DeselectHwInterface();

    /* close the existing handle */
    CloseHandle(sg_hEvent);
    sg_hEvent = nullptr;

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

HRESULT CDIL_CAN_ETAS_BOA::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp   = sg_TimeStamp;
    QueryTickCount = sg_QueryTickCount;
    return S_OK;
}

/**
 * This function will popup hardware selection dialog and gets the user selection of channels.
 *
 * @return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 */
static int ListHardwareInterfaces(HWND hParent, DWORD /*dwDriver*/, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, &objMainWnd);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList);
        sg_SelectedChannels.m_nChannelCount = 0;
        return 0;
    }
    else
    {
        return -1;
    }
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    // VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_DRIVER_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    OCI_URIName acURI[defNO_OF_CHANNELS];
    INT nFound = 0;

    INT unDefaultChannelCnt = nCount;

    if (BOA_SUCCEEDED(OCI_FindCANController(acURI, defNO_OF_CHANNELS, &nFound)))
    {
        nCount = nFound;
        qsort((void*)acURI, nCount, sizeof(OCI_URIName), nCallBackStrCompareFn);

        /* Success only if there exists at least one hw */
        if (nCount > 0)
        {
            INTERFACE_HW psHWInterface[defNO_OF_CHANNELS];
            /* set the current number of channels */
            nCount = min(nCount, defNO_OF_CHANNELS);
            sg_HardwareList.m_nChannelCount = nCount;

            for (INT i = 0; i < nCount; i++)
            {
                psHWInterface[i].m_dwIdInterface = 0;
                psHWInterface[i].m_dwVendor = 0;
                psHWInterface[i].m_acDeviceName = "";
                psHWInterface[i].m_acNameInterface = acURI[i];
                psHWInterface[i].m_acDescription = acURI[i];
                sg_HardwareList.m_omHardwareChannel[i] = psHWInterface[i].m_acNameInterface;
                sg_ControllerDetails[i].m_omHardwareDesc = psHWInterface[i].m_acNameInterface;
            }

            /* List hw interface if there are more than one hw */
            if (nCount > 1)
            {
                /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
                if ( unDefaultChannelCnt && nCount >= unDefaultChannelCnt )
                {
                    for (UINT i = 0; i < unDefaultChannelCnt; i++)
                    {
                        sg_anSelectedItems[i] = GetSelectedChannelIndex(i);
                    }
                    nCount  = unDefaultChannelCnt;
                }
                else if ( ListHardwareInterfaces(nullptr, DRIVER_CAN_ETAS_BOA, psHWInterface, sg_anSelectedItems, nCount) != 0 )
                {
                    /* return if user cancels hardware selection */
                    return HW_INTERFACE_NO_SEL;
                }
            }

            sg_nNoOfChannels = min(nCount, defNO_OF_CHANNELS);
            for (UINT nList = 0; nList < sg_nNoOfChannels; nList++)
            {
                if(sg_anSelectedItems[nList] != -1)
                {
                    asSelHwInterface[nList].m_acNameInterface = psHWInterface[sg_anSelectedItems[nList]].m_acNameInterface;
                    asSelHwInterface[nList].m_acDescription = psHWInterface[sg_anSelectedItems[nList]].m_acDescription;
                    asSelHwInterface[nList].m_dwIdInterface = 100 + nList; // Give a dummy number
                }
            }

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

HRESULT CDIL_CAN_ETAS_BOA::CAN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;

    /* First select only supported number of HW interfaces */
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        strcpy_s(sg_asChannel[i].m_acURI, asSelHwInterface[i].m_acNameInterface.c_str());
        sg_ControllerDetails[i].m_omHardwareDesc = asSelHwInterface[i].m_acNameInterface;
    }

    /* Create the controller instance. */
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        BOA_ResultCode err=0;
        // bool bSUpportCAN_FD = false;
#if BOA_VERSION >= BOA_VERSION_2_0
        BOA_Version                     version = {1, 3, 0, 0};

        BOA_Version                     version1 = {1, 1, 0, 0};

        err =  (*(sBOA_PTRS.createCANController))(sg_asChannel[i].m_acURI,&version,
                &(sg_asChannel[i].m_OCI_HwHandle));

        if(BOA_FAILED(err)  )
        {
            // m_bSupportCANFD
            sg_asChannel[i].m_bSupportCANFD = false;

            err =  (*(sBOA_PTRS.createCANController))(sg_asChannel[i].m_acURI,&version1,
                    &(sg_asChannel[i].m_OCI_HwHandle));

        }
        else
        {
            sg_asChannel[i].m_bSupportCANFD = true;
        }


#else

        err =  (*(sBOA_PTRS.m_sOCI.createCANController))(sg_asChannel[i].m_acURI,
                &(sg_asChannel[i].m_OCI_HwHandle));
#endif
        if (BOA_SUCCEEDED(err))
        {
            /*
             * Assign to userdata of QueueCfg. This will be useful to differentiate
             * between the controller
             */
            sg_asChannel[i].m_OCI_RxQueueCfg.onFrame.userData = (void*)sg_asChannel[i].m_OCI_HwHandle;
            sg_asChannel[i].m_OCI_RxQueueCfg.onEvent.userData = (void*)sg_asChannel[i].m_OCI_HwHandle;
            BOA_ResultCode ErrorCode = OCI_FAILURE;
            OCI_CANConfiguration ociCANConfig;

            /* configure the controller first */
            ErrorCode = (*(sBOA_PTRS.m_sOCI.openCANController))(sg_asChannel[i].m_OCI_HwHandle,
                        &(sg_asChannel[i].m_OCI_CANConfig),
                        &(sg_asChannel[i].m_OCI_CntrlProp));

            // Check if incompatible config
            if(ErrorCode == BOA_ERR_INCOMPATIBLE_CONFIG)
            {
                // Get the config with which channel opened
                ErrorCode = (*(sBOA_PTRS.m_sOCI.getCANConfiguration))(sg_asChannel[i].m_OCI_HwHandle, &ociCANConfig);

                if(BOA_SUCCEEDED(ErrorCode))
                {
                    //Open the channel with existing configuration
                    ErrorCode = (*(sBOA_PTRS.m_sOCI.openCANController))(sg_asChannel[i].m_OCI_HwHandle,
                                &(ociCANConfig),
                                &(sg_asChannel[i].m_OCI_CntrlProp));
                    sg_asChannel[i].m_OCI_CANConfig = ociCANConfig;
                }
            }
            if (BOA_SUCCEEDED(ErrorCode))
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
                            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get timer resolution"));
                        }
                    }
                    else
                    {
                        hResult = ERR_LOAD_HW_INTERFACE;
                        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add filters"));
                    }
                }
                else
                {
                    hResult = ERR_LOAD_HW_INTERFACE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could create Rx Queue"));
                }

            }
            else
            {
                hResult = ERR_LOAD_HW_INTERFACE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not open controller"));
            }
        }
        else
        {
            hResult = ERR_LOAD_HW_INTERFACE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create controller"));
        }
    }

    /* Check for the success */
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    return hResult;
}

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
                if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.closeCANController))(sg_asChannel[i].m_OCI_HwHandle)))
                {
                    if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.destroyCANController))(sg_asChannel[i].m_OCI_HwHandle)))
                    {
                        hResult |= S_OK;
                    }
                    else
                    {
                        hResult = S_FALSE;
                        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not destroy controller"));
                    }
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not close controller"));
                }
            }
            else
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not destroy the queue"));
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove the filter"));
        }
    }

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    }
    return hResult;
}

/**
 * Call back function called from ConfigDialogDIL
 *
 * @return TRUE for success, FALSE for failure
 */
static BOOL Callback_DILBOA(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, INT /*Length*/)
{
    return (sg_pouDIL_CAN_ETAS_BOA->CAN_SetConfigData( pDatStream, 0) == S_OK);
}

/**
 * Displays the configuration dialog for controller
 *
 * @return S_OK for success, S_FALSE for failure
 */
static int DisplayConfigurationDlg(HWND /* hParent */, DILCALLBACK /*ProcDIL*/,
                                   PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

    SCONTROLLER_DETAILS sController[defNO_OF_CHANNELS];

    for(int i =0 ; i < defNO_OF_CHANNELS; i++)
    {
        sController[i] = pControllerDetails[i];
    }

    CChangeRegisters_CAN_ETAS_BOA ouChangeRegister(nullptr, pControllerDetails, nCount);
    ouChangeRegister.DoModal();

    nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(InitData, WARN_INITDAT_NCONFIRM);

    USES_CONVERSION;
    unsigned int l_length = 0;
    INT Result = WARN_INITDAT_NCONFIRM;
    PSCONTROLLER_DETAILS psContrlDets = (PSCONTROLLER_DETAILS)InitData;

    /* First initialize with existing hw description */
    for (INT i = 0; i < min(Length, (INT)sg_nNoOfChannels); i++, l_length++)
    {
        psContrlDets[i].m_omHardwareDesc = sg_asChannel[i].m_acURI;
    }
    if (l_length > 0)
    {
        Result = DisplayConfigurationDlg(sg_hOwnerWnd, Callback_DILBOA,
                                         psContrlDets, l_length);
        switch (Result)
        {
            case WARNING_NOTCONFIRMED:
                Result = WARN_INITDAT_NCONFIRM;
                break;

            case INFO_INIT_DATA_CONFIRMED:
                Length = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
                Result = CAN_SetConfigData(psContrlDets, Length);
                if (Result == S_OK)
                {
                    Result = INFO_INITDAT_CONFIRM_CONFIG;
                }
                break;

            case INFO_RETAINED_CONFDATA:
                Result = INFO_INITDAT_RETAINED;
                break;

            case ERR_CONFIRMED_CONFIGURED: // Not to be addressed at present
            case INFO_CONFIRMED_CONFIGURED: // Not to be addressed at present
            default:
                /* Do nothing... default return value is S_FALSE. */
                break;
        }
    }

    return Result;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_SetConfigData(PSCONTROLLER_DETAILS pInitData, int /*Length*/)
{
    HRESULT hResult = WARNING_NOTCONFIRMED;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(pInitData, hResult);

    BOA_ResultCode ErrCode = OCI_FAILURE;
    PSCONTROLLER_DETAILS pControllerDetails = (PSCONTROLLER_DETAILS)pInitData;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        ((PSCONTROLLER_DETAILS)pControllerDetails)[i].m_omHardwareDesc = sg_ControllerDetails[i].m_omHardwareDesc;
    }
    bLoadDataFromContr(pControllerDetails);
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        /* First remove all the filters */
        ManageFilters(FILTER_REMOVE, i);

        /* First remove all the Rx Tx queues */
        ManageQueue(QUEUE_DESTROY, i);

        /* if controller is open, close the controller. Do not bother about return value */
        ErrCode = (*sBOA_PTRS.m_sOCI.closeCANController)(sg_asChannel[i].m_OCI_HwHandle);
        OCI_CANConfiguration ociCANConfig;
        /* Now load the controller config and open the controller */
        ErrCode = (*sBOA_PTRS.m_sOCI.openCANController)(sg_asChannel[i].m_OCI_HwHandle,
                  &(sg_asChannel[i].m_OCI_CANConfig),
                  &(sg_asChannel[i].m_OCI_CntrlProp));
        // Check if incompatible config
        if(ErrCode == BOA_ERR_INCOMPATIBLE_CONFIG)
        {
            // Get the config with which channel opened
            ErrCode = (*(sBOA_PTRS.m_sOCI.getCANConfiguration))(sg_asChannel[i].m_OCI_HwHandle, &ociCANConfig);

            if(BOA_SUCCEEDED(ErrCode))
            {
                //Open the channel with existing configuration
                ErrCode = (*(sBOA_PTRS.m_sOCI.openCANController))(sg_asChannel[i].m_OCI_HwHandle,
                          &(ociCANConfig),
                          &(sg_asChannel[i].m_OCI_CntrlProp));
                sg_asChannel[i].m_OCI_CANConfig = ociCANConfig;
            }
        }

        /* Fill the hardware description details */
        ((PSCONTROLLER_DETAILS)pInitData)[i].m_omHardwareDesc =
            sg_asChannel[i].m_acURI;

        if (BOA_SUCCEEDED(ErrCode))
        {
            /* Rx Tx queue */
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
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not configure the controller"));
        }
    }
    return hResult;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        OCI_CANControllerCapabilities capabilities;
#if BOA_VERSION >= BOA_VERSION_2_0
        capabilities.canFDSupport = 0;
#endif
        if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.canioVTable.getCANControllerCapabilities))(sg_asChannel[i].m_OCI_HwHandle,
                          &capabilities)))
        {
#if BOA_VERSION >= BOA_VERSION_2_0
            if( capabilities.canFDSupport & OCI_CANFDSUPPORT)
            {
                /*
                 * The OCI_CAN implementation for the CAN port supports CAN-FD. Latch the URI of the CAN port
                 * in our results.
                 */
                int i =0;
            }
#endif
        }
        if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.getCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                          &(sg_asChannel[i].m_OCI_CntrlProp))))
        {
            if ((sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_SUSPENDED)
                    || (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING))
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_RUNNING;
                if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                                  &(sg_asChannel[i].m_OCI_CntrlProp))))
                {
                    hResult |= S_OK;
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not start the controller in running mode"));
                }
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get controller mode"));
        }
    }

    /* Check for the success */
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_CONNECTED;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);

        InitializeCriticalSection(&sg_CritSectForAckBuf);

        QueryPerformanceCounter(&sg_QueryTickCount);

        /* Get frequency of the performance counter */
        QueryPerformanceFrequency(&sg_lnFrequency);

        /* Convert it to time stamp with the granularity of hundreds of microsecond */
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

HRESULT CDIL_CAN_ETAS_BOA::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.getCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                          &(sg_asChannel[i].m_OCI_CntrlProp))))
        {
            if (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING)
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;
                if (BOA_SUCCEEDED((*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                                  &(sg_asChannel[i].m_OCI_CntrlProp))))
                {
                    hResult |= S_OK;
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not stop the controller in suspended mode"));
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
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get controller mode"));
        }
    }

    /* Check for the success */
    if (hResult == S_OK)
    {
        sg_byCurrState = CREATE_MAP_TIMESTAMP;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
        DeleteCriticalSection(&sg_CritSectForAckBuf);
    }

    return hResult;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;

    return S_OK;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_FALSE;

    if (bClientIdExist(dwClientID))
    {
        if (sCanTxMsg.m_ucChannel <= sg_nNoOfChannels)
        {
#if BOA_VERSION >= BOA_VERSION_2_0
            OCI_CANMessageEx sOciCanMsgEx; // Currently both type of messages are created in BOA 2.0;
            OCI_CANFDTxMessage     sOciTxCanFDMsg;
#endif
            OCI_CANMessage sOciCanMsg;

            OCI_CANTxMessage sOciTxCanMsg;
            SACK_MAP sAckMap;
            vCopy_2_OCI_CAN_Data(sOciTxCanMsg, sCanTxMsg);
            sOciCanMsg.type = OCI_CAN_TX_MESSAGE;
            sOciCanMsg.reserved = 0;
            uint32 nRemaining = 0;
            memcpy(&(sOciCanMsg.data.txMessage), &(sOciTxCanMsg), sizeof(OCI_CANTxMessage));
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_Channel  = sCanTxMsg.m_ucChannel;
            sAckMap.m_MsgID    = sOciCanMsg.data.txMessage.frameID;
            EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
            if ( sg_asChannel[sCanTxMsg.m_ucChannel - 1].m_OCI_CANConfig.selfReceptionMode == OCI_SELF_RECEPTION_ON )
            {
                vMarkEntryIntoMap(sAckMap);
            }
            LeaveCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
            BOA_ResultCode ErrCode;
#if BOA_VERSION >= BOA_VERSION_2_0
            if(sg_asChannel[sCanTxMsg.m_ucChannel - 1].m_bSupportCANFD == true)
            {
                /* Currently if Device supports CAN FD, switch to CAN_FD mode by default.But in future based on user input switch to CAN_FD or CAN mode */
                if(vCopy_2_OCI_CANFD_Data(sOciTxCanFDMsg,sCanTxMsg) == FALSE)
                {
                    return ERR_WRITE_INVALID_SIZE;
                }
                sOciCanMsgEx.type = OCI_CANFDTX_MESSAGE;
                memcpy(&(sOciCanMsgEx.data.canFDTxMessage), &(sOciTxCanFDMsg), sizeof(OCI_CANFDTxMessage));
                OCI_CANMessageEx* ociMsgArr[1];

                ociMsgArr[0] = &sOciCanMsgEx;
                sOciCanMsgEx.data.canFDTxMessage.flags = OCI_CAN_MSG_FLAG_FD_DATA | OCI_CAN_MSG_FLAG_FD_DATA_BIT_RATE;

                ErrCode = (*(sBOA_PTRS.m_sOCI.writeCANDataEx))
                          (sg_asChannel[sCanTxMsg.m_ucChannel - 1].m_OCI_TxQueueHandle, OCI_NO_TIME, ociMsgArr, 1, &nRemaining);


            }
            else
            {

#endif
                ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.writeCANData))
                          (sg_asChannel[sCanTxMsg.m_ucChannel - 1].m_OCI_TxQueueHandle, OCI_NO_TIME, &sOciCanMsg, 1, &nRemaining);
#if BOA_VERSION >= BOA_VERSION_2_0
            }
#endif
            if (BOA_SUCCEEDED(ErrCode))
            {
                hResult = S_OK;
            }
            else
            {
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not write can data into bus"));
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

HRESULT CDIL_CAN_ETAS_BOA::CAN_GetLastErrorString(std::string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {
            case NUMBER_HW:
                lParam = sg_nNoOfChannels;
                break;

            case NUMBER_CONNECTED_HW:
                lParam = sg_nNoOfChannels;
                break;

            case DRIVER_STATUS:
                lParam = sg_bIsDriverRunning;
                break;

            case HW_MODE:
                lParam = (nChannel < sg_nNoOfChannels) ? defMODE_ACTIVE : defCONTROLLER_BUSOFF + 1;
                break;

            case CON_TEST:
                lParam = TRUE;
                break;

            default:
                hResult = S_FALSE;
                break;
        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}

HRESULT CDIL_CAN_ETAS_BOA::CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {
                    for (UINT i = 0; i < sg_nNoOfChannels;)
                    {
                        OCI_CANControllerProperties val;
                        val.mode = OCI_CONTROLLER_MODE_RUNNING;
                        (*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                                &val);
                        i++;

                    }
                }
                break;
                case defMODE_PASSIVE:
                {
                    for (UINT i = 0; i < sg_nNoOfChannels;)
                    {
                        OCI_CANControllerProperties val;
                        val.mode  = OCI_CONTROLLER_MODE_SUSPENDED;
                        (*(sBOA_PTRS.m_sOCI.setCANControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                                &val);
                        i++;

                    }
                }
                break;
            }
        }
    }
    return S_OK;
}

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
