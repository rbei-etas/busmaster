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
 * \file      GUI_FormatMsgCAN.cpp
 * \brief     Implementation of CFormatMsgEthernet class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CFormatMsgEthernet class
 */


#include "CommonClass_stdafx.h"
#include "GUI_FormatMsgEthernet.h"
#include "include/Utils_macro.h"
//#include "Include/CAN_Error_Defs.h"
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


//static SERRORMSGINFO sg_asErrorEntry[ERRORS_DEFINED] =
//{
//    {ERROR_UNKNOWN,       "Unknown Error"},
//    {BIT_ERROR_RX,        "Bus Error - Bit Error(Rx)"},
//    {CRC_ERROR_RX,        "Bus Error - CRC Error(Rx)"},
//    {FORM_ERROR_RX,       "Bus Error - Form Error(Rx)"},
//    {STUFF_ERROR_RX,      "Error Frame - Stuff error(Rx)"},
//    {OTHER_ERROR_RX,      "Bus Error - Other Error(Rx)"},
//    {BIT_ERROR_TX,        "Bus Error - Bit Error(Tx)"},
//    {CRC_ERROR_TX,        "Bus Error - CRC Error(Tx)"},
//    {FORM_ERROR_TX,       "Bus Error - Form Error(Tx)"},
//    {STUFF_ERROR_TX,      "Bus Error - Stuff Error(Tx)"},
//    {OTHER_ERROR_TX,      "Bus Error - Other Error(Tx)"},
//    {ERROR_DEVICE_BUFF_OVERFLOW,        "Controller Buffer Overflow"},
//    {ERROR_DRIVER_BUFF_OVERFLOW,        "Driver Buffer Overflow"},
//    {ERROR_APPLICATION_BUFF_OVERFLOW,   "Application Buffer Overflow"},
//    {ERROR_DLLMSG_BUFF_OVERFLOW,        "DLL message buffer Overflow"}
//};

/*******************************************************************************
  Function Name  : CFormatMsgEthernet
  Input(s)       : -
  Output         : -
  Functionality  : Constructor
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  :
*******************************************************************************/
CFormatMsgEthernet::CFormatMsgEthernet(void)
{
}

/*******************************************************************************
  Function Name  : ~CFormatMsgEthernet
  Input(s)       : -
  Output         : -
  Functionality  : Destructor
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  :
*******************************************************************************/
CFormatMsgEthernet::~CFormatMsgEthernet(void)
{
}

/*******************************************************************************
  Function Name  : vFormatTime
  Input(s)       : -
  Output         : -
  Functionality  : Format Time
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  :
*******************************************************************************/
void CFormatMsgEthernet::vFormatTime(BYTE bExprnFlag, SFORMATTEDDATA_ETHERNET* CurrDataEthernet)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataEthernet->m_u64TimeStamp,
                              CurrDataEthernet->m_acTimeAbs);
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataEthernet->m_u64TimeStamp,
                              CurrDataEthernet->m_acTimeRel);
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataEthernet->m_u64TimeStamp,
                              CurrDataEthernet->m_acTimeSys);
    }
}


