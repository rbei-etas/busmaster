/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  J1939_UtilityFuncs.h
  Description   :  
  $Log:   X:/Archive/Sources/DIL_J1939/J1939_UtilityFuncs.h_v  $
 * 
 *    Rev 1.6   09 Jun 2011 17:55:46   CANMNTTM
 * All errors in VS2005 are removed
 * 
 *    Rev 1.5   07 Jun 2011 11:11:16   CANMNTTM
 *  
 * 
 *    Rev 1.4   15 Apr 2011 18:48:36   CANMNTTM
 * Added RBEI Copyright information.
  
  Author(s)     :  Pradeep Kadoor
  Date Created  :  15/04/2011
  Modified By   : 
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 ******************************************************************************/
#if !defined J1939_UTILITYFUNCS_H__INCLUDED_
#define J1939_UTILITYFUNCS_H__INCLUDED_


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

/* TYPES OF CONNECTION STATUS */
typedef enum
{
    T_DISCONNECTED = 0,
    T_CONNECTED,
    T_STARTUP
} eCON_STATUS;

/* TYPES OF CONNECTION MODE */
typedef enum
{
    CM_STANDARD = 0,
    CM_BROADCAST
}eCON_MODE;

/* TYPES OF REASON FOR DISCONNECTION */
typedef enum
{
    T_FAULT = 0,
    T_SHUTDOWN
}eREASON;

/* static functions */

/******************************************************************************
Function Name  :  byGetLastFrameLen
Input(s)       :  UINT unDLC
Output         :  BYTE - length of last frame
Functionality  :  returns length of last frame
Member of      :  
Friend of      :  -
Author(s)      :  Pradeep Kadoor
Date Created   :  23/11/2010
Modifications  :  
******************************************************************************/
static BYTE byGetLastFrameLen(UINT unDLC)
{
    BYTE byLastFrameLen = (BYTE)(unDLC % MAX_TPDU_DATA_SIZE);
    if (byLastFrameLen == 0)
    {
        byLastFrameLen = MAX_TPDU_DATA_SIZE;
    }
    return byLastFrameLen;
}

static BOOL bIsCommandAddress(BYTE byPduFormat, UINT unMsgLen)
{
    return ((byPduFormat == PDU_FORMAT_ACL)
           && (unMsgLen == DATA_LEN_CMD_ADDRESS));
}
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

static BOOL bIsConReqMsg(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
            && (byControlByte == CB_REQ_TO_SEND));
}
static BOOL bIsBAM(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
        && (byControlByte == CB_BAM));
}
static BOOL bIsEOM_ACK(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
        && (byControlByte == CB_EOM_ACK));
}
static BOOL bIsConAbortMsg(UINT32 unExtId, BYTE byControlByte)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
        && (byControlByte == CB_CON_ABORT));
}
static BOOL bIsConAckMsg(STCAN_MSG& sCanMsg)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = sCanMsg.m_unMsgID;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
        && (sCanMsg.m_ucData[0] == CB_CLEAR_TO_SEND) && (0 == sCanMsg.m_ucData[2]));
}
static BOOL bIsDataAckMsg(STCAN_MSG& sCanMsg)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = sCanMsg.m_unMsgID;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPCM)
        && (sCanMsg.m_ucData[0] == CB_CLEAR_TO_SEND) && (sCanMsg.m_ucData[2] > 0));
}
static BOOL bIsTPDT(UINT32 unExtId)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return (uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format == PDU_FORMAT_TPDT);            
}

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
    sMsg.m_sMsgProperties.m_un64TimeStamp = sCanData.m_lTickCount.QuadPart;
    sMsg.m_unDLC = unDLC;
    sMsg.m_pbyData = new BYTE[sMsg.m_unDLC];
    memcpy(sMsg.m_pbyData, pbyData, unDLC);
}
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
/******************************************************************************
Function Name  :  eGetMsgType
Input(s)       :  UINT32 unExtId - 29 bit CAN identifier,BYTE* pbyData - CAN data
Output         :  EJ1939_MSG_TYPE
Functionality  :  returns the type of the message.
Member of      :  
Friend of      :  -
Author(s)      :  Pradeep Kadoor
Date Created   :  23/11/2010
Modifications  :  
******************************************************************************/
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

#endif //J1939_UTILITYFUNCS_H__INCLUDED_