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
#include "DIL_Interface/BaseDIL_J1939.h"
#include "DIL_Interface/Dil_CAN.h"
#include "DIL_Interface/DILC_Dummy.h"
#include "WrapperErrorLogger.h" //For Logging Error in DIL Interface 
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "CAN_Vector_XL/CAN_Vector_XL_Extern.h"

/* Global variables to hold CAN DIL parameters */
CBaseDIL_CAN* g_pouDIL_CAN_Interface = NULL; // CAN   DIL interface
CBaseDILI_J1939* g_pouDIL_J1939      = NULL; // J1939 DIL interface
CDIL_CAN_DUMMY*  g_pouDIL_CAN_DUMMY = new CDIL_CAN_DUMMY;
DILLIST g_ouList;// List of the driver interface layers supported
DWORD g_dwDriverId = DRIVER_CAN_VECTOR_XL;
WrapperErrorLogger g_ouWrapperLogger;	
INTERFACE_HW_LIST g_asINTERFACE_HW;
DWORD g_dwClientID = 0;
SCONTROLLER_DETAILS g_asControllerDetails[defNO_OF_CHANNELS];
INT g_nChannelCount = 0;
CCANBufFSE g_ouCanBuf;
UINT g_unDILCount = 0;
string g_acErrStr = "";
CBaseDIL_CAN_Controller* g_pBaseDILCAN_Controller;

HRESULT InitializeDIL(void)
{   			
    HRESULT hResult = S_OK;
    if (g_pouDIL_CAN_Interface == NULL)
    {		
        hResult = DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);		
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

HRESULT hGetControllerParams()
{
	LONG lParam= 0;
	/* Get Number of channels */
    BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK );
	/* Get Number of connected channels */
    BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_CONNECTED_HW) == S_OK );
	/* Get driver status */
    BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, DRIVER_STATUS) == S_OK );
	/* Get HW mode */
    BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK );
	/* Make a connection test*/
    BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, CON_TEST) == S_OK );	
	/* Negative case*/
    BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, (ECONTR_PARAM)22) == S_FALSE );	
	
	return S_OK;
}
HRESULT hSetControllerParams()
{
	/* Set passive mode */
	BOOST_CHECK (g_pouDIL_CAN_Interface->DILC_SetControllerParams(defMODE_PASSIVE, HW_MODE) == S_OK );
	/* Set active mode */
	BOOST_CHECK (g_pouDIL_CAN_Interface->DILC_SetControllerParams(defMODE_ACTIVE, HW_MODE) == S_OK );

	return S_OK;
}


void vConfigChannelSelection() 
{        
	INT nCount = CHANNEL_ALLOWED;
	HRESULT hResult = g_pouDIL_CAN_Interface->DILC_DeselectHwInterfaces();

    // If the deselection of interfaces is not appropriate
    BOOST_REQUIRE (S_OK == hResult );            

    if (g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(g_asINTERFACE_HW, nCount) == S_OK)
    {					
		hResult = g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(g_asINTERFACE_HW, nCount);
		if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
		{
			/* Updates the number of channels selected */
			g_nChannelCount = nCount; 
		}
	}	
}

BOOST_AUTO_TEST_SUITE( DIL_Exports_Tester )

BOOST_AUTO_TEST_CASE( CAN_Get_DIL_List )
{		
	BOOST_REQUIRE (DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface) == S_OK);		
	g_unDILCount = g_pouDIL_CAN_Interface->DILC_GetDILList(false, &g_ouList);
}