/*******************************************************************************
  Function Name  : vFormatDataAndId
  Input(s)       : -
  Output         : -
  Functionality  : Format data and id
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  :
*******************************************************************************/
void CFormatMsgEthernet::vFormatDataAndId(BYTE bExprnFlag, SFORMATTEDDATA_ETHERNET* CurrDataEthernet)
{
   if (IS_NUM_HEX_SET(bExprnFlag))
    {
        strcpy(CurrDataEthernet->m_acMsgIDHex, "");
        if ( CurrDataEthernet->m_dwMsgID != -1 )
        {
            sprintf_s(CurrDataEthernet->m_acMsgIDHex, FORMAT_STR_ID_HEX, CurrDataEthernet->m_dwMsgID);
        }
        int j = 0;  // j declared outside

        for (int i = 0; i < CurrDataEthernet->m_nDataLength; i++)
        {
            BYTE CurrDat = CurrDataEthernet->m_abData[i];
            _stprintf(&(CurrDataEthernet->m_acDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        CurrDataEthernet->m_acDataHex[j] = L'\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        strcpy(CurrDataEthernet->m_acMsgIDDec, "");
        if ( CurrDataEthernet->m_dwMsgID != -1 )
        {
            sprintf_s(CurrDataEthernet->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataEthernet->m_dwMsgID);
        }
        int j = 0;

        for (int i = 0; i < CurrDataEthernet->m_nDataLength; i++)
        {
            BYTE CurrDat = CurrDataEthernet->m_abData[i];
            _stprintf(&(CurrDataEthernet->m_acDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            j += 4;
            CurrDataEthernet->m_acDataDec[j-1] = L' ';
        }

        CurrDataEthernet->m_acDataDec[j-1] = L'\0';
    }
}

/*******************************************************************************
  Function Name  : usProcessCurrErrorEntry
  Input(s)       : SERROR_INFO& sErrorInfo
  Output         : Returns Error ID as USHORT.
  Functionality  : Processes the current Error entry and returns the Error code.
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  :
*******************************************************************************/
USHORT CFormatMsgEthernet:: usProcessCurrErrorEntryEthernet(sETHERNET_ERROR_INFO& sErrorInfo)
{
   
    return NULL;
}

/*******************************************************************************
  Function Name  : vFormatCurrErrorEntry
  Input(s)       : usErrorID
  Output         : TCHAR* (Error Name)
  Functionality  : Gets the pointer to Message data Buffer from PSDI_CAN DLL
                   and updates the same in List Control.
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  :
*******************************************************************************/
char* CFormatMsgEthernet::vFormatCurrErrorEntry(USHORT usErrorID)
{
    

    return NULL;
}

/*******************************************************************************
  Function Name  : CFormatMsgEthernet
  Input(s)       : -
  Output         : -
  Functionality  : Format Ethernet data bytes
  Member of      : CFormatMsgEthernet
  Author(s)      : Ashwin R Uchil
  Date Created   : 5.6.2014
  Modifications  : 
*******************************************************************************/
void CFormatMsgEthernet::vFormatEthernetDataMsg(STETHERNETDATA* pMsgEthernet,
                           SFORMATTEDDATA_ETHERNET* CurrDataEthernet,
                           BYTE bExprnFlag_Log)
{
    if (RX_FLAG == pMsgEthernet->m_ucDataType)
    {
        CurrDataEthernet->m_eDirection = DIR_RX;
        CurrDataEthernet->m_acMsgDir[0] = _T('R');
    }
    else if (TX_FLAG == pMsgEthernet->m_ucDataType)
    {
        CurrDataEthernet->m_eDirection = DIR_TX;
        CurrDataEthernet->m_acMsgDir[0] = _T('T');
    }
    CurrDataEthernet->m_acMsgDir[1] = _T('x');

    //TYPE_CHANNEL CurrChannel = pMsgEthernet->m_uDataInfo.m_sCANMsg.m_ucChannel;  // Assuming default CAN msg
    //if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_bCANFD) // Incase of CANFD msg
    //{
    //    CurrChannel = pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucChannel;
    //}

    //if ((CurrChannel >= CHANNEL_CAN_MIN) && (CurrChannel <= CHANNEL_CAN_MAX ))
	//{
	//    sprintf_s(CurrDataCAN->m_acChannel, "%d", CurrChannel);
	//}

	char chSrcMAC[12], chDestMAC[12];
	_itoa_s(pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_unDataLen, CurrDataEthernet->m_acDataLen, 10);

	memcpy(chSrcMAC, (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucSourceMAC, 12);
	sprintf(CurrDataEthernet->m_SourceMAC, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c\0",
		chSrcMAC[0],chSrcMAC[1],chSrcMAC[2],chSrcMAC[3],chSrcMAC[4],chSrcMAC[5],chSrcMAC[6],chSrcMAC[7],chSrcMAC[8],chSrcMAC[9],chSrcMAC[10],chSrcMAC[11]);

	memcpy(chDestMAC, (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucDestMac, 12);
	sprintf(CurrDataEthernet->m_DestMAC, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c\0",
		chDestMAC[0],chDestMAC[1],chDestMAC[2],chDestMAC[3],chDestMAC[4],chDestMAC[5],chDestMAC[6],chDestMAC[7],chDestMAC[8],chDestMAC[9],chDestMAC[10],chDestMAC[11]);
	
	//strcpy(CurrDataEthernet->m_DestMAC, (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucDestMac);
	memcpy(CurrDataEthernet->m_SourceIP, (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucSourceIP, 15);
	memcpy(CurrDataEthernet->m_DestIP,  (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucDestIP, 15);
	
	memcpy(CurrDataEthernet->m_acMsgDesc, (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucName, 100);

	//strcpy_s(CurrDataEthernet->m_acMsgDesc, LENGTH_STR_DESCRIPTION_CAN, (char*)pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucName);
	CurrDataEthernet->m_u64TimeStamp = pMsgEthernet->m_lTickCount.QuadPart;
	CurrDataEthernet->m_dwMsgID = pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_unMsgID;
	CurrDataEthernet->m_nDataLength = pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_unDataLen;

	memcpy(CurrDataEthernet->m_abData, pMsgEthernet->m_uDataInfo.m_sETHERNETMsg.m_ucData,
		CurrDataEthernet->m_nDataLength);


	/*PROCESS ERROR MSGS: If Error Message type. Change the data and type fields. */
	//if(ERR_FLAG == pMsgCAN->m_ucDataType)
	//{
	//    USHORT usErrCode = usProcessCurrErrorEntry(pMsgCAN->m_uDataInfo.m_sErrInfo);

    //    if( usErrCode != ERROR_UNKNOWN )
    //    {
    //        // Format error message
    //        char* ptrStrErrName = NULL;
    //        ptrStrErrName = vFormatCurrErrorEntry(usErrCode);

    //        if(ptrStrErrName)
    //        {
    //            strcpy_s(CurrDataCAN->m_acDataDec, LENGTH_STR_DATA_CAN, ptrStrErrName);
    //            strcpy_s(CurrDataCAN->m_acDataHex, LENGTH_STR_DATA_CAN, ptrStrErrName);
    //        }
    //    }

    //    CurrDataCAN->m_dwMsgID = pMsgCAN->m_uDataInfo.m_sCANMsg.m_unMsgID;
    //    sprintf_s(CurrDataCAN->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataCAN->m_dwMsgID);
    //    strcpy_s(CurrDataCAN->m_acType, LENGTH_STR_TYPE_CAN, "ERR");
    //}

    /* PROCESS ERROR MSGS ENDS */
    vFormatTime(bExprnFlag_Log, CurrDataEthernet);
    vFormatDataAndId(bExprnFlag_Log, CurrDataEthernet);
}