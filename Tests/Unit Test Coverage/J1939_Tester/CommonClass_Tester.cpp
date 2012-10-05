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
 * \file      J1939_Tester.cpp
 * \brief     Source file for J1939_Tester
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CFrameProcessor_Common class realising the
 */


// J1939_Tester.cpp : Defines the initialization routines for the DLL.
//

#include <CommonClass_Tester_StdAfx.h>
#include <tchar.h>
//---------------------------------------------------------------------------

#pragma argsused

//---------------------------------------------------------------------------

#define BOOST_TEST_MODULE MyTest
#define USAGE_IMPORT 1
#include <boost/test/unit_test.hpp>

#include "CommonClass/CommonClass_stdafx.h"
#include "CommonClass/GUI_FormatMsgCAN.h"
#include "CommonClass/GUI_FormatMsgCommon.h"
#include "Include/Struct_CAN.h"
#include "CommonClass/GUI_FormatMsgJ1939.h"
#include "include/Utils_macro.h"

CFormatMsgCAN *pCFormatMsgCan = new CFormatMsgCAN();
CFormatMsgCAN objCFormatMsgCan;
CRefTimeKeeper pCRefTimeKeeper;
CFormatMsgJ1939 *pCFormatMsgJ1939 = new CFormatMsgJ1939();


BOOST_AUTO_TEST_SUITE( Format_Msg_Tester )

