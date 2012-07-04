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

#include "Include/BaseDefs.h"
#include "DIL_J1939/DIL_J1939_stdafx.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_J1939/DIL_J1939_Extern.h"
#include "DIL_J1939/ConnectionDet.h"
#include "WrapperErrorLogger.h" //For Logging Error in DIL Interface 
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "DIL_J1939/NodeConManager.h"
#include "DIL_J1939/MonitorNode.h"
#include "DIL_J1939/TransferLayer.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"
#include "FrameProcessor/FrameProcessor_J1939.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "FrameProcessor/FrameProcessor_CAN.h"
#include "FrameProcessor/FrameProcessor_Extern.h"

/* Global variables to hold CAN DIL parameters */
CBaseDIL_CAN* g_pouDIL_CAN_Interface = NULL; // CAN DIL interface
DWORD g_dwDriverId = DRIVER_CAN_VECTOR_XL;
WrapperErrorLogger g_ouWrapperLogger;	
INTERFACE_HW_LIST g_asINTERFACE_HW;
DWORD g_dwClientID = 0;
SCONTROLLER_DETAILS g_asControllerDetails[defNO_OF_CHANNELS];
INT g_nChannelCount = 0;
/* CAN message */
STCAN_MSG sMsg;

/* J1939 Params */
static CBaseDILI_J1939* sg_pouIJ1939DIL = NULL; // J1939 DIL interface
DWORD g_dwJ1939ClientId[2] = {0};
const UINT64 J1939_NODE_NAME     = 0x8000000000000015;
BYTE g_arrNodeAddr[] = {0x05, 0x10};
UINT32 g_arrRqstPGNs[] = {0x5500, 0x3300};
UINT32 g_arrBroadcastPGNs[] = {0xF008, 0xFD58};
CMsgBufVSE g_ouVSEBufJ1939;
SLOGINFO g_sLogStruct;
SLOGINFO g_sLogStructCAN;

/* CAN Logger variables */
CBaseFrameProcessor_CAN* sg_pouICANLogger = NULL;
DWORD g_dwCANClientIdLogger = 0;
CCANBufFSE g_ouCANBufFSE;    //Global CAN buffer

/* J1939 logger variables*/
CBaseFrameProcessor_J1939* sg_pouIJ1939Logger = NULL;
DWORD g_dwJ1939ClientIdLogger = 0;

static void CallBackSample(UINT32 /*unPGN*/, BYTE /*bySrc*/, 
                                BYTE /*byDest*/, BOOL /*bSuccess*/)
{    
}

HRESULT InitializeDIL(void)
{   		
    HRESULT hResult = S_OK;
    if (g_pouDIL_CAN_Interface == NULL)
    {		
        hResult = DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);
		hResult = DIL_GetInterface(J1939, (void**) &sg_pouIJ1939DIL);
    }
    else
    {
        g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();        
    }
    if (hResult == S_OK)
    {        				
        if ((hResult = g_pouDIL_CAN_Interface->DILC_SelectDriver(g_dwDriverId, NULL, &g_ouWrapperLogger)) == S_OK)
        {			
            g_pouDIL_CAN_Interface->DILC_PerformInitOperations();
            INT nCount = defNO_OF_CHANNELS;
            if (g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(g_asINTERFACE_HW, nCount) == S_OK)
            {
                HRESULT hResult = g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(g_asINTERFACE_HW, nCount);

				g_nChannelCount = nCount;
                if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
                {
                    hResult = g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
                    if ((hResult == S_OK)|| (hResult == ERR_CLIENT_EXISTS))
                    {				
		                g_pouDIL_CAN_Interface->DILC_SetConfigData(g_asControllerDetails, 
															sizeof(SCONTROLLER_DETAILS) * nCount);

                    }
                    else
                    {
						//AfxMessageBox("registering client failed");
                    }
                }
                else
                {
                    //AfxMessageBox("Selecting hardware interface failed");
                }
            }
            else
            {
				hResult = S_FALSE;
                //AfxMessageBox("Listing hardware interfaces failed");
            }
        }
        else
        {
		if ( hResult!=DAL_ALREADY_SELECTED )
		{
			//AfxMessageBox("Driver selection failed");	
		}            	
        }
    }
    return hResult;    
}

BOOST_AUTO_TEST_SUITE( J1939_Exports_Tester )

