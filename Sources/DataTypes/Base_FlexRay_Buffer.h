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
 * \file      Base_FLEXRAY_Buffer.h
 * \brief     Common includes for FlexRay buffer
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Common includes for FlexRay buffer
 */

#pragma once

#include "..\Include\BaseDefs.h"

#define RBIN_FLXHDRINFO_SELFRECEPTION                  0x00000800
#define RBIN_FLXHDRINFO_CHANNELID                      0x00000001
#define RBIN_FLXHDRINFO_TXCONFLICT                     0x00000200
#define RBIN_FLXHDRINFO_FRAMETYPE                      0x00000400
#define RBIN_FLXHDRINFO_BOTHCHANNELS                   0x00001000
#define RBIN_FLXHDRINFO_CHANNELID                      0x00000001
#define RBIN_FLXHDRINFO_FRAMETYPE_STATIC               0x00000000
#define RBIN_FLXHDRINFO_FRAMETYPE_DYNAMIC              0x00000400
#define RBIN_FLXHDRINFO_CHANNELID_A                    0x00000000
#define RBIN_FLXHDRINFO_CHANNELID_B                    0x00000001




#define RBIN_FLXHDRINFO_CHANNELID                      0x00000001
#define RBIN_FLXHDRINFO_PAYLOADPREAMBLE                0x00000002
#define RBIN_FLXHDRINFO_NULLFRAME                      0x00000004
#define RBIN_FLXHDRINFO_SYNCFRAME                      0x00000008
#define RBIN_FLXHDRINFO_STARTUPFRAME                   0x00000010
#define RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT        0x00000020
#define RBIN_FLXHDRINFO_SYNTAXERROR                    0x00000040
#define RBIN_FLXHDRINFO_CONTENTERROR                   0x00000080
#define RBIN_FLXHDRINFO_BOUNDARYVIOLATION              0x00000100
#define RBIN_FLXHDRINFO_NONDBMSG                       0x00001000

#define RBIN_FLXHDRINFO_TXSTATICMSG                    0x00000001
#define RBIN_FLXHDRINFO_TXDYNAMICMSG                   0x00000002
#define RBIN_FLXHDRINFO_CHANNELAENABLE                 0x00000004
#define RBIN_FLXHDRINFO_CHANNELBENABLE                 0x00000008


#define RBIN_FLXTXMSGINFO_CHANGEABLE_IDENTIFIER        0x0001
#define RBIN_FLXTXMSGINFO_CHANGEABLE_CYCLENUMBER       0x0002
#define RBIN_FLXTXMSGINFO_CHANGEABLE_DATALENGTH        0x0004
#define RBIN_FLXTXMSGINFO_CHANGEABLE_CHANNELID         0x0008
#define RBIN_FLXTXMSGINFO_CHANGEABLE_PAYLOADPREAMBLE   0x0010
#define RBIN_FLXTXMSGINFO_CHANGEABLE_FRAMETYPE         0x0020
#define RBIN_FLXTXMSGINFO_CHANGEABLE_DATA              0x0040


//// Different states of the controller
//enum
//{
//    RESET = 0, INITIALISED, WAITING, NORMAL_ACTIVE, NORMAL_PASSIVE, UNDEFINED
//};

#define LEN_10          10
#define LEN_256         256
#define LEN_1024        1024
enum eTxColumnList
{
    TX_COLUMN_TYPE,
    TX_COLUMN_MESSAGE_ID,
    TX_COLUMN_DESCRIPTION,
    TX_COLUMN_CYCLE_NO,
    TX_COLUMN_CHANNEL,
    TX_COLUMN_DATA_LENGTH,
    TX_COLUMN_DATA_BYTES
};
enum
{
    UNKNOWN = 0, MESSAGE_STATUS, MESSAGE_DATA
};

enum eFlexRayPOCStatus
{
    FLEXRAY_POCS_DEFAULT_CONFIG,
    FLEXRAY_POCS_READY,
    FLEXRAY_POCS_NORMAL_ACTIVE,
    FLEXRAY_POCS_NORMAL_PASSIVE,
    FLEXRAY_POCS_HALT,
    FLEXRAY_POCS_MONITOR_MODE,
    FLEXRAY_POCS_CONFIG,
    FLEXRAY_POCS_WAKEUP_STANDBY,
    FLEXRAY_POCS_WAKEUP_LISTEN,
    FLEXRAY_POCS_WAKEUP_SEND,
    FLEXRAY_POCS_WAKEUP_DETECT,
    FLEXRAY_POCS_STARTUP_PREPARE,
    FLEXRAY_POCS_COLDSTART_LISTEN,
    FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION,
    FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK,
    FLEXRAY_POCS_COLDSTART_GAP,
    FLEXRAY_POCS_COLDSTART_JOIN,
    FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK,
    FLEXRAY_POCS_INTEGRATION_LISTEN,
    FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK,
    FLEXRAY_POCS_INITIALIZE_SCHEDULE,
    FLEXRAY_POCS_ABORT_STARTUP ,
    FLEXRAY_POCS_END
};

