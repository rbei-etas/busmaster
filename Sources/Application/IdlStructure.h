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


typedef enum { None, Start, Stop, Both, Stopped} eLoggingTriggerState;
typedef enum { System, Absolute, Relative } eTimeMode;
typedef enum { Hex, Deci } eNumericMode;
typedef enum { Append, OverWrite } eFileMode;
typedef enum { Signal_watch, network_statistics, trace, graph} eWindow;
typedef enum { Active, Passive, Simulation } eControllerMode;
typedef enum { all, timers, key, message, error} eHandlerType;
typedef enum { Log, Display} EFILTERMODULE;

typedef struct
{
    unsigned int m_unMsgID;    // 11/29 Bit-
    unsigned char m_bEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_bRTR;      // true, for remote request
    unsigned char m_ucDataLen; // Data len (0..8)
    unsigned char m_ucData[8]; // Databytes 0..7
    unsigned char m_ucChannel;
} CAN_MSGS;

typedef struct
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

typedef struct
{
    unsigned long m_dMessageCode;       // Message ID
    unsigned int m_dNumberOfSignals;   // Number Of Signals a Message has
    unsigned int m_dMessageLength;     // Message Length
    bool m_bMessageFrameFormat;  // Message Frame Format
    int m_nMsgDataFormat;        // 1-Intel, 0-Motorola
} sMESSAGESTRUCT ;

// Transmission block related //
typedef struct tagTxBlock_Usr
{
    BSTR    m_strBlockName; // Tx block name
    bool    m_bPeriodical;// TRUE for periodic transmission; FALSE for monoshot
    USHORT  m_ushInterval;// If not monoshot, then the transmission period.
    // Must be ignored if m_bPeriodical is FALSE.
    unsigned char m_ucTrigger; //For time triggered - 1, Key triggered - 2, BOTH - 3
    BOOL    m_bTxAllFrame;   // TRUE - all messages at a time, FALSE- one by one
    UCHAR   m_ucKeyValue;   // Key value for key trigger
    UINT    m_unTimeInterval; // Time interval for time triggered
} STXBLOCK_USR, *PSTXBLOCK_USR;

// Filter Data structure
// Individual Filter
typedef struct
{
    UCHAR m_ucFilterType;   // 0 - Message ID and 1 - ID Range
    UINT  m_unMsgIDFrom;    // From Message ID incase of range.
    // Msg ID in case of Single ID
    UINT  m_unMsgIDTo  ;    // To Message ID incase of range.
    UCHAR m_ucDirection;    // 2 - Rx Msg, 1 - Tx Msg, 0 - For All
    UCHAR m_ucEXTENDED;     // 0 - Standard, 1 - Extended, 2 - For All
    UCHAR m_ucRTR;          // 0 - Non RTR, 1 - RTR Message, 2 - For All
    UCHAR m_ucChannel;      // Channel Number, 0 - for all
} SFILTER_USR, *PSFILTER_USR;


// Logging related //
typedef struct
{
    eLoggingTriggerState m_unTriggerType; // 0 - None, 1 - Start, 2 - Stop, 3 - Both
    UINT             m_unStartID;     // Start Trigger Id
    UINT             m_unStopID;      // Stop Trigger Id
} SLOGTRIGGER_USR,*PSLOGTRIGGER_USR;
// A logging block is perceived by the user based on the following parameters:
// 1. Format. This is defined by ~
//    1.1 The timer mode (absolute / relative)
//    1.2 Number format (decimal / hexadecimal)
//    1.3 Source channel (A / B / AB)
// 2. Log file name.
// 3. Operating mode (overwrite/append current log file during the next session)
// 4. Trigger condition of logging.
typedef struct tagLoggingBlockUsr
{
    eTimeMode       m_eLogTimerMode;
    eNumericMode    m_eNumFormat;
    eFileMode       m_eFileMode;
    SLOGTRIGGER_USR m_sLogTrigger;
    char            m_sLogFileName[MAX_FILE_PATH]; // MAX_PATH defined in windef.h

    //tagLoggingBlockUsr();

} SLOGGINGBLOCK_USR,*PSLOGGINGBLOCK_USR;