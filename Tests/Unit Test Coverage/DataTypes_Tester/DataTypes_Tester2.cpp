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
#include "DataTypes/MsgBufVVSE.h"
#include "DataTypes/MsgSignal_Datatypes.h"
//#include "DataTypes/MsgSignal_Datatypes.h"
#include "DataTypes/ProjConfig_DataTypes.h"
//#include "DataTypes/SigWatch_Datatypes.h"
//#include "DataTypes/SigGrphWnd_Datatypes.h"
#include "include/Struct_CAN.h"

const int SIZE_CHAR = sizeof(char);
BOOST_AUTO_TEST_SUITE( DataTypes_Tester )

CMsgBufVSE  oMsgBufVSE;
CMsgBufVVSE oMsgBufVVSE;

BOOST_AUTO_TEST_CASE(CMsgBufVSE_WriteIntoBuffer_Test )
{
	INT nType = 0;
	//CMsgBufVSE  oMsgBufVSE;
	int	nBufferSize = oMsgBufVSE.GetBufferLength();
	BYTE* pByte = new BYTE[nBufferSize];

	oMsgBufVSE.WriteIntoBuffer(nType, pByte, nBufferSize);

	nBufferSize += 100;
	oMsgBufVSE.WriteIntoBuffer(nType, pByte, nBufferSize);

	nBufferSize -= 110;
	oMsgBufVSE.WriteIntoBuffer(nType, pByte, nBufferSize);

}

BOOST_AUTO_TEST_CASE(CMsgBufVSE_ReadFromBuffer_Test )
{
	INT nType = 0;
	//CMsgBufVSE  oMsgBufVSE;
	int	nBufferSize = oMsgBufVSE.GetBufferLength();
	BYTE* pByte = new BYTE[nBufferSize], *pByteCpy = new BYTE;
	
	pByteCpy = pByte;
	*pByte = (BYTE)12344;
	pByte += sizeof(BYTE);
	*pByte = (BYTE)45645;
	pByte += sizeof(BYTE);
	*pByte = (BYTE)45645;
	pByte += sizeof(BYTE);
	*pByte = (BYTE)45645;
	pByte += sizeof(BYTE);
	*pByte = (BYTE)45645;
	pByte += sizeof(BYTE);
	*pByte = (BYTE)45645;

	oMsgBufVSE.ReadFromBuffer(nType, pByte, nBufferSize);

	nBufferSize += 100;
	
	oMsgBufVSE.ReadFromBuffer(nType, pByte, nBufferSize);

	nBufferSize -= 110;
	oMsgBufVSE.ReadFromBuffer(nType, pByte, nBufferSize);
	
	oMsgBufVSE.AdvanceToNextMsg();

	CMsgBufVSE  oMsgBufVSE1;
	nBufferSize += 110;
	oMsgBufVSE1.ReadFromBuffer(nType, pByte, nBufferSize);
	
	
}

BOOST_AUTO_TEST_CASE(CMsgBufVSE_GetMsgCnt_NotifyEvent_Test )
{
	int nMsgCnt = oMsgBufVSE.GetMsgCount();

	HANDLE hHandle = oMsgBufVSE.hGetNotifyingEvent(); 
	
}

BOOST_AUTO_TEST_CASE(CMsgBufVSE_SetBufferSize_Test )
{
	int	nBufferSize = 4000;
	oMsgBufVSE.nSetBufferSize(nBufferSize);

		nBufferSize = 140000;
	oMsgBufVSE.nSetBufferSize(nBufferSize);

	nBufferSize = oMsgBufVSE.GetBufferLength();
	oMsgBufVSE.nSetBufferSize(nBufferSize);
}

BOOST_AUTO_TEST_CASE(CMsgBufVSE_AdvanceToNxtMsg_Test )
{
	CMsgBufVSE  oMsgBufVSE2;
	oMsgBufVSE2.AdvanceToNextMsg();	
}

BOOST_AUTO_TEST_CASE(CMsgBufVVSE_WriteIntoBuffer_Test )
{
	INT nType = 0;
	//CMsgBufVSE  oMsgBufVSE;
	int	nBufferSize = 50000;
	BYTE* pByte = new BYTE[nBufferSize];

	oMsgBufVVSE.WriteIntoBuffer(nType, pByte, nBufferSize);

	nBufferSize += 100;
	oMsgBufVVSE.WriteIntoBuffer(nType, pByte, nBufferSize);

	nBufferSize -= 110;
	oMsgBufVVSE.WriteIntoBuffer(nType, pByte, nBufferSize);
}