BOOST_AUTO_TEST_CASE( CAN_J1939_Initializations )
{
	HRESULT hr = S_FALSE;	

	/* Initialise message structure */
	sMsg.m_unMsgID = 0x100;              // Message ID
	sMsg.m_ucEXTENDED = FALSE;           // Standard Message type
	sMsg.m_ucRTR = FALSE;                // Not RTR type
	sMsg.m_ucDataLen = 8;                // Length is 8 Bytes
	for( int i = 0; i<8;i++)
		sMsg.m_ucData[i] = i+1;
	sMsg.m_ucData[7] = 0;

	sMsg.m_ucChannel = 1;                // First CAN channel

	
	/********************************** CAN Related activities  - Start ************************************/
	/* Initialize CAN DIL section */
	hr = InitializeDIL();

	if (hr == S_OK)
	{		
		//AfxMessageBox( "CAN initializations done!" );
		/* Connect to CAN network */
		hr = g_pouDIL_CAN_Interface->DILC_StartHardware();
		Sleep(1000);
		if (hr == S_OK)
		{
			/* Send a test CAN message */
			hr = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, sMsg);
		}
	}
	else
	{
		BOOST_FAIL( "Failed to Initialize CAN Network..." );  
		exit(0);
	}
	/********************************** CAN Related activities  - End    ***********************************/	
}

BOOST_AUTO_TEST_CASE( J1939_Register_Go_Online )
{
	/********************************** J1939 Related activities - Start ***********************************/
	/* J1939 initialization */

	BOOST_REQUIRE ( S_OK == DILJ_Initialise(&g_ouWrapperLogger, g_pouDIL_CAN_Interface ) );	

	/* Register client - Montior node */
	Sleep(100);
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(TRUE, 
                    J1939_MONITOR_NODE, J1939_ECU_NAME, g_arrNodeAddr[0], g_dwJ1939ClientId[0]) );
	
	/* Register client - Normal J1939 node */
	Sleep(100);
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(TRUE, 
                    _T("Node 1"), J1939_NODE_NAME, g_arrNodeAddr[1], g_dwJ1939ClientId[1]) );

	/*                        Go online                  */
	BOOST_CHECK ( S_OK == DILJ_GoOnline() );		
}

/* Get J1939 logger module */
BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Interface_Getter )
{
	/* J1939 intializations for logger */
	sg_pouIJ1939DIL->DILIJ_Initialise(&g_ouWrapperLogger, g_pouDIL_CAN_Interface);

	BOOST_REQUIRE ( S_OK == sg_pouIJ1939DIL->DILIJ_RegisterClient(TRUE, 
                   J1939_MONITOR_NODE, J1939_ECU_NAME, g_arrNodeAddr[0], g_dwJ1939ClientId[0]) );

	BOOST_CHECK ( S_OK == sg_pouIJ1939DIL->DILIJ_GoOnline() );

	/* J1939 Logging interface getter*/
	FP_GetInterface(FRAMEPROC_J1939, (void**)&sg_pouIJ1939Logger);
	//sg_pouIJ1939Logger = new CFrameProcessor_J1939;
	/* CAN Logging interface getter*/
	FP_GetInterface(FRAMEPROC_CAN, (void**)&sg_pouICANLogger);
	//sg_pouICANLogger = new CFrameProcessor_CAN;
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_InitInstance )
{
	/* Call init instance of J1939 logger to get the logger thread started */
	BOOST_REQUIRE ( ((CFrameProcessor_J1939*)sg_pouIJ1939Logger)->InitInstance() == TRUE );

	/* Call init instance of J1939 logger to get the logger thread started */
	BOOST_REQUIRE ( ((CFrameProcessor_CAN*)sg_pouICANLogger)->InitInstance() == TRUE );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Initialization )
{
	/* initialise the J1939 logger interface */
	CParamLoggerJ1939 ouParam;
	CString omVerStr("");       // First get the version information
	omVerStr.Format("Ver 1.6.5");   // Set BUSMASTER version
	strcpy_s(ouParam.m_acVersion, MAX_PATH, omVerStr.GetBuffer(MAX_CHAR));

	ouParam.m_pILog = &g_ouWrapperLogger;
	ouParam.dwClientID = g_dwJ1939ClientId[0];	//Log for monitor node
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_DoInitialisation(&ouParam) == S_OK );
       
	/* initialise the CAN logger interface */
	SCANPROC_PARAMS sCANProcParams;
	strncpy_s(sCANProcParams.m_acVersion, 64, omVerStr, omVerStr.GetLength());
	sCANProcParams.dwClientID		= g_dwClientID;
    sCANProcParams.m_pouCANBuffer	= &g_ouCANBufFSE;
    sCANProcParams.m_pILog			= &g_ouWrapperLogger;
	
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_DoInitialisation(&sCANProcParams) == S_OK );	
	/* negative test case */
	BOOST_CHECK ( sg_pouIJ1939Logger->FPJ1_DoInitialisation(NULL) == S_FALSE );
	BOOST_CHECK ( sg_pouICANLogger->FPC_DoInitialisation(NULL) == S_FALSE );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Start_Editing_Session )
{
	/* Start editing session */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_StartEditingSession() == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_StartEditingSession() == S_OK );
}

