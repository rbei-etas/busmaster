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
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains definitions of various CAN related structures.
 */

#pragma once

/* C++ includes */
#include <string>
#include <sstream>

/* libxml file includes */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <Windows.h>

/**
 * This structure is used for sending/reciving messages to/from the CAN network
 */
typedef struct sTCAN_MSG
{
    /** 11/29 Bit */
    unsigned int  m_unMsgID;

    /** true, for (29 Bit) Frame */
    unsigned char m_ucEXTENDED;

    /** true, for remote request */
    unsigned char m_ucRTR;

    /** Data Length (0..8, 12, 16, 20, 24, 32, 48, 64) */
    unsigned char m_ucDataLen;

    /** Message Length */
    unsigned char m_ucChannel;

    /** Databytes 0..63 */
    unsigned char m_ucData[64];

    /* CAN FD frame */
    bool m_bCANFD;
} STCAN_MSG, *PSTCAN_MSG;

/**
 * This structure holds the error and the channel number
 */
typedef struct sCAN_ERR
{
    /** transmit error */
    unsigned char m_ucTxError;

    /** receive error */
    unsigned char m_ucRxError;

    /** channel */
    unsigned char m_ucChannel;
} SCAN_ERR, *SPCAN_ERR;

/** To copy the data and advance the pointer of the target data stream */
#ifndef COPY_DATA
#define COPY_DATA(TgtStream, SrcStream, TotBytes) { memcpy(TgtStream, SrcStream, TotBytes); TgtStream += TotBytes; }
#endif

/** To copy the data and advance the pointer of the source data stream */
#ifndef COPY_DATA_2
#define COPY_DATA_2(TgtStream, SrcStream, TotBytes) { memcpy(TgtStream, SrcStream, TotBytes); SrcStream += TotBytes; }
#endif

enum defMODE
{
    defMODE_ACTIVE = 1,
    defMODE_PASSIVE = 2,
    defMODE_SIMULATE = 3
};

enum defCONTROLLER
{
    defCONTROLLER_ACTIVE = 1,
    defCONTROLLER_PASSIVE = 2,
    defCONTROLLER_BUSOFF = 3
};

enum defUSB_MODE
{
    defUSB_MODE_ACTIVE = 1,
    defUSB_MODE_PASSIVE = 2,
    defUSB_MODE_SIMULATE = 3
};

#define defWARNING_LIMIT_INT    96

#define defBAUD_RATE_VAL                    500
#define defBAUD_RATE                        0xC03A // 500 KBPS
#define defMAX_TIME_DELAY                   60000

/**
 * This enumeration defines current error state
 */
enum eERROR_STATE
{
    ERROR_ACTIVE = 0,
    ERROR_WARNING_LIMIT,
    ERROR_PASSIVE,
    ERROR_BUS_OFF,
    ERROR_FRAME,
    ERROR_INVALID
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
 * Controller details
 */
class sCONTROLLERDETAILS
{
private:
    void vCopyMembers(const sCONTROLLERDETAILS& objRef)
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

        for(int i = 0; i < CAN_MSG_IDS; i++)
        {
            m_omStrAccCodeByte1[i] = objRef.m_omStrAccCodeByte1[i];
            m_omStrAccCodeByte2[i] = objRef.m_omStrAccCodeByte2[i];
            m_omStrAccCodeByte3[i] = objRef.m_omStrAccCodeByte3[i];
            m_omStrAccCodeByte4[i] = objRef.m_omStrAccCodeByte4[i];
            m_omStrAccMaskByte1[i] = objRef.m_omStrAccMaskByte1[i];
            m_omStrAccMaskByte2[i] = objRef.m_omStrAccMaskByte2[i];
            m_omStrAccMaskByte3[i] = objRef.m_omStrAccMaskByte3[i];
            m_omStrAccMaskByte4[i] = objRef.m_omStrAccMaskByte4[i];

            m_enmHWFilterType[i] = objRef.m_enmHWFilterType[i];
        }
        m_omHardwareDesc    = objRef.m_omHardwareDesc;
        m_bAccFilterMode    = objRef.m_bAccFilterMode;
        m_ucControllerMode  = objRef.m_ucControllerMode;
        m_bSelfReception    = objRef.m_bSelfReception;
        m_bLowSpeed         = objRef.m_bLowSpeed;
        m_unBTL_Cycles      = objRef.m_unBTL_Cycles;