BOOST_AUTO_TEST_CASE(CMsgBufVVSE_AdvanceToNxtMsg_SetStartPos_Test )
{
	int nType = 0  ;
	int	nBufferSize = 50000;
	BYTE* pByte = new BYTE[nBufferSize];

	oMsgBufVVSE.SetStartPos(-1);
	oMsgBufVVSE.SetStartPos(10);
	oMsgBufVVSE.SetStartPos(1);

	oMsgBufVVSE.ReadEntry(nType, pByte, nBufferSize,0,TRUE);
	oMsgBufVVSE.ReadEntry(nType, pByte, nBufferSize,1,TRUE);
	oMsgBufVVSE.ReadEntry(nType, pByte, nBufferSize,-1,TRUE);
	oMsgBufVVSE.ReadEntry(nType, pByte, nBufferSize,10,TRUE);
	
}

BOOST_AUTO_TEST_CASE(CMsgBufVVSE_SetBufferSize_Test )
{
	int	nBufferSize = 4000;
	oMsgBufVVSE.nSetBufferSize(nBufferSize);

		nBufferSize = 240000;
	oMsgBufVVSE.nSetBufferSize(nBufferSize);

	nBufferSize = 150000;
	oMsgBufVVSE.nSetBufferSize(nBufferSize);
}

STCANDATA		sCanData;
BOOST_AUTO_TEST_CASE(STCANDATA_Test )
{
	int		nField = 1;
	sCanData.vSetSortField(nField);
	sCanData.vSetSortAscending(TRUE);
	sCanData.vSetSortAscending(FALSE);
}
BOOST_AUTO_TEST_CASE(STCANDATA_CompareDiv_Test )
{
	STCANDATA	*pCanData1 = new STCANDATA;
	STCANDATA	*pCanData2 = new STCANDATA;
	for(int nCnt =0;nCnt <= 5; nCnt++)
	{
		sCanData.vSetSortField(nCnt);
		sCanData.DoCompareIndiv((void*)pCanData1,(void*)pCanData2);
	}

	pCanData1->m_uDataInfo.m_sCANMsg.m_ucChannel = 1;
	pCanData1->m_uDataInfo.m_sCANMsg.m_unMsgID = 124;
	pCanData1->m_lTickCount.QuadPart			 = 56865;

	for(int nCnt =0;nCnt <= 5; nCnt++)
	{
		sCanData.vSetSortField(nCnt);
		sCanData.DoCompareIndiv((void*)pCanData1,(void*)pCanData2);
	}
}

BOOST_AUTO_TEST_CASE(STCANDATA_GetSlotID_Test )
{
	STCANDATA	*pCanData1 = new STCANDATA;
	sCanData.GetSlotID(*pCanData1);
	pCanData1->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 0;
	sCanData.GetSlotID(*pCanData1); 
	pCanData1->m_ucDataType = 0;
	sCanData.GetSlotID(*pCanData1); 
	pCanData1->m_ucDataType = 2;
	sCanData.GetSlotID(*pCanData1);
}

PROJECTDATA			sProjectData;


BOOST_AUTO_TEST_CASE(PROJECTDATA_Test )
{
	SYSTEMTIME		nSys;
	sProjectData.m_ProjectName = "Name";  
	sProjectData.m_Language = "Lang";      
	sProjectData.m_ProjSysTime = nSys;
	sProjectData.m_fAppVersion = 1.0;  
	sProjectData.m_dwAppUniqueId = 568; 

	PROJECTDATA			sProjectData1;
	sProjectData1 = sProjectData;
}

BOOST_AUTO_TEST_CASE(PROJECTDATA_Write_Test )
{
	FILE* pFile;
	pFile = fopen ( "myfile.txt" , "wb" );
	sProjectData.Write(pFile);
	sProjectData.m_Language = "\0"; 
	sProjectData.Write(pFile);
	fclose (pFile);
}

BOOST_AUTO_TEST_CASE(PROJECTDATA_Read_Test )
{
	FILE* pFile;
	pFile = fopen ( "myfile.txt" , "rb" );
	sProjectData.Read(pFile);
	fclose (pFile);
}


SECTIONDATA			sSectionData;

BOOST_AUTO_TEST_CASE(SECTIONDATA_Test )
{
    sSectionData.m_omSectionName = "NAME"; 
    sSectionData.m_nBLOBLen = 4;       
    sSectionData.m_bBLOB = new BYTE;  

	SECTIONDATA			sSectionData1;
	sSectionData1 = sSectionData;
}

