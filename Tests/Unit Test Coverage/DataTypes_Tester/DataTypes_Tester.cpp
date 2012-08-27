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

#include <J1939_Tester_StdAfx.h>
#include <tchar.h>
//---------------------------------------------------------------------------

#pragma argsused

//---------------------------------------------------------------------------

#define BOOST_TEST_MODULE MyTest
#define USAGE_IMPORT 1
#include <boost/test/unit_test.hpp>



//#include "DataTypes/BaseAppServices.h"
//#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/BaseMsgBufAll.h"
//#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "DataTypes/J1939_DataTypes.h"
//#include "DataTypes/KeyPanel_Datatypes.h"
#include "DataTypes/Log_Datatypes.h"
#include "DataTypes/MainSubEntry.h"
#include "DataTypes/MsgBufCANVFSE.h"
//#include "DataTypes/MsgBufFSE.h"
//#include "DataTypes/MsgBufVFSE.h"
#include "DataTypes/MsgBufVSE.h"
//#include "DataTypes/MsgBufVVSE.h"
//#include "DataTypes/MsgSignal_Datatypes.h"
//#include "DataTypes/ProjConfig_DataTypes.h"
//#include "DataTypes/SigWatch_Datatypes.h"
//#include "DataTypes/SigGrphWnd_Datatypes.h"


const int SIZE_CHAR = sizeof(char);
BOOST_AUTO_TEST_SUITE( DataTypes_Tester )


BOOST_AUTO_TEST_CASE( SFILTER_CAN_GetSize_Test )
{
	PSFILTER_CAN		psFilter_Can = new SFILTER_CAN;
	UINT uSize = psFilter_Can->unGetSize();
	delete psFilter_Can;
}

BOOST_AUTO_TEST_CASE( pbGetConfigData_CAN_Test )
{
	//CMainFrame		*pMainFrame = new CMainFrame;
	
	BYTE* pbyResult = NULL;
	SFILTER_CAN		sFilter_Can /*= new SFILTER_CAN*/;
	int nSize = sFilter_Can.unGetSize();
	BYTE* pbyCurrData   = new BYTE[nSize];
	sFilter_Can.m_byIDType = true;
	sFilter_Can.m_byMsgType = true;
	sFilter_Can.m_eChannel = 0;
	sFilter_Can.m_dwMsgIDTo = 123;
	sFilter_Can.m_eDrctn = DIR_TX;

	pbyResult = sFilter_Can.pbGetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( pbSetConfigData_CAN_Test )
{
	//CMainFrame		*pMainFrame = new CMainFrame;
	
	BYTE* pbyResult = NULL;
	SFILTER_CAN		sFilter_Can /*= new SFILTER_CAN*/;
	int nSize = sFilter_Can.unGetSize();
	BYTE* pbyCurrData   = new BYTE[nSize];

	sFilter_Can.m_byIDType = true;
	sFilter_Can.m_byMsgType = true;
	sFilter_Can.m_eChannel = 0;
	sFilter_Can.m_dwMsgIDTo = 123;
	sFilter_Can.m_eDrctn = DIR_TX;

	pbyResult = sFilter_Can.pbSetConfigData(pbyCurrData);
	//delete pbyCurrData;
}




BOOST_AUTO_TEST_CASE( bDoesFrameOccur_Basic_CAN_Test )
{
	SFRAMEINFO_BASIC_CAN		*psCurrFrame = new SFRAMEINFO_BASIC_CAN;

	psCurrFrame->m_dwFrameID = 12;
	psCurrFrame->m_eChannel =TYPE_ID_CAN_ALL;
	psCurrFrame->m_eDrctn = DIR_ALL;  


	PSFILTER_CAN		psFilter_Can = new SFILTER_CAN;
	psFilter_Can->m_dwMsgIDFrom = 12;
	psFilter_Can->m_eChannel =TYPE_ID_CAN_ALL;
	psFilter_Can->m_eDrctn = DIR_ALL;  
	psFilter_Can->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
	psFilter_Can->m_byIDType = TYPE_ID_CAN_ALL;
	psFilter_Can->m_byMsgType = TYPE_MSG_CAN_ALL;

	bool bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame);

	psFilter_Can->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame);

	psCurrFrame->m_eChannel = TYPE_ID_CAN_EXTENDED;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 
	psFilter_Can->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 

	psCurrFrame->m_eDrctn = DIR_TX; 
	psCurrFrame->m_eChannel = TYPE_ID_CAN_EXTENDED;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 
	psFilter_Can->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 

	psFilter_Can->m_byMsgType = TYPE_MSG_CAN_NON_RTR;
	psCurrFrame->m_eChannel = TYPE_ID_CAN_EXTENDED;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 
	psFilter_Can->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 

	psCurrFrame->m_dwFrameID = 13;
	psCurrFrame->m_eChannel = TYPE_ID_CAN_EXTENDED;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 
	psFilter_Can->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame); 

	psFilter_Can->m_ucFilterType = 3;
	bOccurs = psFilter_Can->bDoesFrameOccur(psCurrFrame);

	delete psFilter_Can;
}

