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
 * \file      IdlStructure.h
 * \brief     CCANMonitorApp class implementation file
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CCANMonitorApp class implementation file
 */

const USHORT MAX_BLOCK_NAME = 32;
const USHORT MAX_FILE_PATH  = 256;

typedef  [uuid(B78C73E2-4F4D-42C1-AF94-04D1AA8C6410), version(1.0)] enum { None, Start, Stop, Both, Stopped} eLoggingTriggerState;
typedef [uuid(194345BB-4E5D-4A96-8A35-EBF3FCA06E8E), version(1.0)] enum { System, Absolute, Relative } eTimeMode;
typedef [uuid(87458EB5-7047-4C71-A569-606110E60230), version(1.0)] enum { Hex, Deci } eNumericMode;
typedef [uuid(11FA404A-0517-4B9E-9B86-BEC3AF44E2F1), version(1.0)] enum { Append, OverWrite } eFileMode;
typedef [uuid(B687F83A-3BFC-46DF-ACBC-22789F22D34A), version(1.0)] enum { Signal_watch, network_statistics, trace, graph} eWindow;
typedef [uuid(C73746FF-B6F1-49A9-82AD-ACD29318D86E), version(1.0)] enum { Active, Passive, Simulation } eControllerMode;
typedef [uuid(CB996B66-3B04-49AF-BC24-5B0870CE4783), version(1.0)] enum { all, timers, key, message, error} eHandlerType;
typedef [uuid(966A2D62-9CC2-4E7A-BBA2-FF84426F30C2), version(1.0)] enum { Log, Display} EFILTERMODULE;

typedef  [uuid(4B92B469-C5A8-497C-B4ED-D533E4434292), version(1.0)] struct
{
    unsigned int m_unMsgID;    // 11/29 Bit-
    unsigned char m_bEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_bRTR;      // true, for remote request
    unsigned char m_ucDataLen; // Data len (0..8)
    unsigned char m_ucData[8]; // Databytes 0..7
    unsigned char m_ucChannel;
} CAN_MSGS;

typedef [uuid(7404B3EA-6EA9-4423-AAE4-5FCC80A10F38), version(1.0)] struct
{
    double          m_dBusLoad;
    double          m_dPeakBusLoad;
    unsigned int    m_unTotalMsgCount;
    unsigned int    m_unMsgPerSecond;

    unsigned int    m_unTotalTxMsgCount;
    double          m_dTotalTxMsgRate;

    unsigned int    m_unTxSTDMsgCount;
    double          m_dTxSTDMsgRate;
    unsigned int    m_unTxEXTDMsgCount;
    double          m_dTxEXTMsgRate;
    unsigned int    m_unTxSTD_RTRMsgCount;
    unsigned int    m_unTxEXTD_RTRMsgCount;

    unsigned int    m_unTotalRxMsgCount;
    double          m_dTotalRxMsgRate;

    unsigned int    m_unRxSTDMsgCount;
    double          m_dRxSTDMsgRate;
    unsigned int    m_unRxEXTDMsgCount;
    double          m_dRxEXTMsgRate;
    unsigned int    m_unRxSTD_RTRMsgCount;
    unsigned int    m_unRxEXTD_RTRMsgCount;

    unsigned int    m_unErrorTxCount;
    double          m_dErrorTxRate;
    unsigned int    m_unErrorRxCount;
    double          m_dErrorRxRate;
    unsigned int    m_unErrorTotalCount;
    double          m_dErrorRate;

    unsigned int    m_unDLCCount;

    double          m_dBaudRate;

    double          m_dTotalBusLoad;
    int             m_nSamples;
    double          m_dAvarageBusLoad;

    unsigned char   m_ucTxErrorCounter;
    unsigned char   m_ucRxErrorCounter;
    unsigned char   m_ucTxPeakErrorCount;
    unsigned char   m_ucRxPeakErrorCount;
    unsigned char   m_ucStatus;

} sBUSSTATISTICS_USR;