BOOST_AUTO_TEST_CASE( J1939_Logger_Module_Add_Log_Blocks )
{
	// Get the working directory
	CString strPath;
	CString omCurrExe;
    char* pstrExePath = strPath.GetBuffer (MAX_PATH);
	::GetModuleFileName (0, pstrExePath, MAX_PATH);
	strPath.ReleaseBuffer ();	
	strPath = strPath.Left(strPath.ReverseFind(92));		

    /* Add the new logging object in the datastore.  */
    
	/* Select All CAN channels */
    g_sLogStruct.m_ChannelSelected = CAN_CHANNEL_ALL;	

	omCurrExe.Format("%s\\J1939LogTestSystem.log", strPath);
	strcpy_s(g_sLogStruct.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));

	/* Block with system time */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_AddLoggingBlock(g_sLogStruct) == S_OK );

	g_sLogStruct.m_eLogTimerMode = TIME_MODE_ABSOLUTE;
	/* Set Trigger - Both */
	g_sLogStruct.m_sLogTrigger.m_unTriggerType  = BOTH;
	g_sLogStruct.m_sLogTrigger.m_unStartID      = 0x00EC00;
	g_sLogStruct.m_sLogTrigger.m_unStopID	    = 0x00EB00;

	omCurrExe.Format("%s\\J1939LogTestAbs_Rst_OFF.log", strPath);
	strcpy_s(g_sLogStruct.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with Absolute time - Reset Timestamp OFF*/
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_AddLoggingBlock(g_sLogStruct) == S_OK );


	g_sLogStruct.m_bResetAbsTimeStamp = TRUE;
	/* Set Trigger - Start*/
	g_sLogStruct.m_sLogTrigger.m_unTriggerType	= START;
	g_sLogStruct.m_sLogTrigger.m_unStartID		= 0x00EC00;
	omCurrExe.Format("%s\\J1939LogTestAbs_Rst_ON.log", strPath);
	strcpy_s(g_sLogStruct.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with Absolute time - Reset Timestamp ON*/
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_AddLoggingBlock(g_sLogStruct) == S_OK );

	g_sLogStruct.m_eLogTimerMode = TIME_MODE_RELATIVE;
	/* Set Trigger - Stop*/
	g_sLogStruct.m_sLogTrigger.m_unTriggerType = STOP;
	g_sLogStruct.m_sLogTrigger.m_unStopID      = 0x00EB00;

	omCurrExe.Format("%s\\J1939LogTestRelative.log", strPath);
	strcpy_s(g_sLogStruct.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with Relative time */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_AddLoggingBlock(g_sLogStruct) == S_OK );
}