BOOST_AUTO_TEST_CASE( bDoesFrameOccur_SFRAMEINFO_BASIC_FLEXRAY_Test )
{
	SFRAMEINFO_BASIC_FLEXRAY *psCurrFrame = new SFRAMEINFO_BASIC_FLEXRAY;
	psCurrFrame->m_dwFrameID = 12;
	psCurrFrame->m_eChannel =TYPE_ID_CAN_ALL;
	psCurrFrame->m_eDrctn = DIR_ALL;  

	PSFILTER_FLEXRAY		psFilter_Flexray = new SFILTER_FLEXRAY;
	psFilter_Flexray->m_dwMsgIDFrom = 12;
	psFilter_Flexray->m_dwMsgIDTo = 15;
	psFilter_Flexray->m_eChannel =TYPE_ID_CAN_ALL;
	psFilter_Flexray->m_eDrctn = DIR_ALL;  
	psFilter_Flexray->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
	bool bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);

	psFilter_Flexray->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);

	psFilter_Flexray->m_eDrctn = DIR_TX;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);
	psFilter_Flexray->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);

	psFilter_Flexray->m_eChannel =TYPE_ID_CAN_EXTENDED;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);
	psFilter_Flexray->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);

	psCurrFrame->m_dwFrameID = 24;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);
	psCurrFrame->m_dwFrameID = 2;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);
	psFilter_Flexray->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);

	psFilter_Flexray->m_ucFilterType = 3;
	bOccurs = psFilter_Flexray->bDoesFrameOccur(psCurrFrame);

	//delete psCurrFrame;
	delete psFilter_Flexray;
}

//BOOST_AUTO_TEST_CASE( SFILTER_FLEXRAY_GetSize_Test )
//{
//	SFILTER_FLEXRAY		sFilter_FlexRay ;
//	UINT uSize = sFilter_FlexRay.unGetSize();
//}

