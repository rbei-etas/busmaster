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
 * \file      LIN_ISOLAR_EVE_VLIN_Defines.h
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
#define LIN_MAX_ERRSTR 256
/**
 * Client and Client Buffer map
 */
typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseLINBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
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

//typedef struct sTLIN_FRAME
//{
//  unsigned char m_ucMsgID;        // Protected Identifier
//    unsigned char m_ucData[8];        // Databytes 0..8
//  unsigned char m_ucChksum;       // Checksum
//} STLIN_FRAME, *PSTLIN_FRAME;

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
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseLINBufFSE* pBuf);
static BOOL bRemoveClientBuffer(CBaseLINBufFSE* RootBufferArray[MAX_BUFF_ALLOWED],
                                UINT& unCount, CBaseLINBufFSE* BufferToRemove);


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

#define  LIN_PID_MAX      63
#define  DISCONNECT       2
#define  RESET_CONTROLLER 3



// Numerical values of commands to Client
#define RECEIVE_MESSAGE_ASYNC 1
#define BUSOFF 2
#define WAKEUP 3

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

//
//enum
//{
//    LIN_INVALID_RESPONSE = 0,
//    LIN_MASTER_RESPONSE,
//    LIN_SLAVE_RESPONSE,
//    LIN_SLAVE_SLAVE,
//  LIN_WAKEUP_REQUEST,
//  LIN_SLEEP_FRAME,
//  LIN_NO_ANSWER,
//  LIN_CHECKSUM_ERROR,
//  LIN_MSG_TYPE_MAX,
//}ENUM_LIN_MSG_TYPE;

//class CLIN_MSG_TYPE
//{
//public:
//    ENUM_LIN_MSG_TYPE m_enMsgType;
//    string                m_strMsgType;
//
//};
//
//static CLIN_MSG_TYPE sg_ListDIL_MSG_TYPE[] =
//{
//    {LIN_INVALID_RESPONSE,       "LIN INVALID RESPONSE"       },
//    {LIN_MASTER_RESPONSE,   "LIN MASTER RESPONSE"         },
//    {LIN_SLAVE_RESPONSE,       "LIN SLAVE RESPONSE"       },
//    {LIN_SLAVE_SLAVE,   "LIN SLAVE2SLAVE"         },
//    {LIN_WAKEUP_REQUEST,       "LIN WAKEUP REQUEST"       },
//    {LIN_SLEEP_FRAME,   "LIN SLEEP FRAME"         },
//  {LIN_NO_ANSWER,       "LIN NO ANSWER"       },
//    {LIN_CHECKSUM_ERROR,   "LIN CHECKSUM ERROR"         },
//};

//static string sg_ListDIL_MSG_TYPE[LIN_MSG_TYPE_MAX] =
//{
//    "LIN INVALID RESPONSE",
//    "LIN MASTER RESPONSE",
//    "LIN SLAVE RESPONSE",
//    "LIN SLAVE2SLAVE",
//    "LIN WAKEUP REQUEST",
//    "LIN SLEEP FRAME",
//  "LIN NO ANSWER",
//    "LIN CHECKSUM ERROR",
//};

/* End BUSMASTER global variables */

/*END Server Node Defenitions*/


