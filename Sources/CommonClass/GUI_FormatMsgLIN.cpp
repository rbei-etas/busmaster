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
* \file      GUI_FormatMsgLIN.cpp
* \brief     Implementation of CFormatMsgLIN class
* \author    Shashank
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Implementation of CFormatMsgLIN class
*/


#include "CommonClass_stdafx.h"
#include "GUI_FormatMsgLIN.h"
#include "include/Utils_macro.h"
#include "Include/LIN_Error_Defs.h"
//#include "LIN_ISOLAR_EVE_VLIN/LIN_ISOLAR_EVE_VLIN_Defines.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

struct sERRORMSGINFO
{
    unsigned short m_usErrorCode; // Error code
    char* m_ptcErorMsg;          // Error message
};
typedef sERRORMSGINFO SERRORMSGINFO;
typedef sERRORMSGINFO* PERRORMSGINFO;
const int ERRORS_DEFINED = 15;


static SERRORMSGINFO sg_asErrorEntry[ERRORS_DEFINED] =
{
    {ERROR_UNKNOWN,       "Unknown Error"},
    {BIT_ERROR_RX,        "Bus Error - Bit Error(Rx)"},
    {CRC_ERROR_RX,        "Bus Error - CRC Error(Rx)"},
    {FORM_ERROR_RX,       "Bus Error - Form Error(Rx)"},
    {STUFF_ERROR_RX,      "Error Frame - Stuff error(Rx)"},
    {OTHER_ERROR_RX,      "Bus Error - Other Error(Rx)"},
    {BIT_ERROR_TX,        "Bus Error - Bit Error(Tx)"},
    {CRC_ERROR_TX,        "Bus Error - CRC Error(Tx)"},
    {FORM_ERROR_TX,       "Bus Error - Form Error(Tx)"},
    {STUFF_ERROR_TX,      "Bus Error - Stuff Error(Tx)"},
    {OTHER_ERROR_TX,      "Bus Error - Other Error(Tx)"},
    {ERROR_DEVICE_BUFF_OVERFLOW,        "Controller Buffer Overflow"},
    {ERROR_DRIVER_BUFF_OVERFLOW,        "Driver Buffer Overflow"},
    {ERROR_APPLICATION_BUFF_OVERFLOW,   "Application Buffer Overflow"},
    {ERROR_DLLMSG_BUFF_OVERFLOW,        "DLL message buffer Overflow"}
};

/*******************************************************************************
Function Name  : CFormatMsgLIN
Input(s)       : -
Output         : -
Functionality  : Constructor
Member of      : CFormatMsgLIN
Author(s)      : Shashank
Date Created   : 29.01.2014
Modifications  :
*******************************************************************************/
CFormatMsgLIN::CFormatMsgLIN(void)
{
}

/*******************************************************************************
Function Name  : ~CFormatMsgLIN
Input(s)       : -
Output         : -
Functionality  : Destructor
Member of      : CFormatMsgLIN
Author(s)      : Shashank
Date Created   : 29.01.2014
Modifications  :
*******************************************************************************/
CFormatMsgLIN::~CFormatMsgLIN(void)
{
}

/*******************************************************************************
Function Name  : vFormatTime
Input(s)       : -
Output         : -
Functionality  : Format Time
Member of      : CFormatMsgLIN
Author(s)      : Shashank
Date Created   : 29.01.2014
Modifications  :
*******************************************************************************/
//[RS_LIN_06_07]
void CFormatMsgLIN::vFormatTime(BYTE bExprnFlag,
                                SFORMATTEDDATA_LIN* CurrDataLIN)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_SET(bExprnFlag))
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


