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
* \file      FormatMsgLIN.cpp
* \brief     Source of CFormatMsgLIN class.
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Source of CFormatMsgLIN class.
*/

#include "../FrameProcessor_stdafx.h"
#include "FormatMsgLIN.h"
#include "include/Utils_macro.h"

CFormatMsgLIN::CFormatMsgLIN(CRefTimeKeeper& ouRefTimeKeeper) :
    CFormatMsgCommon(ouRefTimeKeeper)
{
}

CFormatMsgLIN::~CFormatMsgLIN(void)
{
}

void CFormatMsgLIN::vFormatTime(BYTE bExprnFlag,
                                SFORMATTEDDATA_LIN* CurrDataLIN)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_RES(bExprnFlag))//for Absolute reset timestamp
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS_RES(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataLIN->m_u64TimeStamp,
                              CurrDataLIN->m_acTimeAbsReset);
    }
    if (IS_TM_ABS_SET(bExprnFlag))//for Absolute non-reset timestamp
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataLIN->m_u64TimeStamp,
                              CurrDataLIN->m_acTimeAbs);
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataLIN->m_u64TimeStamp,
                              CurrDataLIN->m_acTimeRel);
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataLIN->m_u64TimeStamp,
                              CurrDataLIN->m_acTimeSys);
    }
}


