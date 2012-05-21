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
 * \file      SimENG.cpp
 * \brief     Implementation of CSimENG
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CSimENG
 */

#define STRICT
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit
// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

/* MFC includes */
#include <atlbase.h>
#include <atlcom.h>
#include <time.h>

using namespace ATL;

/* C++ includes */
#include <map>

/* Project includes */
#include "resource_BusSim.h"
#include "DataTypes/MsgBufVSE.h"
#include "DataTypes/DIL_Datatypes.h"
#include "SimENG.h"
#include "Utility/Utility.h"
#include "Utility/Utility_Thread.h"

using namespace std;

#define BASE_PIPENAME   "\\\\.\\Pipe\\"
#define PIPE_TIMEOUT    500

const INT SIZE_TIMESTAMP = sizeof(UINT64);

typedef struct
{
    HANDLE  m_hWrite;
    HANDLE  m_hEvent;
    BOOL    m_bActive;
    BYTE    m_bCurrState;
    INT     m_nBus;
    DWORD   m_dwDataSize;

} SPARAM_CLIENT;

typedef map<USHORT, SPARAM_CLIENT> CLIENT_MAP;

static CLIENT_MAP  sg_ClientMap;
static CMsgBufVSE sg_MessageBuf;
static CPARAM_THREADPROC sg_sThreadCtrlObj;

static long long int sg_lnFrequency;
static long long int sg_lnCurrCounter;
static long long int sg_lnTimeStamp;
static SYSTEMTIME sg_CurrSysTime;

//const int MAX_STRING    = 256;
//#define INITIALISE_DATA(Data)   memset(Data, 0, sizeof(Data))

// Buffer for the driver operation related error messages
static string sg_acErrStr;

static CRITICAL_SECTION sg_CriticalSection;

static BYTE* sg_pbEntry = NULL; // This is used as a temporary placeholder for
static INT sg_nEntryLen = 0;    // a frame for the transmitting threads.
static CRITICAL_SECTION sg_CSMsgEntry; // This critical section is to guard the
// aforementioned resource.

static BYTE* sg_pbEntry2 = NULL; // This is used as a temporary placeholder for
static INT sg_nEntryLen2 = 0;    // a frame for the delegating threads.

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
        sg_acErrStr = "system error message retrieval operation failed";
    }
    else
    {
        LPSTR pBuf = T2A((LPTSTR) lpMsgBuf);
        sg_acErrStr = pBuf;
        // Free the buffer.
        LocalFree(lpMsgBuf);
    }
}

/* Format of a message entry:
1. Rx/Tx flag (1 byte; Rx = 0x0, Tx = 0x1)
2. Time stamp (UINT64)
3. Message entry */

DWORD WINAPI MsgDelegatingThread(LPVOID pParam)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pParam;
    WORD wSenderID = 0;
    INT Type = -1;
    UINT64 TimeStamp = 0x0;
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                // Retrieve message from the circular buffer
                while (sg_MessageBuf.GetMsgCount() > 0)
                {
                    EnterCriticalSection(&sg_CriticalSection);
                    BYTE* pbCurrEntry = sg_pbEntry2;
                    INT CurrLength = sg_nEntryLen2;
                    sg_MessageBuf.ReadFromBuffer(Type, pbCurrEntry, CurrLength);
                    // Save the sender id for reference
                    memcpy(&TimeStamp, pbCurrEntry + 1, SIZE_TIMESTAMP);
                    wSenderID = (WORD) TimeStamp;
                    // Calculate the current time stamp assigning the same to
                    // the message
                    long long int CurrCounter;
                    QueryPerformanceCounter((LARGE_INTEGER*) &CurrCounter);

                    // Convert it to time stamp with the granularity of hundreds of us
                    if (CurrCounter * 10000 > CurrCounter)
                    {
                        TimeStamp =
                            (CurrCounter * 10000) / sg_lnFrequency;
                    }
                    else
                    {
                        TimeStamp =
                            (CurrCounter / sg_lnFrequency) * 10000;
                    }

                    // Now save the time stamp calculated
                    memcpy(pbCurrEntry + 1, &TimeStamp, SIZE_TIMESTAMP);
                    CLIENT_MAP::iterator itr;

                    for (itr= sg_ClientMap.begin(); itr != sg_ClientMap.end(); ++itr)
                    {
                        BOOL Result = TRUE;

                        // If the current client is meant for the same bus,
                        // then continue with the same.
                        if (itr->second.m_nBus == Type)
                        {
                            Result = itr->second.m_bActive;
                        }

                        if (Result)
                        {
                            if (itr->first == wSenderID)
                            {
                                // Make the self reception bit up
                                //CurrMsgDat.stcDataMsg.dwHeaderInfoFlags |= 0x800;
                                *pbCurrEntry = 0x1;
                            }

                            DWORD Count = 0;
                            Result = WriteFile(itr->second.m_hWrite, pbCurrEntry,
                                               itr->second.m_dwDataSize, &Count, NULL);
                            //&CurrMsgDat, SIZE_ENTRY, &Count, NULL);
                            SetEvent(itr->second.m_hEvent);
                            FlushFileBuffers(itr->second.m_hWrite);

                            if (itr->first == wSenderID)
                            {
                                // Make the self reception bit down
                                //CurrMsgDat.stcDataMsg.dwHeaderInfoFlags &= ~0x800;
                                *pbCurrEntry = 0x0;
                            }
                        }
                    }

                    LeaveCriticalSection(&sg_CriticalSection);
                }

                ResetEvent(pThreadParam->m_hActionEvent);
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

    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/*void CSimENG::UpdateState(void)
{
    s_FLXMSG CurrMsgDat;
    CurrMsgDat.wMessageType = MESSAGE_STATUS;
    CurrMsgDat.qwTimestamp = 0x0;

    CurrMsgDat.stcStatusMsg.wControllerStatus = (sg_ClientMap.size() > 1) ?
                                                NORMAL_ACTIVE : NORMAL_PASSIVE;

    sg_MessageBuf.WriteIntoBuffer(&CurrMsgDat);
}*/

