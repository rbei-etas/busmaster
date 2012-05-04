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
 * \file      Struct_CAN.h
 * \brief     This contains definitions of various CAN related structures.
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains definitions of various CAN related structures.
 */

#pragma once

/* C++ includes */
#include <string>

using namespace std;

/**
 * This structure is used for sending/reciving messages to/from the CAN network
 */
struct sTCAN_MSG
{
    unsigned int  m_unMsgID;    // 11/29 Bit-
    unsigned char m_ucEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_ucRTR;      // true, for remote request
    unsigned char m_ucDataLen;  // Data len (0..8)
    unsigned char m_ucChannel;  // Message Length
    unsigned char m_ucData[8];  // Databytes 0..7
};
typedef sTCAN_MSG STCAN_MSG;
typedef sTCAN_MSG* PSTCAN_MSG;

/**
 * This structure holds the error and the channel number
 */
typedef struct sCAN_ERR
{
    unsigned char m_ucTxError;
    unsigned char m_ucRxError;
    unsigned char m_ucChannel;

} SCAN_ERR, *SPCAN_ERR;

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

/**
 * This enumeration defines current error state
 */
enum eERROR_STATE
{
    ERROR_ACTIVE  =   0,
    ERROR_WARNING_LIMIT,
    ERROR_PASSIVE,
    ERROR_BUS_OFF,
    ERROR_FRAME
};

const short CAN_MSG_IDS = 2;

/**
 * This enum defines different filter types
 */
enum eHW_FILTER_TYPES
{
    HW_FILTER_ACCEPT_ALL = 0,
    HW_FILTER_REJECT_ALL,
    HW_FILTER_MANUAL_SET
};


/**
 * \brief Controller details
 *
 * information on the baud rate
 */
class sCONTROLLERDETAILS
{
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
    string  m_omStrAccCodeByte1[CAN_MSG_IDS];    // acceptance code byte1 information
    string  m_omStrAccCodeByte2[CAN_MSG_IDS];    // acceptance code byte2 information
    string  m_omStrAccCodeByte3[CAN_MSG_IDS];    // acceptance code byte3 information
    string  m_omStrAccCodeByte4[CAN_MSG_IDS];    // acceptance code byte4 information
    string  m_omStrAccMaskByte1[CAN_MSG_IDS];    // acceptance mask byte1 information
    string  m_omStrAccMaskByte2[CAN_MSG_IDS];    // acceptance mask byte2 information
    string  m_omStrAccMaskByte3[CAN_MSG_IDS];    // acceptance mask byte3 information
    string  m_omStrAccMaskByte4[CAN_MSG_IDS];    // acceptance mask byte4 information
    string  m_omHardwareDesc;                    // Hw description which user can
    // differentiate between the channels
    int     m_bAccFilterMode;                    // acceptance filter mode(0: single, 1: Dual)
    int     m_ucControllerMode;                  // Controller mode (1: Active, 2: Passive)
    int     m_bSelfReception;

    //Filter type: 1. Accept All 2. Reject All 3. Manual setting
    eHW_FILTER_TYPES m_enmHWFilterType[CAN_MSG_IDS];
    sCONTROLLERDETAILS()
    {
        vInitialize();
    }
    void vInitialize()
    {
        // The default baudrate is taken as 500 kbps
        m_nItemUnderFocus = 64;
        m_nBTR0BTR1 = 49210;
        m_omStrCNF1 = "7";
        m_omStrCNF2 = "B8";
        m_omStrCNF3 = "5";
        m_omStrBTR0 = "C0";
        m_omStrBTR1 = "3A";
        m_omStrBaudrate = "500";
        m_omStrClock = "16";
        m_omStrSamplePercentage = "75";
        m_omStrSampling = "1";
        m_omStrWarningLimit = "96";
        m_omStrPropagationDelay = "ALL";
        m_omStrSjw = "4";
        m_omStrAccCodeByte1[0] = "0";
        m_omStrAccCodeByte2[0] = "0";
        m_omStrAccCodeByte3[0] = "0";
        m_omStrAccCodeByte4[0] = "0";
        m_omStrAccCodeByte1[1] = "0";
        m_omStrAccCodeByte2[1] = "0";
        m_omStrAccCodeByte3[1] = "0";
        m_omStrAccCodeByte4[1] = "0";
        m_omStrAccMaskByte1[0] = "FF";
        m_omStrAccMaskByte2[0] = "FF";
        m_omStrAccMaskByte3[0] = "FF";
        m_omStrAccMaskByte4[0] = "FF";
        m_omStrAccMaskByte1[1] = "FF";
        m_omStrAccMaskByte2[1] = "FF";
        m_omStrAccMaskByte3[1] = "FF";
        m_omStrAccMaskByte4[1] = "FF";
        m_omHardwareDesc = "";
        m_bAccFilterMode = FALSE;
        m_ucControllerMode = 0x1;
        m_enmHWFilterType[0] = HW_FILTER_ACCEPT_ALL;
        m_enmHWFilterType[1] = HW_FILTER_ACCEPT_ALL;
        m_bSelfReception = TRUE;
    };
};
typedef sCONTROLLERDETAILS   SCONTROLLER_DETAILS;
typedef sCONTROLLERDETAILS*  PSCONTROLLER_DETAILS;

