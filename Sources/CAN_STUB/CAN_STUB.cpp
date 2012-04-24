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
 * \file      CAN_STUB/CAN_STUB.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CStub
 */

/* C++ includes */
#include <string>

/* Project includes */
#include "CAN_STUB_stdafx.h"
#include "CAN_STUB.h"
#include "include/Error.h"
#include "Include/Struct_CAN.h"
#include "Include/BaseDefs.h"
#include "Include/CanUSBDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "BusEmulation/BusEmulation.h"
#include "BusEmulation/BusEmulation_i.c"
#include "DataTypes/DIL_DataTypes.h"
#include "Utility/Utility_Thread.h"
#include "Utility/Utility.h"
#include "DIL_Interface/BaseDIL_CAN_Controller.h"


#define USAGE_EXPORT
#include "CAN_STUB_Extern.h"

using namespace std;

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCAN_STUBApp

BEGIN_MESSAGE_MAP(CCAN_STUBApp, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_STUBApp construction
 */
CCAN_STUBApp::CCAN_STUBApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/**
 * The one and only CCAN_STUBApp object
 */
CCAN_STUBApp theApp;

/**
 * CCAN_STUBApp initialization
 */
BOOL CCAN_STUBApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/* Parameters to relay outcome of the requested action by any thread to the 
   broker thread to the bus simulation component */
static CRITICAL_SECTION sg_CSBroker;
static HRESULT          sg_hResult          = S_FALSE;
static HANDLE           sg_hNotifyFinish    = NULL;
static STCAN_MSG*       sg_pouCanTxMsg      = NULL;
static SYSTEMTIME       sg_CurrSysTime      = {'\0'};
static LARGE_INTEGER    sg_QueryTickCount;
static UINT64           sg_TimeStampRef     = 0x0;
static HWND             sg_hOwnerWnd        = NULL;

/**
 * Broker thread for the bus emulation
 */
static CPARAM_THREADPROC sg_sBrokerObjBusEmulation;

/**
 * Different action codes
 */
enum
{
    CONNECT = 0x64,
	DISCONNECT,
	STOP_HARDWARE,
	START_HARDWARE,
	SEND_MESSAGE, 
    GET_TIME_MAP,
	REGISTER,
	UNREGISTER,
};

/**
 * \brief Buffer to read msg from the PIPE
 *
 * This buffer include CAN Msg + 1 Byte to indicate TX/RX + UINT64 to indicate timestamp
 */
struct tagPIPE_CANMSG
{
    BYTE m_byTxRxFlag;
    UINT64 m_unTimeStamp;
    STCAN_MSG m_sCanMsg;    
};
typedef tagPIPE_CANMSG SPIPE_CANMSG;

const int SIZE_PIPE_CANMSG       = sizeof(BYTE) + sizeof(UINT64) + sizeof(STCAN_MSG);
const BYTE SIZE_TIMESTAMP = sizeof(UINT64);

#define MAX_CLIENT_ALLOWED 16
#define MAX_BUFF_ALLOWED 16

/**
 * Client and Client Buffer map
 */
struct tagClientBufMap
{
    DWORD dwClientID;
    HANDLE hClientHandle;
    HANDLE hPipeFileHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    TCHAR pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = NULL;
        hPipeFileHandle = NULL;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (TCHAR) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }
    }
};
typedef tagClientBufMap sClientBufMap;
typedef sClientBufMap SCLIENTBUFMAP;
typedef sClientBufMap* PSCLIENTBUFMAP;

/**
 * global client count
 */
UINT sg_unClientCnt = 0;
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
// Forward declarations

/**
 * Application buffer, logging interface and client id
 */
static CPARAM_THREADPROC sg_sParmRThreadStub;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

/* Starts definitions of static global variables */
static USHORT sg_ushTempClientID = 0;
static HANDLE sg_hTmpClientHandle = NULL;
static HANDLE sg_hTmpPipeHandle = NULL;
/* Ends definitions of static global variables */


/**
 * Buffer for the driver operation related error messages
 */
static CHAR sg_acErrStr[MAX_STRING] = {'\0'};

/**
 * Starts code for the state machine
 */
enum
{
    STATE_PRIMORDIAL    = 0x0,
    STATE_RESET,
    STATE_REGISTERED,
    STATE_INITIALISED,
    STATE_CONNECTED
};

