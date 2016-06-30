/******************************************************************************
  Description   :  Source of CFormatMsgJ1939 class.
  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "../FrameProcessor_stdafx.h"
#include "DataTypes/J1939_dataTypes.h"
#include "FormatMsgJ1939.h"
#include "include/Utils_macro.h"

CFormatMsgJ1939::CFormatMsgJ1939(CRefTimeKeeper& ouRefTimeKeeper) :
    CFormatMsgCommon(ouRefTimeKeeper)
{
}

CFormatMsgJ1939::~CFormatMsgJ1939(void)
{
}

void CFormatMsgJ1939::vFormatTime(BYTE bExprnFlag,
                                  PSTJ1939_MSG psJ1939BData,
                                  PSFORMATTEDATA_J1939 psJ1939FData)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_RES(bExprnFlag))//for Absolute reset timestamp
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS_RES(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp,
                              psJ1939FData->m_acTimeAbsReset, sizeof(psJ1939FData->m_acTimeAbsReset));
    }
    if (IS_TM_ABS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp,
                              psJ1939FData->m_acTimeAbs, sizeof(psJ1939FData->m_acTimeAbs));
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp,
                              psJ1939FData->m_acTimeRel, sizeof(psJ1939FData->m_acTimeRel));
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp,
                              psJ1939FData->m_acTimeSys, sizeof(psJ1939FData->m_acTimeSys));
    }
}

void CFormatMsgJ1939::vFormatDataAndId(BYTE bExprnFlag,
                                       PSTJ1939_MSG psJ1939BData,
                                       PSFORMATTEDATA_J1939 psJ1939FData)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        sprintf(psJ1939FData->m_acPGNHex, FORMAT_PGN_ID_HEX,
                psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        USHORT j = 0;       //j declared outside
        for (USHORT i = 0; i < psJ1939BData->m_unDLC; i++)
        {
            BYTE CurrDat = *(psJ1939BData->m_pbyData + i);
            sprintf(&(psJ1939FData->m_pcDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        psJ1939FData->m_pcDataHex[j] = L'\0';
    }
    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        sprintf(psJ1939FData->m_acPGNDec, FORMAT_PGN_ID_DEC,
                psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        USHORT j = 0;   //j declared outside
        for (USHORT i = 0; i < psJ1939BData->m_unDLC; i++)
        {
            BYTE CurrDat = *(psJ1939BData->m_pbyData + i);
            sprintf(&(psJ1939FData->m_pcDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            psJ1939FData->m_pcDataDec[j + 3] = L' '; //known issue #51 srinivas R
            j += 4;
        }
        psJ1939FData->m_pcDataDec[j] = L'\0';
    }
}

void CFormatMsgJ1939::vInit(/*CJ1939MessageMap* podMsgMap*/)
{
    /*ASSERT(nullptr != podMsgMap);
    m_podJ1939Rules = podMsgMap;*/
}

void CFormatMsgJ1939::vFormatJ1939DataMsg(PSTJ1939_MSG psJ1939BData,
        PSFORMATTEDATA_J1939 psJ1939FData,
        BYTE bExprnFlag_Log)
{
    psJ1939FData->m_dwMsgID = psJ1939BData->m_sMsgProperties.m_uExtendedID.m_unExtID;

    GetMessageTypeStr(psJ1939BData->m_sMsgProperties.m_eType, psJ1939FData->m_acMsgType);

    TYPE_CHANNEL CurrChannel = psJ1939BData->m_sMsgProperties.m_byChannel;
    if ((CurrChannel >= CHANNEL_CAN_MIN) && (CurrChannel <= CHANNEL_CAN_MAX ))
    {
        sprintf(psJ1939FData->m_acChannel, "%d", CurrChannel);
    }

    if (IS_NUM_HEX_SET(bExprnFlag_Log))
    {
        sprintf(psJ1939FData->m_acSrcHex, FORMAT_STR_DATA_HEX, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
        sprintf(psJ1939FData->m_acDestHex, FORMAT_STR_DATA_HEX, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    }

    if (IS_NUM_DEC_SET(bExprnFlag_Log))
    {
        sprintf(psJ1939FData->m_acSrcDec, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
        sprintf(psJ1939FData->m_acDestDec, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    }

    sprintf(psJ1939FData->m_acPriority, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority);
    if (DIR_RX == psJ1939BData->m_sMsgProperties.m_eDirection)
    {
        psJ1939FData->m_acMsgDir[0] = L'R';
    }
    else if (DIR_TX == psJ1939BData->m_sMsgProperties.m_eDirection)
    {
        psJ1939FData->m_acMsgDir[0] = L'T';
    }
    else
    {
        ASSERT(false);
    }
    _itot(psJ1939BData->m_unDLC, psJ1939FData->m_acDataLen, 10);

    vFormatTime(bExprnFlag_Log, psJ1939BData, psJ1939FData);
    vFormatDataAndId(bExprnFlag_Log, psJ1939BData, psJ1939FData);
}