typedef [uuid(A739CC36-8E06-4AB2-81D7-E18DACB459C1), version(1.0)] struct
{
    unsigned long m_dMessageCode;       // Message ID
    unsigned int m_dNumberOfSignals;   // Number Of Signals a Message has
    unsigned int m_dMessageLength;     // Message Length
    bool m_bMessageFrameFormat;  // Message Frame Format
    int m_nMsgDataFormat;        // 1-Intel, 0-Motorola
} sMESSAGESTRUCT ;

// Transmission block related //
typedef [uuid(D178B15F-81F0-4BCA-8898-34077760ED90), version(1.0)]
struct tagTxBlock_Usr
{
    BSTR    m_strBlockName; // Tx block name
    bool    m_bPeriodical;// TRUE for periodic transmission; FALSE for monoshot
    USHORT  m_ushInterval;// If not monoshot, then the transmission period.
    // Must be ignored if m_bPeriodical is FALSE.
    unsigned char m_ucTrigger; //For time triggered - 1, Key triggered - 2, BOTH - 3
    BOOL    m_bTxAllFrame;   // TRUE - all messages at a time, FALSE- one by one
    UCHAR   m_ucKeyValue;   // Key value for key trigger
    UINT    m_unTimeInterval; // Time interval for time triggered
} STXBLOCK_USR/*, *PSTXBLOCK_USR*/;

typedef [uuid(C06EA17C-964D-4089-83CC-C129811E9435), version(1.0)] STXBLOCK_USR* PSTXBLOCK_USR;

// Filter Data structure
// Individual Filter
typedef [uuid(674F8D0C-A7DE-4672-8C5A-3FBF31EAF828), version(1.0)] struct
{
    UCHAR m_ucFilterType;   // 0 - Message ID and 1 - ID Range
    UINT  m_unMsgIDFrom;    // From Message ID incase of range.
    // Msg ID in case of Single ID
    UINT  m_unMsgIDTo  ;    // To Message ID incase of range.
    UCHAR m_ucDirection;    // 2 - Rx Msg, 1 - Tx Msg, 0 - For All
    UCHAR m_ucEXTENDED;     // 0 - Standard, 1 - Extended, 2 - For All
    UCHAR m_ucRTR;          // 0 - Non RTR, 1 - RTR Message, 2 - For All
    UCHAR m_ucChannel;      // Channel Number, 0 - for all
} SFILTER_USR/*, *PSFILTER_USR*/;

typedef [uuid(17A319D9-499B-4B2F-8701-B39EE14A44BE), version(1.0)] SFILTER_USR* PSFILTER_USR;

// Logging related //
typedef [uuid(84BBC375-4560-4793-8695-A3D53DE2F787), version(1.0)] struct
{
    eLoggingTriggerState m_unTriggerType; // 0 - None, 1 - Start, 2 - Stop, 3 - Both
    UINT             m_unStartID;     // Start Trigger Id
    UINT             m_unStopID;      // Stop Trigger Id
} SLOGTRIGGER_USR/*,*PSLOGTRIGGER_USR*/;

typedef [uuid(410C3330-0D5A-4AA6-98B9-5D3DFFC9B161), version(1.0)] SLOGTRIGGER_USR* PSLOGTRIGGER_USR;
// A logging block is perceived by the user based on the following parameters:
// 1. Format. This is defined by ~
//    1.1 The timer mode (absolute / relative)
//    1.2 Number format (decimal / hexadecimal)
//    1.3 Source channel (A / B / AB)
// 2. Log file name.
// 3. Operating mode (overwrite/append current log file during the next session)
// 4. Trigger condition of logging.
typedef [uuid(153552BE-D9D1-4281-BDB5-47E3698E09A5), version(1.0)] struct tagLoggingBlockUsr
{
    eTimeMode       m_eLogTimerMode;
    eNumericMode    m_eNumFormat;
    eFileMode       m_eFileMode;
    SLOGTRIGGER_USR m_sLogTrigger;
    char            m_sLogFileName[MAX_FILE_PATH]; // MAX_PATH defined in windef.h

    //tagLoggingBlockUsr();

} SLOGGINGBLOCK_USR/*,*PSLOGGINGBLOCK_USR*/;

typedef [uuid(76E5E4DE-8505-42A9-9BF9-7E147B5D4943), version(1.0)] SLOGGINGBLOCK_USR* PSLOGGINGBLOCK_USR;