BOOST_AUTO_TEST_CASE( Format_CANMSG_Tester )
{
	// Formatting Standard Tx Message
	STCANDATA sStandardCanMsg;
	SFORMATTEDDATA_CAN sBasicSCanInfo;
	
	strcpy_s(sBasicSCanInfo.m_acTimeAbs,  sizeof(sBasicSCanInfo.m_acTimeAbs),  "");

	sBasicSCanInfo.m_u64TimeStamp = 0;
	
	sStandardCanMsg.m_ucDataType = TX_FLAG;
	sStandardCanMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 0;
	sStandardCanMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = 0;
	sStandardCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = 100;
	sStandardCanMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = 8;
	sStandardCanMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
	
	for(INT i = 0;i < 8;i++)
	{
		sStandardCanMsg.m_uDataInfo.m_sCANMsg.m_ucData[i] = 'FF';
	}

	pCFormatMsgCan->vFormatCANDataMsg(&sStandardCanMsg, &sBasicSCanInfo, FALSE);

	// Formatting CAN message with Absolute time stamp
	objCFormatMsgCan.vFormatCANDataMsg(&sStandardCanMsg, &sBasicSCanInfo, BIT_TM_ABS);

	// Formatting CAN message with Relative time stamp
	objCFormatMsgCan.vFormatCANDataMsg(&sStandardCanMsg, &sBasicSCanInfo, BIT_TM_REL);

	// Formatting CAN message with System time stamp
	objCFormatMsgCan.vFormatCANDataMsg(&sStandardCanMsg, &sBasicSCanInfo, BIT_TM_SYS);

	// Formatting Standard Tx Message End

	// Formatting Extended Tx Message
	STCANDATA sExtndCanMsg;
	SFORMATTEDDATA_CAN sBasicECanInfo;

	strcpy_s(sBasicECanInfo.m_acTimeAbs,  sizeof(sBasicECanInfo.m_acTimeAbs),  "");
	strcpy_s(sBasicECanInfo.m_acTimeRel,  sizeof(sBasicECanInfo.m_acTimeRel),  "");
	strcpy_s(sBasicECanInfo.m_acTimeSys,  sizeof(sBasicECanInfo.m_acTimeSys),  "");

	sBasicECanInfo.m_u64TimeStamp = 0;

	sExtndCanMsg.m_ucDataType = TX_FLAG;
	sExtndCanMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 1;
	sExtndCanMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = 1;
	sExtndCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = 100;
	sExtndCanMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = 8;
	sExtndCanMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
	
	for(INT i = 0;i < 8;i++)
	{
		sExtndCanMsg.m_uDataInfo.m_sCANMsg.m_ucData[i] = '00';
	}

	pCFormatMsgCan->vFormatCANDataMsg(&sExtndCanMsg, &sBasicECanInfo, FALSE);

	pCFormatMsgCan->vFormatCANDataMsg(&sExtndCanMsg, &sBasicECanInfo, BIT_TM_ABS);
	// Formatting Extended Tx Message End

	// Formatting Standard Rx Message
	STCANDATA sStandardRCanMsg;
	SFORMATTEDDATA_CAN sBasicSRCanInfo;

	sStandardRCanMsg.m_ucDataType = RX_FLAG;
	sStandardRCanMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 0;
	sStandardRCanMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = 0;
	sStandardRCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = 101;
	sStandardRCanMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = 6;
	sStandardRCanMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
	
	for(INT i = 0;i < 8;i++)
	{
		sStandardRCanMsg.m_uDataInfo.m_sCANMsg.m_ucData[i] = 'FF';
	}

	strcpy_s(sBasicSRCanInfo.m_acTimeRel,  sizeof(sBasicSRCanInfo.m_acTimeRel),  "");

	sBasicSRCanInfo.m_u64TimeStamp = 0;

	pCFormatMsgCan->vFormatCANDataMsg(&sStandardRCanMsg, &sBasicSRCanInfo, FALSE);

	pCFormatMsgCan->vFormatCANDataMsg(&sStandardRCanMsg, &sBasicSRCanInfo, BIT_TM_ABS);

	// Extended Rx Message
	STCANDATA sExtndRxCanMsg;
	SFORMATTEDDATA_CAN sBasicERCanInfo;

	sExtndRxCanMsg.m_ucDataType = RX_FLAG;
	sExtndRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 1;
	sExtndRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = 1;
	sExtndRxCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = 102;
	sExtndRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = 4;
	sExtndRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = 2;
	
	for(INT i = 0;i < 8;i++)
	{
		sExtndRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucData[i] = 'CC';
	}

	strcpy_s(sBasicERCanInfo.m_acTimeAbs,  sizeof(sBasicERCanInfo.m_acTimeAbs),  "");

	sBasicERCanInfo.m_u64TimeStamp = 0;

	pCFormatMsgCan->vFormatCANDataMsg(&sExtndRxCanMsg, &sBasicERCanInfo, FALSE);

	pCFormatMsgCan->vFormatCANDataMsg(&sExtndRxCanMsg, &sBasicERCanInfo, BIT_TM_ABS);
	// Formatting Standard Rx Message End

	// Formatting Error Flag
	STCANDATA sStndErrRxCanMsg;
	SFORMATTEDDATA_CAN sBasicSERCanInfo;

	sStndErrRxCanMsg.m_ucDataType = ERR_FLAG;
	sStndErrRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 0;
	sStndErrRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = 0;
	sStndErrRxCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = 103;
	sStndErrRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = 8;
	sStndErrRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
	
	for(INT i = 0;i < 8;i++)
	{
		sStndErrRxCanMsg.m_uDataInfo.m_sCANMsg.m_ucData[i] = 'AA';
	}

	strcpy_s(sBasicSERCanInfo.m_acTimeAbs,  sizeof(sBasicSERCanInfo.m_acTimeAbs),  "");

	sBasicSERCanInfo.m_u64TimeStamp = 0;

	pCFormatMsgCan->vFormatCANDataMsg(&sStndErrRxCanMsg, &sBasicSERCanInfo, FALSE);

	pCFormatMsgCan->vFormatCANDataMsg(&sStndErrRxCanMsg, &sBasicSERCanInfo, BIT_TM_ABS);
	// Formatting Error Flag End

	if(pCFormatMsgCan != NULL)
	{
		delete pCFormatMsgCan;
	}
}

