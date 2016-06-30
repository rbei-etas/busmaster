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
 * @file      Struct_LIN.h
 * @brief     This contains definitions of various LIN related structures.
 * @authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains definitions of various LIN related structures.
 */

#pragma once

/* C++ includes */
#include <string>
#include <sstream>

/* libxml includes */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

/**
 * This structure is used for sending/reciving messages to/from the LIN network
 */
struct sTLIN_FRAME
{
    unsigned char m_ucMsgID;        // Protected Identifier
    unsigned char m_ucData[8];      // Databytes 0..8
    unsigned char m_ucChksum;       // Checksum
};
typedef sTLIN_FRAME STLIN_FRAME;
typedef sTLIN_FRAME* PSTLIN_FRAME;

struct sTLIN_MSG
{
    unsigned char m_ucMsgTyp;       // Message Type (0 - Header / 1 - Response)
    unsigned char m_ucChksumTyp;    // Checksum Type (0 - Classical / 1 - Enhanced)
    unsigned char m_ucDataLen;      // Data len (0..8)
    unsigned char m_ucMsgID;        // Protected Identifier
    unsigned char m_ucData[8];      // Databytes 0..8
    unsigned long m_ulTimeStamp;
    unsigned char m_ucChannel;      // Channel Number
    unsigned char m_ucChksum;       // Checksum
};
typedef sTLIN_MSG STLIN_MSG;
typedef sTLIN_MSG* PSTLIN_MSG;

enum eLinBusEventType
{
    EVENT_LIN_ERRMSG,
    EVENT_LIN_ERRSYNC,
    EVENT_LIN_ERRNOANS,
    EVENT_LIN_ERRCRC,
    EVENT_LIN_WAKEUP,
    EVENT_LIN_SLEEP,
    EVENT_LIN_CRCINFO,
    EVENT_LIN_DLC,
    EVENT_LIN_ERRBIT,
    EVENT_LIN_ERRHEADER,
    EVENT_LIN_ERRTOUT, // For Timeout
    EVENT_LIN_COLLISION,
    EVENT_LIN_NO_MSTR_SCHED,
    EVENT_LIN_NO_TX_DATA,
    /*EVENT_LIN_TX_OK,*/
    EVENT_LIN_NONE
};

enum eLinSleepEvent
{
    LIN_SLEEPMODE_SET,
    LIN_SLEEPMODE_WAKEUP,
    LIN_SLEEPMODE_STAYALIVE,
};

enum eCheckSumType
{
    CHECKSUM_CLASSIC,
    CHECKSU_ENHANCED
};

enum eLinMsgType
{
    LIN_MSG,
    LIN_EVENT
};

struct SEVENT_CHECKSUM_LIN
{
    unsigned char m_ucId;
    unsigned char m_ucCrc;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
} ;

struct SEVENT_RECEIVE_LIN
{
    unsigned int  m_ulTime;
};

struct SEVENT_SLAVE_NORESP_LIN
{
    unsigned char m_ucId;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;

};

struct SEVENT_SYNC_LIN
{
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
};

/**
 * This structure holds Error info
 */
struct sERROR_INFO_LIN
{
    unsigned char m_ucId;       // Msg Id - Useful in case of NoAns
    unsigned char m_ucCrc;
    eLinBusEventType m_eEventType;
    //SEVENT_INFO_LIN  m_eEventInfo;
    eLinSleepEvent m_eSleepEvent;
    eCheckSumType m_eCheckSumType;
    //shashank
    unsigned char m_ucErrType;

    unsigned char m_ucReg_ErrCap; // Stores the value of err capture register in
    // case of bus error
    unsigned char m_ucTxErrCount;
    unsigned char m_ucRxErrCount;
    unsigned char m_ucChannel;
    unsigned char m_ucDlc;
    unsigned char m_ucDir;
    ULONGLONG m_ulTimeStamp;
    int           m_nSubError;   //added for providing Error bit details
};
typedef sERROR_INFO_LIN SERROR_INFO_LIN;
typedef sERROR_INFO_LIN* PSERROR_INFO_LIN;

/**
 * This structure holds the error and the channel number
 */
struct sLIN_ERR
{
    unsigned char m_ucTxError;
    unsigned char m_ucRxError;
    unsigned char m_ucChannel;

};
typedef sLIN_ERR SLIN_ERR;
typedef sLIN_ERR* SPLIN_ERR;

// To copy the data and advance the pointer of the target data stream
#ifndef COPY_DATA
#define COPY_DATA(TgtStream, SrcStream, TotBytes) { memcpy(TgtStream, SrcStream, TotBytes); TgtStream += TotBytes; }
#endif

// To copy the data and advance the pointer of the source data stream
#ifndef COPY_DATA_2
#define COPY_DATA_2(TgtStream, SrcStream, TotBytes) { memcpy(TgtStream, SrcStream, TotBytes); SrcStream += TotBytes; }
#endif

#define defWARNING_LIMIT_INT    96