BYTE sg_bCurrState = STATE_PRIMORDIAL;

/* CDIL_CAN_STUB class definition */
class CDIL_CAN_STUB : public CBaseDIL_CAN_Controller
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
	HRESULT CAN_GetLastErrorString(string& acErrorStr);
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

static CDIL_CAN_STUB* sg_pouDIL_CAN_STUB = NULL;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{	
	HRESULT hResult = S_OK;
	if ( NULL == sg_pouDIL_CAN_STUB )
	{
		if ((sg_pouDIL_CAN_STUB = new CDIL_CAN_STUB) == NULL)
		{
			hResult = S_FALSE;
		}
	}
	*ppvInterface = (void *) sg_pouDIL_CAN_STUB; /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */

	return hResult;
}

// Worker function declarations: start
HRESULT Worker_Connect(ISimENG*, Base_WrapperErrorLogger*);
HRESULT Worker_Disconnect(ISimENG*, Base_WrapperErrorLogger*);
HRESULT Worker_StopHardware(ISimENG*, Base_WrapperErrorLogger*);
HRESULT Worker_StartHardware(ISimENG*, Base_WrapperErrorLogger*);
HRESULT Worker_SendCanMsg(ISimENG*, Base_WrapperErrorLogger*, STCAN_MSG*);
HRESULT Worker_GetTimeModeMapping(ISimENG*, Base_WrapperErrorLogger*);
HRESULT Worker_RegisterClient(ISimENG* pISimENG, Base_WrapperErrorLogger* pIlog);
HRESULT Worker_UnregisterClient(ISimENG* pISimENG, Base_WrapperErrorLogger* pIlog);
// Worker function declarations: end

// Prototype declarations
HRESULT PerformAnOperation(BYTE bActionCode);
DWORD WINAPI BrokerThreadBusEmulation(LPVOID pVoid);

BYTE GetCurrState(void)
{
    return sg_bCurrState;
}

void SetCurrState(BYTE bNextState)
{
    sg_bCurrState = bNextState;
}

/* Ends code for the state machine */


static void vInitialiseAllData(void)
{
    // Initialise both the time parameters
    INITIALISE_DATA(sg_acErrStr);
}

static void GetSystemErrorString()
{
    LPVOID lpMsgBuf;
    DWORD dwResult = 0;

    dwResult = FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL );
    if (dwResult <= 0)
    {
        strcpy_s(sg_acErrStr, "system error message retrieval operation failed");
    }
    else
    {
        LPSTR pBuf = T2A((LPTSTR) lpMsgBuf);
        INITIALISE_DATA(sg_acErrStr);
        lstrcpy(sg_acErrStr, pBuf);
        // Free the buffer.
        LocalFree(lpMsgBuf);
    }
}

/* USEFUL MACROS AND FUNCTIONS: END */

static BOOL bGetClientObj(HANDLE hClientHandle, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].hClientHandle == hClientHandle)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; //break the loop
        }
    }
    return bExist;
}

/* START OF READ THREAD FUNCTION WITH ITS HELPER FUNCTION */
#define FLAG_RX     0x0
#define FLAG_TX     0x1 /*these definitions are expected to 
                        change if there is any modifications in SimEng.cpp*/

const USHORT SIZE_DAT_P = sizeof(SPIPE_CANMSG);
static void ProcessCanMsg(HANDLE hClientHandle, UINT unIndex)
{
    static SPIPE_CANMSG sPipeCanMsg;
    static STCANDATA sCanData;
    static DWORD dwBytes = 0;

    /* To be noted - there is no validation for any pointer. This is because 
    this function assumes them to have been duly validated beforehand and it
    is so by implementation. Efficiency is the motivation behind. */
    BYTE abyData[SIZE_DAT_P] = {'\0'};

    //if (ReadFile(hClientHandle, &sPipeCanMsg, SIZE_PIPE_CANMSG, &dwBytes, NULL))
    if (ReadFile(hClientHandle, abyData, SIZE_DAT_P, &dwBytes, NULL))
    {
        memcpy(&(sPipeCanMsg.m_byTxRxFlag), abyData, 1);
        memcpy(&(sPipeCanMsg.m_unTimeStamp), abyData + 1, SIZE_TIMESTAMP);
        memcpy(&(sPipeCanMsg.m_sCanMsg), abyData + 1 + SIZE_TIMESTAMP, sizeof(STCAN_MSG));
        if (dwBytes == SIZE_PIPE_CANMSG)
        {   
            sCanData.m_lTickCount.QuadPart = sPipeCanMsg.m_unTimeStamp;
            sCanData.m_uDataInfo.m_sCANMsg = sPipeCanMsg.m_sCanMsg;

            if (sPipeCanMsg.m_byTxRxFlag == FLAG_TX)
            {
                sCanData.m_ucDataType = TX_FLAG;
            }
            else
            {                
                sCanData.m_ucDataType = RX_FLAG;            
            }
            for (UINT i = 0; i < sg_asClientToBufMap[unIndex].unBufCount; i++)
            {
                sg_asClientToBufMap[unIndex].pClientBuf[i]->WriteIntoBuffer(&sCanData);
            }
        }
    }
    else
    {
        GetSystemErrorString();
    }
}