BOOST_AUTO_TEST_CASE( Format_J1939MSG_Tester )
{
	STJ1939_MSG  sJ1939Data;
	SFORMATTEDATA_J1939 sCurrFormatDat;
	INT Length;

	// Formatting Extended Tx message
	sJ1939Data.m_unDLC = 20;
	sJ1939Data.m_pbyData = new BYTE[sJ1939Data.m_unDLC];// For basic data object
    ASSERT(NULL != sJ1939Data.m_pbyData);

	sJ1939Data.m_sMsgProperties.m_byChannel = 1;
	sJ1939Data.m_sMsgProperties.m_eDirection = DIR_TX;
	sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = 0x15;
	sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress = 0x5;


	sCurrFormatDat.m_pcDataHex = new char[Length];
    ASSERT(NULL != m_sCurrFormatDat.m_pcDataHex);
    memset(sCurrFormatDat.m_pcDataHex, '\0', Length * sizeof(char));

	Length = (MAX_DATA_LEN_J1939 * 4 + 1);
    
    sCurrFormatDat.m_pcDataDec = new char[Length];
    ASSERT(NULL != sCurrFormatDat.m_pcDataDec);
    memset(sCurrFormatDat.m_pcDataDec, '\0', Length * sizeof(char));

	pCFormatMsgJ1939->vFormatJ1939DataMsg(&sJ1939Data, 
                                          &sCurrFormatDat, FALSE);

	pCFormatMsgJ1939->vFormatJ1939DataMsg(&sJ1939Data, 
                                          &sCurrFormatDat, TRUE);
	// Formatting Extended Tx message End

	// Formatting Extended Rx message
	sJ1939Data.m_unDLC = 60;
	sJ1939Data.m_pbyData = new BYTE[sJ1939Data.m_unDLC];// For basic data object
    ASSERT(NULL != sJ1939Data.m_pbyData);

	sJ1939Data.m_sMsgProperties.m_byChannel = 1;
	sJ1939Data.m_sMsgProperties.m_eDirection = DIR_RX;
	sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = 0x20;
	sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress = 0x2;


	sCurrFormatDat.m_pcDataHex = new char[Length];
    ASSERT(NULL != m_sCurrFormatDat.m_pcDataHex);
    memset(sCurrFormatDat.m_pcDataHex, '\0', Length * sizeof(char));

	Length = (MAX_DATA_LEN_J1939 * 4 + 1);
    
    sCurrFormatDat.m_pcDataDec = new char[Length];
    ASSERT(NULL != sCurrFormatDat.m_pcDataDec);
    memset(sCurrFormatDat.m_pcDataDec, '\0', Length * sizeof(char));

	pCFormatMsgJ1939->vFormatJ1939DataMsg(&sJ1939Data, 
                                          &sCurrFormatDat, TRUE);

	pCFormatMsgJ1939->vFormatJ1939DataMsg(&sJ1939Data, 
                                          &sCurrFormatDat, FALSE);
	// Formatting Extended Rx message End

	sJ1939Data.m_unDLC = 200;
	sJ1939Data.m_pbyData = new BYTE[sJ1939Data.m_unDLC];// For basic data object
    ASSERT(NULL != sJ1939Data.m_pbyData);

	sJ1939Data.m_sMsgProperties.m_byChannel = 1;
	sJ1939Data.m_sMsgProperties.m_eDirection = DIR_RX;
	sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = 0x20;
	sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress = 0x2;


	sCurrFormatDat.m_pcDataHex = new char[Length];
    ASSERT(NULL != m_sCurrFormatDat.m_pcDataHex);
    memset(sCurrFormatDat.m_pcDataHex, '\0', Length * sizeof(char));

	Length = (MAX_DATA_LEN_J1939 * 4 + 1);
    
    sCurrFormatDat.m_pcDataDec = new char[Length];
    ASSERT(NULL != sCurrFormatDat.m_pcDataDec);
    memset(sCurrFormatDat.m_pcDataDec, '\0', Length * sizeof(char));

	strcpy_s(sCurrFormatDat.m_acTimeAbs,  sizeof(sCurrFormatDat.m_acTimeAbs),  "");

	pCFormatMsgJ1939->vFormatJ1939DataMsg(&sJ1939Data, 
                                          &sCurrFormatDat, TRUE);

	pCFormatMsgJ1939->vFormatJ1939DataMsg(&sJ1939Data, 
                                          &sCurrFormatDat, FALSE);

	if(pCFormatMsgJ1939 != NULL)
	{
		delete pCFormatMsgJ1939;
	}

}

BOOST_AUTO_TEST_CASE( REFTIME_Tester)
{
	// Setting Time Params
	UINT64 qwTimeStamp = 0;

	SYSTEMTIME CurrSysTime, systemTime;
    UINT64 unAbsTime;

	GetSystemTime(&systemTime);

	memcpy(&CurrSysTime, &systemTime, sizeof(SYSTEMTIME));

	// Set timing params
	pCRefTimeKeeper.vSetTimeParams(CurrSysTime, qwTimeStamp);
}

BOOST_AUTO_TEST_SUITE_END()