        /* CAN FD related parameters */
        m_unDataBitRate                 = objRef.m_unDataBitRate;
        m_unDataSamplePoint             = objRef.m_unDataSamplePoint;
        m_unDataBTL_Cycles              = objRef.m_unDataBTL_Cycles;
        m_unDataSJW                     = objRef.m_unDataSJW;
        m_bTxDelayCompensationControl        = objRef.m_bTxDelayCompensationControl;
        m_unTxSecondarySamplePointOffset   = objRef.m_unTxSecondarySamplePointOffset;
        m_bytCanRxMode            = objRef.m_bytCanRxMode;
        m_bytCanFdRxMode            = objRef.m_bytCanFdRxMode;
        m_bytCanFdTxConfig            = objRef.m_bytCanFdTxConfig;

        m_bDebug            = objRef.m_bDebug;
        m_bPassiveMode      = objRef.m_bPassiveMode;
        m_omStrLocation     = objRef.m_omStrLocation;
        m_bHWTimestamps     = objRef.m_bHWTimestamps;
    }
public:
    int     m_nItemUnderFocus;                    // item number under focus
    int     m_nBTR0BTR1;                          // packed value of bit timing register 0
    // and bit timing register 1
    std::string m_omStrCNF1;                      // bit timing register 1 information
    std::string m_omStrCNF2;                      // bit timing register 2 information
    std::string m_omStrCNF3;                      // bit timing register 3 information
    std::string m_omStrBTR0;                      // bit timing register 0 information
    std::string m_omStrBTR1;                      // bit timing register 1 information
    std::string m_omStrBaudrate;                  // baudrate information
    std::string m_omStrClock;                     // clock information
    std::string m_omStrSamplePercentage;          // sampling information
    std::string m_omStrSampling;                  // sampling information
    std::string m_omStrWarningLimit;              // Warning limit of CAN Controller
    std::string m_omStrPropagationDelay;          // Propagation Delay
    std::string m_omStrSjw;
    std::string m_omStrAccCodeByte1[CAN_MSG_IDS]; // acceptance code byte1 information
    std::string m_omStrAccCodeByte2[CAN_MSG_IDS]; // acceptance code byte2 information
    std::string m_omStrAccCodeByte3[CAN_MSG_IDS]; // acceptance code byte3 information
    std::string m_omStrAccCodeByte4[CAN_MSG_IDS]; // acceptance code byte4 information
    std::string m_omStrAccMaskByte1[CAN_MSG_IDS]; // acceptance mask byte1 information
    std::string m_omStrAccMaskByte2[CAN_MSG_IDS]; // acceptance mask byte2 information
    std::string m_omStrAccMaskByte3[CAN_MSG_IDS]; // acceptance mask byte3 information
    std::string m_omStrAccMaskByte4[CAN_MSG_IDS]; // acceptance mask byte4 information
    std::string m_omHardwareDesc;                 // Hw description which user can
    // differentiate between the channels
    int     m_bAccFilterMode;                     // acceptance filter mode(0: single, 1: Dual)
    unsigned short m_ucControllerMode;            // Controller mode (1: Active, 2: Passive)
    int     m_bSelfReception;
    int     m_bLowSpeed;
    UINT32  m_unBTL_Cycles;

    /* CAN FD related parameters */
    UINT32  m_unDataBitRate;
    UINT32  m_unDataSamplePoint;
    UINT32  m_unDataBTL_Cycles;
    UINT32  m_unDataSJW;
    bool    m_bTxDelayCompensationControl;
    UINT32  m_unTxSecondarySamplePointOffset;
    unsigned short m_bytCanRxMode;
    unsigned short m_bytCanFdRxMode;
    unsigned short m_bytCanFdTxConfig;