DWORD WINAPI FlexMsgReadThreadProc_Stub(LPVOID pVoid)
{
    VALIDATE_POINTER_RETURN_VAL(sg_pIlog, (DWORD)-1);

	CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;

    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
    // Validate certain required pointers
   
	//Set the action event
	if ( sg_unClientCnt > 0 )
	{
		pThreadParam->m_hActionEvent = sg_asClientToBufMap[0].hClientHandle;
	}

    bool bLoopON = true;

	while (bLoopON)
	{
        static HANDLE ahClientReadHandle[MAX_CLIENT_ALLOWED] = {0};
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            ahClientReadHandle[i] = sg_asClientToBufMap[i].hClientHandle;
        }
        DWORD dwIndex = WaitForMultipleObjects(sg_unClientCnt, ahClientReadHandle, FALSE, INFINITE);

        if (dwIndex == WAIT_FAILED)
        {
            GetSystemErrorString();
        }
        else
        {
            UINT Index = dwIndex - WAIT_OBJECT_0;
            switch (pThreadParam->m_unActionCode)
		    {
			    case INVOKE_FUNCTION:
			    {
				    // Retrieve message from the pipe
                    ProcessCanMsg(sg_asClientToBufMap[Index].hPipeFileHandle, Index);
			    }
			    break;
                case CREATE_TIME_MAP:
                {
                    PerformAnOperation(GET_TIME_MAP);
                    ProcessCanMsg(sg_asClientToBufMap[Index].hPipeFileHandle, Index);
                    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
                }
			    break;
			    case EXIT_THREAD:
			    {
				    bLoopON = false;
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
	}
    SetEvent(pThreadParam->hGetExitNotifyEvent());

    return 0;
}

HRESULT PerformAnOperation(BYTE bActionCode)
{
    HRESULT hResult = S_FALSE;

    // Lock so that no other thread may use the common resources
    EnterCriticalSection(&sg_CSBroker);

    // Identify current assignment of the broker thread
    sg_sBrokerObjBusEmulation.m_unActionCode = bActionCode;
    // Now release the harness
    SetEvent(sg_sBrokerObjBusEmulation.m_hActionEvent);
    // Wait until current assignment of broker thread is over
    WaitForSingleObject(sg_hNotifyFinish, INFINITE);
    // Save the result
    hResult = sg_hResult;

    // Work is over, now unlock for others to use common resources
    LeaveCriticalSection(&sg_CSBroker);

    return hResult;
}

/* END OF READ THREAD FUNCTION WITH ITS HELPER FUNCTION */

/* CDIL_CAN_STUB function definitions */

HRESULT CDIL_CAN_STUB::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    HRESULT hResult = S_FALSE;

    if (GetCurrState() == STATE_PRIMORDIAL) // Only now this operation makes sense
    {
        if ((pILog != NULL))
        {
            sg_hOwnerWnd = hWndOwner;       // Owner window handle
            sg_pIlog = pILog;               // Log interface pointer
            vInitialiseAllData();           // Initialise other data

            SetCurrState(STATE_RESET);      // Reset state from primordial
            hResult = S_OK;                 // All okey
        }
        else
        {
            strcpy_s(sg_acErrStr, "Null argument value(s) in SetAppParams");
        }
    }
    else
    {
        strcpy_s(sg_acErrStr, "Improper current state to call SetAppParams");
    }

    return hResult;
}

HRESULT CDIL_CAN_STUB::CAN_DisplayConfigDlg(PCHAR& InitData, int& Length)
{
    HRESULT Result = S_FALSE;
    char acInitFile[MAX_PATH] = {'\0'};

    // Assuming that InitData points to a CHAR array with size MAX_PATH
    strcpy_s(acInitFile, InitData);

    int nResult = WARNING_NOTCONFIRMED;//DisplayConfigurationDlg(sg_hOwnerWnd, Callback_DILStub, 
                                          //acInitFile, DRIVER_STUB);
    switch (nResult)
    {
        case WARNING_NOTCONFIRMED:
        {
            Result = WARN_INITDAT_NCONFIRM;
        }
        break;
        case INFO_INIT_DATA_CONFIRMED:
        {
            strcpy(InitData, acInitFile); // Copy init file path
            Length = lstrlen(acInitFile) + 1;
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

    return Result;
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
            //clear the client first 
            sg_ushTempClientID  = (SHORT)sg_asClientToBufMap[unClientIndex].dwClientID;
            sg_hTmpClientHandle = sg_asClientToBufMap[unClientIndex].hClientHandle;
            sg_hTmpPipeHandle   = sg_asClientToBufMap[unClientIndex].hPipeFileHandle;
            HRESULT hResult = PerformAnOperation(UNREGISTER);
            if (hResult == S_OK)
            {       
                sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                sg_asClientToBufMap[unClientIndex].hClientHandle = NULL;
                sg_asClientToBufMap[unClientIndex].hPipeFileHandle = NULL;

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
                sg_pIlog->vLogAMessage(__FILE__, __LINE__, _T("Unregister failed"));
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
 * Register Client
 */
HRESULT CDIL_CAN_STUB::CAN_RegisterClient(BOOL bRegister,DWORD& ClientID, TCHAR* pacClientName)
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
                strcpy_s(sg_asClientToBufMap[sg_unClientCnt].pacClientName, pacClientName);
                if (PerformAnOperation(REGISTER) == S_OK)
                {
                    ClientID = sg_asClientToBufMap[sg_unClientCnt].dwClientID = sg_ushTempClientID;
                    sg_asClientToBufMap[sg_unClientCnt].hClientHandle = sg_hTmpClientHandle;
                    sg_asClientToBufMap[sg_unClientCnt].hPipeFileHandle = sg_hTmpPipeHandle;
                    sg_asClientToBufMap[sg_unClientCnt].unBufCount = 0;
                    sg_unClientCnt++;
                    hResult = S_OK;
                }
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

HRESULT CDIL_CAN_STUB::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_FALSE;
}

HRESULT CDIL_CAN_STUB::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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
                //Add msg buffer
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
                //clear msg buffer
                //clear msg buffer
                if (pBufObj != NULL) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = NULL;
                    }
                    sClientObj.unBufCount = 0;
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
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, NULL);                        
            }
            hResult = S_OK;
        }        
    }
    
    return hResult;
}