BOOST_AUTO_TEST_CASE( CAN_Logger_Module_Add_Log_Blocks )
{
	// Get the working directory
	CString strPath;
	CString omCurrExe;
    char* pstrExePath = strPath.GetBuffer (MAX_PATH);
	::GetModuleFileName (0, pstrExePath, MAX_PATH);
	strPath.ReleaseBuffer ();	
	strPath = strPath.Left(strPath.ReverseFind(92));		

    /* Add the new logging object in the datastore.  */
    
	/* Select All CAN channels */    
	g_sLogStructCAN.m_ChannelSelected = CAN_CHANNEL_ALL;

	omCurrExe.Format("%s\\CANLogTestSystem.log", strPath);
	strcpy_s(g_sLogStructCAN.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with system time */	
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_AddLoggingBlock(g_sLogStructCAN) == S_OK );
	
	g_sLogStructCAN.m_eLogTimerMode = TIME_MODE_ABSOLUTE;
	/* Set Trigger - Both */
	g_sLogStructCAN.m_sLogTrigger.m_unTriggerType  = BOTH;
	g_sLogStructCAN.m_sLogTrigger.m_unStartID      = 0x1C551005;
	g_sLogStructCAN.m_sLogTrigger.m_unStopID	   = 0x1CEBFF05;

	omCurrExe.Format("%s\\CANLogTestAbs_Rst_OFF.log", strPath);
	strcpy_s(g_sLogStructCAN.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with Absolute time - Reset Timestamp OFF*/	
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_AddLoggingBlock(g_sLogStructCAN) == S_OK );
	
	g_sLogStructCAN.m_bResetAbsTimeStamp = TRUE;
	/* Set Trigger - Start */
	g_sLogStructCAN.m_sLogTrigger.m_unTriggerType  = START;
	g_sLogStructCAN.m_sLogTrigger.m_unStartID      = 0x1C551005;

	omCurrExe.Format("%s\\CANLogTestAbs_Rst_ON.log", strPath);
	strcpy_s(g_sLogStructCAN.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with Absolute time - Reset Timestamp ON*/
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_AddLoggingBlock(g_sLogStructCAN) == S_OK );
	
	g_sLogStructCAN.m_eLogTimerMode = TIME_MODE_RELATIVE;
	/* Set Trigger - Stop */
	g_sLogStructCAN.m_sLogTrigger.m_unTriggerType  = STOP;
	g_sLogStructCAN.m_sLogTrigger.m_unStopID	   = 0x1CEBFF05;

	omCurrExe.Format("%s\\CANLogTestRelative.log", strPath);
	strcpy_s(g_sLogStructCAN.m_sLogFileName, _MAX_PATH, omCurrExe.GetBuffer(_MAX_PATH));
	/* Block with Relative time */
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_AddLoggingBlock(g_sLogStructCAN) == S_OK );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Set_Logging_Block )
{
	/* Set a logging block */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_SetLoggingBlock(3, g_sLogStruct) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_SetLoggingBlock(3, g_sLogStructCAN) == S_OK );

	/* Negative Test case - Try to update a block ID which is out of range*/
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_SetLoggingBlock(5, g_sLogStruct) == S_FALSE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_SetLoggingBlock(5, g_sLogStruct) == S_FALSE );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Configuration_Data )
{
	/* Get logger configuration data */
	BYTE* pbyConfigData = NULL;
	UINT nLength = 0;
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_GetConfigData(&pbyConfigData, nLength) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_GetConfigData(&pbyConfigData, nLength) == S_OK );	

	/* set logger configuration data */
	CString strVersion("Ver1.6.5");
	sg_pouIJ1939Logger->FPJ1_SetConfigData(pbyConfigData, strVersion);
	sg_pouICANLogger->FPC_SetConfigData(pbyConfigData, strVersion);	
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Stop_Editing_Session )
{
	/* Stop editing session */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_StopEditingSession(TRUE) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_StopEditingSession(TRUE) == S_OK );	
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Set_Database_Files )
{
	CStringArray aomDataBaseFiles;
	aomDataBaseFiles.Add("J1939_SAE_71.dbf");
	/* Set database information */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_SetDatabaseFiles(aomDataBaseFiles) == S_OK );
	aomDataBaseFiles.RemoveAll();
	aomDataBaseFiles.Add("SampleCANDB.dbf");
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_SetDatabaseFiles(aomDataBaseFiles) == S_OK );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Set_BaudRate_Details )
{
	/* Set Channel baudrate information */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_SetChannelBaudRateDetails(g_asControllerDetails, g_nChannelCount) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_SetChannelBaudRateDetails(g_asControllerDetails, g_nChannelCount) == S_OK );
}


BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Check_Logging_off )
{
	/* Check to make sure logging is OFF*/
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_IsLoggingON() == FALSE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_IsLoggingON() == FALSE );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Enable_Block )
{
	/* Enable/Disable a particualar logging block */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_EnableLoggingBlock(0, FALSE) == S_OK );
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_EnableLoggingBlock(0, TRUE) == S_OK );

	BOOST_REQUIRE ( sg_pouICANLogger->FPC_EnableLoggingBlock(0, FALSE) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_EnableLoggingBlock(0, TRUE) == S_OK );

	/* Negative test case */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_EnableLoggingBlock(5, TRUE) == S_FALSE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_EnableLoggingBlock(5, TRUE) == S_FALSE );
}

BOOST_AUTO_TEST_CASE( J1939_Logger_Module_Enable_Logging )
{
	char chLog = 0;
	printf("\n Enable CAN(C) Or J1939(J) Logging ? (Please Enter c or j): ");
	scanf("%c", &chLog);
	if ( chLog == 'c')
	{
		/* Enable CAN logging */
		BOOST_REQUIRE ( sg_pouICANLogger->FPC_EnableLogging(TRUE) == S_OK );
	}
	else
	{
		/* Enable J1939 logging */
		BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_EnableLogging(TRUE) == S_OK );
	}	
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Check_Logging_on )
{
	/* Check to make sure logging is ON*/
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_IsLoggingON() == TRUE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_IsLoggingON() == TRUE );
}

BOOST_AUTO_TEST_CASE( J1939_Configure_Timeouts )
{
	/* Configure Timeouts */
	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_BROADCAST, 100) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_RESPONSE, 200) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_HOLDING, 500) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T1, 750) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T2, 1250) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T3, 1250) );

	BOOST_CHECK ( S_OK == DILJ_ConfigureTimeOut(TYPE_TO_T4, 1050) );

	/* Get Timeouts */
	UINT unBroadcast, unResponse, unHolding, unT1, unT2, unT3, unT4;		

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_BROADCAST, unBroadcast) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_RESPONSE, unResponse) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_HOLDING, unHolding) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T1, unT1) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T2, unT2) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T3, unT3) );

	BOOST_CHECK ( S_OK == DILJ_GetTimeOut(TYPE_TO_T4, unT4) );

	BOOST_CHECK ( 100  == unBroadcast );
	BOOST_CHECK ( 200  == unResponse );
	BOOST_CHECK ( 500  == unHolding );
	BOOST_CHECK ( 750  == unT1 );
	BOOST_CHECK ( 1250 == unT2 );
	BOOST_CHECK ( 1250 == unT3 );
	BOOST_CHECK ( 1050 == unT4 );
}