    int     m_bDebug;                            // debug mode for channel driver
    int     m_bPassiveMode;                      // passive mode (no bus interaction, acknowledge, etc.)
    std::string  m_omStrLocation;                // location (serial port, ip-address, ...)
    int     m_bHWTimestamps;                     // timestamps from the controllers hardware

    //Filter type: 1. Accept All 2. Reject All 3. Manual setting
    eHW_FILTER_TYPES m_enmHWFilterType[CAN_MSG_IDS];

    sCONTROLLERDETAILS()
    {
        vInitialize(TRUE);
    }
    sCONTROLLERDETAILS(const sCONTROLLERDETAILS& objRef)
    {
        vCopyMembers(objRef);
    }
    sCONTROLLERDETAILS& operator=(sCONTROLLERDETAILS& objRef)
    {
        vCopyMembers(objRef);
        return *this;
    }
    void vInitialize(BOOL bUpdateHWDesc)
    {
        // The default baudrate is taken as 500 kbps
        m_nItemUnderFocus = 64;
        m_nBTR0BTR1 = 49210;
        m_omStrCNF1 = "7";
        m_omStrCNF2 = "B8";
        m_omStrCNF3 = "5";
        m_omStrBTR0 = "C0";
        m_omStrBTR1 = "3A";
        m_omStrBaudrate = "500000";
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

        if ( bUpdateHWDesc )
        {
            m_omHardwareDesc = "";
        }
        m_bAccFilterMode = FALSE;
        m_ucControllerMode = 0x1;
        m_enmHWFilterType[0] = HW_FILTER_ACCEPT_ALL;
        m_enmHWFilterType[1] = HW_FILTER_ACCEPT_ALL;
        m_bSelfReception = TRUE;
        m_bLowSpeed = FALSE;
        m_unBTL_Cycles = 10;

        /* CAN FD related parameters */
        m_unDataBitRate                  = 2000000;
        m_unDataSamplePoint              = 70;
        m_unDataBTL_Cycles               = 10;
        m_unDataSJW                      = 03;
        m_bTxDelayCompensationControl    = 0;   // OCI_CANFD_TX_DELAY_COMPENSATION_OFF
        m_unTxSecondarySamplePointOffset = 0;
        m_bytCanRxMode                   = 2;   // OCI_CAN_RXMODE_CAN_FRAMES_USING_CAN_MESSAGE
        m_bytCanFdRxMode                 = 4;   // OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE
        m_bytCanFdTxConfig               = 2;   // OCI_CANFDTX_USE_CANFD_FRAMES_ONLY
    }

    void LoadControllerConfigData(xmlNodePtr& /*pNodePtr*/)
    {
    }