BOOST_AUTO_TEST_CASE( pbGetConfigData_FlexRay_Test )
{
	
	
	BYTE* pbyResult = NULL;
	SFILTER_FLEXRAY		sFilter_FlexRay;
	UINT uSize = sFilter_FlexRay.unGetSize();
	BYTE* pbyCurrData   = new BYTE[uSize];

	sFilter_FlexRay.m_eChannel = 0;
	sFilter_FlexRay.m_dwMsgIDTo = 123;
	sFilter_FlexRay.m_eDrctn = DIR_TX;

	pbyResult = sFilter_FlexRay.pbGetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( pbSetConfigData_FlexRay_Test )
{
	
	BYTE* pbyCurrData   = new BYTE;
	BYTE* pbyResult = NULL;
	SFILTER_FLEXRAY		*psFilter_FlexRay = new SFILTER_FLEXRAY;

	psFilter_FlexRay->m_eChannel = 0;
	psFilter_FlexRay->m_dwMsgIDTo = 123;
	psFilter_FlexRay->m_eDrctn = DIR_TX;

	pbyResult = psFilter_FlexRay->pbSetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( bDoesFrameOccur_MCNET_Test )
{
	SFRAMEINFO_BASIC_MCNET		*psCurrFrame = new SFRAMEINFO_BASIC_MCNET;

	psCurrFrame->m_dwFrameID = 12;

	SFILTER_MCNET	*psFilter_MCNET = new SFILTER_MCNET;
	psFilter_MCNET->m_dwMsgIDFrom = 12;
	psFilter_MCNET->m_dwMsgIDTo = 12;
	
	psFilter_MCNET->m_eDrctn = DIR_ALL;  
	psFilter_MCNET->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;

	bool bOccurs = psFilter_MCNET->bDoesFrameOccur(psCurrFrame);

	psFilter_MCNET->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs = psFilter_MCNET->bDoesFrameOccur(psCurrFrame);

	psCurrFrame->m_dwFrameID = 13;
	bOccurs = psFilter_MCNET->bDoesFrameOccur(psCurrFrame);
	psCurrFrame->m_dwFrameID = 1;
	bOccurs = psFilter_MCNET->bDoesFrameOccur(psCurrFrame);
	psFilter_MCNET->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
	bOccurs = psFilter_MCNET->bDoesFrameOccur(psCurrFrame);

	psFilter_MCNET->m_ucFilterType = 3;
	bOccurs = psFilter_MCNET->bDoesFrameOccur(psCurrFrame);

	delete psFilter_MCNET;
}

BOOST_AUTO_TEST_CASE(SFILTER_MCNET_GetSize_Test )
{
	SFILTER_MCNET		sFilter_MCNET ;
	UINT uSize = sFilter_MCNET.unGetSize();
}

BOOST_AUTO_TEST_CASE( pbGetConfigData_MCNET_Test )
{
	
	BYTE* pbyCurrData   = new BYTE;
	BYTE* pbyResult = NULL;
	SFILTER_MCNET		sFilter_MCNET;

	
	sFilter_MCNET.m_dwMsgIDTo = 123;
	sFilter_MCNET.m_eDrctn = DIR_TX;

	pbyResult = sFilter_MCNET.pbGetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( pbSetConfigData_MCNET_Test )
{
	
	BYTE* pbyCurrData   = new BYTE;
	BYTE* pbyResult = NULL;
	SFILTER_MCNET		sFilter_MCNET ;


	sFilter_MCNET.m_dwMsgIDTo = 123;
	sFilter_MCNET.m_eDrctn = DIR_TX;

	pbyResult = sFilter_MCNET.pbSetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( bDoesFrameOccur_J1939_Test )
{
	SFRAMEINFO_BASIC_J1939		*psCurrFrame = new SFRAMEINFO_BASIC_J1939;

	psCurrFrame->m_dwPGN = 12;

	SFILTER_J1939		*psFilter_J1939 = new SFILTER_J1939;
	psFilter_J1939->m_dwMsgIDFrom = 12;
	psFilter_J1939->m_dwMsgIDTo = 12;

	psFilter_J1939->m_eDrctn = DIR_ALL;  
	psFilter_J1939->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;

	bool bOccurs = psFilter_J1939->bDoesFrameOccur(psCurrFrame);

	psFilter_J1939->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
	bOccurs =psFilter_J1939->bDoesFrameOccur(psCurrFrame);
}

BOOST_AUTO_TEST_CASE(SFILTER_J1939_GetSize_Test)
{
	SFILTER_J1939		sFilter_J1939 ;
	UINT uSize = sFilter_J1939.unGetSize();
}

BOOST_AUTO_TEST_CASE( pbGetConfigData_J1939_Test )
{

	BYTE* pbyCurrData   = new BYTE;
	BYTE* pbyResult = NULL;
	SFILTER_J1939		sFilter_J1939;


	//sFilter_MCNET.m_dwMsgIDTo = 123;
	//sFilter_MCNET.m_eDrctn = DIR_TX;

	pbyResult = sFilter_J1939.pbGetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( pbSetConfigData_J1939_Test )
{

	BYTE* pbyCurrData   = new BYTE;
	BYTE* pbyResult = NULL;
	SFILTER_J1939		sFilter_J1939 ;


	//sFilter_J1939.m_dwMsgIDTo = 123;
	//sFilter_J1939.m_eDrctn = DIR_TX;

	pbyResult = sFilter_J1939.pbSetConfigData(pbyCurrData);
	//delete pbyCurrData;
}

BOOST_AUTO_TEST_CASE( tagFilterSet_Clear_Test )
{
	tagFilterSet	sFilterSet;

	sFilterSet.m_eCurrBus = CAN;
	sFilterSet.m_psFilterInfo = new SFILTER_CAN[2];
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus = FLEXRAY;
	sFilterSet.m_psFilterInfo = new SFILTER_FLEXRAY[2];
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus =MCNET;
	sFilterSet.m_psFilterInfo = new SFILTER_MCNET[2];
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus =J1939;
	sFilterSet.m_psFilterInfo = new SFILTER_J1939[2];
	sFilterSet.vClear();
	
}

BOOST_AUTO_TEST_CASE( tagFilterSet_Clone_Test )
{
	tagFilterSet	sFilterSet, sFilterSetClone;
	sFilterSetClone.m_ushFilters = 1;
	sFilterSetClone.m_eCurrBus = CAN;
	sFilterSetClone.m_psFilterInfo = new SFILTER_CAN[2];
	sFilterSet.bClone(sFilterSetClone);
	sFilterSetClone.vClear();
	sFilterSetClone.m_eCurrBus = FLEXRAY;
	sFilterSet.bClone(sFilterSetClone);
	sFilterSetClone.m_psFilterInfo = new SFILTER_FLEXRAY[2];
	sFilterSetClone.vClear();
	sFilterSetClone.m_eCurrBus =MCNET;
	sFilterSet.bClone(sFilterSetClone);
	sFilterSetClone.m_psFilterInfo = new SFILTER_MCNET[2];
	sFilterSetClone.vClear();
	sFilterSetClone.m_eCurrBus =J1939;
	sFilterSet.bClone(sFilterSetClone);
	sFilterSetClone.m_psFilterInfo = new SFILTER_J1939[2];
	sFilterSetClone.vClear();
	
}

BOOST_AUTO_TEST_CASE( tagFilterSet_GetSize_Test )
{
	UINT			unSize;
	tagFilterSet	sFilterSet;

	sFilterSet.m_eCurrBus = CAN;
	sFilterSet.m_psFilterInfo = new SFILTER_CAN[2];
	unSize = sFilterSet.unGetSize();
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus = FLEXRAY;
	sFilterSet.m_psFilterInfo = new SFILTER_FLEXRAY[2];
	unSize = sFilterSet.unGetSize();
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus =MCNET;
	sFilterSet.m_psFilterInfo = new SFILTER_MCNET[2];
	unSize = sFilterSet.unGetSize();
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus =J1939;
	sFilterSet.m_psFilterInfo = new SFILTER_J1939[2];
	unSize = sFilterSet.unGetSize();
	sFilterSet.vClear();
}

BOOST_AUTO_TEST_CASE( tagFilterSet_pbGetConfigDatae_Test )
{
	bool			bResult;
	tagFilterSet	sFilterSet;
	UINT uSize = sFilterSet.unGetSize();
	BYTE* pbyCurrData   = new BYTE[uSize +1];
	BYTE* pbyResult = NULL;
	char pName[128] = "HI";

	//strcpy(sFilterSet.m_sFilterName.m_acFilterName,pName) ;
    COPY_DATA(pbyCurrData, (void *)pName, sizeof(LENGTH_FILTERNAME * SIZE_CHAR));
	//COPY_DATA(pbyCurrData, (void *)pName, sizeof(pName));

	sFilterSet.m_eCurrBus = CAN;
	sFilterSet.m_psFilterInfo = new SFILTER_CAN[2];
	pbyResult = sFilterSet.pbSetConfigData(pbyCurrData,bResult);
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus = FLEXRAY;
	sFilterSet.m_psFilterInfo = new SFILTER_FLEXRAY[2];
	pbyResult = sFilterSet.pbSetConfigData(pbyCurrData,bResult);
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus =MCNET;
	sFilterSet.m_psFilterInfo = new SFILTER_MCNET[2];
	pbyResult = sFilterSet.pbSetConfigData(pbyCurrData,bResult);
	sFilterSet.vClear();
	sFilterSet.m_eCurrBus =J1939;
	sFilterSet.m_psFilterInfo = new SFILTER_J1939[2];
	pbyResult = sFilterSet.pbSetConfigData(pbyCurrData,bResult);
	sFilterSet.vClear();
}

//J1939 Datatypes
BOOST_AUTO_TEST_CASE( tagSTJ1939_MSG_unGetPGN_Test )
{
	UINT32			u32PGN;
	tagSTRUCT_29_BIT_ID		s29BitID;
	s29BitID.vSetPGN(1234556);
	u32PGN = 	s29BitID.unGetPGN();

}

BOOST_AUTO_TEST_CASE( tagSTJ1939_MSG_unGetSize_Test )
{
	tagSTJ1939_MSG   sJ1939Msg;
	UINT uSize = sJ1939Msg.unGetSize();
}

BOOST_AUTO_TEST_CASE( tagSTJ1939_MSG_vGetDataStream_Test )
{
	BYTE* pbyResult = new BYTE;
	tagSTJ1939_MSG   sJ1939Msg;
	sJ1939Msg.m_sMsgProperties.m_byChannel = 0;
	sJ1939Msg.m_sMsgProperties.m_eDirection =   DIR_ALL;
	sJ1939Msg.m_unDLC = 8;
	sJ1939Msg.m_pbyData = new BYTE;
	sJ1939Msg.vGetDataStream(pbyResult);
}

BOOST_AUTO_TEST_CASE( tagSTJ1939_MSG_vSetDataStream_Test )
{
	BYTE* pbyResult = new BYTE;
	//BYTE*  m_pbyData = new BYTE;
	int nData = 1234;

	UINT unDLC = 8;
	STJ1939_MSG_PROPERTIES			*psMsgProperties = new STJ1939_MSG_PROPERTIES;
	psMsgProperties->m_byChannel = 0;
	psMsgProperties->m_eDirection = DIR_ALL;
	psMsgProperties->m_eType = MSG_TYPE_DATA;

	COPY_DATA(pbyResult, psMsgProperties, sizeof(STJ1939_MSG_PROPERTIES));
	COPY_DATA(pbyResult, &unDLC, sizeof (UINT));
	COPY_DATA(pbyResult, &nData, (sizeof (BYTE) * unDLC));

	tagSTJ1939_MSG   sJ1939Msg;
	sJ1939Msg.m_sMsgProperties.m_byChannel = 0;
	sJ1939Msg.m_sMsgProperties.m_eDirection =   DIR_ALL;
	sJ1939Msg.m_unDLC = 8;
	sJ1939Msg.vSetDataStream(pbyResult);
}

BOOST_AUTO_TEST_CASE( tagSTJ1939_MSG_vIntiialize_Clear_Test )
{
	tagSTJ1939_MSG   sJ1939Msg;
	sJ1939Msg.vInitialize(10);
	sJ1939Msg.vSetSortField(5);
	sJ1939Msg.vClear();
}

BOOST_AUTO_TEST_CASE( tagSTJ1939_DoCompareIndiv_Clear_Test )
{
	tagSTJ1939_MSG   sJ1939Msg, sJ1939Msg1, sJ1939Msg2;
	sJ1939Msg.m_nMFactor = 2;
	sJ1939Msg1.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = 123;
	sJ1939Msg1.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress =999;
	sJ1939Msg1.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_unPGN = 888;
	sJ1939Msg1.m_sMsgProperties.m_uExtendedID.m_unExtID = 888888;
	sJ1939Msg1.m_sMsgProperties.m_byChannel = 3;
	sJ1939Msg1.m_sMsgProperties.m_un64TimeStamp = 5689;


	int nResult =0;
	for(int nCnt =0; nCnt <= 8; nCnt++)
	{
		sJ1939Msg.m_nSortField = nCnt;
		nResult = sJ1939Msg.DoCompareIndiv(&sJ1939Msg1, &sJ1939Msg2);
	}
	
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = 123;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress =999;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_unPGN = 888;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_unExtID = 888888;
	sJ1939Msg2.m_sMsgProperties.m_byChannel = 3;
	sJ1939Msg2.m_sMsgProperties.m_un64TimeStamp = 5689;
	
	
	 nResult =0;
	for(int nCnt =0; nCnt <= 8; nCnt++)
	{
		sJ1939Msg.m_nSortField = nCnt;
		nResult = sJ1939Msg.DoCompareIndiv(&sJ1939Msg1, &sJ1939Msg2);
	}
}

BOOST_AUTO_TEST_CASE( tagSTJ1939_GetSlotID_Test )
{
	tagSTJ1939_MSG   sJ1939Msg2;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific = 123;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress =999;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_unPGN = 888;
	sJ1939Msg2.m_sMsgProperties.m_uExtendedID.m_unExtID = 888888;
	sJ1939Msg2.m_sMsgProperties.m_byChannel = 3;
	sJ1939Msg2.m_sMsgProperties.m_un64TimeStamp = 5689;
	sJ1939Msg2.m_sMsgProperties.m_eType = MSG_TYPE_BROADCAST;
	sJ1939Msg2.GetSlotID(sJ1939Msg2);
}

BOOST_AUTO_TEST_CASE(tagFormattedData_J1939_Test )
{
	tagFormattedData_J1939	*psFormattedData_J1939 = new tagFormattedData_J1939;
	delete psFormattedData_J1939;
}

BOOST_AUTO_TEST_CASE(GetMessageTypeStr_Test )
{
	char	cResult[32];
	for(int nCnt =0; nCnt <= 15; nCnt++ )
	{
		GetMessageTypeStr((EJ1939_MSG_TYPE)nCnt, cResult);
	}
}

BOOST_AUTO_TEST_CASE(tagLogInfo_GetSize_Test )
{
	tagLogInfo		*psTagLogInfo = new tagLogInfo;
	UINT uSize = psTagLogInfo->unGetSize();
	delete psTagLogInfo;
}

BOOST_AUTO_TEST_CASE(tagLogInfo_GetConfigData_Test )
{
	tagLogInfo		*psTagLogInfo = new tagLogInfo;
	BYTE*			pResByte = new BYTE;
	BYTE*			pSrcByte = new BYTE;
	pResByte = psTagLogInfo->pbGetConfigData(pSrcByte);
	delete psTagLogInfo;
}

BOOST_AUTO_TEST_CASE(tagLogInfo_SetConfigData_Test )
{
	tagLogInfo		sTagLogInfo/* = new tagLogInfo*/;
	UINT uSize = sTagLogInfo.unGetSize();
	BYTE*			pResByte = new BYTE[uSize+1];
	BYTE*			pSrcByte = new BYTE[uSize+1];
	BYTE*			pLogByte = new BYTE[uSize+1];
	*pLogByte = 2;
	pResByte = sTagLogInfo.pbSetConfigData(pSrcByte,*pLogByte );

	*pLogByte = 1;
	pResByte = sTagLogInfo.pbSetConfigData(pSrcByte,*pLogByte);
	//delete psTagLogInfo;
	//delete pResByte;
	//delete pSrcByte;
	//delete pLogByte;
}

BOOST_AUTO_TEST_CASE(SGUIPARAMS_Test )
{
	SGUIPARAMS   *psSGUIPARAMS = new SGUIPARAMS;
	delete psSGUIPARAMS;
}

BOOST_AUTO_TEST_CASE(tagSubEntry_Test )
{
	tagSubEntry		sSubEntry;
	sSubEntry.m_omSubEntryName = "Name";
	tagSubEntry   sSubEntry1 ;
	sSubEntry1.m_omSubEntryName = "Name";
	bool bCheck = (sSubEntry == sSubEntry1);
	sSubEntry1.m_omSubEntryName = "N";
	bCheck = (sSubEntry ==sSubEntry1);

	//delete psSubEntry;
}




BOOST_AUTO_TEST_CASE(tagMainEntry_Test )
{
	SMAINENTRY   sMainEntry1;
	SMAINENTRY	sMainEntry;	

	sMainEntry.m_omMainEntryName = "Name";
	sMainEntry1.m_omMainEntryName = "Name";

	sMainEntry.m_unMainEntryID= 123;
	sMainEntry1.m_omMainEntryName = 123;

	bool bCheck = (sMainEntry == sMainEntry1);

	sMainEntry1.m_omMainEntryName = "Na";
	bCheck = (sMainEntry == sMainEntry1);

	sMainEntry1.m_omMainEntryName = 12;
	bCheck = (sMainEntry == sMainEntry1);

	sMainEntry1.m_omMainEntryName = "Name";
	bCheck = (sMainEntry == sMainEntry1);
}

BOOST_AUTO_TEST_CASE(CMsgBufVSE_ReadFromBuffer_Test )
{
	INT nType = 0;
	CMsgBufVSE  oMsgBufVSE;
	int	nBufferSize = oMsgBufVSE.GetBufferLength();
	BYTE* pByte = new BYTE[nBufferSize];

	oMsgBufVSE.WriteIntoBuffer(nType, pByte, nBufferSize);
}


BOOST_AUTO_TEST_SUITE_END()