struct sSUBBUSSTATISTICS
{
    unsigned int    m_unErrorTxCount;
    unsigned int    m_unTotalTxMsgCount;
    unsigned int    m_unTxSTDMsgCount;
    unsigned int    m_unTotalBitsperSec;
    unsigned int    m_unTxEXTDMsgCount;
    unsigned int    m_unTxSTD_RTRMsgCount;
    unsigned int    m_unTxEXTD_RTRMsgCount;
    unsigned int    m_unTotalRxMsgCount;
    unsigned int    m_unRxSTDMsgCount;
    unsigned int    m_unRxEXTDMsgCount;
    unsigned int    m_unRxSTD_RTRMsgCount;
    unsigned int    m_unRxEXTD_RTRMsgCount;
    unsigned int    m_unErrorTotalCount;
    unsigned int    m_unErrorRxCount;
    unsigned int    m_unDLCCount;
};
typedef sSUBBUSSTATISTICS SSUBBUSSTATISTICS;
typedef sSUBBUSSTATISTICS* PSSUBBUSSTATISTICS;

/**
 * Bus statistics structure
 */
struct sBUSSTATISTICS
{
    unsigned int    m_unTotalBitsperSec;
    //unsigned int    m_unRxSTDMsgBits;

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
    sBUSSTATISTICS& operator = (sSUBBUSSTATISTICS& objRef)
    {
        m_unErrorTxCount = objRef.m_unErrorTxCount;
        m_unTotalTxMsgCount = objRef.m_unTotalTxMsgCount;
        m_unTxSTDMsgCount = objRef.m_unTxSTDMsgCount;
        m_unTotalBitsperSec = objRef.m_unTotalBitsperSec;
        m_unTxEXTDMsgCount = objRef.m_unTxEXTDMsgCount;
        m_unTxSTD_RTRMsgCount = objRef.m_unTxSTD_RTRMsgCount;
        m_unTxEXTD_RTRMsgCount = objRef.m_unTxEXTD_RTRMsgCount;
        m_unTotalRxMsgCount = objRef.m_unTotalRxMsgCount;
        m_unRxSTDMsgCount = objRef.m_unRxSTDMsgCount;
        m_unRxEXTDMsgCount = objRef.m_unRxEXTDMsgCount;
        m_unRxSTD_RTRMsgCount = objRef.m_unRxSTD_RTRMsgCount;
        m_unRxEXTD_RTRMsgCount = objRef.m_unRxEXTD_RTRMsgCount;
        m_unErrorTotalCount = objRef.m_unErrorTotalCount;
        m_unErrorRxCount = objRef.m_unErrorRxCount;
        m_unDLCCount = objRef.m_unDLCCount;
        return *this;
    }
};
typedef sBUSSTATISTICS SBUSSTATISTICS;
typedef sBUSSTATISTICS* PBUSSTATISTICS;

/**
 * This structure holds Error info
 */
struct sERROR_INFO
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
typedef sERROR_INFO SERROR_INFO;
typedef sERROR_INFO* PSERROR_INFO;

/**
 * This structure is used for communicating between Driver & CAN Application
 */
union sTDATAINFO
{
    STCAN_MSG     m_sCANMsg;      //The received / transmitted message
    SERROR_INFO   m_sErrInfo;
};
typedef sTDATAINFO STDATAINFO;
typedef sTDATAINFO* PSTDATAINFO;

/**
 * This structure is used for communicating between Driver & CAN Application
 */
typedef struct sTCANDATA
{
private:
    static int  m_nSortField;
    static int  m_nMFactor;     // Multiplication factor

public:
    unsigned char    m_ucDataType;  //Type of the message
    long long int    m_lTickCount;  //Time stamp, Contains the val returned from
    //QueryPerf..Counter()
    STDATAINFO       m_uDataInfo;

    static void vSetSortField(int nField);
    static void vSetSortAscending(bool bAscending);
    static int DoCompareIndiv(const void* pEntry1, const void* pEntry2);
    static __int64 GetSlotID(sTCANDATA& pDatCAN);

} STCANDATA, *PSTCANDATA;

/**
 * This structure holds acceptance filter details
 */
struct sACC_FILTER_INFO
{
    unsigned char m_ucACC_Filter_Type;
    unsigned char m_ucACC_Code0;
    unsigned char m_ucACC_Code1;
    unsigned char m_ucACC_Code2;
    unsigned char m_ucACC_Code3;
    unsigned char m_ucACC_Mask0;
    unsigned char m_ucACC_Mask1;
    unsigned char m_ucACC_Mask2;
    unsigned char m_ucACC_Mask3;
};
typedef sACC_FILTER_INFO SACC_FILTER_INFO;
typedef sACC_FILTER_INFO* PSACC_FILTER_INFO;


/**
 * This structure holds Error count of Rx & Tx Process
 */
struct sERROR_CNT
{
    unsigned char m_ucTxErrCount;
    unsigned char m_ucRxErrCount;
};
typedef sERROR_CNT SERROR_CNT;
typedef sERROR_CNT* SPERROR_CNT;

#define TX_FLAG                 0x01
#define RX_FLAG                 0x02
#define ERR_FLAG                0x04
#define INTR_FLAG               0x08

#define IS_TX_MESSAGE(a)        (a & TX_FLAG)
#define IS_RX_MESSAGE(a)        (a & RX_FLAG)
#define IS_A_MESSAGE(a)         ((a) & 0x03)
#define IS_ERR_MESSAGE(a)       (a & ERR_FLAG)
#define IS_INTR_MESSAGE(a)      (((a) & INTR_FLAG))
