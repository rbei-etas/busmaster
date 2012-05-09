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
 * \file      J1939_UtilityFuncs.h
 * \brief     Definition file for J1939 Utility Functions
 * \author    Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for J1939 Utility Functions
 */

#pragma once

/* Project includes */
#include "Include/BaseDefs.h"
#include "Datatypes/J1939_DataTypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Utility/Utility_Thread.h"

const UCHAR ucWD_MSG_LEN                  = 2;
const UCHAR ucCONTEST_MSG_LEN             = 2;
const UCHAR ucDATAACK_MSG_LEN             = 1;
const UCHAR ucCONSET_MSG_LEN              = 3;
const UCHAR ucCONACK_MSG_LEN              = 2;
#define MAX_BROAD_FRAME_SIZE                  8
#define MAX_WD_FRAME_SIZE                     2

#define DATA_FAILURE        0
#define DATA_SUCCESS        1
#define DATA_OUT_OF_ORDER   2
#define DATA_CON_ABORT      3
#define DATA_EOM            4
#define DATA_TIMEOUT        5
#define DATA_LAST_PACKET    6
#define DATA_CLEAR_2_SEND   7
#define DATA_DELAY_2_SEND   8
#define EXECUTE_CLBCK_FN(TYPE, PGN, SRC, DEST, SUCCESS) \
    vExecuteClbckFuncPtrs(TYPE, PGN, SRC, DEST, SUCCESS)

/* Waiting for */
const WORD WF_BROADCAST     = 0xA1;
const WORD WF_LONGDATA      = 0xA2;
const WORD WF_CLEAR2SEND    = 0xA3;

/* Message types */
const UINT32 REQUEST_MSG            = 0x18EAFEFE;
const UINT32 ADDRESS_CLAIMED_MSG    = 0x18EEFEFE;
const UINT32 COMMANDED_MSG          = 0x18FED8FE;
const UINT32 ACKNOWLEGDE_MSG        = 0x18E8FEFE;
const UINT32 TPCM_MSG               = 0x18ECFEFE;
const UINT32 TPDT_MSG               = 0x18EBFEFE;
const UINT32 BROADCAST_ANNOUNCE_MSG = 0x18ECFF01;

/* Different types of delay*/
const UINT TO_BROADCAST = 100;  //ms
const UINT TO_RESPONSE  = 200;  //ms
const UINT TO_HOLDING   = 500;  //ms
const UINT TO_T1        = 750;  //ms
const UINT TO_T2        = 1250; //ms
const UINT TO_T3        = 1250; //ms
const UINT TO_T4        = 1050; //ms

/* PDU FORMATS */
const BYTE PDU_FORMAT_TPCM = 236;
const BYTE PDU_FORMAT_TPDT = 235;
const BYTE PDU_FORMAT_REQUEST4 = 234;
const BYTE PDU_FORMAT_ACL  = 238;
const BYTE PDU_FORMAT_ACK  = 232;

/* CONTROL BYTE */
const BYTE CB_REQ_TO_SEND = 16;
const BYTE CB_CLEAR_TO_SEND = 17;
const BYTE CB_EOM_ACK = 19;
const BYTE CB_BAM = 32;
const BYTE CB_CON_ABORT = 255;
const BYTE CB_ACK_POS   = 0;
const BYTE CB_ACK_NEG   = 1;

/* required macros */
const BYTE MAX_TPDU_DATA_SIZE   = 7;
const BYTE MAX_FRAME_DATA_SIZE  = 8;
const UINT MAX_RETRANSMISSION   = 5;

const BYTE MAX_SEQ_NO           = 0xFF;
const INT REQUEST_PDU_SIZE      = 3;        //DATA TYPE is added
const INT DATA_LEN_CMD_ADDRESS  = 9;        //DATA TYPE is added

const INT REASON_NODE_ENGAGED   = 0x1;      //DATA TYPE is added
const int nTWD                  = 1024;//ms

/**
 * \brief Connection Status
 *
 * Type of connection status.
 */
typedef enum
{
    T_DISCONNECTED = 0,
    T_CONNECTED,
    T_STARTUP
} eCON_STATUS;

/**
 * \brief Connection Mode
 *
 * Type of connection mode.
 */
typedef enum
{
    CM_STANDARD = 0,
    CM_BROADCAST
} eCON_MODE;

/**
 * \bref Disconnection Reason
 *
 * Type of reason for disconnection.
 */
typedef enum
{
    T_FAULT = 0,
    T_SHUTDOWN
} eREASON;

/**
 * \brief     Get Last Frame Len
 * \param[in] unDLC Data Length Code
 * \return    Length of last frame
 *
 * Returns length of last frame.
 */