HRESULT CDIL_CAN_STUB::CAN_StopHardware(void)
{
    HRESULT hResult = PerformAnOperation(DISCONNECT);
    if (hResult == S_OK)
    {
      hResult = PerformAnOperation(STOP_HARDWARE);
    }
    return hResult;
}

HRESULT CDIL_CAN_STUB::CAN_SetConfigData(PCHAR ConfigFile, int /*Length*/)
{
    switch (GetCurrState())
    {
        case STATE_REGISTERED:
        case STATE_CONNECTED:
        case STATE_INITIALISED:
        break;
        case STATE_PRIMORDIAL:
        case STATE_RESET:
        default:
        {
            strcpy_s(sg_acErrStr, "CAN_STUB_SetConfigData called at improper state");
            return S_FALSE;
        }
        break;
    }

	/* Fill the hardware description details */
    for (UINT nCount = 0; nCount < defNO_OF_CHANNELS; nCount++)
	{		
		strcpy_s(((PSCONTROLER_DETAILS)ConfigFile)[nCount].m_omHardwareDesc, 
				"Simulation");		
	}    

    // First disconnect the node
    CAN_StopHardware();
    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_StartHardware(void)
{
    //First stop the hardware
    HRESULT hResult = PerformAnOperation(CONNECT);
    if (hResult == S_OK)
    {
      hResult = PerformAnOperation(START_HARDWARE);
    }
    return hResult;    
}

HRESULT CDIL_CAN_STUB::CAN_ResetHardware(void)
{
    // Clear the transmitable message list

    // Now disconnect the node
    return CAN_StopHardware();
}

HRESULT CDIL_CAN_STUB::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    HRESULT hResult = S_FALSE;

    // Lock so that no other thread may use the common resources
    EnterCriticalSection(&sg_CSBroker);

    // Assign parameter
    STCAN_MSG sTmpMsg = sCanTxMsg;
    sg_pouCanTxMsg = &sTmpMsg;
    sg_ushTempClientID = (USHORT)dwClientID;
    // Identify current assignment of the broker thread
    sg_sBrokerObjBusEmulation.m_unActionCode = SEND_MESSAGE;
    // Now release the harness
    SetEvent(sg_sBrokerObjBusEmulation.m_hActionEvent);
    // Wait until current assignment of broker thread is over
    WaitForSingleObject(sg_hNotifyFinish, INFINITE);
    // Save the result
    hResult = sg_hResult;

    // Work is over, now unlock for others to use common resources
    LeaveCriticalSection(&sg_CSBroker);

    return hResult;
}