#define DB_MSG_NAME_SIZE         64

//*** Maximum amount of FlexRay Data [in WORDS]
//#define FLX_MAX_MSG_DATA_SIZE     127
#define FLX_MAX_MSG_DATA_SIZE     254

//*** FlexRay Controller states
#define FLX_CNTRL_STATES     5
enum eFLXCTRLSTATEList
{
    FLXCTRLSTATE_RESET,
    FLXCTRLSTATE_STOPPED_INITIALIZED,
    FLXCTRLSTATE_STARTED_STARTUP,
    FLXCTRLSTATE_STARTED_NORMALACTIVE,
    FLXCTRLSTATE_STARTED_NORMALPASSIVE
};

//----------------------------------------------------------------------------
// declaration of FlexRay status message
//----------------------------------------------------------------------------
typedef struct struct_FLXSTATUSMSG
{
    //BYTE  bCycleNumber;                       // Cycle Number (0-63 {6 bit})
    //DWORD dwApplicationCycleNumber;           // Application Cycle Number.
    // One FlexRay application cycle corresponds
    // to 64 FlexRay cycles
    WORD  wControllerStatus;                  // Current FlexRay controller state
    // (UCI_FLXCTRLSTATE_???)
    // 0 := reset
    // 1 := stopped / initialized
    // 2 := started / waiting for startup completion
    // 3 := started / normal active (running)
    // 4 := started / normal passiv
    // 5 := started / halt mode
    DWORD dwStatusInfoFlags;                  // Flagfield of status information (UCI_FLXSTSINFO_???)
    //WORD  wChannelAStatusError;               // Counts errors on channel A since start
    //WORD  wChannelBStatusError;               // Counts errors on channel B since start
    eFlexRayPOCStatus m_oeFlexRayStatus;
} s_FLXSTATUSMSG, *ps_FLXSTATUSMSG;

//----------------------------------------------------------------------------
// declaration of FlexRay data message
//----------------------------------------------------------------------------
typedef struct struct_FLXDATAMSG
{
    short int       m_nSlotID;                        // FlexRay Identifier (0x001-0x7FF {11 bit})
    unsigned char   m_ucCycleNumber;                       // Cycle Number (0-63 {6 bit})
    unsigned char   m_ucRepetition;
    int             m_nBaseCycle;
    short int       m_nCluster;                         //Cluster
    ECHANNEL        m_eChannel;                     //A or B or AB
    short int       m_nDLC;                        // Data length (number of data words) (0-127 {7 bit})
    unsigned long   m_lHeaderInfoFlags;                  // Flagfield of Header information (RBIN_FLXHDRINFO_???)
    bool m_bIsRxMsg;
    unsigned char   m_ucData[FLX_MAX_MSG_DATA_SIZE];      // FlexRay Data
} s_FLXDATAMSG, *ps_FLXDATAMSG;

enum eFLXMESSAGETYPE
{
    FLXMSGTYPE_UNKNOWN,
    FLXMSGTYPE_STATUS,
    FLXMSGTYPE_DATA
};
//----------------------------------------------------------------------------
// union for FlexRay data and status message types
//----------------------------------------------------------------------------
typedef struct struct_UCI_FLXMSG
{
    eFLXMESSAGETYPE m_eMessageType;            // Identifies the message type
    // One of the following message types:
    // FLXMSGTYPE_UNKNOWN := 0 -> Message invalid
    // FLXMSGTYPE_STATUS  := 1 -> stcStatusMsg is valid
    // FLXMSGTYPE_DATA    := 2 -> stcDataMsg is valid
    UINT64          m_unTimestamp;             // 64bit Timestamp with 100ns resolution
    union
    {
        s_FLXSTATUSMSG stcStatusMsg;          // Status message structure
        s_FLXDATAMSG   stcDataMsg;            // Data message structure
    };

    static void vSetSortField(int nField);
    static void vSetSortAscending(bool bAscending);
    static int DoCompareIndiv(const void* pEntry1, const void* pEntry2);
    static __int64 GetSlotID(struct_UCI_FLXMSG& pDatFLEX);
} s_FLXMSG, *ps_FLXMSG;

typedef struct tagRxMSG
{
    char cMsgType[4];
    char cChannel[4];
    char cCycleCnt[4];
    char cDataLen[4];
    char cMsgDesc[LEN_256];
    char cMsgID[8];
    char cDataBytes[LEN_1024];
    char cTime[32];
    char cMsgName[DB_MSG_NAME_SIZE];
    WORD m_wMsgID;
    __int64 m_n64MapId;
    COLORREF m_ColourCode;
    UINT64 m_u64TimeStamp;
    BYTE  m_byDataLength;
    WORD m_awData[FLX_MAX_MSG_DATA_SIZE];
    EINTERPRET_MODE m_eInterpretMode;
    DWORD dwChangleable;
    tagRxMSG(void)
    {
        m_ColourCode = RGB(0, 0, 0);
        memset (m_awData, 0, sizeof (m_awData));
        m_eInterpretMode = NON_INTERPRETABLE ;
    };
} RxMSG;