static BYTE byGetLastFrameLen(UINT unDLC)
{
    BYTE byLastFrameLen = (BYTE)(unDLC % MAX_TPDU_DATA_SIZE);

    if (byLastFrameLen == 0)
    {
        byLastFrameLen = MAX_TPDU_DATA_SIZE;
    }

    return byLastFrameLen;
}

/**
 * \brief Is Command Address
 *
 * Checks if it's an command address.
 */
static BOOL bIsCommandAddress(BYTE byPduFormat, UINT unMsgLen)
{
    return ((byPduFormat == PDU_FORMAT_ACL)
            && (unMsgLen == DATA_LEN_CMD_ADDRESS));
}

/**
 * \brief Prepare End-Of-Message Acknowdlege message
 *
 * Prepare end-of-message acknowledge message.
 */
static void PrepareEOM_ACK(BYTE* byCANData, UINT unDLC, BYTE byNoOfPackets, UINT32 unPGN)
{
    byCANData[0] = CB_EOM_ACK;
    byCANData[1] = (BYTE)unDLC;
    byCANData[2] = (BYTE)(unDLC >> 8);
    byCANData[3] = byNoOfPackets;
    byCANData[4] = 0xFF;
    byCANData[5] = (BYTE)unPGN;
    byCANData[6] = (BYTE)(unPGN >> 8);
    byCANData[7] = (BYTE)(unPGN >> 16);
}

/**
 * \brief Prepare Clear-2-Send message
 *
 * Prepares a clear-to-send message.
 */
static void PrepareClear_2_Send(BYTE* byCANData, BYTE byNoOfPackets, BYTE byNextPacket, UINT32 unPGN)
{
    byCANData[0] = CB_CLEAR_TO_SEND;
    byCANData[1] = byNoOfPackets;
    byCANData[2] = byNextPacket;
    byCANData[3] = 0xFF;
    byCANData[4] = 0xFF;
    byCANData[5] = (BYTE)unPGN;
    byCANData[6] = (BYTE)(unPGN >> 8);
    byCANData[7] = (BYTE)(unPGN >> 16);
}

/**
 * \brief     Get Number of Packets Required
 * \param[in] unSizeInBytes Size in Bytes
 * \return    Number of packets
 *
 * Returns the number of packets required.
 */
static UINT unGetNoOfPacketsRequired(UINT unSizeInBytes)
{
    int nLastFrameNo = (int)(unSizeInBytes / MAX_TPDU_DATA_SIZE);
    int nLastFrameLen = unSizeInBytes % MAX_TPDU_DATA_SIZE;

    if (nLastFrameLen > 0)
    {
        //If there are data bytes remaining then next frame is last frame
        nLastFrameNo++;
    }

    return nLastFrameNo;
}

/**
 * \brief Prepare Data
 *
 * Prepares data.
 */
static void vPrepareData(BYTE* byFrameData, BYTE byControlByte, UINT unDLC, UINT32 unPGN)
{
    byFrameData[0] = byControlByte;
    WORD* wTemp = (WORD*)&(byFrameData[1]);
    *wTemp = (WORD)unDLC;
    int nPackets = (int)unGetNoOfPacketsRequired(unDLC);
    byFrameData[3] = (BYTE)nPackets;
    byFrameData[4] = 0xFF;
    byFrameData[5] = (BYTE)unPGN;
    byFrameData[6] = (BYTE)(unPGN >> 8);
    byFrameData[7] = (BYTE)(unPGN >> 16);
}

/**
 * \brief Prepare P2P Id
 *
 * Prepares a P2P Id
 */
static UINT32 Prepare_P2P_Id(BYTE byPDUFormat, BYTE bySrcAdres,
                             BYTE byDestAdres, BYTE byPriority)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPriority = byPriority;
    uExtId.m_s29BitId.m_bySrcAddress = bySrcAdres;
    uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byDataPage = 0;
    uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byReserved = 0;
    uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format = byPDUFormat;
    uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = byDestAdres;
    return uExtId.m_unExtID;
}

/**
 * \brief Is Connection Req Message
 *
 * Checks if it's a connection req message.
 */
static BOOL bIsConReqMsg(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (byControlByte == CB_REQ_TO_SEND));
}

/**
 * \brief Is Broadcast-Announce-Message
 *
 * Check if it's a Broadcast Announce Message.
 */
static BOOL bIsBAM(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (byControlByte == CB_BAM));
}

/**
 * \brief Is End-Of-Message Acknowledge message?
 *
 * Check if it's an end-of-message acknowledge message.
 */
static BOOL bIsEOM_ACK(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (byControlByte == CB_EOM_ACK));
}