HRESULT CDIL_CAN_STUB::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& unCntrlStatus)
{
    HRESULT hResult = S_OK;
    //EnterCriticalSection(&sg_CSBroker);
    switch (GetCurrState())
    {
        case STATE_CONNECTED:
        {
            unCntrlStatus = NORMAL_ACTIVE;
        }
        break;
        case STATE_INITIALISED:
        {
            unCntrlStatus = INITIALISED;
        }
        break;
        case STATE_REGISTERED:
        {
            unCntrlStatus = RESET_STATE;
        }
        break;
        case STATE_PRIMORDIAL:
        case STATE_RESET:
        default:
        {
            hResult = S_FALSE;
        }
        break;
    }
    //LeaveCriticalSection(&sg_CSBroker);

    return hResult;
}

HRESULT CDIL_CAN_STUB::CAN_FilterFrames(FILTER_TYPE /*FilterType*/, TYPE_CHANNEL /*Channel*/, UINT* /*punMsgIds*/, UINT /*nLength*/)
{
    return S_FALSE;
}

HRESULT CDIL_CAN_STUB::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return S_FALSE;
}

HRESULT CDIL_CAN_STUB::CAN_GetBoardInfo(s_BOARDINFO& /*BoardInfo*/)
{
    return S_FALSE;
}

HRESULT CDIL_CAN_STUB::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStampRef;
    if(QueryTickCount != NULL)
    {
        *QueryTickCount = sg_QueryTickCount;
    }
    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_GetVersionInfo(VERSIONINFO& /*sVerInfo*/)
{
    // Return error value; this doesn't make sense in case of simulation
    return S_FALSE;
}

