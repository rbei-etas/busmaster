
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  FormatMsgJ1939.cpp
  Description   :  Source of CFormatMsgJ1939 class.
  $Log:   X:/Archive/Sources/FrameProcessor/FormatMsgJ1939.cpv  $
   
      Rev 1.4   09 Jun 2011 18:00:20   CANMNTTM
   All errors in VS2005 are removed
   
      Rev 1.3   15 Apr 2011 19:19:42   rac2kor
   Inserted RBEI Copyright information text into the file header.
   
      Rev 1.2   23 Mar 2011 14:59:14   CANMNTTM
   Support upto 32 channels
   
      Rev 1.1   13 Dec 2010 21:25:56   rac2kor
   In the structure PSFORMATTEDATA_J1939, the data member m_dwMsgID
   is now being assigned value.
   
      Rev 1.0   06 Dec 2010 18:56:20   rac2kor
    

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "FrameProcessor_stdafx.h"
#include "DataTypes/J1939_dataTypes.h"
#include "FormatMsgJ1939.h"  
#include "include/Utils_macro.h"



CFormatMsgJ1939::CFormatMsgJ1939(void)
{
    //m_podJ1939Rules = NULL;
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
                              psJ1939FData->m_acTimeAbsReset);
    }
    if (IS_TM_ABS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp, 
                              psJ1939FData->m_acTimeAbs);
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp, 
                              psJ1939FData->m_acTimeRel);
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp, 
                              psJ1939FData->m_acTimeSys);
    }
}

void CFormatMsgJ1939::vFormatDataAndId(BYTE bExprnFlag, 
                                       PSTJ1939_MSG psJ1939BData, 
                                       PSFORMATTEDATA_J1939 psJ1939FData)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        _stprintf(psJ1939FData->m_acPGNHex, FORMAT_PGN_ID_HEX, 
                  psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        USHORT j = 0;       //j declared outside
        for (USHORT i = 0; i < psJ1939BData->m_unDLC; i++)
        {
            BYTE CurrDat = *(psJ1939BData->m_pbyData + i);
            _stprintf(&(psJ1939FData->m_pcDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        psJ1939FData->m_pcDataHex[j] = L'\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        _stprintf(psJ1939FData->m_acPGNDec, FORMAT_PGN_ID_DEC, 
                  psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        USHORT j = 0;   //j declared outside
        for (USHORT i = 0; i < psJ1939BData->m_unDLC; i++)
        {
            BYTE CurrDat = *(psJ1939BData->m_pbyData + i);
            _stprintf(&(psJ1939FData->m_pcDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            j += 4;
        }
        psJ1939FData->m_pcDataDec[j] = L'\0';
    }
}

void CFormatMsgJ1939::vInit(/*CJ1939MessageMap* podMsgMap*/)
{
    /*ASSERT(NULL != podMsgMap);
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
        _stprintf(psJ1939FData->m_acChannel, _T("%d"), CurrChannel);
    }

    if (IS_NUM_HEX_SET(bExprnFlag_Log))
    {
        _stprintf(psJ1939FData->m_acSrcHex, FORMAT_STR_DATA_HEX, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
        _stprintf(psJ1939FData->m_acDestHex, FORMAT_STR_DATA_HEX, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    }

    if (IS_NUM_DEC_SET(bExprnFlag_Log))
    {
        _stprintf(psJ1939FData->m_acSrcDec, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
        _stprintf(psJ1939FData->m_acDestDec, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    }

    _stprintf(psJ1939FData->m_acPriority, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority);
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
        ASSERT(FALSE);
    }
    _itot(psJ1939BData->m_unDLC, psJ1939FData->m_acDataLen, 10);

    vFormatTime(bExprnFlag_Log, psJ1939BData, psJ1939FData);
    vFormatDataAndId(bExprnFlag_Log, psJ1939BData, psJ1939FData);
}