typedef struct tagBOARDINFO
{
    WORD      m_wHWVersion;         // Hardware version e.g.: 01.00 as 0x100
    WORD      m_wFWVersion;         // Firmware version (0xFFFF if not available)
    WORD      m_wDDVersion;         // device driver version
    // (0xFFFF if not available)
    WORD      m_wUCIVersion;        // UCI software version
    BYTE      m_bCanNum;            // Number of supported CAN controllers
    BYTE      m_bBftNum;            // Number of supported byteflight controllers
    BYTE      m_bFlxNum;            // Number of supported FlexRay controllers
    char      m_acSerialNum[16];    // hw serial number as string, e.g "12345678"
    char      m_acHardwareType[40]; // hw type as string e.g: "PCI04-ISA"
} s_BOARDINFO;

typedef struct tagFLXINFO
{
    BYTE  m_abHwVersion[4];            // Hardware Version
    BYTE  m_abFwVersion[4];            // Firmware Version
    BYTE  m_abFlexRayVersion[4];       // Version of FlexRay Controller / Protocol
    DWORD m_dwSupportedFeatures;       // Flagfield of Supported Features according to
    // UCI_FLXDATAMSGs dwHeaderInfoFlags
    // (RBIN_FLXHDRINFO_???)
    DWORD m_dwInitInfoFlags;           // FlagField of Information after Init was called
    WORD  m_wLastStaticSlotId;         // ID of the last static slot
    WORD  m_wLastDynamicSlotId;        // ID of the last dynamic slot Note: This ID is only
    // valid if no other dynamic slots are on the bus
    WORD  m_wStaticDataLength;         // Data length in [WORDS] for all static messages
    WORD  m_wMaxDynamicDataLength;     // Maximum Data length in [WORDS] of a dynamic message
    DWORD m_dwBaudrate;                // Baudrate in [BAUD]
    DWORD m_dwCycleLength;             // Length of one cycle in [ns]
    DWORD m_dwStaticSlotLength;        // Length of one static slot in [ns]
    WORD  m_wMacrotickLength;          // Length of one Macrotick in [ns]
    WORD  m_wMicrotickLength;          // Length of one Microtick in [ns]
    char  m_szBoardManufacturer[24];   // Zero terminated string of board manufacturer
    char  m_szChipManufacturer[24];    // Zero terminated string of chip manufacturer
    char  m_szBoardName[24];           // Zero terminated string of board name

} s_FLXINFO;

struct s_FLXControllerCapabilities
{
    int m_nMaximumTxBuffers;
    bool m_bSRSupport;          //Self Receiption
};

typedef struct struct_FLXTXMSG
{
    DWORD        m_dwChangeable;
    WORD         m_wFirstDynSlotID;
    WORD         m_wLastDynSlotID;
    WORD         m_wDynDataLenMax;
    DWORD        m_dwMsgIndex;
    BOOL         m_bToTransmit;
    s_FLXDATAMSG m_sFlxMsg;

    //Below functions are included for generalising the data retrieving/saving procedure
    UINT unCalculateSize()
    {
        return sizeof(struct_FLXTXMSG);
    };

    // Retrieve data from the location(DestAddress)
    //and return the location for reading next data
    BYTE* RetrieveStructdata(BYTE* DestAddress)
    {
        //The start address of the structure will be same as
        //the address of its first element
        memcpy(&m_dwChangeable, DestAddress, sizeof(struct_FLXTXMSG));
        DestAddress += sizeof(struct_FLXTXMSG);

        return DestAddress;
    };

    // Save data at the location(SrcAddress)
    //and return the location for saving next data
    BYTE* SaveStructData(BYTE* SrcAddress)
    {
        memcpy(SrcAddress,&m_dwChangeable, sizeof(struct_FLXTXMSG));
        SrcAddress += sizeof(struct_FLXTXMSG);

        return SrcAddress;
    };
} s_FLXTXMSG, *ps_FLXTXMSG;


const int SIZE_FLX_APP_BUFFER       = 5000;
const int FLXMSG_STRUCT_SIZE        = sizeof(s_FLXMSG);

//const int CALL_SUCCESS          = 0;
//const int EMPTY_APP_BUFFER      = 1;
const int FULL_APP_BUFFER       = 2;

#define IS_FLEX_TX_MESSAGE(a)       (a & RBIN_FLXHDRINFO_SELFRECEPTION)
#define IS_FLEX_STATUS_MESSAGE(a)   ((a) & MESSAGE_STATUS)
#define IS_FLEX_DATA_MESSAGE(a)     ((a) & MESSAGE_DATA)

class CBase_FlexRayBuffer
{
public:
    CBase_FlexRayBuffer();
    ~CBase_FlexRayBuffer();
    virtual int ReadFromBuffer(ps_FLXMSG psFlexMsg);
    virtual int WriteIntoBuffer(ps_FLXMSG psFlexMsg);
    virtual int GetBufferLength(void);
    virtual void vClearMessageBuffer(void);
    virtual HANDLE hGetNotifyingEvent(void);
};