HRESULT CDIL_CAN_STUB::CAN_GetLastErrorString(string& acErrorStr)
{
    acErrorStr = sg_acErrStr;

    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_PerformInitOperations(void)
{
	HRESULT hResult = S_FALSE;

    // Initialize the critical section
    InitializeCriticalSection(&sg_CSBroker);

    // Create the notification event
    sg_hNotifyFinish = CreateEvent(NULL, false, false, NULL);
    if (NULL != sg_hNotifyFinish)
    {
        // Then create the broker worker thread
        sg_sBrokerObjBusEmulation.m_hActionEvent = CreateEvent(NULL, false, 
                                                               false, NULL);
        ResetEvent(sg_sBrokerObjBusEmulation.m_hActionEvent);
        sg_sBrokerObjBusEmulation.m_unActionCode = INACTION;
        if (sg_sBrokerObjBusEmulation.bStartThread(BrokerThreadBusEmulation))
        {
            hResult = S_OK;
        }
        else
        {
            CloseHandle(sg_hNotifyFinish);
            sg_hNotifyFinish = NULL;
        }
    }

    return hResult;
}

/**
 * Function to get Controller status
 */
HRESULT CDIL_CAN_STUB::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
	StatusData.wControllerStatus = NORMAL_ACTIVE;
    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_PerformClosureOperations(void)
{
    //First remove all the client
    for (UINT nCount = 0; nCount < sg_unClientCnt; nCount++)
    {
        CAN_RegisterClient(FALSE, sg_asClientToBufMap[nCount].dwClientID, _T(""));
    }    
    // First disconnect from the simulation engine
    CAN_DeselectHwInterface();
    // Then terminate the broker thread
    sg_sBrokerObjBusEmulation.bTerminateThread();
    // Close the notification event
    if (NULL != sg_hNotifyFinish)
    {
        CloseHandle(sg_hNotifyFinish);
        sg_hNotifyFinish = NULL;
    }
    // Delete the critical section
    DeleteCriticalSection(&sg_CSBroker);

    sg_pIlog = NULL;         // Log interface pointer

    SetCurrState(STATE_PRIMORDIAL);

    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
    for (UINT i = 0; i < CHANNEL_ALLOWED; i++)
    {
        sSelHwInterface[i].m_dwIdInterface = 0x100;
        strcpy_s(sSelHwInterface[i].m_acNameInterface, "Simulation");
        strcpy_s(sSelHwInterface[i].m_acDescription, "A simulation engine to create a virtual bus system");
    }
    nCount = CHANNEL_ALLOWED;
    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*sSelHwInterface*/, INT /*nSize*/)
{
    SetCurrState(STATE_INITIALISED);
    return S_OK;
}

HRESULT CDIL_CAN_STUB::CAN_DeselectHwInterface(void)
{
    return PerformAnOperation(DISCONNECT);
}

HRESULT CDIL_CAN_STUB::CAN_LoadDriverLibrary(void)
{
	return S_OK;
}
HRESULT CDIL_CAN_STUB::CAN_UnloadDriverLibrary(void)
{
	return S_OK;
}


HRESULT CDIL_CAN_STUB::CAN_GetControllerParams(LONG& lParam, UINT /*nChannel*/, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    switch (eContrParam)
    {
        
        case NUMBER_HW:
        {
            //venkat
            lParam = CHANNEL_ALLOWED;
        }
        break;
        case NUMBER_CONNECTED_HW:
        {
            //venkat
            lParam = CHANNEL_ALLOWED;
        }
        break;
        case DRIVER_STATUS:
        {
            lParam = TRUE;
        }
        break;
        case HW_MODE:
        {
            lParam = defMODE_SIMULATE;            
        }
        break;
        case CON_TEST:
        {
            lParam = (LONG)-1;            
        }
        break;
        default:
        {
            hResult = S_FALSE;
        }
        break;

    }
    return hResult;
}

HRESULT CDIL_CAN_STUB::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    memset(&sErrorCnt, 0, sizeof(SERROR_CNT));
    return S_OK;
}

