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
 * \file      Struct_LIN.h
 * \brief     This contains definitions of various LIN related structures.
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains definitions of various LIN related structures.
 */

#pragma once
//MVN
//libxml file includes
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
//~MVN
/* C++ includes */
#include <string>
#include <sstream>

using namespace std;
/**
 * This structure is used for sending/reciving messages to/from the LIN network
 */

typedef struct sTLIN_FRAME
{
    unsigned char m_ucMsgID;        // Protected Identifier
    unsigned char m_ucData[8];      // Databytes 0..8
    unsigned char m_ucChksum;       // Checksum
} STLIN_FRAME, *PSTLIN_FRAME;

typedef struct sTLIN_MSG
{
    unsigned char m_ucMsgTyp;       // Message Type (0 - Header / 1 - Response)
    unsigned char m_ucChksumTyp;    // Checksum Type (0 - Classical / 1 - Enhanced)
    unsigned char m_ucChannel;      // Channel Number
    unsigned char m_ucDataLen;      // Data len (0..8)
    unsigned char m_ucMsgID;        // Protected Identifier
    unsigned char m_ucData[8];      // Databytes 0..8
    unsigned char m_ucChksum;       // Checksum
} STLIN_MSG, *PSTLIN_MSG;
//typedef sTLIN_MSG* PSTLIN_MSG;

// This structure holds Error info
struct sERROR_INFO_LIN
{
    unsigned char m_ucErrType;    // ERROR_BUS, ERROR_DEVICE_BUFF_OVERFLOW
    // ERROR_DRIVER_BUFF_OVERFLOW, ERROR_UNKNOWN
    unsigned char m_ucReg_ErrCap; // Stores the value of err capture register in
    // case of bus error
    unsigned char m_ucTxErrCount;
    unsigned char m_ucRxErrCount;
    unsigned char m_ucChannel;
    int           m_nSubError;   //added for providing Error bit details
};
typedef sERROR_INFO_LIN SERROR_INFO_LIN;
typedef sERROR_INFO_LIN* PSERROR_INFO_LIN;


// This structure holds the error and the channel number
typedef struct sLIN_ERR
{
    unsigned char m_ucTxError;
    unsigned char m_ucRxError;
    unsigned char m_ucChannel;

} SLIN_ERR, *SPLIN_ERR;
// To copy the data and advance the pointer of the target data stream
#define COPY_DATA(TgtStream, SrcStream, TotBytes) {memcpy(TgtStream, SrcStream, TotBytes); TgtStream += TotBytes;}

// To copy the data and advance the pointer of the source data stream
#define COPY_DATA_2(TgtStream, SrcStream, TotBytes) {memcpy(TgtStream, SrcStream, TotBytes); SrcStream += TotBytes;}

#define defMODE_ACTIVE                         1
#define defMODE_PASSIVE                        2
#define defMODE_SIMULATE                       3

#define defCONTROLLER_ACTIVE                   1
#define defCONTROLLER_PASSIVE                  2
#define defCONTROLLER_BUSOFF                   3

#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3

#define defWARNING_LIMIT_INT    96

#define defBAUD_RATE_VAL                    500
#define defBAUD_RATE                        0xC03A // 500 KBPS
#define defMAX_TIME_DELAY                   60000

#define LIN_PID_LENGTH                          1
#define LIN_CHECKSUM_LENGTH                     1
#define LIN_CHANNEL_1                           1

const short LIN_MSG_IDS = 2;

//This enum defines different filter types
//enum eHW_FILTER_TYPES_LIN
//{
//    HW_FILTER_ACCEPT_ALL = 0,
//    HW_FILTER_REJECT_ALL,
//    HW_FILTER_MANUAL_SET
//};