/**
 * \brief Is Connection Abort Message
 *
 * Check if it's a connection abort message.
 */
static BOOL bIsConAbortMsg(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (byControlByte == CB_CON_ABORT));
}

/**
 * \brief Is Connection Acknowledge Message
 *
 * Checks if it's a connection acknowledge message.
 */
static BOOL bIsConAckMsg(STCAN_MSG& sCanMsg)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = sCanMsg.m_unMsgID;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (sCanMsg.m_ucData[0] == CB_CLEAR_TO_SEND) && (0 == sCanMsg.m_ucData[2]));
}

/**
 * \brief Is Data Acknowledge Message
 *
 * Check if it's a data acknowledge message.
 */
static BOOL bIsDataAckMsg(STCAN_MSG& sCanMsg)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = sCanMsg.m_unMsgID;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (sCanMsg.m_ucData[0] == CB_CLEAR_TO_SEND) && (sCanMsg.m_ucData[2] > 0));
}

/**
 * \brief Is TPDT
 *
 * Check if it's a TPDT message.
 */
static BOOL bIsTPDT(UINT32 unExtId)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return (uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPDT);
}

/**
 * \brief Create Temporary J1939 Message
 *
 * Creates a temporary J1939 message.
 */
static void vCreateTempJ1939Msg(STJ1939_MSG& sMsg, const STCANDATA& sCanData,
                                UINT unDLC, BYTE* pbyData, EJ1939_MSG_TYPE eType)
{
    const STCAN_MSG& sCanMsg = sCanData.m_uDataInfo.m_sCANMsg;
    sMsg.vClear();
    sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID = sCanMsg.m_unMsgID;
    sMsg.m_sMsgProperties.m_eDirection = (sCanData.m_ucDataType == RX_FLAG)?
                                         DIR_RX : DIR_TX;
    sMsg.m_sMsgProperties.m_eType = eType;
    sMsg.m_sMsgProperties.m_byChannel = sCanMsg.m_ucChannel;
    sMsg.m_sMsgProperties.m_un64TimeStamp = sCanData.m_lTickCount;
    sMsg.m_unDLC = unDLC;
    sMsg.m_pbyData = new BYTE[sMsg.m_unDLC];
    memcpy(sMsg.m_pbyData, pbyData, unDLC);
}

/**
 * \brief Is Connection Level Message
 *
 * Check if it's a connection level message.
 */
static BOOL bIsConLevelMsg(UINT32 unExtId)
{
    BOOL bReturn = FALSE;
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;

    if ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPDT)
            || (uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM))
    {
        bReturn = TRUE;
    }

    return bReturn;
}

/**
 * \brief     Get Message Type
 * \param[in] unExtId 29 bit CAN identifier
 * \param[in] pbyData CAN data
 * \return    J1939 Message Type
 *
 * Returns the type of the message.
 */
static EJ1939_MSG_TYPE eGetMsgType(UINT32 unExtId,BYTE* pbyData)
{
    EJ1939_MSG_TYPE eType = MSG_TYPE_DATA;
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    BYTE byPDU_Format = uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format;

    switch (byPDU_Format)
    {
        case PDU_FORMAT_ACK:
        {
            eType = MSG_TYPE_ACKNOWLEDGEMENT;
        }
        break;

        case PDU_FORMAT_REQUEST4:
        {
            BYTE byCB = pbyData[1];

            if (byCB == PDU_FORMAT_ACL)
            {
                eType = MSG_TYPE_NM_RQST_ACL;
            }
            else
            {
                eType = MSG_TYPE_REQUEST;
            }
        }
        break;

        case PDU_FORMAT_ACL:
        {
            eType = MSG_TYPE_NM_ACL;
        }
        break;

        case PDU_FORMAT_TPCM:
        {
            BYTE byCB = pbyData[0];

            switch (byCB)
            {
                case CB_BAM:
                {
                    eType = MSG_TYPE_NM_TPCM_BAM;
                }
                break;

                case CB_REQ_TO_SEND:
                {
                    eType = MSG_TYPE_NM_TPCM_RTS;
                }
                break;

                case CB_CLEAR_TO_SEND:
                {
                    eType = MSG_TYPE_NM_TPCM_CTS;
                }
                break;

                case CB_CON_ABORT:
                {
                    eType = MSG_TYPE_NM_TPCM_CON_ABORT;
                }
                break;

                case CB_EOM_ACK:
                {
                    eType = MSG_TYPE_NM_TPCM_EOM_ACK;
                }
                break;
            }
        }
        break;

        case PDU_FORMAT_TPDT:
        {
            eType = MSG_TYPE_NM_TPDT;
        }
        break;
    }

    return eType;
}