#define defBAUD_RATE_VAL                    500
#define defBAUD_RATE                        0xC03A // 500 KBPS
#define defMAX_TIME_DELAY                   60000

#define LIN_PID_LENGTH                          1
#define LIN_CHECKSUM_LENGTH                     1
#define LIN_CHANNEL_1                           1

const short LIN_MSG_IDS = 2;

/**
 * Controller details, e.g. information on the baud rate
 */
class sCONTROLLERDETAILSLIN
{
private:
    void vCopyMembers(const sCONTROLLERDETAILSLIN& objRef)
    {
        m_nItemUnderFocus   = objRef.m_nItemUnderFocus;                   // item number under focus
        m_nBTR0BTR1         = objRef.m_nBTR0BTR1;

        m_omStrCNF1 = objRef.m_omStrCNF1;
        m_omStrCNF2 = objRef.m_omStrCNF2;
        m_omStrCNF3 = objRef.m_omStrCNF3;
        m_omStrBTR0 = objRef.m_omStrBTR0;
        m_omStrBTR1 = objRef.m_omStrBTR1;
        m_omStrBaudrate = objRef.m_omStrBaudrate;
        m_omStrClock = objRef.m_omStrClock;
        m_omStrSamplePercentage = objRef.m_omStrSamplePercentage;
        m_omStrSampling = objRef.m_omStrSampling;
        m_omStrWarningLimit = objRef.m_omStrWarningLimit;
        m_omStrPropagationDelay = objRef.m_omStrPropagationDelay;
        m_omStrSjw = objRef.m_omStrSjw;

        for(int i = 0; i < LIN_MSG_IDS; i++)
        {
            m_omStrAccCodeByte1[i] = objRef.m_omStrAccCodeByte1[i];
            m_omStrAccCodeByte2[i] = objRef.m_omStrAccCodeByte2[i];
            m_omStrAccCodeByte3[i] = objRef.m_omStrAccCodeByte3[i];
            m_omStrAccCodeByte4[i] = objRef.m_omStrAccCodeByte4[i];
            m_omStrAccMaskByte1[i] = objRef.m_omStrAccMaskByte1[i];
            m_omStrAccMaskByte2[i] = objRef.m_omStrAccMaskByte2[i];
            m_omStrAccMaskByte3[i] = objRef.m_omStrAccMaskByte3[i];
            m_omStrAccMaskByte4[i] = objRef.m_omStrAccMaskByte4[i];
        }
        m_omHardwareDesc    = objRef.m_omHardwareDesc;
        m_bAccFilterMode    = objRef.m_bAccFilterMode;
        m_ucControllerMode  = objRef.m_ucControllerMode;
        m_bSelfReception    = objRef.m_bSelfReception;

        /* CAN FD related parameters */
        m_unDataBitRate                 = objRef.m_unDataBitRate;
        m_unDataSamplePoint             = objRef.m_unDataSamplePoint;
        m_unDataBTL_Cycles              = objRef.m_unDataBTL_Cycles;
        m_unDataSJW                     = objRef.m_unDataSJW;
        m_bTxDelayCompensationON        = objRef.m_bTxDelayCompensationON;
        m_unTxDelayCompensationQuanta   = objRef.m_unTxDelayCompensationQuanta;
        m_bytRxCompatibility            = objRef.m_bytRxCompatibility;
        m_bytTxCompatibility            = objRef.m_bytTxCompatibility;

        m_bDebug            = objRef.m_bDebug;
        m_bPassiveMode      = objRef.m_bPassiveMode;
        m_omStrLocation     = objRef.m_omStrLocation;
        m_bHWTimestamps     = objRef.m_bHWTimestamps;
    }
public:
    sCONTROLLERDETAILSLIN()
    {
        m_strHwUri.reserve(100);
        m_strProtocolVersion.reserve(100);
    }

    std::string m_strHwUri;
    int m_nBaudRate;
    std::string  m_strProtocolVersion;



    int     m_nItemUnderFocus;                   // item number under focus
    int     m_nBTR0BTR1;                         // packed value of bit timing register 0
    // and bit timing register 1
    std::string  m_omStrCNF1;                         // bit timing register 1 information
    std::string  m_omStrCNF2;                         // bit timing register 2 information
    std::string  m_omStrCNF3;                         // bit timing register 3 information
    std::string  m_omStrBTR0;                         // bit timing register 0 information
    std::string  m_omStrBTR1;                         // bit timing register 1 information
    std::string  m_omStrBaudrate;                     // baudrate information
    std::string  m_omStrClock;                        // clock information
    std::string  m_omStrSamplePercentage;             // sampling information
    std::string  m_omStrSampling;                     // sampling information
    std::string  m_omStrWarningLimit;                 // Warning limit of CAN Controller
    std::string  m_omStrPropagationDelay;             // Propagation Delay
    std::string  m_omStrSjw;
    std::string  m_omStrAccCodeByte1[LIN_MSG_IDS];    // acceptance code byte1 information
    std::string  m_omStrAccCodeByte2[LIN_MSG_IDS];    // acceptance code byte2 information
    std::string  m_omStrAccCodeByte3[LIN_MSG_IDS];    // acceptance code byte3 information
    std::string  m_omStrAccCodeByte4[LIN_MSG_IDS];    // acceptance code byte4 information
    std::string  m_omStrAccMaskByte1[LIN_MSG_IDS];    // acceptance mask byte1 information
    std::string  m_omStrAccMaskByte2[LIN_MSG_IDS];    // acceptance mask byte2 information
    std::string  m_omStrAccMaskByte3[LIN_MSG_IDS];    // acceptance mask byte3 information
    std::string  m_omStrAccMaskByte4[LIN_MSG_IDS];    // acceptance mask byte4 information
    std::string  m_omHardwareDesc;                    // Hw description which user can
    // differentiate between the channels
    int     m_bAccFilterMode;                    // acceptance filter mode(0: single, 1: Dual)
    int     m_ucControllerMode;                  // Controller mode (1: Active, 2: Passive)
    int     m_bSelfReception;

