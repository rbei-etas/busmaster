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
 * \author    Anish Kumar
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
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
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
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
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
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  :
*******************************************************************************/
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
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  :
*******************************************************************************/
void CFormatMsgLIN::vFormatDataAndId(BYTE bExprnFlag,
                                     SFORMATTEDDATA_LIN* CurrDataLIN)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        sprintf_s(CurrDataLIN->m_acMsgIDHex, FORMAT_STR_ID_HEX, CurrDataLIN->m_dwMsgID);

        {
            int j = 0;  // j declared outside

            for (int i = 0; i < CurrDataLIN->m_byDataLength; i++)
            {
                BYTE CurrDat = CurrDataLIN->m_abData[i];
                _stprintf(&(CurrDataLIN->m_acDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
                j += 3;
            }
            CurrDataLIN->m_acDataHex[j] = L'\0';
        }
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        sprintf_s(CurrDataLIN->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataLIN->m_dwMsgID);

        {
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

    if (sErrorInfo.m_ucErrType == ERROR_BUS)
    {
        // Update Statistics information
        usErrorID = sErrorInfo.m_ucReg_ErrCap /*& 0xE0*/;
    }
    else if (sErrorInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        usErrorID = ERROR_UNKNOWN;
    }
    else if (sErrorInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        usErrorID = ERROR_UNKNOWN;
    }
    else
    {
        usErrorID = sErrorInfo.m_ucErrType;
    }

    return usErrorID;
}

/*******************************************************************************
  Function Name  : vFormatCurrErrorEntry
  Input(s)       : usErrorID
  Output         : TCHAR* (Error Name)
  Functionality  : Gets the pointer to Message data Buffer from PSDI_LIN DLL
                   and updates the same in List Control.
  Member of      : CFormatMsgLIN
  Author(s)      : Arunkumar K
  Date Created   : 08-09-2010
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
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  : ArunKumar K
                   08.09.2010,
                   Added handler for error messages. ERR_FLAG
*******************************************************************************/
void CFormatMsgLIN::vFormatLINDataMsg(STLINDATA* pMsgLIN,
                                      SFORMATTEDDATA_LIN* CurrDataLIN,
                                      BYTE bExprnFlag_Log)
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

    ///* Validate message type */
    //pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgTyp
    //   {
    //       CurrDataLIN->m_byIDType = TYPE_ID_LIN_STANDARD;
    //       strcpy_s(CurrDataLIN->m_acType, LENGTH_STR_DESCRIPTION_LIN, _("s"));
    //   }

    //sprintf_s(CurrDataLIN->m_acMsgType, "%s", sg_ListDIL_MSG_TYPE[pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgTyp]);
    strcpy_s(CurrDataLIN->m_acType, LENGTH_STR_MSGTYPE_LIN, sg_ListDIL_MSG_TYPE[pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgTyp].c_str());
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

        if (IS_NUM_HEX_SET(bExprnFlag_Log))
        {
            sprintf(CurrDataLIN->m_acChecksum, "0x%X"/*FORMAT_STR_DATA_HEX*/, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum);
        }

        if (IS_NUM_DEC_SET(bExprnFlag_Log))
        {
            /*sprintf(CurrDataLIN->m_acChecksum, "%x", pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum);*/
            //_stprintf(&(CurrDataLIN->m_acChecksum), FORMAT_STR_DATA_DEC, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum);
            sprintf(CurrDataLIN->m_acChecksum, FORMAT_STR_DATA_DEC, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChksum);
        }

        memcpy(CurrDataLIN->m_abData, pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucData,
               CurrDataLIN->m_byDataLength);
    }

    /*PROCESS ERROR MSGS: If Error Message type. Change the data and type fields. */
    if(ERR_FLAG == pMsgLIN->m_ucDataType)
    {
        USHORT usErrCode = usProcessCurrErrorEntryLin(pMsgLIN->m_uDataInfo.m_sErrInfo);

        if( usErrCode != ERROR_UNKNOWN )
        {
            // Format error message
            char* ptrStrErrName = NULL;
            ptrStrErrName = vFormatCurrErrorEntry(usErrCode);

            if(ptrStrErrName)
            {
                strcpy_s(CurrDataLIN->m_acDataDec, LENGTH_STR_DATA_LIN, ptrStrErrName);
                strcpy_s(CurrDataLIN->m_acDataHex, LENGTH_STR_DATA_LIN, ptrStrErrName);
            }
        }

        CurrDataLIN->m_dwMsgID = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgID;
        sprintf_s(CurrDataLIN->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataLIN->m_dwMsgID);
        strcpy_s(CurrDataLIN->m_acType, LENGTH_STR_TYPE_LIN, "ERR");
    }

    /* PROCESS ERROR MSGS ENDS */
    vFormatTime(bExprnFlag_Log, CurrDataLIN);
    vFormatDataAndId(bExprnFlag_Log, CurrDataLIN);
}