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
 * \file      VectorDILTester.cpp
 * \brief     Source file
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CFrameProcessor_Common class realising the
 */


// VectorDILTester.cpp : Defines the initialization routines for the DLL.
//

#include <DIL_Tester_StdAfx.h>
#include <tchar.h>
//---------------------------------------------------------------------------

#pragma argsused

//---------------------------------------------------------------------------

#define BOOST_TEST_MODULE MyTest
#define USAGE_IMPORT 1
#include <boost/test/unit_test.hpp>


#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/Dil_CAN.h"
#include "WrapperErrorLogger.h" //For Logging Error in DIL Interface 
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "CAN_STUB/CAN_STUB_Extern.h"
//#include "BusEmulation/stdafx_BusSim.h"
//#include "BusEmulation/SimENG.h"
//#include <objBase.h>

typedef HRESULT (__stdcall *DLLGETCLASSOBJECT)(REFCLSID rclsid, REFIID riid, LPVOID * ppvObj);
static DLLGETCLASSOBJECT pfDLLGetClassObject;


/* Declaration of variables */
WrapperErrorLogger g_ouWrapperLogger;
INTERFACE_HW_LIST g_asINTERFACE_HW;
INT g_nCount = defNO_OF_CHANNELS;
DWORD g_dwClientID = 0;
SCONTROLLER_DETAILS g_asControllerDetails[defNO_OF_CHANNELS];
CBaseDIL_CAN_Controller* g_pBaseDILCAN_Controller = NULL;
CCANBufFSE g_ouCanBuf;
string g_acErrStr = "";

typedef HRESULT (__stdcall *GETIDIL_CAN_CONTROLLER)(void** ppvInterface);
static GETIDIL_CAN_CONTROLLER pfGetIDILCAN_Controller;

BOOST_AUTO_TEST_SUITE( VECTOR_DIL_Tester )

BOOST_AUTO_TEST_CASE( DIL_Object_Creation)
{	
	/* Get Vector DIL interface */
	GetIDIL_CAN_Controller((void**)&g_pBaseDILCAN_Controller);
	BOOST_REQUIRE ( g_pBaseDILCAN_Controller!=NULL );
}

BOOST_AUTO_TEST_CASE( DIL_Perform_Initializations)
{
	HWND hWnd = GetDesktopWindow();
	/* Provide logger object */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_SetAppParams(hWnd, &g_ouWrapperLogger) == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Load_Driver_Library)
{
	/* Load respective driver Library */
	HRESULT hResult = g_pBaseDILCAN_Controller->CAN_LoadDriverLibrary();
	BOOST_REQUIRE ( hResult == S_OK );	
}

BOOST_AUTO_TEST_CASE( DIL_Initial_Operations)
{
	/* perform initial operations in DIL */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_PerformInitOperations() == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_List_Interfaces)
{
	/* List hardware interfaces available */	
	BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_ListHwInterfaces(g_asINTERFACE_HW, g_nCount) == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Select_Interfaces)
{
	/* Select hardware interfaces */	
	HRESULT hResult = g_pBaseDILCAN_Controller->CAN_SelectHwInterface(g_asINTERFACE_HW, g_nCount);
	BOOST_REQUIRE ( hResult == S_OK || hResult == HW_INTERFACE_ALREADY_SELECTED );
}

BOOST_AUTO_TEST_CASE( DIL_Register_Client)
{	
	/* Register Client */
	HRESULT hResult = g_pBaseDILCAN_Controller->CAN_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
	BOOST_CHECK ( hResult == S_OK  );
	/* Register Client - Negative case */
	hResult = g_pBaseDILCAN_Controller->CAN_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
	BOOST_CHECK ( hResult == ERR_CLIENT_EXISTS  );
}