    /* CAN FD related parameters */
    UINT32  m_unDataBitRate;
    UINT32  m_unDataSamplePoint;
    UINT32  m_unDataBTL_Cycles;
    UINT32  m_unDataSJW;
    bool    m_bTxDelayCompensationON;
    UINT32  m_unTxDelayCompensationQuanta;
    BYTE    m_bytRxCompatibility;
    BYTE    m_bytTxCompatibility;

    int     m_bDebug;                            // debug mode for channel driver
    int     m_bPassiveMode;                      // passive mode (no bus interaction, acknowledge, etc.)
    std::string  m_omStrLocation;                // location (serial port, ip-address, ...)
    int     m_bHWTimestamps;                     // timestamps from the controllers hardware
};
typedef sCONTROLLERDETAILSLIN SCONTROLLER_DETAILS_LIN;
typedef sCONTROLLERDETAILSLIN* PSCONTROLLER_DETAILS_LIN;

/**
 * This structure is used for communicating between Driver and LIN Application
 */
struct sTLINDATAINFO
{
    /* The received / transmitted message */
    STLIN_MSG         m_sLINMsg;

    SERROR_INFO_LIN   m_sErrInfo;
};
typedef sTLINDATAINFO STLINDATAINFO;
typedef sTLINDATAINFO* PSTLINDATAINFO;


/**
 * This structure is used for communicating between Driver and LIN Application
 */
struct sTLINDATA
{
private:
    static int  m_nSortField;

    /* Multiplication factor */
    static int  m_nMFactor;

public:
    eLinMsgType     m_eLinMsgType;

    /** Type of the message */
    unsigned char    m_ucDataType;

    /** Time stamp, Contains the val returned from */
    LARGE_INTEGER    m_lTickCount;
    // QueryPerf..Counter()
    STLINDATAINFO       m_uDataInfo;

    static void vSetSortField(int nField);
    static void vSetSortAscending(bool bAscending);
    static int DoCompareIndiv(const void* pEntry1, const void* pEntry2);
    static __int64 GetSlotID(sTLINDATA& pDatLIN);
};
typedef sTLINDATA STLINDATA;
typedef sTLINDATA* PSTLINDATA;

enum eLIN_MSG_TYPE
{
    LIN_INVALID_RESPONSE = 0,
    LIN_MASTER_RESPONSE,
    LIN_SLAVE_RESPONSE,
    LIN_SLAVE_SLAVE,
    LIN_WAKEUP_REQUEST,
    LIN_SLEEP_FRAME,
    LIN_NO_ANSWER,
    LIN_RESPONSE_NOT_CONFIGURED,
    LIN_CHECKSUM_ERROR,
    LIN_DLC_ERROR,
    LIN_MSG_TYPE_MAX, /* Do not add msg type after LIN_MSG_TYPE_MAX */
};

enum eLIN_CHECKSUM_TYPE
{
    LIN_CHECKSUM_CLASSIC = 0,
    LIN_CHECKSUM_ENHANCED,
};

const std::string sg_ListDIL_MSG_TYPE[LIN_MSG_TYPE_MAX] =
{
    "LIN INVALID RESPONSE",
    "LIN MASTER RESPONSE",
    "LIN SLAVE RESPONSE",
    "LIN SLAVE2SLAVE",
    "LIN WAKEUP REQUEST",
    "LIN SLEEP FRAME",
    "LIN NO ANSWER",
    "LIN_RESPONSE_NOT_CONFIGURED",
    "LIN CHECKSUM ERROR",
    "LIN DLC ERROR",
};



#define TX_FLAG                 0x01
#define RX_FLAG                 0x02
#define ERR_FLAG                0x04
#define INTR_FLAG               0x08

#define IS_TX_MESSAGE(a)        (a & TX_FLAG)
#define IS_RX_MESSAGE(a)        (a & RX_FLAG)
#define IS_A_MESSAGE(a)         ((a) & 0x03)
#define IS_ERR_MESSAGE(a)       (a & ERR_FLAG)
#define IS_INTR_MESSAGE(a)      (((a) & INTR_FLAG))