BOOST_AUTO_TEST_CASE(SECTIONDATA_Write_Test )
{
	FILE* pFile;
	pFile = fopen ( "myfile.txt" , "wb" );
	sSectionData.Write(pFile);
	fclose (pFile);
}

BOOST_AUTO_TEST_CASE(SECTIONDATA_Read_Test )
{
	FILE* pFile;
	pFile = fopen ( "myfile.txt" , "rb" );
	sSectionData.Read(pFile);
	fclose (pFile);
}

BOOST_AUTO_TEST_CASE(CSignalDescVal_Test )
{
	CSignalDescVal *pCSignalDescVal = new CSignalDescVal();
	CSignalDescVal objSig, objSig1;

	objSig.m_DescValue.n64Value = 20;
	objSig.m_DescValue.un64Value = 200;

	// operator =
	objSig1 = objSig;

	objSig1.m_pouNextSignalSignalDescVal = new CSignalDescVal();

	objSig1.vClearNext();

	if(pCSignalDescVal != NULL)
	{
		delete pCSignalDescVal;
	}
}

BOOST_AUTO_TEST_CASE(sWaveformInfo_Test )
{
	sWaveformInfo *pWaveForm = new sWaveformInfo();
	sWaveformInfo objsWaveformInfo, objWaveform1;

	objsWaveformInfo.m_eSignalWaveType = eWave_SINE;
	objsWaveformInfo.m_fAmplitude = 2.0;
	objsWaveformInfo.m_fFrequency = 3.0;
	objsWaveformInfo.m_fGranularity = 4.0;

	objWaveform1 = objsWaveformInfo;

	objWaveform1.omGetWaveformName(objsWaveformInfo.m_eSignalWaveType);

	objsWaveformInfo.m_eSignalWaveType = eWave_NONE;

	objWaveform1.omGetWaveformName(objsWaveformInfo.m_eSignalWaveType);

	objsWaveformInfo.m_eSignalWaveType = eWave_TRIANGLE;

	objWaveform1.omGetWaveformName(objsWaveformInfo.m_eSignalWaveType);

	objsWaveformInfo.m_eSignalWaveType = eWave_COS;

	objWaveform1.omGetWaveformName(objsWaveformInfo.m_eSignalWaveType);

	if(pWaveForm != NULL)
	{
		delete pWaveForm;
	}
}

BOOST_AUTO_TEST_CASE(sSigWaveMap_Test )
{
	sSigWaveMap *pSigWaveMap = new sSigWaveMap();

	sSigWaveMap objsSigWaveMap, objsSigWaveMap1;

	objsSigWaveMap.m_omSigName ="Signal1";

	objsSigWaveMap.sWaveInfo.m_eSignalWaveType = eWave_SINE;
	objsSigWaveMap.sWaveInfo.m_fAmplitude = 2.0;
	objsSigWaveMap.sWaveInfo.m_fFrequency = 3.0;
	objsSigWaveMap.sWaveInfo.m_fGranularity = 4.0;

	objsSigWaveMap1 = objsSigWaveMap;

	if(pSigWaveMap != NULL)
	{
		delete pSigWaveMap;
	}
}

BOOST_AUTO_TEST_CASE(SSigGeneration_Test )
{
	SSigGeneration *pSSigGeneration = new SSigGeneration();
	SSigGeneration objSigGen;

	objSigGen.m_nMsgID = 100;
	objSigGen.m_fDefaultAmplitude = 1.0;

	sSigWaveMap objsSigWaveMap;
	objsSigWaveMap.m_omSigName ="Signal1";

	objsSigWaveMap.sWaveInfo.m_eSignalWaveType = eWave_SINE;
	objsSigWaveMap.sWaveInfo.m_fAmplitude = 2.0;
	objsSigWaveMap.sWaveInfo.m_fFrequency = 3.0;
	objsSigWaveMap.sWaveInfo.m_fGranularity = 4.0;

	//sSigWaveMap TmpObj = CurrObj;
	objSigGen.m_omSigWaveMapList.AddTail(objsSigWaveMap);

	objSigGen.Reset();

	if(pSSigGeneration != NULL)
	{
		delete pSSigGeneration;
	}

}