BOOST_AUTO_TEST_CASE( J1939_Addr_Claim_Transmissions )
{	
	/* Claim Address - Success case */
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		for ( int nClientId = 0; nClientId < 2 ; nClientId++ )
		{
			BOOST_CHECK ( S_OK == DILJ_NM_ClaimAddress(g_dwJ1939ClientId[nClientId], nChnlId, g_arrNodeAddr[nClientId]) );
		}
	}

	/* Check if address is claimed or not - both success and failure cases*/	
	BOOST_CHECK ( TRUE  == DILJ_NM_bIsAddressClaimed(g_arrNodeAddr[0]) );
	BOOST_CHECK ( FALSE == DILJ_NM_bIsAddressClaimed(0x15) );

	/* Check if Online - should be true */
	BOOST_CHECK ( TRUE == DILJ_bIsOnline() );

	BYTE bytJ1939Data[] = "12345678123456781234567812345678";
	/* Send J1939 Message through monitor node*/
	Sleep(500);

	/* Fetch Node Name */
	TCHAR strNodeName[256];

	/* Pass a valid registered node address */
	BOOST_CHECK ( S_OK == DILJ_NM_GetNodeName(g_arrNodeAddr[0],strNodeName) );
	BOOST_CHECK ( S_OK == DILJ_NM_GetNodeName(g_arrNodeAddr[1],strNodeName) );

	/* Pass an in-valid registered node address */
	BOOST_CHECK ( S_OK == DILJ_NM_GetNodeName(0x20,strNodeName) );
	
	/* Add message buffer for monitor node*/
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_ADD, g_dwJ1939ClientId[0], &g_ouVSEBufJ1939) );
	/* Try to add it again */
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_ADD, g_dwJ1939ClientId[1], &g_ouVSEBufJ1939) );
	/* Try to add for an invalid Client ID */
	BOOST_CHECK ( ERROR_NOCLIENT == DILJ_ManageMsgBuf(MSGBUF_ADD, 0x255, &g_ouVSEBufJ1939) );


	/* Get Byte address */	
	for ( int nClientId = 0; nClientId < 2 ; nClientId++ )
	{
			BOOST_CHECK ( S_OK == DILJ_NM_GetByteAddres(g_arrNodeAddr[nClientId], 
														g_dwJ1939ClientId[nClientId]) );
	}

	/* Request PGN */		
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		for ( int nClientId = 0; nClientId < 2 ; nClientId++ )
		{
			BOOST_CHECK ( S_OK == DILJ_RequestPGN( g_dwJ1939ClientId[nClientId], 
												   nChnlId, g_arrRqstPGNs[nClientId]) );
		}
	}

	/* Set the callback - all possible cases */
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_LDATA_CONF, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_LDATA_IND, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_BC_LDATA_CONF, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_BC_LDATA_IND, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], CLBCK_FN_NM_ACL, (void*)CallBackSample) );
	BOOST_CHECK ( S_OK == DILJ_SetCallBckFuncPtr(g_dwJ1939ClientId[0], (ETYPE_CLBCK_FN)5, (void*)CallBackSample) );

	/* Send J1939 Broadcast Message through monitor node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[0], 1, MSG_TYPE_BROADCAST, g_arrBroadcastPGNs[0], 
											bytJ1939Data, 14,
											DEFAULT_PRIORITY, g_arrNodeAddr[0], 0x08) );

	Sleep(500);
	/* Send J1939 Broadcast Message through normal node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[1], 1, MSG_TYPE_BROADCAST, g_arrBroadcastPGNs[1], 
											bytJ1939Data, 14,
											DEFAULT_PRIORITY, g_arrNodeAddr[1], 0x58) );
	Sleep(500);

	/* Send J1939 8 byte Data Message through monitor node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[0], 1, MSG_TYPE_DATA, g_arrRqstPGNs[0], bytJ1939Data, 8,
							DEFAULT_PRIORITY, g_arrNodeAddr[0], g_arrNodeAddr[1]) );
	
	/* Send J1939 8 byte Message through normal node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[1], 1, MSG_TYPE_DATA, g_arrRqstPGNs[1], bytJ1939Data, 8,
							DEFAULT_PRIORITY, g_arrNodeAddr[1], g_arrNodeAddr[0]) );

	///* Send J1939 10 byte Data Message through monitor node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[0], 1, MSG_TYPE_DATA, g_arrRqstPGNs[0], bytJ1939Data, 10,
							DEFAULT_PRIORITY, g_arrNodeAddr[0], g_arrNodeAddr[1]) );

	//Sleep(10);
	/* Send J1939  10 byte Data Message through normal node*/
	BOOST_CHECK ( S_OK == DILJ_SendJ1939Msg(g_dwJ1939ClientId[1], 1, MSG_TYPE_DATA, g_arrRqstPGNs[1], bytJ1939Data, 10,
							DEFAULT_PRIORITY, g_arrNodeAddr[1], g_arrNodeAddr[0]) );

	SYSTEMTIME objSysTime;
	UINT64 unAbsTime;

	BOOST_CHECK ( S_OK == DILJ_GetTimeModeMapping(objSysTime, unAbsTime) );

	/* Send Ack Messages */
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		/* Monitor node to Node1 */
		BOOST_CHECK ( S_OK == DILJ_SendAckMsg( g_dwJ1939ClientId[0], nChnlId, 
											   ACK_POS, g_arrRqstPGNs[1], g_arrNodeAddr[0], g_arrNodeAddr[1]) );

		/* Node1 to Monitor node */
		BOOST_CHECK ( S_OK == DILJ_SendAckMsg( g_dwJ1939ClientId[1], nChnlId, 
											   ACK_NEG, g_arrRqstPGNs[0], g_arrNodeAddr[1], g_arrNodeAddr[0]) );
	}

	/* Command Address - Success case */
	/* Monitor Node commanding ECU node to assume address - 0x15 */
	
	g_arrNodeAddr[1] = 0x15;
	for ( int nChnlId = 1; nChnlId <= g_nChannelCount ; nChnlId++ )
	{
		BOOST_CHECK ( S_OK == DILJ_NM_CommandAddress(g_dwJ1939ClientId[0], nChnlId, 
			                                         J1939_NODE_NAME, g_arrNodeAddr[1]));
	}

	/* Request Address from another node */
	DILJ_NM_RequestAddress(g_dwJ1939ClientId[0], 1, DEFAULT_PRIORITY,g_arrNodeAddr[0], g_arrNodeAddr[1]);
	DILJ_NM_RequestAddress(g_dwJ1939ClientId[1], 1, DEFAULT_PRIORITY,g_arrNodeAddr[1], g_arrNodeAddr[0]);
}