static void vCreateTimeModeMapping()
{
    // Save the current value of the high-resolution performance counter,
    // associated to the saved system time to the closest proximity.
    QueryPerformanceCounter((LARGE_INTEGER*) &sg_lnCurrCounter);
    // Get frequency of the performance counter
    QueryPerformanceFrequency((LARGE_INTEGER*) &sg_lnFrequency);

    // Convert it to time stamp with the granularity of hundreds of microsecond
    if (sg_lnCurrCounter * 10000 > sg_lnCurrCounter)
    {
        sg_lnTimeStamp = (sg_lnCurrCounter * 10000)
                         / sg_lnFrequency;
    }
    else
    {
        sg_lnTimeStamp = (sg_lnCurrCounter
                          / sg_lnFrequency) * 10000;
    }
}

CSimENG::CSimENG()
{
    InitializeCriticalSection(&sg_CriticalSection);
    InitializeCriticalSection(&sg_CSMsgEntry);
    //MessageBox(NULL, "in CSimENG()", "Member function", MB_OK);
    // First save the current system time
    GetLocalTime(&sg_CurrSysTime);
    vCreateTimeModeMapping();
}


HRESULT CSimENG::FinalConstruct()
{
    // Initialise the random number generator
    srand((unsigned) time(NULL));
    // To create the worker thread that relays messages to other nodes
    // First initialise the parameters
    sg_sThreadCtrlObj.m_hActionEvent = sg_MessageBuf.hGetNotifyingEvent();
    sg_sThreadCtrlObj.m_pBuffer = &sg_MessageBuf;
    sg_sThreadCtrlObj.m_unActionCode = INVOKE_FUNCTION;
    // Now start the thread
    sg_sThreadCtrlObj.bStartThread(MsgDelegatingThread);
    //MessageBox(NULL, "in FinalConstruct()", "Member function", MB_OK);
    return S_OK;
}

void CSimENG::FinalRelease()
{
    // Close the message relay worker thread
    sg_sThreadCtrlObj.bTerminateThread();
    DeleteCriticalSection(&sg_CriticalSection);
    DeleteCriticalSection(&sg_CSMsgEntry);

    if (NULL != sg_pbEntry)
    {
        delete[] sg_pbEntry;
        sg_pbEntry = NULL;
        sg_nEntryLen = 0;
    }

    if (NULL != sg_pbEntry2)
    {
        delete[] sg_pbEntry2;
        sg_pbEntry2 = NULL;
        sg_nEntryLen2 = 0;
    }
}

// CSimENG

#define FLAG_RX     0x0
#define FLAG_TX     0x1

/**
 * \brief     Send Message
 * \req       RSI_5_006 - SendMessage
 *
 * Call this function to send a message to the virtual bus or other nodes.
 */