    void LoadControllerConfigData(BYTE*& pbyTemp)
    {
        COPY_DATA_2(&m_nItemUnderFocus, pbyTemp, sizeof(INT));
        COPY_DATA_2(&m_nBTR0BTR1, pbyTemp, sizeof(INT));
        COPY_DATA_2(&m_bAccFilterMode, pbyTemp, sizeof(INT));
        COPY_DATA_2(&m_ucControllerMode, pbyTemp, sizeof(INT));
        COPY_DATA_2(&m_bSelfReception, pbyTemp, sizeof(INT));
        COPY_DATA_2(&m_bLowSpeed, pbyTemp, sizeof(INT));
        COPY_DATA_2(&m_enmHWFilterType[0], pbyTemp, sizeof(eHW_FILTER_TYPES));
        COPY_DATA_2(&m_enmHWFilterType[1], pbyTemp, sizeof(eHW_FILTER_TYPES));

        char chTemp[1024] = {'\0'};
        int nSize;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrCNF1 = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrCNF2 = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrCNF3 = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrBTR0 = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrBTR1 = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrBaudrate = chTemp;
        double fBaudRate = atof(m_omStrBaudrate.c_str());
        fBaudRate *= 1000;   //convert from Kbps to bps
        std::stringstream ss;
        ss << fBaudRate;
        m_omStrBaudrate = ss.str();

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrClock = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrSamplePercentage = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrSampling = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrWarningLimit = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrPropagationDelay = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrSjw = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte1[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte1[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte2[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte2[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte3[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte3[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte4[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccCodeByte4[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte1[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte1[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte2[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte2[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte3[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte3[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte4[0] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omStrAccMaskByte4[1] = chTemp;

        COPY_DATA_2(&nSize, pbyTemp, sizeof(INT));
        COPY_DATA_2(chTemp, pbyTemp, sizeof(char)*nSize);
        chTemp[nSize] = '\0';
        m_omHardwareDesc = chTemp;
    }

    void GetControllerConfigSize(int& nSize)
    {
        nSize = 34 * sizeof(int);
        nSize += 2 * sizeof(eHW_FILTER_TYPES);
        nSize += m_omStrCNF1.length();
        nSize += m_omStrCNF2.length();
        nSize += m_omStrCNF3.length();
        nSize += m_omStrBTR0.length();
        nSize += m_omStrBTR1.length();
        nSize += m_omStrBaudrate.length();
        nSize += m_omStrClock.length();
        nSize += m_omStrSamplePercentage.length();
        nSize += m_omStrSampling.length();
        nSize += m_omStrWarningLimit.length();
        nSize += m_omStrPropagationDelay.length();
        nSize += m_omStrSjw.length();
        nSize += m_omStrAccCodeByte1[0].length();
        nSize += m_omStrAccCodeByte1[1].length();
        nSize += m_omStrAccCodeByte2[0].length();
        nSize += m_omStrAccCodeByte2[1].length();
        nSize += m_omStrAccCodeByte3[0].length();
        nSize += m_omStrAccCodeByte3[1].length();
        nSize += m_omStrAccCodeByte4[0].length();
        nSize += m_omStrAccCodeByte4[1].length();
        nSize += m_omStrAccMaskByte1[0].length();
        nSize += m_omStrAccMaskByte1[1].length();
        nSize += m_omStrAccMaskByte2[0].length();
        nSize += m_omStrAccMaskByte2[1].length();
        nSize += m_omStrAccMaskByte3[0].length();
        nSize += m_omStrAccMaskByte3[1].length();
        nSize += m_omStrAccMaskByte4[0].length();
        nSize += m_omStrAccMaskByte4[1].length();
        nSize += m_omHardwareDesc.length();
    }

    void SaveConfigDataToXML(xmlNodePtr pNodePtr)
    {
        double fBaudRate = atof(m_omStrBaudrate.c_str());
        // if( m_omHardwareDesc.find("Vector") == -1)      //if its not VECTOR then convert to Kbps
        {
            fBaudRate = fBaudRate/1000;    //convert to Kbps before saving to XML
        }
        std::stringstream ss;
        ss << fBaudRate;
        std::string omStrBaudrate = ss.str();
        const char* strBaudRate = omStrBaudrate.c_str();
        const char* strCNF1 = m_omStrCNF1.c_str();
        const char* strCNF2 = m_omStrCNF2.c_str();
        const char* strCNF3 = m_omStrCNF3.c_str();
        const char* strVar;
        const char* strLocation = m_omStrLocation.c_str();

        xmlNewChild(pNodePtr, nullptr, BAD_CAST "BaudRate", BAD_CAST strBaudRate);
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CNF1", BAD_CAST strCNF1);
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CNF2", BAD_CAST strCNF2);
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CNF3", BAD_CAST strCNF3);

        strVar = m_omStrBTR0.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "BTR0", BAD_CAST strVar);

        strVar = m_omStrBTR1.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "BTR1", BAD_CAST strVar);

        strVar = m_omStrClock.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "Clock", BAD_CAST strVar);

        strVar = m_omStrSamplePercentage.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "SamplePerc", BAD_CAST strVar);

        strVar = m_omStrSampling.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "Sampling", BAD_CAST strVar);

        std::stringstream stream16a;
        std::string strBTLCycles;
        stream16a << m_unBTL_Cycles;
        strBTLCycles = stream16a.str();
        strVar = strBTLCycles.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "BTLCycles", BAD_CAST strVar);
        strVar = m_omStrWarningLimit.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "Warning_Limit", BAD_CAST strVar);

        strVar = m_omStrPropagationDelay.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "Propagation_Delay", BAD_CAST strVar);

        strVar = m_omStrSjw.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "SJW", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte1[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte1_0", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte1[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte1_1", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte2[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte2_0", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte2[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte2_1", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte3[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte3_0", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte3[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte3_1", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte4[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte4_0", BAD_CAST strVar);

        strVar = m_omStrAccCodeByte4[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccCodeByte4_1", BAD_CAST strVar);

        //------------------------
        strVar = m_omStrAccMaskByte1[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte1_0", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte1[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte1_1", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte2[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte2_0", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte2[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte2_1", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte3[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte3_0", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte3[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte3_1", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte4[0].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte4_0", BAD_CAST strVar);

        strVar = m_omStrAccMaskByte4[1].c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccMaskByte4_1", BAD_CAST strVar);

        strVar = m_omHardwareDesc.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "HardwareDesc", BAD_CAST strVar);

        std::string strData;

        std::stringstream stream;
        stream << m_nItemUnderFocus;
        strData = stream.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "ItemUnderFocus", BAD_CAST strVar);

        std::stringstream stream1;
        stream1 << m_nBTR0BTR1;
        strData = stream1.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "BTR0BTR1", BAD_CAST strVar);

        std::stringstream stream2;
        stream2 << m_bAccFilterMode;
        strData = stream2.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "AccFilterMode", BAD_CAST strVar);

        std::stringstream stream3;
        stream3 << m_ucControllerMode;
        strData = stream3.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "ControllerMode", BAD_CAST strVar);

        std::stringstream stream4;
        stream4 << m_bSelfReception;
        strData = stream4.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "SelfReception", BAD_CAST strVar);

        std::stringstream stream5;
        stream5 << (int)m_enmHWFilterType[0];
        strData = stream5.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "HWFilterType_0", BAD_CAST strVar);

        std::stringstream stream6;
        stream6 << (int)m_enmHWFilterType[1];
        strData = stream6.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "HWFilterType_1", BAD_CAST strVar);

        std::stringstream stream7;
        stream7 << m_bDebug;
        strData = stream7.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "Debug", BAD_CAST strVar);

        std::stringstream stream8;
        stream8 << m_bPassiveMode;
        strData = stream8.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "PassiveMode", BAD_CAST strVar);

        std::stringstream stream9;
        stream9 << m_bHWTimestamps;
        strData = stream9.str();

        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "HWTimestamps", BAD_CAST strVar);

        xmlNewChild(pNodePtr, nullptr, BAD_CAST "Location", BAD_CAST strLocation);

        std::stringstream stream10;
        stream10 << m_bLowSpeed;
        strData = stream10.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "LowSpeed", BAD_CAST strVar);

        std::stringstream stream11;
        stream11 << m_unDataBitRate;
        strData = stream11.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_BaudRate", BAD_CAST strVar);

        std::stringstream stream12;
        stream12 << m_unDataSamplePoint;
        strData = stream12.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_SamplePoint", BAD_CAST strVar);

        std::stringstream stream13;
        stream13 << m_unDataBTL_Cycles;
        strData = stream13.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_BTLCycles", BAD_CAST strVar);

        std::stringstream stream14;
        stream14 << m_unDataSJW;
        strData = stream14.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_SJW", BAD_CAST strVar);

        std::stringstream stream15;
        stream15 << m_bTxDelayCompensationControl;
        strData = stream15.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_TxDelayCompensation", BAD_CAST strVar);

        std::stringstream stream16;
        stream16 << m_unTxSecondarySamplePointOffset;
        strData = stream16.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_TxSecondarySamplePoint", BAD_CAST strVar);

        std::stringstream stream17;
        stream17 << (int)m_bytCanRxMode;
        strData = stream17.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_CanRxMode", BAD_CAST strVar);

        std::stringstream stream18;
        stream18 << (int)m_bytCanFdRxMode;
        strData = stream18.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_CanFdRxMode", BAD_CAST strVar);

        std::stringstream stream19;
        stream19 << (int)m_bytCanFdTxConfig;
        strData = stream19.str();
        strVar = strData.c_str();
        xmlNewChild(pNodePtr, nullptr, BAD_CAST "CANFD_CanFdTxConfig", BAD_CAST strVar);
    }

    void GetControllerConfigData(BYTE*& pbyTemp, int& nSize)
    {
        INT nIntSize = sizeof(INT);
        COPY_DATA(pbyTemp, &m_nItemUnderFocus, sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_nBTR0BTR1,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_bAccFilterMode,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_ucControllerMode,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_bSelfReception,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_bLowSpeed,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_enmHWFilterType[0],  sizeof(eHW_FILTER_TYPES));
        nSize += sizeof(eHW_FILTER_TYPES);
        COPY_DATA(pbyTemp, &m_enmHWFilterType[1],  sizeof(eHW_FILTER_TYPES));
        nSize += sizeof(eHW_FILTER_TYPES);
        COPY_DATA(pbyTemp, &m_bDebug,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_bPassiveMode,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, &m_bHWTimestamps,  sizeof(INT));
        nSize += nIntSize;

        int nStrSize;

        nStrSize = m_omStrCNF1.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrCNF1.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrCNF2.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrCNF2.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrCNF3.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrCNF3.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrBTR0.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrBTR0.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrBTR1.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrBTR1.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrBaudrate.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrBaudrate.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrClock.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrClock.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrSamplePercentage.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrSamplePercentage.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrSampling.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrSampling.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrWarningLimit.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrWarningLimit.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrPropagationDelay.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrPropagationDelay.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrSjw.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrSjw.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte1[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte1[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte1[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte1[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte2[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte2[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte2[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte2[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte3[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte3[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte3[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte3[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte4[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte4[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccCodeByte4[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccCodeByte4[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte1[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte1[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte1[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte1[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte2[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte2[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrCNF1.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte2[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte3[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte3[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte3[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte3[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte4[0].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte4[0].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrAccMaskByte4[1].length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrAccMaskByte4[1].c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omHardwareDesc.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omHardwareDesc.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;

        nStrSize = m_omStrLocation.length();
        COPY_DATA(pbyTemp, &nStrSize,  sizeof(INT));
        nSize += nIntSize;
        COPY_DATA(pbyTemp, m_omStrLocation.c_str(),  sizeof(char)*nStrSize);
        nSize += nStrSize;
    }
};
typedef sCONTROLLERDETAILS   SCONTROLLER_DETAILS;
typedef sCONTROLLERDETAILS*  PSCONTROLLER_DETAILS;

struct sSUBBUSSTATISTICS
{
    /** error transmit count */
    unsigned int m_unErrorTxCount;

    /** total transmit message count */
    unsigned int m_unTotalTxMsgCount;

    /** transmit standard message count */
    unsigned int m_unTxSTDMsgCount;

    /** total bits per second */
    unsigned int m_unTotalBitsperSec;

    /** transmit extended message count */
    unsigned int m_unTxEXTDMsgCount;

    /** transmit standard remote message count */
    unsigned int m_unTxSTD_RTRMsgCount;

    /** transmit extended remote message count */
    unsigned int m_unTxEXTD_RTRMsgCount;

    /** total receive message count */
    unsigned int m_unTotalRxMsgCount;

    /** receive standard message count */
    unsigned int m_unRxSTDMsgCount;

    /** receive extended message count */
    unsigned int m_unRxEXTDMsgCount;

    /** receive standard remote message count */
    unsigned int m_unRxSTD_RTRMsgCount;

    /** receive extended remote message count */
    unsigned int m_unRxEXTD_RTRMsgCount;

    /** error total count */
    unsigned int m_unErrorTotalCount;

    /** error receive count */
    unsigned int m_unErrorRxCount;

    /** data length code count */
    unsigned int m_unDLCCount;
};
typedef sSUBBUSSTATISTICS SSUBBUSSTATISTICS;
typedef sSUBBUSSTATISTICS* PSSUBBUSSTATISTICS;

/**
 * Bus statistics structure
 */
struct sBUSSTATISTICS
{
    /** total bits per second */
    unsigned int m_unTotalBitsperSec;

    /** bus load */
    double m_dBusLoad;

    /** peak bus load */
    double m_dPeakBusLoad;

    /** total message count */
    unsigned int m_unTotalMsgCount;

    /** messages per second */
    unsigned int m_unMsgPerSecond;

    /** total transmit message count */
    unsigned int m_unTotalTxMsgCount;

    /** total transmit message rate */
    double m_dTotalTxMsgRate;

    /** transmit standard message count */
    unsigned int m_unTxSTDMsgCount;

    /** transmit standard message rate */
    double m_dTxSTDMsgRate;

    /** transmit extended message count */
    unsigned int m_unTxEXTDMsgCount;

    /** transmit extended message rate */
    double m_dTxEXTMsgRate;

    /** transmit standard remote message count */
    unsigned int m_unTxSTD_RTRMsgCount;

    /** transmit extended remote message count */
    unsigned int m_unTxEXTD_RTRMsgCount;

    /** total receive message count */
    unsigned int m_unTotalRxMsgCount;

    /** total receive message rate */
    double m_dTotalRxMsgRate;

    /** receive standard message count */
    unsigned int m_unRxSTDMsgCount;

    /** receive standard message rate */
    double m_dRxSTDMsgRate;

    /** receive extended message count */
    unsigned int m_unRxEXTDMsgCount;

    /** receive extended message rate */
    double m_dRxEXTMsgRate;

    /** receive standard remote message count */
    unsigned int m_unRxSTD_RTRMsgCount;

    /** receive extended remote message count */
    unsigned int m_unRxEXTD_RTRMsgCount;

    /** error transmit count */
    unsigned int m_unErrorTxCount;

    /** error transmit rate */
    double m_dErrorTxRate;

    /** error receive count */
    unsigned int m_unErrorRxCount;

    /** error receive rate */
    double m_dErrorRxRate;

    /** error total count */
    unsigned int m_unErrorTotalCount;

    /** error rate */
    double m_dErrorRate;

    /** data length code count */
    unsigned int m_unDLCCount;

    /** baud rate */
    double m_dBaudRate;

    /** total bus load */
    double m_dTotalBusLoad;

    /** samples */
    int m_nSamples;

    /** average bus load */
    double m_dAvarageBusLoad;

    /** transmit error counter */
    unsigned char m_ucTxErrorCounter;

    /** receive error counter */
    unsigned char m_ucRxErrorCounter;

    /** transmit peak error count */
    unsigned char m_ucTxPeakErrorCount;

    /** receive peak error count */
    unsigned char m_ucRxPeakErrorCount;

    /** status */
    unsigned char m_ucStatus;


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
    /**
     * Error Type.
     * Can be ERROR_BUS, ERROR_DEVICE_BUFF_OVERFLOW, ERROR_DRIVER_BUFF_OVERFLOW, ERROR_UNKNOWN
     */
    unsigned char m_ucErrType;

    /** Stores the value of err capture register in case of bus error */
    unsigned char m_ucReg_ErrCap;

    /** transmit error count */
    unsigned char m_ucTxErrCount;

    /** receive error count */
    unsigned char m_ucRxErrCount;

    /** channel */
    unsigned char m_ucChannel;

    /** added for providing Error bit details */
    int m_nSubError;
};
typedef sERROR_INFO SERROR_INFO;
typedef sERROR_INFO* PSERROR_INFO;

/**
 * This structure is used for communicating between Driver and CAN Application
 */
union sTDATAINFO
{
    /** The received / transmitted message */
    STCAN_MSG m_sCANMsg;

    /** error info */
    SERROR_INFO m_sErrInfo;
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

    /** Multiplication factor */
    static int  m_nMFactor;

public:
    /** Type of the message */
    unsigned char    m_ucDataType;

    /** Time stamp, Contains the val returned from QueryPerf..Counter() */
    LARGE_INTEGER    m_lTickCount;
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
 * This structure holds Error count of Rx and Tx Process
 */
struct sERROR_CNT
{
    /** transmit error count */
    unsigned char m_ucTxErrCount;

    /** receive error count */
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