BOOST_AUTO_TEST_CASE( DIL_Set_Config_Data)
{	
	/* Set controller configuration data */	
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_SetConfigData(g_asControllerDetails, 
				  sizeof(SCONTROLLER_DETAILS) * g_nCount) == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Get_Cntrlr_Status)
{	
	HANDLE hEvent;
	UINT unStatus;
	/* Get controller status */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_GetCntrlStatus(hEvent, unStatus) == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Display_Controller_Config_Dlg)
{	
	/* Display controller configuration dialog */
	int nSize = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
	g_pBaseDILCAN_Controller->CAN_DisplayConfigDlg(g_asControllerDetails, nSize);	
}

BOOST_AUTO_TEST_CASE( DIL_Show_Hardware_Selection_Dlg)
{
	/* Show hardware selection dialog */
	INT nCount = CHANNEL_ALLOWED;
	HRESULT hResult = g_pBaseDILCAN_Controller->CAN_DeselectHwInterface();

    // If the deselection of interfaces is not appropriate
    BOOST_REQUIRE (S_OK == hResult );            

    if (g_pBaseDILCAN_Controller->CAN_ListHwInterfaces(g_asINTERFACE_HW, nCount) == S_OK)
    {					
		hResult = g_pBaseDILCAN_Controller->CAN_SelectHwInterface(g_asINTERFACE_HW, nCount);
		if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
		{ 
			/* Updates the number of channels selected */
			g_nCount = nCount;
		}
	}
	Sleep(100);
}

BOOST_AUTO_TEST_CASE( DIL_Connect_To_CAN)
{
	/* Connect to CAN network */
	BOOST_REQUIRE (S_OK == g_pBaseDILCAN_Controller->CAN_StartHardware() ); 
	Sleep(1000);
}

BOOST_AUTO_TEST_CASE( DIL_Get_Connect_Time)
{
	SYSTEMTIME CurrSysTime;
	UINT64 TimeStamp;
	/* Get Time mode mapping */	
	g_pBaseDILCAN_Controller->CAN_GetTimeModeMapping(CurrSysTime, TimeStamp);
}

BOOST_AUTO_TEST_CASE( DIL_Get_Controller_Params)
{
	LONG lParam= 0;
	/* Get Number of channels */
    BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK );
	/* Get Number of connected channels */
    BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, 0, NUMBER_CONNECTED_HW) == S_OK );
	/* Get driver status */
    BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, 0, DRIVER_STATUS) == S_OK );
	/* Get HW mode */
    BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, 0, HW_MODE) == S_OK );
	/* Make a connection test*/
    BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, 0, CON_TEST) == S_OK );	
	/* Negative case*/
    BOOST_REQUIRE ( g_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, 0, (ECONTR_PARAM)22) == S_FALSE );	
}

BOOST_AUTO_TEST_CASE( DIL_Set_Controller_Params)
{
	/* Set passive mode */
	BOOST_CHECK (g_pBaseDILCAN_Controller->CAN_SetControllerParams(defMODE_PASSIVE, HW_MODE) == S_OK );
	/* Set active mode */
	BOOST_CHECK (g_pBaseDILCAN_Controller->CAN_SetControllerParams(defMODE_ACTIVE, HW_MODE) == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Add_Message_Buffer)
{
	/* Add message buffer */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_ManageMsgBuf(MSGBUF_ADD, g_dwClientID, &g_ouCanBuf) == S_OK );
	/* Negative case */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_ManageMsgBuf(MSGBUF_ADD, NULL, &g_ouCanBuf) == S_FALSE);
}