STDMETHODIMP CSimENG::SendMessage(USHORT ClientID, USHORT CurrDataLength,
                                  BYTE pbCurrDataByte[128])
{
    HRESULT Result = S_FALSE;
    // First ensure this node is active
    CLIENT_MAP::iterator itr = sg_ClientMap.find(ClientID);

    if (itr != sg_ClientMap.end())
    {
        if (itr->second.m_bActive)
        {
            EnterCriticalSection(&sg_CSMsgEntry);
            // First reinitialise
            memset(sg_pbEntry, '\0', sg_nEntryLen);
            BYTE* pbFrame = sg_pbEntry;
            // Calculate total number of bytes of the present entry
            INT TotalLength = 1 + SIZE_TIMESTAMP + CurrDataLength;
            /* Start assigning values to the temporary message entry.
            Rx/Tx flag field is kept as Rx as a default value because except
            for one case this will be FLAG_RX */
            UINT64 un64TS = (UINT64) ClientID;
            memcpy(pbFrame + 1, &un64TS, SIZE_TIMESTAMP);
            memcpy(pbFrame + 1 + SIZE_TIMESTAMP, pbCurrDataByte, CurrDataLength);
            // Add to the internal buffer
            sg_MessageBuf.WriteIntoBuffer(itr->second.m_nBus, pbFrame, TotalLength);
            LeaveCriticalSection(&sg_CSMsgEntry);
            Result = S_OK;
        }
    }

    return Result;
}

/**
 * \brief     Register Client
 * \req       RSI_5_001 - RegisterClient
 *
 * This function makes an entry in the client list of the simulation engine
 * assigning a unique client id. Also, it opens a communication channel
 * betweenthe engine and the client in the form of a pipe and an event for
 * notification.
 */
STDMETHODIMP CSimENG::RegisterClient(USHORT Bus, USHORT MaxLenFrame,
                                     USHORT* ClientID, BSTR* pPipeName,
                                     BSTR* pEventName)
{
    HRESULT Result = S_FALSE;
    EnterCriticalSection(&sg_CriticalSection);
    // Generate a unique random number as client identifier
    USHORT ushTempID;

    // Zero has a special meaning; hence can't be taken as a client ID
    do
    {
        ushTempID = (USHORT) rand();
    }
    while ((sg_ClientMap.find(ushTempID) != sg_ClientMap.end()) && (ushTempID != 0x0));

    // From the client identifier we will now generate a pipe & mutex name
    // First generate mutex name
    char EventName[32] = {'\0'};
    sprintf_s(EventName, "%X", ushTempID);
    // followed by the pipe name
    char PipeName[64] = BASE_PIPENAME;
    strcat_s(PipeName, EventName);
    // Pipe name; convert from ASCII string to BSTR
    BSTR bstrPipe = A2BSTR(PipeName);
    // Mutex name; convert from ASCII string to BSTR
    BSTR bstrEvent = A2BSTR(EventName);
    bool bProceed = ((bstrPipe != NULL) && (bstrEvent != NULL));

    if (bProceed)
    {
        *ClientID = ushTempID;          // Assign client id
        *pPipeName = bstrPipe;          // Assign pipe name
        *pEventName = bstrEvent;        // Assign mutex name
        SPARAM_CLIENT sParams;
        // Both input / output buffer include both Rx/Tx and time stamp fields
        // too. Hence calculate accordingly.
        sParams.m_dwDataSize = 1 + SIZE_TIMESTAMP + MaxLenFrame;
        // Create the conduit for communication
        sParams.m_hWrite = CreateNamedPipe(
                               PipeName,                 // pipe name
                               PIPE_ACCESS_OUTBOUND,     // read/write access
                               PIPE_TYPE_BYTE |          // message type pipe
                               PIPE_READMODE_BYTE |      // message-read mode
                               PIPE_NOWAIT,              // blocking mode
                               PIPE_UNLIMITED_INSTANCES, // max. instances
                               sParams.m_dwDataSize,     // output buffer size
                               sParams.m_dwDataSize,     // input buffer size
                               PIPE_TIMEOUT,             // client time-out
                               NULL);                    // no security attribute

        if (sParams.m_hWrite == INVALID_HANDLE_VALUE)
        {
            bProceed = false;
            GetSystemErrorString();
            MessageBox(NULL, sg_acErrStr.c_str(), "Error", MB_OK);
        }

        if (bProceed)
        {
            ConnectNamedPipe(sParams.m_hWrite, NULL);
            // Generate the communication event
            bProceed = bProceed && ((sParams.m_hEvent
                                     = CreateEvent(NULL, FALSE, FALSE, EventName)) != NULL);
        }

        if (bProceed)
        {
            if ((INT)sParams.m_dwDataSize > sg_nEntryLen2)
            {
                delete[] sg_pbEntry2;
                sg_pbEntry2 = new BYTE[sParams.m_dwDataSize];
                sg_nEntryLen2 = sParams.m_dwDataSize;
            }

            sParams.m_bCurrState = INITIALISED;
            sParams.m_bActive = FALSE;
            sParams.m_nBus = Bus;
            // Now add to the map object
            sg_ClientMap.insert(CLIENT_MAP::value_type(ushTempID, sParams));
            Result = S_OK;
        }
    }

    LeaveCriticalSection(&sg_CriticalSection);

    if (sg_nEntryLen2 > sg_nEntryLen)
    {
        if (S_OK == Result)
        {
            EnterCriticalSection(&sg_CSMsgEntry);
            delete[] sg_pbEntry;
            sg_pbEntry = new BYTE[sg_nEntryLen2];
            sg_nEntryLen = sg_nEntryLen2;
            LeaveCriticalSection(&sg_CSMsgEntry);
        }
        else
        {
            _ASSERT(FALSE);
        }
    }

    return Result;
}