BOOST_AUTO_TEST_CASE(sSIGNALS_Test )
{
	sSIGNALS *pSig = new sSIGNALS();

	sSIGNALS objsSIGNALS;

	// INT Type
	objsSIGNALS.m_omStrSignalName = "Signal1";
	objsSIGNALS.m_unStartByte = 0;
	objsSIGNALS.m_unSignalLength = 6;
	objsSIGNALS.m_byStartBit = 0;
	objsSIGNALS.m_bySignalType = 'I';
	objsSIGNALS.m_SignalMinValue.n64Value = 1;
	objsSIGNALS.m_SignalMinValue.un64Value = 1;
	objsSIGNALS.m_SignalMaxValue.n64Value = 100;
	objsSIGNALS.m_SignalMaxValue.un64Value = 100;
	objsSIGNALS.m_fSignalFactor = 1.0;
	objsSIGNALS.m_fSignalOffset = 1.0;
	objsSIGNALS.m_omStrSignalUnit = "";
	objsSIGNALS.m_eFormat = DATA_FORMAT_INTEL;

	objsSIGNALS.m_oSignalIDVal = new CSignalDescVal();

	objsSIGNALS.m_oSignalIDVal->m_DescValue.n64Value = 4;
	objsSIGNALS.m_oSignalIDVal->m_DescValue.un64Value = 10;

	objsSIGNALS.m_psNextSignalList = new sSIGNALS();

	UCHAR aucData[8];

	for(INT nIndex = 0; nIndex < 2; nIndex++)
	{
		aucData[nIndex] = 'A';
	}

	UINT64 unSigVal = 10;

	pSig->vSetSignalValue(&objsSIGNALS, aucData, unSigVal);

	pSig->un64GetBitMask(&objsSIGNALS);

	// BOOL type
	objsSIGNALS.m_bySignalType = 'B';
	objsSIGNALS.m_eFormat = DATA_FORMAT_MOTOROLA;
	objsSIGNALS.m_SignalMinValue.n64Value = 0;
	objsSIGNALS.m_SignalMinValue.un64Value = 0;
	objsSIGNALS.m_SignalMaxValue.n64Value = 1;
	objsSIGNALS.m_SignalMaxValue.un64Value = 1;
	objsSIGNALS.m_unStartByte = 0;
	objsSIGNALS.m_unSignalLength = 1;

	pSig->vSetSignalValue(&objsSIGNALS, aucData, unSigVal);

	pSig->un64GetBitMask(&objsSIGNALS);

	// UNSIGNED INT
	objsSIGNALS.m_bySignalType = 'U';
	objsSIGNALS.m_eFormat = DATA_FORMAT_INTEL;
	objsSIGNALS.m_SignalMinValue.n64Value = 1;
	objsSIGNALS.m_SignalMinValue.un64Value = 1;
	objsSIGNALS.m_SignalMaxValue.n64Value = 100;
	objsSIGNALS.m_SignalMaxValue.un64Value = 100;
	objsSIGNALS.m_unStartByte = 0;
	objsSIGNALS.m_unSignalLength = 4;

	pSig->vSetSignalValue(&objsSIGNALS, aucData, unSigVal);

	pSig->un64GetBitMask(&objsSIGNALS);

	// UNSIGNED INT motorola
	objsSIGNALS.m_bySignalType = 'U';
	objsSIGNALS.m_eFormat = DATA_FORMAT_MOTOROLA;
	objsSIGNALS.m_SignalMinValue.n64Value = 1;
	objsSIGNALS.m_SignalMinValue.un64Value = 1;
	objsSIGNALS.m_SignalMaxValue.n64Value = 100;
	objsSIGNALS.m_SignalMaxValue.un64Value = 100;
	objsSIGNALS.m_unStartByte = 0;
	objsSIGNALS.m_unSignalLength = 4;

	pSig->vSetSignalValue(&objsSIGNALS, aucData, unSigVal);

	pSig->un64GetBitMask(&objsSIGNALS);

	if(objsSIGNALS.m_oSignalIDVal != NULL)
	{
		delete objsSIGNALS.m_oSignalIDVal;
	}

}