BOOST_AUTO_TEST_CASE( J1939_CAN_Closure_Activities )
{
	HRESULT hr = S_FALSE;
	Sleep(500);
	/*                      Go offline                       */
	BOOST_REQUIRE ( S_OK == DILJ_GoOffline() );

	/* Check if Offline - should be false */
	BOOST_REQUIRE ( FALSE == DILJ_bIsOnline() );

	/* Clear message buffer for monitor node*/
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_CLEAR, g_dwJ1939ClientId[0], &g_ouVSEBufJ1939) );
	/* Try to clear it again */
	BOOST_CHECK ( S_OK == DILJ_ManageMsgBuf(MSGBUF_CLEAR, g_dwJ1939ClientId[1], &g_ouVSEBufJ1939) );
	/* Try to clear for an invalid Client ID */
	BOOST_CHECK ( ERROR_NOCLIENT == DILJ_ManageMsgBuf(MSGBUF_CLEAR, 0x255, &g_ouVSEBufJ1939) );

	/* Un-Register client - Montior node */	
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(FALSE, 
                    J1939_MONITOR_NODE, J1939_ECU_NAME, g_arrNodeAddr[0], g_dwJ1939ClientId[0]) );

	/* Un-Register client - Normal J1939 node */	
	BOOST_REQUIRE ( S_OK == DILJ_RegisterClient(FALSE, 
                    _T("Node 1"), J1939_NODE_NAME, g_arrNodeAddr[1], g_dwJ1939ClientId[1]) );

	/* Uninitialize J1939 */
	BOOST_CHECK ( S_OK == DILJ_Uninitialise() );
	/********************************** J1939 Related activities - End  ***********************************/

	/********************************** CAN Closure activities  - Start ***********************************/
	/* Disconnect from CAN network */
	hr = g_pouDIL_CAN_Interface->DILC_StopHardware();
	/********************************** CAN Closure activities  - End  ***********************************/
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Log_String )
{
	CString strMsgJ1939("00:00:00:0000 2 18eeff79 0x00EE00 ACL 79  FF  006 Tx 8 79 00 00 00 00 00 00 80\n");
	CString strMsgCAN("15:28:00:1718 Rx 1 CF00400 x 8 00 7D 7D 00 00 00 F0 7D\n");
	/* Log a string */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_LogString(strMsgJ1939) == S_OK );	
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_LogString(strMsgCAN) == S_OK );
}


BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Disable_Logging )
{	
	if ( sg_pouIJ1939Logger->FPJ1_IsLoggingON() )
	{
		/* Disable J1939 logging */
		BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_EnableLogging(FALSE) == S_OK );
	}
	else
	{
		/* Disable CAN logging */
		BOOST_REQUIRE ( sg_pouICANLogger->FPC_EnableLogging(FALSE) == S_OK );
	}
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Reset )
{
	/* Reset */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_Reset() == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_Reset() == S_OK );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Confirm )
{
	/* negative case - try to confirm without enabling editing session */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_Confirm() == S_FALSE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_Confirm() == S_FALSE );

	sg_pouIJ1939Logger->FPJ1_StartEditingSession();
	sg_pouICANLogger->FPC_StartEditingSession();
	/* confirm */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_Confirm() == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_Confirm() == S_OK );

	sg_pouIJ1939Logger->FPJ1_StopEditingSession(TRUE);
	sg_pouICANLogger->FPC_StopEditingSession(TRUE);
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Get_Block_Count )
{
	/* Make sure the logginf block count is 4 */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_GetLoggingBlockCount() == 4 );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_GetLoggingBlockCount() == 4 );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Get_Logging_Block )
{
	/* Try to get a logging block */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_GetLoggingBlock(3, g_sLogStruct) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_GetLoggingBlock(3, g_sLogStruct) == S_OK );
	/* negative test case - Try to get a block ID which is out of bounds */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_GetLoggingBlock(6, g_sLogStruct) == S_FALSE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_GetLoggingBlock(6, g_sLogStruct) == S_FALSE );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Remove_Logging_Block )
{
	/* Remove logging block */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_RemoveLoggingBlock(3) == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_RemoveLoggingBlock(3) == S_OK );
	/* Negative Test case - Try to Remove logging block which doesn't exist */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_RemoveLoggingBlock(4) == S_FALSE );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_RemoveLoggingBlock(4) == S_FALSE );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Clear_Log_Blocks )
{
	/* Remove all the logging blocks */
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_ClearLoggingBlockList() == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_ClearLoggingBlockList() == S_OK );

	/* Try to clear while editing is ON */
	sg_pouIJ1939Logger->FPJ1_AddLoggingBlock(g_sLogStruct);
	sg_pouICANLogger->FPC_AddLoggingBlock(g_sLogStruct);	
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_StartEditingSession() == S_OK );	
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_ClearLoggingBlockList() == S_OK );
	BOOST_REQUIRE ( sg_pouIJ1939Logger->FPJ1_StopEditingSession(TRUE) == S_OK );

	BOOST_REQUIRE ( sg_pouICANLogger->FPC_StartEditingSession() == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_ClearLoggingBlockList() == S_OK );
	BOOST_REQUIRE ( sg_pouICANLogger->FPC_StopEditingSession(TRUE) == S_OK );
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Check_Thread_Block_Status )
{
	BOOL bThreadBlocked = sg_pouIJ1939Logger->FPJ1_IsJ1939ThreadBlocked();
	BOOL bDataLogged    = sg_pouIJ1939Logger->FPJ1_IsJ1939DataLogged();
	sg_pouIJ1939Logger->FPJ1_DisableJ1939DataLogFlag();

	sg_pouICANLogger->FPC_IsThreadBlocked();
	sg_pouICANLogger->FPC_IsDataLogged();
	sg_pouICANLogger->FPC_DisableDataLogFlag();
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_Filters_Yet_To_Implement )
{
	/* Test filter functions */
	SFILTERAPPLIED_J1939 objFilterJ1939;	
	sg_pouIJ1939Logger->FPJ1_ApplyFilteringScheme(0, objFilterJ1939);
	sg_pouIJ1939Logger->FPJ1_EnableFilter(0, TRUE);
	BOOL bFilterON = sg_pouIJ1939Logger->FPJ1_IsFilterON();
	sg_pouIJ1939Logger->FPJ1_GetFilteringScheme(0, objFilterJ1939);
	sg_pouIJ1939Logger->FPJ1_GetFilteringScheme(2, objFilterJ1939);

	SFILTERAPPLIED_CAN objFilterCAN;
	sg_pouICANLogger->FPC_ApplyFilteringScheme(0, objFilterCAN);
	sg_pouICANLogger->FPC_EnableFilter(0, TRUE);
	BOOL bFilterCAN_ON = sg_pouICANLogger->FPC_IsFilterON();
	sg_pouICANLogger->FPC_GetFilteringScheme(0, objFilterCAN);
	sg_pouICANLogger->FPC_GetFilteringScheme(2, objFilterCAN);
}