/**
 * \brief     Unegister Client
 * \req       RSI_5_002 - UnregisterClient
 *
 * Call this function to get unregistered from SimENG.
 */
STDMETHODIMP CSimENG::UnregisterClient(USHORT ClientID)
{
    HRESULT Result = S_FALSE;
    EnterCriticalSection(&sg_CriticalSection);
    CLIENT_MAP::iterator itr = sg_ClientMap.find(ClientID);

    if (itr != sg_ClientMap.end())
    {
        // Close the handle of the communication conduit
        CloseHandle(itr->second.m_hEvent);
        itr->second.m_hEvent = NULL;
        DisconnectNamedPipe(itr->second.m_hWrite);
        CloseHandle(itr->second.m_hWrite);
        itr->second.m_hWrite = NULL;
        sg_ClientMap.erase(itr);
        Result = S_OK;
    }

    LeaveCriticalSection(&sg_CriticalSection);
    return Result;
}

/**
 * \brief     Connect Node
 * \req       RSI_5_003 - ConnectNode
 *
 * Connects the caller to the virtual bus. On successful calling of this
 * function, receiving and sending of messages shall be possible.
 */
STDMETHODIMP CSimENG::ConnectNode(USHORT ClientID)
{
    HRESULT Result = S_FALSE;
    CLIENT_MAP::iterator itr = sg_ClientMap.find(ClientID);

    if (itr != sg_ClientMap.end())
    {
        itr->second.m_bActive = TRUE;
        itr->second.m_bCurrState = NORMAL_ACTIVE;
        Result = S_OK;
    }

    vCreateTimeModeMapping();
    return Result;
}

/**
 * \brief     Disconnect Node
 * \req       RSI_5_004 - DisconnectNode
 *
 * Disconnects the caller from the virtual bus.
 */
STDMETHODIMP CSimENG::DisconnectNode(USHORT ClientID)
{
    HRESULT Result = S_FALSE;
    CLIENT_MAP::iterator itr = sg_ClientMap.find(ClientID);

    if (itr != sg_ClientMap.end())
    {
        itr->second.m_bActive = FALSE;
        itr->second.m_bCurrState = INITIALISED;
        Result = S_OK;
    }

    return Result;
}

/**
 * \brief     Get Time Mode Mapping
 * \req       RSI_5_005 - GetTimeModeMapping
 *
 * Call this function to get a system time and the time stamp associated to it.
 */
STDMETHODIMP CSimENG::GetTimeModeMapping(SYSTEMTIME* CurrSysTime, ULONGLONG* TimeStamp, long long int* lQueryTickCount)
{
    // TODO: Add your implementation code here
    memcpy(CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    *TimeStamp = (ULONGLONG) (sg_lnTimeStamp);
    *lQueryTickCount = sg_lnCurrCounter;
    return S_OK;
}

/**
 * \brief     Get Current Status
 * \req       RSI_5_007 - GetCurrentStatus
 *
 * Call this function to get the current state of the node.
 */
STDMETHODIMP CSimENG::GetCurrentStatus(USHORT ClientID, VARIANT* /*pNodeStatus*/)
{
    CLIENT_MAP::iterator itr = sg_ClientMap.find(ClientID);

    if (itr != sg_ClientMap.end())
    {
        //s_FLXSTATUSMSG* pStatusData = (s_FLXSTATUSMSG *) pNodeStatus;
        //pStatusData->wControllerStatus = itr->second.m_bCurrState;
    }

    return S_OK;
}