void CFormatMsgLIN::vFormatDataAndId(BYTE bExprnFlag,
                                     SFORMATTEDDATA_LIN* CurrDataLIN)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        sprintf(CurrDataLIN->m_acMsgIDHex, FORMAT_STR_ID_HEX, CurrDataLIN->m_dwMsgID);

        int i, j;
        for (i = 0, j = 0; i < CurrDataLIN->m_byDataLength; i++)
        {
            BYTE CurrDat = CurrDataLIN->m_abData[i];
            sprintf(&(CurrDataLIN->m_acDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        CurrDataLIN->m_acDataHex[j] = L'\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        sprintf(CurrDataLIN->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataLIN->m_dwMsgID);

        int i, j;
        for (i = 0, j = 0; i < CurrDataLIN->m_byDataLength; i++)
        {
            BYTE CurrDat = CurrDataLIN->m_abData[i];
            sprintf(&(CurrDataLIN->m_acDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            j += 4;
            CurrDataLIN->m_acDataDec[j-1] = L' ';
        }
        CurrDataLIN->m_acDataDec[j] = L'\0';
    }
}

void CFormatMsgLIN::vFormatLINDataMsg(STLINDATA* pMsgLIN,
                                      SFORMATTEDDATA_LIN* CurrDataLIN,
                                      BYTE bExprnFlag_Log)
{
    if ( pMsgLIN->m_eLinMsgType == LIN_MSG )
    {

        if (RX_FLAG == pMsgLIN->m_ucDataType)
        {
            CurrDataLIN->m_eDirection = DIR_RX;
            CurrDataLIN->m_acMsgDir[0] = L'R';
        }
        else if (TX_FLAG == pMsgLIN->m_ucDataType)
        {
            CurrDataLIN->m_eDirection = DIR_TX;
            CurrDataLIN->m_acMsgDir[0] = L'T';
        }
        else
        {
            ASSERT(false);
        }

        CurrDataLIN->m_eChannel = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChannel;
        if ((CurrDataLIN->m_eChannel >= CHANNEL_LIN_MIN) && (CurrDataLIN->m_eChannel <= CHANNEL_LIN_MAX ))
        {
            sprintf(CurrDataLIN->m_acChannel, "%d", CurrDataLIN->m_eChannel);
        }


        //     CurrDataLIN->m_byIDType = TYPE_ID_LIN_STANDARD;
        //    CurrDataLIN->m_acType[0] = L's';

        CurrDataLIN->m_eEventType = EVENT_LIN_NONE;

        CurrDataLIN->m_byChecksum='\0';
        _itot(pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucDataLen, CurrDataLIN->m_acDataLen, 10);
        _tcscpy(CurrDataLIN->m_acMsgDesc,  "Description");

        CurrDataLIN->m_u64TimeStamp = pMsgLIN->m_lTickCount.QuadPart;
        CurrDataLIN->m_dwMsgID = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgID;

        CurrDataLIN->m_byDataLength = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucDataLen;
        CurrDataLIN->m_byChecksum=pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum;
        //CurrDataLIN->m_byChecksumTypestr=pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksumTypstr;

        CurrDataLIN->m_byChecksumType=pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksumTyp;

        CurrDataLIN->m_byMsgType=pMsgLIN->m_eLinMsgType;
        memcpy(CurrDataLIN->m_abData, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucData,
               CurrDataLIN->m_byDataLength);
    }

    else if ( pMsgLIN->m_eLinMsgType == LIN_EVENT )
    {
        /*PROCESS ERROR MSGS: If Error Message type. Change the data and type fields. */

        std::string strText;
        vGetLinEventDescription(pMsgLIN->m_uDataInfo.m_sErrInfo, strText);

        strcpy(CurrDataLIN->m_acType, strText.c_str());

        CurrDataLIN->m_byChecksum='\0';

        strcpy(CurrDataLIN->m_acDataLen,  "");
        strcpy(CurrDataLIN->m_acMsgDesc,  "");
        strcpy(CurrDataLIN->m_acChecksum,  "");
        strcpy(CurrDataLIN->m_acMsgIDHex,  "");
        strcpy(CurrDataLIN->m_acMsgIDDec,  "");
        strcpy(CurrDataLIN->m_acDataHex,  "");
        strcpy(CurrDataLIN->m_acDataDec,  "");
        strcpy(CurrDataLIN->m_acMsgDir,  "");

        CurrDataLIN->m_eEventType = pMsgLIN->m_uDataInfo.m_sErrInfo.m_eEventType;

        CurrDataLIN->m_dwMsgID = -1;
        CurrDataLIN->m_u64TimeStamp = pMsgLIN->m_lTickCount.QuadPart;
        if ( pMsgLIN->m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_ERRNOANS || pMsgLIN->m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_ERRCRC )
        {
            CurrDataLIN->m_dwMsgID = pMsgLIN->m_uDataInfo.m_sErrInfo.m_ucId;
        }

        CurrDataLIN->m_byDataLength = 0;
        TYPE_CHANNEL CurrChannel = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChannel;  // Assuming default LIN msg
        if ((CurrChannel >= CHANNEL_LIN_MIN) && (CurrChannel <= CHANNEL_LIN_MAX ))
        {
            sprintf_s(CurrDataLIN->m_acChannel, "%d", CurrChannel);
        }
        CurrDataLIN->m_byMsgType=pMsgLIN->m_eLinMsgType;
    }





    vFormatTime(bExprnFlag_Log, CurrDataLIN);
    vFormatDataAndId(bExprnFlag_Log, CurrDataLIN);
}

void CFormatMsgLIN::vGetLinEventDescription(SERROR_INFO_LIN sLinErrorInfo, std::string& strDesc)
{
    switch ( sLinErrorInfo.m_eEventType )
    {
        case EVENT_LIN_ERRSYNC:
            strDesc ="Error - Sync";
            break;
        case EVENT_LIN_ERRNOANS:
            strDesc ="Error - Slave Not Responding";
            break;
        case EVENT_LIN_ERRCRC:
            strDesc ="Error - CRC Error";
            break;
        case EVENT_LIN_WAKEUP:
            strDesc ="Event - Wake up";
            break;
        case EVENT_LIN_SLEEP:
            strDesc ="Event - Sleep";
            break;
        case EVENT_LIN_CRCINFO:
            strDesc ="";
            break;
        case EVENT_LIN_COLLISION:
            strDesc = "Event - Collision";
            break;
        case EVENT_LIN_ERRBIT:
            strDesc = "Event - Bit Error";
            break;
        case EVENT_LIN_ERRHEADER:
            strDesc = "Event - Frame Header Error";
            break;
        case EVENT_LIN_ERRTOUT:
            strDesc = "Event - Timeout Error";
            break;
        case EVENT_LIN_NO_MSTR_SCHED:
            strDesc = "Event - No Master Scheduled";
            break;
        case EVENT_LIN_NO_TX_DATA:
            strDesc = "Event - No Data available";
            break;
            /*case EVENT_LIN_TX_OK:
            break;*/
        case EVENT_LIN_ERRMSG:
        default:
            strDesc ="Error - Unknown";
            break;
    }
}