DWORD WINAPI BrokerThreadBusEmulation(LPVOID pVoid)
{
	CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;

    // Validate certain required pointers
    //VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam, -1);
    //VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam->m_hActionEvent, -1);

    // Get hold of the bus simulation and error logger interfaces
    HRESULT hResult = CoInitialize(NULL);

    ISimENG* pISimENG = NULL;
    hResult = CoCreateInstance(CLSID_SimENG, NULL, CLSCTX_LOCAL_SERVER,
                               IID_ISimENG, (LPVOID *) &pISimENG);
    if ((S_OK != hResult) || (NULL == pISimENG))
    {
        return 0L;
    }


    bool bLoopON = true;

	while (bLoopON)
	{
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
		{
            case CONNECT:
            {
                sg_hResult = Worker_Connect(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
            }
            break;
            case DISCONNECT:
            {
                sg_hResult = Worker_Disconnect(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
            }
            break;
            case STOP_HARDWARE:
            {
                sg_hResult = Worker_StopHardware(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
            }
            break;
			case START_HARDWARE:
			{
                sg_hResult = Worker_StartHardware(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
			}
			break;
			case SEND_MESSAGE:
			{
                sg_hResult = Worker_SendCanMsg(pISimENG, sg_pIlog, sg_pouCanTxMsg);
                SetEvent(sg_hNotifyFinish);
			}
			break;
            case GET_TIME_MAP:
            {
                sg_hResult = Worker_GetTimeModeMapping(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
            }
            break;
            case REGISTER:
            {
                sg_hResult = Worker_RegisterClient(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
            }
            break;
            case UNREGISTER:
            {
                sg_hResult = Worker_UnregisterClient(pISimENG, sg_pIlog);
                SetEvent(sg_hNotifyFinish);
            }
            break;
			case EXIT_THREAD:
			{
				bLoopON = false;
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

    // Release the bus simulation & error logger out-of-proc server components
    pISimENG->Release();
    // Reset the interface pointers
    pISimENG = NULL;

    SetEvent(pThreadParam->hGetExitNotifyEvent());

    return 0;
}

/* Worker function definitions: start */

HRESULT Worker_Connect(ISimENG* pISimENGLoc, Base_WrapperErrorLogger* pIlogLoc)
{
    if (GetCurrState() == STATE_PRIMORDIAL)
    {
        strcpy_s(sg_acErrStr, "CAN_STUB_Connect called at STATE_PRIMORDIAL");
        return S_FALSE;
    }
    else if (GetCurrState() != STATE_INITIALISED)
    {
        sg_pIlog->vLogAMessage(__FILE__, __LINE__, 
            ("CAN_STUB_Connect called at improper state"));
        return S_FALSE;
    }

    sg_sParmRThreadStub.m_unActionCode = INVOKE_FUNCTION;
	if (sg_sParmRThreadStub.bStartThread(FlexMsgReadThreadProc_Stub) == FALSE)
	{
        sg_sParmRThreadStub.m_hActionEvent = NULL;
        // Unregister from the simulation engine
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            sg_ushTempClientID  = (USHORT)sg_asClientToBufMap[i].dwClientID;
            sg_hTmpClientHandle = sg_asClientToBufMap[i].hClientHandle;
            sg_hTmpPipeHandle   = sg_asClientToBufMap[i].hPipeFileHandle;
            Worker_UnregisterClient(pISimENGLoc, pIlogLoc);
            sg_asClientToBufMap[i].dwClientID = 0;
            sg_asClientToBufMap[i].hClientHandle = NULL;
            sg_asClientToBufMap[i].hPipeFileHandle = NULL;
        }
    	sg_pIlog->vLogAMessage(__FILE__, __LINE__,
                    ("Unable to start the reading thread"));
        return S_FALSE;
    }

    // Reaching upto this point means all the necessary activities are over
    SetCurrState(STATE_REGISTERED);
    
    return S_OK;
}

HRESULT Worker_Disconnect(ISimENG* /*pISimENGLoc*/, Base_WrapperErrorLogger* pIlogLoc)
{
    if (GetCurrState() == STATE_PRIMORDIAL)
    {
        strcpy_s(sg_acErrStr, "CAN_STUB_DeselectHwInterface called at STATE_PRIMORDIAL");
        return S_FALSE;
    }
    else if (GetCurrState() == STATE_RESET)
    {
        pIlogLoc->vLogAMessage(__FILE__, __LINE__, 
            ("CAN_STUB_DeselectHwInterface called at improper state"));
        return S_FALSE;
    }

    // Close the message reading thread
    sg_sParmRThreadStub.bTerminateThread();

    return S_OK;
}

HRESULT Worker_StopHardware(ISimENG* pISimENGLoc, Base_WrapperErrorLogger* pIlogLoc)
{
    HRESULT hResult = S_FALSE;
    if (GetCurrState() == STATE_CONNECTED)
    {
        // Just disconnect the node from the bus simulation
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            hResult = pISimENGLoc->DisconnectNode((USHORT)sg_asClientToBufMap[i].dwClientID);
        }        
        if (hResult == S_OK)
        {
            SetCurrState(STATE_INITIALISED);
        }
    }
    else
    {
		if ( pIlogLoc )
        pIlogLoc->vLogAMessage(__FILE__, __LINE__, 
            ("CAN_STUB_StopHardware called at improper state"));
    }

    return hResult;
}

HRESULT Worker_StartHardware(ISimENG* pISimENGLoc, Base_WrapperErrorLogger* pIlogLoc)
{
    HRESULT hResult = S_FALSE;
    if ((GetCurrState() == STATE_INITIALISED) || (GetCurrState() == STATE_REGISTERED))
    {
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            hResult = pISimENGLoc->ConnectNode((USHORT)sg_asClientToBufMap[i].dwClientID);
        }
        if (hResult == S_OK)
        {
            SetCurrState(STATE_CONNECTED);
        }
    }
    else
    {
        pIlogLoc->vLogAMessage(__FILE__, __LINE__, 
            ("CAN_STUB_StartHardware called at improper state"));
    }

    return hResult;
}

HRESULT Worker_SendCanMsg(ISimENG* pISimENGLoc, Base_WrapperErrorLogger* /*pIlogLoc*/, 
                          STCAN_MSG* pouCanTxMsg)
{
	//VALIDATE_POINTER_RETURN_VAL(pIlogLoc, S_FALSE);
	//VALIDATE_POINTER_RETURN_VALUE_LOG(pouFlxTxMsg, S_FALSE);

    return pISimENGLoc->SendMessage(sg_ushTempClientID, sizeof(STCAN_MSG), (BYTE *)pouCanTxMsg);
}

HRESULT Worker_GetTimeModeMapping(ISimENG* pISimENGLoc, Base_WrapperErrorLogger* /*pIlogLoc*/)
{
    return pISimENGLoc->GetTimeModeMapping(&sg_CurrSysTime, &sg_TimeStampRef, &sg_QueryTickCount);
}

HRESULT Worker_RegisterClient(ISimENG* pISimENG, Base_WrapperErrorLogger* pIlog)
{ 
    USHORT ushClientID = 0;     // Client ID issued from the simulation engine
    BSTR PipeName;              // Name of the conduit
    BSTR EventName;             // Name of the message notifying event
    HRESULT hResult = S_OK;

    // Register to the simulation engine
    hResult = pISimENG->RegisterClient(CAN, sizeof (STCAN_MSG), &ushClientID, &PipeName, &EventName);
    if (hResult != S_OK)
    {
        pIlog->vLogAMessage(__FILE__, __LINE__, 
                    ("unable to register to the simulation engine"));
        return S_FALSE;
    }

    sg_ushTempClientID = ushClientID;

    // Extract the pipe and event name in normal string
    char acPipeName[64] = {'\0'};
    char acEventName[32] = {'\0'};
    if (BSTR_2_PCHAR(PipeName, acPipeName, 64) == false)
    {
        hResult = S_FALSE;
    }

    if (BSTR_2_PCHAR(EventName, acEventName, 32) == false)
    {
        hResult = S_FALSE;
    }

    if (S_FALSE == hResult)
    {
        // Unregister from the simulation engine
        pISimENG->UnregisterClient(ushClientID);
        pIlog->vLogAMessage(__FILE__, __LINE__,
                    ("Can't convert from BSTR to ASCII string"));
        return S_FALSE;
    }

    // Get the pipe handle
    sg_hTmpPipeHandle = CreateFile(
        acPipeName,     // pipe name 
        GENERIC_READ,   // read access 
        0,              // no sharing 
        NULL,           // no security attributes
        OPEN_EXISTING,  // opens existing pipe 
        0,              // default attributes 
        NULL);          // no template file 

    if (sg_hTmpPipeHandle == INVALID_HANDLE_VALUE) 
    {
        // Unregister from the simulation engine
        pISimENG->UnregisterClient(ushClientID);
        GetSystemErrorString();
        LOG_ERR_MSG();
        return S_FALSE;
    }

    sg_hTmpClientHandle = OpenEvent(EVENT_ALL_ACCESS, FALSE, acEventName);
    if (sg_hTmpClientHandle == NULL)
    {
        CloseHandle(sg_hTmpPipeHandle);
        sg_hTmpPipeHandle = NULL;
        // Unregister from the simulation engine
        pISimENG->UnregisterClient(ushClientID);
        GetSystemErrorString();
        LOG_ERR_MSG();
        return S_FALSE;
    }
    return hResult;
}

HRESULT Worker_UnregisterClient(ISimENG* pISimENG, Base_WrapperErrorLogger* /*pIlog*/)
{    
    // Close reading handle of the pipe from simulation engine
    if (NULL != sg_hTmpPipeHandle)
    {
        CloseHandle(sg_hTmpPipeHandle);
        sg_hTmpPipeHandle = NULL;
    }

    if (NULL != pISimENG)
    {
        // Unregister from the simulation engine
        if (pISimENG->UnregisterClient(sg_ushTempClientID) == S_OK)
        {
            sg_ushTempClientID = 0;
        }
    }
    // just make read event null, don't close it
    sg_hTmpClientHandle = NULL;

    //SetCurrState(STATE_RESET);

    return S_OK;
}

/* Worker function definitions: end */