BOOST_AUTO_TEST_CASE( DIL_Transmit_Catch_Error_If_Any)
{
	Sleep(1000);
	/* CAN message */
	STCAN_MSG sMsg;

	/* Initialise message structure */
	sMsg.m_unMsgID = 0x100;              // Message ID
	sMsg.m_ucEXTENDED = FALSE;           // Standard Message type
	sMsg.m_ucRTR = FALSE;                // Not RTR type
	sMsg.m_ucDataLen = 8;                // Length is 8 Bytes
	for( int i = 0; i<8;i++)
		sMsg.m_ucData[i] = i+1;
	sMsg.m_ucData[7] = 0;

	sMsg.m_ucChannel = 1;                // First CAN channel

	for ( int i = 0; i < 255; i++ )
	{
		/* Send a test CAN message */
		HRESULT hr = g_pBaseDILCAN_Controller->CAN_SendMsg(g_dwClientID, sMsg);
		/* Introduce a delay of 10 msec */
		Sleep(10);
		/* Get any error strings */
		g_pBaseDILCAN_Controller->CAN_GetLastErrorString(g_acErrStr);

		SERROR_CNT sErrorCounter;
		sErrorCounter.m_ucRxErrCount = 0;
		sErrorCounter.m_ucTxErrCount = 0;
		/* Get error count */
		g_pBaseDILCAN_Controller->CAN_GetErrorCount( sErrorCounter, 0, ERR_CNT);
		g_pBaseDILCAN_Controller->CAN_GetErrorCount( sErrorCounter, 0, PEAK_ERR_CNT);
		g_pBaseDILCAN_Controller->CAN_GetErrorCount( sErrorCounter, 10, ERR_CNT);
	}
	/* Negative test cases for CAN Send message */
	/* Invalid Client ID */
	g_pBaseDILCAN_Controller->CAN_SendMsg(5, sMsg);
	/* Invalid Client ID */
	sMsg.m_ucChannel = 5;                // Invalid CAN channel
	g_pBaseDILCAN_Controller->CAN_SendMsg(g_dwClientID, sMsg);

}

BOOST_AUTO_TEST_CASE( DIL_Clear_Message_Buffer)
{
	/* Clear message buffer */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_ManageMsgBuf(MSGBUF_CLEAR, g_dwClientID, &g_ouCanBuf) == S_OK );
	/* Negative case */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_ManageMsgBuf(MSGBUF_CLEAR, NULL, &g_ouCanBuf) == S_FALSE);
}

BOOST_AUTO_TEST_CASE( DIL_Unregister_Client)
{	
	/* Unregister Client */
	HRESULT hResult = g_pBaseDILCAN_Controller->CAN_RegisterClient(FALSE, g_dwClientID, _T("CAN_MONITOR"));
	BOOST_CHECK ( hResult == S_OK  );
	/* Unregister Client - Negative case */
	hResult = g_pBaseDILCAN_Controller->CAN_RegisterClient(FALSE, g_dwClientID, _T("CAN_MONITOR"));
	BOOST_CHECK ( hResult == ERR_CLIENT_EXISTS  );
}

BOOST_AUTO_TEST_CASE( DIL_Disconnect_From_CAN)
{
	/* Disconnect from CAN network */
	s_STATUSMSG stStatusData;
	g_pBaseDILCAN_Controller->CAN_GetCurrStatus(stStatusData);
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_StopHardware() == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Reset_CAN)
{
	/* Reset CAN hardware */
//	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_ResetHardware() == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Perform_Closure_Operations)
{
	/* Perform Closure Operations for DIL */
	BOOST_CHECK ( g_pBaseDILCAN_Controller->CAN_PerformClosureOperations() == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Unload_Driver_Library)
{
	/* Load respective driver Library */
	HRESULT hResult = g_pBaseDILCAN_Controller->CAN_UnloadDriverLibrary();
	BOOST_CHECK ( hResult == S_OK );
}

BOOST_AUTO_TEST_CASE( DIL_Negative_Test_Cases)
{
	STCAN_MSG sMsg;
	SERROR_CNT sErrorCounter;
	//g_pBaseDILCAN_Controller->CAN_StartHardware();
	g_pBaseDILCAN_Controller->CAN_SelectHwInterface(g_asINTERFACE_HW, g_nCount);
	g_pBaseDILCAN_Controller->CAN_GetErrorCount( sErrorCounter, 10, ERR_CNT);
	
	delete g_pBaseDILCAN_Controller;
}

BOOST_AUTO_TEST_SUITE_END()