// Controller details
// information on the baud rate
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

            /* m_enmHWFilterType[i] = objRef.m_enmHWFilterType[i];*/
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
    int     m_nItemUnderFocus;                   // item number under focus
    int     m_nBTR0BTR1;                         // packed value of bit timing register 0
    // and bit timing register 1
    string  m_omStrCNF1;                         // bit timing register 1 information
    string  m_omStrCNF2;                         // bit timing register 2 information
    string  m_omStrCNF3;                         // bit timing register 3 information
    string  m_omStrBTR0;                         // bit timing register 0 information
    string  m_omStrBTR1;                         // bit timing register 1 information
    string  m_omStrBaudrate;                     // baudrate information
    string  m_omStrClock;                        // clock information
    string  m_omStrSamplePercentage;             // sampling information
    string  m_omStrSampling;                     // sampling information
    string  m_omStrWarningLimit;                 // Warning limit of CAN Controller
    string  m_omStrPropagationDelay;             // Propagation Delay
    string  m_omStrSjw;
    string  m_omStrAccCodeByte1[LIN_MSG_IDS];    // acceptance code byte1 information
    string  m_omStrAccCodeByte2[LIN_MSG_IDS];    // acceptance code byte2 information
    string  m_omStrAccCodeByte3[LIN_MSG_IDS];    // acceptance code byte3 information
    string  m_omStrAccCodeByte4[LIN_MSG_IDS];    // acceptance code byte4 information
    string  m_omStrAccMaskByte1[LIN_MSG_IDS];    // acceptance mask byte1 information
    string  m_omStrAccMaskByte2[LIN_MSG_IDS];    // acceptance mask byte2 information
    string  m_omStrAccMaskByte3[LIN_MSG_IDS];    // acceptance mask byte3 information
    string  m_omStrAccMaskByte4[LIN_MSG_IDS];    // acceptance mask byte4 information
    string  m_omHardwareDesc;                    // Hw description which user can
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
    string  m_omStrLocation;                     // location (serial port, ip-address, ...)
    int     m_bHWTimestamps;                     // timestamps from the controllers hardware

    //Filter type: 1. Accept All 2. Reject All 3. Manual setting
    //eHW_FILTER_TYPES_LIN m_enmHWFilterType[LIN_MSG_IDS];


};
typedef sCONTROLLERDETAILSLIN   SCONTROLLER_DETAILS_LIN;
typedef sCONTROLLERDETAILSLIN*  PSCONTROLLER_DETAILS_LIN;

/*****************************************************************************/
/*This structure is used for communicating between Driver & LIN Application*/
union sTLINDATAINFO
{
    STLIN_MSG         m_sLINMsg;      //The received / transmitted message
    SERROR_INFO_LIN   m_sErrInfo;
};
typedef sTLINDATAINFO STLINDATAINFO;
typedef sTLINDATAINFO* PSTLINDATAINFO;


/*****************************************************************************/

/*****************************************************************************/
/*This structure is used for communicating between Driver & LIN Application*/

typedef struct sTLINDATA
{
private:
    static int  m_nSortField;
    static int  m_nMFactor;     // Multiplication factor

public:
    unsigned char    m_ucDataType;  //Type of the message
    LARGE_INTEGER    m_lTickCount;  //Time stamp, Contains the val returned from
    //QueryPerf..Counter()
    STLINDATAINFO       m_uDataInfo;

    static void vSetSortField(int nField);
    static void vSetSortAscending(bool bAscending);
    static int DoCompareIndiv(const void* pEntry1, const void* pEntry2);
    static __int64 GetSlotID(sTLINDATA& pDatLIN);

} STLINDATA, *PSTLINDATA;

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
    LIN_MSG_TYPE_MAX,           /* Do not add msg type after LIN_MSG_TYPE_MAX */
};

enum eLIN_CHECKSUM_TYPE
{
    CLASSIC_CHECKSUM = 0,
    ENHANCED_CHECKSUM,
};

const string sg_ListDIL_MSG_TYPE[LIN_MSG_TYPE_MAX] =
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