BOOST_AUTO_TEST_CASE(tagSMSGENTRY_Test )
{
	sMESSAGE objMessage;

	objMessage.m_omStrMessageName = "Msg1";
	objMessage.m_unMessageCode = 100;
	objMessage.m_unNumberOfSignals = 1;
	objMessage.m_unMessageLength = 8;
	// Standard
	objMessage.m_bMessageFrameFormat = 0;
	objMessage.m_psSignals = new sSIGNALS();

	// Signal Pointer creation
	sSIGNALS objsSIGNALS;
	objsSIGNALS.m_omStrSignalName = "Signal1";
	objsSIGNALS.m_unStartByte = 0;
	objsSIGNALS.m_unSignalLength = 6;
	objsSIGNALS.m_byStartBit = 0;
	objsSIGNALS.m_bySignalType = 'I';
	objsSIGNALS.m_SignalMinValue.n64Value = 1;
	objsSIGNALS.m_SignalMinValue.un64Value = 1;
	objsSIGNALS.m_SignalMaxValue.n64Value = 100;
	objsSIGNALS.m_SignalMaxValue.un64Value = 100;
	objsSIGNALS.m_fSignalFactor = 1.0;
	objsSIGNALS.m_fSignalOffset = 1.0;
	objsSIGNALS.m_omStrSignalUnit = "";
	objsSIGNALS.m_eFormat = DATA_FORMAT_INTEL;

	objsSIGNALS.m_oSignalIDVal = new CSignalDescVal();

	objsSIGNALS.m_oSignalIDVal->m_DescValue.n64Value = 4;
	objsSIGNALS.m_oSignalIDVal->m_DescValue.un64Value = 10;

	objsSIGNALS.m_psNextSignalList = new sSIGNALS();

	objsSIGNALS.m_psNextSignalList = NULL;

	UCHAR aucData[8];

	for(INT nIndex = 0; nIndex < 2; nIndex++)
	{
		aucData[nIndex] = 'A';
	}

	objMessage.m_psSignals = new sSIGNALS();
	objMessage.m_psSignals = &objsSIGNALS;

	tagSMSGENTRY objMsgEntry;

	objMsgEntry.psCopyMsgVal(&objMessage);
	objMsgEntry.psCopySignalVal(objMessage.m_psSignals);	

	SMSGENTRY* m_psMsgRoot = new SMSGENTRY();
	m_psMsgRoot->m_psMsg = new sMESSAGE();
	m_psMsgRoot->psCopyMsgVal(&objMessage);
	
	m_psMsgRoot->m_psMsg->m_psSignals = new sSIGNALS();
	m_psMsgRoot->psCopySignalVal(objMessage.m_psSignals);

	SMSGENTRY* pDestRoot = NULL;
	SMSGENTRY* pTempSrc = m_psMsgRoot;

	m_psMsgRoot->m_psMsg->m_psSignals->m_oSignalIDVal = new CSignalDescVal();

	m_psMsgRoot->m_psMsg->m_psSignals->m_oSignalIDVal->m_DescValue.n64Value = 1;
	m_psMsgRoot->m_psMsg->m_psSignals->m_oSignalIDVal->m_DescValue.un64Value = 1;

	
	m_psMsgRoot->m_psMsg->m_psSignals = &objsSIGNALS;
	m_psMsgRoot->psCopySignalList(&objsSIGNALS);	

	//const SMSGENTRY *pconstmsg = new SMSGENTRY();

	sMESSAGE* pMsg12s = NULL;

	objMsgEntry.bGetMsgPtrFromMsgId(m_psMsgRoot, 100, pMsg12s);

	m_psMsgRoot->bUpdateMsgList(m_psMsgRoot, &objMessage);	
	
	objMsgEntry.vClearSignalList(&objsSIGNALS);
	objMsgEntry.vClearMsgList(m_psMsgRoot);	

	if(objsSIGNALS.m_oSignalIDVal != NULL)
	{
		delete objsSIGNALS.m_oSignalIDVal;
	}

	if(objsSIGNALS.m_psNextSignalList != NULL)
	{
		delete objsSIGNALS.m_psNextSignalList;
	}

	if(m_psMsgRoot != NULL)
	{
		delete m_psMsgRoot;
	}

	

	//objMsgEntry.m_psNext = &objMsgEntry;

	//m_psMsgRoot->m_psMsg = &objMessage;

	//objMsgEntry.psCopyMsgVal(&objMessage);
	//objMsgEntry.psCopySignalVal(objMessage.m_psSignals);

	//objMsgEntry.vClearMsgList(NULL);
	//objMsgEntry.vClearSignalList(NULL);

}

BOOST_AUTO_TEST_CASE(sINTERPRETSIGNALINFO_Test )
{
	sINTERPRETSIGNALINFO *pIntepretMsg = new sINTERPRETSIGNALINFO();

	sINTERPRETSIGNALINFO objInterpret;

	objInterpret.m_omStrSignalName = "Sig1";

	objInterpret.m_un64RawValue = 1;
    objInterpret.m_dPhyValue = 1;
    objInterpret.m_omStrSignalDescValue = "20";
    objInterpret.m_omStrUnit = "ms";
    objInterpret.m_ucSigLength = 8;
}

BOOST_AUTO_TEST_SUITE_END()