BOOST_AUTO_TEST_CASE( CAN_Initializations_simulation )
{
	HRESULT hr = S_FALSE;
	SYSTEMTIME CurrSysTime;
	UINT64 TimeStamp;
	HANDLE hEvent;
	UINT unStatus;

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
	
	/********************************** CAN Related activities  - Start ************************************/

	for ( int i = 0; i < g_unDILCount ; i++ )
	{
		/* Assign driver ID */
		g_dwDriverId = g_ouList[i].m_dwDriverID;

		/* Ignore list */
		if ( g_dwDriverId == DRIVER_CAN_ETAS_ES581 ||
			 g_dwDriverId == DRIVER_CAN_ICS_NEOVI  ||
			 g_dwDriverId == DRIVER_CAN_IXXAT      ||
			 g_dwDriverId == DRIVER_CAN_MHS        ||
			 g_dwDriverId == DRIVER_CAN_VECTOR_XL  ||
			 g_dwDriverId == DRIVER_CAN_PEAK_USB   )
		{
			 continue;
		}

		/* Initialize CAN DIL section */
		hr = InitializeDIL();

		if (hr == S_OK)
		{
			/* Get controller status */
			BOOST_CHECK (g_pouDIL_CAN_Interface->DILC_GetCntrlStatus(hEvent, unStatus) == S_OK );

			/* Verify selected driver */
			BOOST_REQUIRE ( g_pouDIL_CAN_Interface->DILC_GetSelectedDriver() == g_dwDriverId );

			if ( g_dwDriverId != DRIVER_CAN_STUB )	/* If not a simulation driver */
			{
				/* Display controller configuration dialog */
				int nSize = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
				g_pouDIL_CAN_Interface->DILC_DisplayConfigDlg(g_asControllerDetails, nSize);
				
				/* Show hardware selection dialog */
				vConfigChannelSelection();
			}

			/* Connect to CAN network */
			hr = g_pouDIL_CAN_Interface->DILC_StartHardware();
			Sleep(1000);

			SYSTEMTIME CurrSysTime;
			UINT64 TimeStamp;
			/* Get Time mode mapping */
			g_pouDIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTime, TimeStamp);

			/* Get controller parameters */
			hGetControllerParams();

			/* Set controller parameters */
			hSetControllerParams();

			/* Add message buffer */
			BOOST_CHECK ( g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, g_dwClientID, &g_ouCanBuf) == S_OK );
			/* Negative case */
			BOOST_CHECK ( g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, NULL, &g_ouCanBuf) == S_FALSE);

			if (hr == S_OK)
			{
				Sleep(1000);
				for ( int i = 0; i < 255; i++ )
				{
					/* Send a test CAN message */
					hr = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, sMsg);
					/* Introduce a delay of 10 msec */
					Sleep(10);
					/* Get any error strings */
					g_pouDIL_CAN_Interface->DILC_GetLastErrorString(g_acErrStr);

					SERROR_CNT sErrorCounter;
					sErrorCounter.m_ucRxErrCount = 0;
					sErrorCounter.m_ucTxErrCount = 0;
					/* Get error count */
					g_pouDIL_CAN_Interface->DILC_GetErrorCount( sErrorCounter, 0, ERR_CNT);
					g_pouDIL_CAN_Interface->DILC_GetErrorCount( sErrorCounter, 0, PEAK_ERR_CNT);				
				}
			}

			/* Clear message buffer */
			BOOST_CHECK ( g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, g_dwClientID, &g_ouCanBuf) == S_OK );
			/* Negative case */
			BOOST_CHECK ( g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, NULL, &g_ouCanBuf) == S_OK);

			/* Reset CAN hardware */
			//hr = g_pouDIL_CAN_Interface->DILC_ResetHardware();

			/* Disconnect from CAN network */
			hr = g_pouDIL_CAN_Interface->DILC_StopHardware();
		}
		else
		{
			BOOST_FAIL( "Failed to Initialize CAN Network..." );
			exit(0);
		}
	}
	/* Test Dummy interfaces */
	((CDIL_CAN*)g_pouDIL_CAN_Interface)->vSelectInterface_Dummy();
	g_pouDIL_CAN_Interface->DILC_PerformInitOperations();	
	g_pouDIL_CAN_Interface->DILC_DeselectHwInterfaces();
	int nLength = 0, nCount = 0;
	g_pouDIL_CAN_Interface->DILC_DisplayConfigDlg(g_asControllerDetails, nLength);
	g_pouDIL_CAN_Interface->DILC_GetCntrlStatus(hEvent, unStatus);
	LONG lng;
	g_pouDIL_CAN_Interface->DILC_GetControllerParams(lng, 0, NUMBER_HW);
	SERROR_CNT sErrorCounter;
	g_pouDIL_CAN_Interface->DILC_GetErrorCount(sErrorCounter, 0, ERR_CNT);
	g_pouDIL_CAN_Interface->DILC_GetLastErrorString(g_acErrStr);
	g_pouDIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTime, TimeStamp);
	g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(g_asINTERFACE_HW, nCount);
	g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, g_dwClientID, &g_ouCanBuf);
	g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
	g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
	//g_pouDIL_CAN_Interface->DILC_ResetHardware();
	g_pouDIL_CAN_Interface->DILC_SelectDriver(g_dwDriverId, NULL, &g_ouWrapperLogger);
	g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(g_asINTERFACE_HW, nCount);
	g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, sMsg);
	g_pouDIL_CAN_Interface->DILC_SetConfigData(g_asControllerDetails, nLength);
	g_pouDIL_CAN_Interface->DILC_SetControllerParams(defMODE_PASSIVE, HW_MODE);

	s_STATUSMSG objMsg;
	FILTER_TYPE FilterType;	
	UINT* punMsgIds;
	
	//g_pouDIL_CAN_DUMMY->CAN_FilterFrames(FilterType, CHANNEL_CAN_MIN, punMsgIds, 0);
	g_pouDIL_CAN_DUMMY->CAN_GetCurrStatus(objMsg);
	g_pouDIL_CAN_DUMMY->CAN_LoadDriverLibrary();
	g_pouDIL_CAN_DUMMY->CAN_SelectHwInterface(g_asINTERFACE_HW, nCount);
	g_pouDIL_CAN_DUMMY->CAN_SendMsg(g_dwClientID, sMsg);
	g_pouDIL_CAN_DUMMY->CAN_SetAppParams(NULL, &g_ouWrapperLogger);
	g_pouDIL_CAN_DUMMY->CAN_SetConfigData(g_asControllerDetails, nLength);
	g_pouDIL_CAN_DUMMY->CAN_SetControllerParams(defMODE_PASSIVE, HW_MODE);
	g_pouDIL_CAN_DUMMY->CAN_StartHardware();
	g_pouDIL_CAN_DUMMY->CAN_StopHardware();
	g_pouDIL_CAN_DUMMY->CAN_UnloadDriverLibrary(); 

	/* DIL Interface calls */
	DIL_GetInterface(J1939, (void**)&g_pouDIL_J1939);
	/********************************** CAN Related activities  - End    ***********************************/	
}

BOOST_AUTO_TEST_CASE( CAN_simulation_Start_Transmission)
{
}




BOOST_AUTO_TEST_SUITE_END()