/*******************************************************************************
Function Name  : vFormatDataAndId
Input(s)       : -
Output         : -
Functionality  : Format data and id
Member of      : CFormatMsgLIN
Author(s)      : Shashank
Date Created   : 20.01.2014
Modifications  :
*******************************************************************************/
//[RS_LIN_06_09]
void CFormatMsgLIN::vFormatDataAndId(BYTE bExprnFlag,
                                     SFORMATTEDDATA_LIN* CurrDataLIN)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        strcpy(CurrDataLIN->m_acMsgIDHex, "");
        if ( CurrDataLIN->m_dwMsgID != -1 )
        {
            sprintf_s(CurrDataLIN->m_acMsgIDHex, FORMAT_STR_ID_HEX, CurrDataLIN->m_dwMsgID);
        }
        int j = 0;  // j declared outside

        for (int i = 0; i < CurrDataLIN->m_byDataLength; i++)
        {
            BYTE CurrDat = CurrDataLIN->m_abData[i];
            _stprintf(&(CurrDataLIN->m_acDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        CurrDataLIN->m_acDataHex[j] = L'\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        strcpy(CurrDataLIN->m_acMsgIDDec, "");
        if ( CurrDataLIN->m_dwMsgID != -1 )
        {
            sprintf_s(CurrDataLIN->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataLIN->m_dwMsgID);
        }
        int j = 0;

        for (int i = 0; i < CurrDataLIN->m_byDataLength; i++)
        {
            BYTE CurrDat = CurrDataLIN->m_abData[i];
            _stprintf(&(CurrDataLIN->m_acDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            j += 4;
            CurrDataLIN->m_acDataDec[j-1] = L' ';
        }

        CurrDataLIN->m_acDataDec[j-1] = L'\0';
    }
}

/*******************************************************************************
Function Name  : usProcessCurrErrorEntry
Input(s)       : SERROR_INFO& sErrorInfo
Output         : Returns Error ID as USHORT.
Functionality  : Processes the current Error entry and returns the Error code.
Member of      : CFormatMsgLIN
Author(s)      : Arunkumar K
Date Created   : 08-09-2010
Modifications  :
*******************************************************************************/
USHORT CFormatMsgLIN::usProcessCurrErrorEntryLin(SERROR_INFO_LIN& sErrorInfo)
{
    // Get the Error code
    USHORT usErrorID;
    // Get the channel number
    CHAR nChannel = sErrorInfo.m_ucChannel - 1;
    if( nChannel < 0 || nChannel >= defNO_OF_CHANNELS )
    {
        ASSERT( FALSE );
        // Take prevension
        nChannel = 0;
    }

    // Decide which module(s) to notify by analysing the error code
    // Accordingly notify the modules by sending/posting message

    usErrorID = sErrorInfo.m_eEventType;

    return usErrorID;
}

/*******************************************************************************
Function Name  : vFormatCurrErrorEntry
Input(s)       : usErrorID
Output         : TCHAR* (Error Name)
Functionality  : Gets the pointer to Message data Buffer from PSDI_LIN DLL
and updates the same in List Control.
Member of      : CFormatMsgLIN
Author(s)      : Shashank
Date Created   : 20.01.2014
Modifications  :
*******************************************************************************/
char* CFormatMsgLIN::vFormatCurrErrorEntry(USHORT usErrorID)
{
    BOOL bErrProcessed = FALSE;
    int nCount = 0;

    while ((nCount < ERRORS_DEFINED) && (bErrProcessed == FALSE))
    {
        if (usErrorID == sg_asErrorEntry[nCount].m_usErrorCode)
        {
            bErrProcessed = TRUE;
            return sg_asErrorEntry[nCount].m_ptcErorMsg;
        }

        nCount++;
    }

    return NULL;
}

/*******************************************************************************
Function Name  : vFormatLINDataMsg
Input(s)       : -
Output         : -
Functionality  : Format LIN data bytes
Member of      : CFormatMsgLIN
Author(s)      : Shashank
Date Created   : 20.01.2014
/*******************************************************************************/
void CFormatMsgLIN::vFormatLINDataMsg(STLINDATA* pMsgLIN,
                                      SFORMATTEDDATA_LIN* CurrDataLIN,
                                      BYTE bExprnFlag_Log)
{
    if ( pMsgLIN->m_eLinMsgType == LIN_MSG )
    {
        if (RX_FLAG == pMsgLIN->m_ucDataType)
        {
            CurrDataLIN->m_eDirection = DIR_RX;
            CurrDataLIN->m_acMsgDir[0] = _T('R');
        }
        else if (TX_FLAG == pMsgLIN->m_ucDataType)
        {
            CurrDataLIN->m_eDirection = DIR_TX;
            CurrDataLIN->m_acMsgDir[0] = _T('T');
        }
        CurrDataLIN->m_acMsgDir[1] = _T('x');

        TYPE_CHANNEL CurrChannel = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChannel;  // Assuming default LIN msg

        if ((CurrChannel >= CHANNEL_LIN_MIN) && (CurrChannel <= CHANNEL_LIN_MAX ))
        {
            sprintf_s(CurrDataLIN->m_acChannel, "%d", CurrChannel);
        }

        memset(CurrDataLIN->m_acType,'\0',sizeof(CurrDataLIN->m_acType));

        CurrDataLIN->m_eEventType = EVENT_LIN_NONE;
        ///* Validate message type */
        //pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgTyp
        //   {
        //       CurrDataLIN->m_byIDType = TYPE_ID_LIN_STANDARD;
        //       strcpy_s(CurrDataLIN->m_acType, LENGTH_STR_DESCRIPTION_LIN, _("s"));
        //   }

        //sprintf_s(CurrDataLIN->m_acMsgType, "%s", sg_ListDIL_MSG_TYPE[pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgTyp]);
        //strcpy_s(CurrDataLIN->m_acType, LENGTH_STR_MSGTYPE_LIN, sg_ListDIL_MSG_TYPE[pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgTyp].c_str());
        /* If it is a LIN FD frame */
        //if ( pMsgLIN->m_bLINFDMsg )
        //{
        //    _itoa_s(pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucDataLen, CurrDataLIN->m_acDataLen, 10);
        //    strcpy_s(CurrDataLIN->m_acMsgDesc, LENGTH_STR_DESCRIPTION_LIN, "Description");
        //    CurrDataLIN->m_u64TimeStamp = pMsgLIN->m_lTickCount.QuadPart;
        //    CurrDataLIN->m_dwMsgID = pMsgLIN->m_uDataInfo.m_sLINMsg.m_unMsgID;
        //    CurrDataLIN->m_byDataLength = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucDataLen;

        //    memcpy(CurrDataLIN->m_abData, pMsgLIN->m_uDataInfo.m_sLINMsg.m_aucLINFDData,
        //           CurrDataLIN->m_byDataLength);
        //}
        //else
        {
            _itoa_s(pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucDataLen, CurrDataLIN->m_acDataLen, 10);
            strcpy_s(CurrDataLIN->m_acMsgDesc, LENGTH_STR_DESCRIPTION_LIN, "Description");
            CurrDataLIN->m_u64TimeStamp = pMsgLIN->m_lTickCount.QuadPart;
            CurrDataLIN->m_dwMsgID = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgID;
            CurrDataLIN->m_byDataLength = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucDataLen;
            CurrDataLIN->m_byChecksum=pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum;
            strcpy(CurrDataLIN->m_acType, "LIN Message");
            if ( CurrDataLIN->m_dwMsgID ==0x3c || CurrDataLIN->m_dwMsgID == 0x3D )
            {
                strcpy(CurrDataLIN->m_acType, "Diagnostic Message");
            }

            string str = "Classic";
            if ( pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksumTyp == LIN_CHECKSUM_ENHANCED )
            {
                str = "Enhanced";
            }

            CurrDataLIN->m_byChecksumType=pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksumTyp;
            if (IS_NUM_HEX_SET(bExprnFlag_Log))
            {
                sprintf(CurrDataLIN->m_acChecksum, "0x%X \(\"%s\"\)"/*FORMAT_STR_DATA_HEX*/, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum, str.c_str());
            }

            if (IS_NUM_DEC_SET(bExprnFlag_Log))
            {
                /*sprintf(CurrDataLIN->m_acChecksum, "%x", pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum);*/
                //_stprintf(&(CurrDataLIN->m_acChecksum), FORMAT_STR_DATA_DEC, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum);
                sprintf(CurrDataLIN->m_acChecksum, "%03d \(\"%s\"\)", pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum, str.c_str());
            }

            memcpy(CurrDataLIN->m_abData, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucData,
                   CurrDataLIN->m_byDataLength);
        }


    }
    else if ( pMsgLIN->m_eLinMsgType == LIN_EVENT )
    {
        /*PROCESS ERROR MSGS: If Error Message type. Change the data and type fields. */

        string strText;
        vGetLinEventDescription(pMsgLIN->m_uDataInfo.m_sErrInfo, strText);

        strcpy(CurrDataLIN->m_acType, strText.c_str());

        CurrDataLIN->m_eEventType = pMsgLIN->m_uDataInfo.m_sErrInfo.m_eEventType;

        strcpy(CurrDataLIN->m_acDataLen,  "");
        strcpy(CurrDataLIN->m_acMsgDesc,  "");
        strcpy(CurrDataLIN->m_acChecksum,  "");
        strcpy(CurrDataLIN->m_acMsgIDHex,  "");
        strcpy(CurrDataLIN->m_acMsgIDDec,  "");
        strcpy(CurrDataLIN->m_acDataHex,  "");
        strcpy(CurrDataLIN->m_acDataDec,  "");

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

    }
    vFormatTime(bExprnFlag_Log, CurrDataLIN);
    vFormatDataAndId(bExprnFlag_Log, CurrDataLIN);
    /* PROCESS ERROR MSGS ENDS */


}




void CFormatMsgLIN::vGetLinEventDescription(SERROR_INFO_LIN sLinErrorInfo, string& strDesc)
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
    return;
}
