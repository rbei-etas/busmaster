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
 * \file      CAN_ISOLAR_EVE_VCAN_Defines.h
 * \brief     Contains all the definitions and Function pointers
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains all the definitions and Function pointers
 */

/* Vendor header files */

/* Project headers definitions */
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Utility/Utility_Thread.h"


#define MAX_BUFF_ALLOWED    16
#define MAX_CLIENT_ALLOWED  16
#define CAN_MAX_ERRSTR 256
/**
 * Client and Client Buffer map
 */
typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    char pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = NULL;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }

    }
} SCLIENTBUFMAP;

/* Project definitions */
static UINT sg_unClientCnt = 0;
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

/* Receive Thread object */
static CPARAM_THREADPROC sg_sParmRThread;

/* For maintaining Acknwoledgement map entries */
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
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED],
                                UINT& unCount, CBaseCANBufFSE* BufferToRemove);


/*Server Node Defenitions*/


/* Start BUSMASTER global variables */
// Numerical values of commands from Client
#define  CONNECT          1
#define  DISCONNECT       2
#define  RESET_CONTROLLER 3
#define  SEND_MESSAGE     4
#define  WRITE_TO_LOGFILE 5
#define  TRACE            6
#define  GET_VERSION_INFO 7
#define  DISABLE_INTERRUPT 8
#define  ENABLE_INTERRUPT 9
#define  ENABLE_DISABLE_MSG_TX 10

// Numerical values of commands to Client
#define RECEIVE_MESSAGE_ASYNC 1
#define BUSOFF 2
#define WAKEUP 3


typedef int  (CALLBACK* LPFNDLLFUNC_Initialize)();
typedef void (CALLBACK* LPFNDLLFUNC_CleanUp)();
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveCANMessageFromClient)(STCAN_MSG* TxMsg);
typedef int  (CALLBACK* LPFNDLLFUNC_SendCANMessageToClient)(STCAN_MSG* RxMsg);
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveCommandFromClient)(int* functionType, unsigned int* dataSize);
typedef int  (CALLBACK* LPFNDLLFUNC_SendCommandToClient)(int functionType);
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveTraceFromClient)(char* traceBuffer, unsigned int bufferSize);
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveLogFromClient)(char* traceBuffer, unsigned int bufferSize);

LPFNDLLFUNC_Initialize Initialize;
LPFNDLLFUNC_CleanUp CleanUp;
LPFNDLLFUNC_ReceiveCANMessageFromClient ReceiveCANMessageFromClient;
LPFNDLLFUNC_SendCANMessageToClient SendCANMessageToClient;
LPFNDLLFUNC_ReceiveCommandFromClient ReceiveCommandFromClient;
LPFNDLLFUNC_SendCommandToClient SendCommandToClient;
LPFNDLLFUNC_ReceiveTraceFromClient ReceiveTraceFromClient;
LPFNDLLFUNC_ReceiveLogFromClient ReceiveLogFromClient;


typedef int  (CALLBACK* LPFNDLLFUNC_Initialize_tx)();
typedef void (CALLBACK* LPFNDLLFUNC_CleanUp_tx)();
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveCANMessageFromClient_tx)(STCAN_MSG* TxMsg);
typedef int  (CALLBACK* LPFNDLLFUNC_SendCANMessageToClient_tx)(STCAN_MSG* RxMsg);
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveCommandFromClient_tx)(int* functionType, unsigned int* dataSize);
typedef int  (CALLBACK* LPFNDLLFUNC_SendCommandToClient_tx)(int functionType);
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveTraceFromClient_tx)(char* traceBuffer, unsigned int bufferSize);
typedef int  (CALLBACK* LPFNDLLFUNC_ReceiveLogFromClient_tx)(char* traceBuffer, unsigned int bufferSize);

LPFNDLLFUNC_Initialize Initialize_tx;
LPFNDLLFUNC_CleanUp CleanUp_tx;
LPFNDLLFUNC_ReceiveCANMessageFromClient ReceiveCANMessageFromClient_tx;
LPFNDLLFUNC_SendCANMessageToClient SendCANMessageToClient_tx;
LPFNDLLFUNC_ReceiveCommandFromClient ReceiveCommandFromClient_tx;
LPFNDLLFUNC_SendCommandToClient SendCommandToClient_tx;
LPFNDLLFUNC_ReceiveTraceFromClient ReceiveTraceFromClient_tx;
LPFNDLLFUNC_ReceiveLogFromClient ReceiveLogFromClient_tx;



/* End BUSMASTER global variables */

/*END Server Node Defenitions*/