BOOST_AUTO_TEST_CASE( J1939_CAN_Logger_Module_ExitInstance )
{
	/* Exit instance of J1939 logging module */
	BOOST_REQUIRE ( ((CFrameProcessor_J1939*)sg_pouIJ1939Logger)->ExitInstance() == S_OK );
	delete sg_pouIJ1939Logger;

	/* Exit instance of CAN logging module */
	BOOST_REQUIRE ( ((CFrameProcessor_CAN*)sg_pouICANLogger)->ExitInstance() == S_OK );
	delete sg_pouICANLogger;
}


BOOST_AUTO_TEST_CASE( J1939_Classes_And_Utility_Functions )
{
	BYTE bytDataAck[8]= {0x00};
	PrepareEOM_ACK(bytDataAck, 14, 2, 0x5500);

	BYTE bytDataCTS[8]= {0x00};
	PrepareClear_2_Send(bytDataCTS, 2, 1, 0xFDC5);

	BYTE bytDataCM[8]= {0x00};
	vPrepareData(bytDataCM, 0x16, 16, 0xFE05);

	bIsCommandAddress(PDU_FORMAT_ACL, DATA_LEN_CMD_ADDRESS);

	/* RQST message type */
	BYTE bytDataRQSTMsg[8] = {0x00, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BOOST_CHECK ( MSG_TYPE_REQUEST == eGetMsgType(0x1CEA5800, bytDataRQSTMsg) );  	
	bytDataRQSTMsg[1] = PDU_FORMAT_ACL;	
	BOOST_CHECK ( MSG_TYPE_NM_RQST_ACL == eGetMsgType(0x1CEA5800, bytDataRQSTMsg) );  

	/* RTS message */
	BYTE bytDataMsg[8] = {0x10, 0x10, 0x00, 0x03, 0xFF, 0x00, 0xA8, 0x00};
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_RTS == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* BAM message */
	bytDataMsg[0] = CB_BAM;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_BAM == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* CTS message */
	bytDataMsg[0] = CB_CLEAR_TO_SEND;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_CTS == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* CON_ABORT message */
	bytDataMsg[0] = CB_CON_ABORT;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_CON_ABORT == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* EOM ACK message */
	bytDataMsg[0] = CB_EOM_ACK;
	BOOST_CHECK ( MSG_TYPE_NM_TPCM_EOM_ACK == eGetMsgType(0x1CEC5800, bytDataMsg) );  

	/* CConnectionDet class testing */	
	CConnectionDet* pConDet = new CConnectionDet(0x05, 0x10);	
	BOOL bRet = pConDet->bIsMsgRxForThisConnection(0xCF004EE);
	pConDet->bIsMsgRxForThisConnection(0xCF004EE);

	/* CNodeConManager class testing */
	CNodeConManager* pNodeConMgr = NULL;
	pNodeConMgr = new CNodeConManager(0, J1939_MONITOR_NODE, 0x8000000000000025, 0x25);			
	pNodeConMgr->bAddConDetObj(pConDet);

	pNodeConMgr->vStartTimerFunction(1000);
	//pNodeConMgr->vSendRequestForPGN(0xFECA, 0xFF, 1);	//private function
	pNodeConMgr->vSendConAckMsg(0x05, 4, 0xD400, 1);
	//pNodeConMgr->vSendConAbortMsg(0xCD40005, 0x5, 1); //private function
	//pNodeConMgr->vSendACLMsg();	//private function	
	BYTE bytCount = pNodeConMgr->byGetConCount();

	pConDet = pNodeConMgr->pouGetConDetObj(bytCount-1);		
	if ( pConDet )
	{		
		STJ1939_MSG msgJ1939;
		pNodeConMgr->SendLongMsg(&msgJ1939, pConDet, DATA_CLEAR_2_SEND);		
	}	

	BOOL bStopped = pNodeConMgr->bStopTimerFunction();

	pNodeConMgr->WaitForMorePackets();
	pNodeConMgr->WaitAfterClear2Send();
	
	delete pConDet;
	pConDet = NULL;	

	/* CTransferLayer class testing */
	CTransferLayer::ouGetTransLayerObj().lTConnectReq(0,0,CM_BROADCAST);
	CTransferLayer::ouGetTransLayerObj().vTConnectCon(0, T_CONNECTED, 0, CM_STANDARD);
	CTransferLayer::ouGetTransLayerObj().vTConnectInd( 0, 0, 0);
	CTransferLayer::ouGetTransLayerObj().vTDisconnectInd(0, T_SHUTDOWN);
	CTransferLayer::ouGetTransLayerObj().vTLongDataCon(0, 'F');
	CTransferLayer::ouGetTransLayerObj().vTConTestCon( 0, 0, 0, 0);
	CTransferLayer::ouGetTransLayerObj().vTBroadDataInd( 0, 0, NULL);

}
BOOST_AUTO_TEST_SUITE_END()