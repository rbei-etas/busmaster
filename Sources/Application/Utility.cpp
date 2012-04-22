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
 * \file      Utility.cpp
 * \brief     This file contain all global utility function definition 
 * \authors   Amitesh Bharti, Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain all global utility function definition 
 */

#include "stdafx.h" 
// For "SSENDMSGINFO" structure defn
#include "Struct.h" 
// For all utility function definition
#include "Utility.h"
// Definition of App class
#include "BUSMASTER.h"
// Macro's are defined here
#include "HashDefines.h" 
// Mainframe definition file
#include "MainFrm.h" 

//#include "ExecuteManager.h"

using namespace std;

//extern HWND GUI_hDisplayWindow;
extern CCANMonitorApp theApp;
extern BOOL g_bStopSendMultMsg;

// Handle of the main GUI (mainframe)
extern HWND g_hMainGUI;

extern long lThreadCount;
extern BOOL gbSendStrToTrace(CHAR* pcOutStrTrace);
extern BOOL g_bReadDllMsg;
extern  CRITICAL_SECTION g_CritSectNodeBufferRead;

extern CEvent omMainTimerEvent;
extern CRITICAL_SECTION g_omDllMsgCritiSec;
 


/******************************************************************************/
/*  Function Name    :  bGetMsgInfo                                           */
/*  Input(s)         :  UINT unMsgIndex,                                      */
/*                      UCHAR aunData[]                                       */
/*                      PSENDMULTIMSGINFO psSendMultipleMsgInfo               */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Gets data bytes                                       */
/*  Member of        :  Global function                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath S                                            */
/*  Date Created     :  08.11.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  31.12.2002, function name & second parameter changed. */
/******************************************************************************/
BOOL bGetMsgData(UINT unMsgIndex,
                 UCHAR aunData[],
                 PSENDMULTIMSGINFO psSendMultipleMsgInfo)
{
    BOOL bReturn = FALSE;
    if ( psSendMultipleMsgInfo != NULL)
    {
        for ( INT nCount = 0; 
        nCount < psSendMultipleMsgInfo->pSendMsgInfo[unMsgIndex].nDLC;
        nCount++)
        {
            aunData[nCount] = 
                (UCHAR)psSendMultipleMsgInfo->pSendMsgInfo[unMsgIndex].
                    pSendSgInfo[nCount].nValue;
        }
        bReturn = TRUE;
    }
    return 1;
}


/******************************************************************************
    Function Name    :  unKeyHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined key 
                        handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        user-defined key handler function.
                        - User will be notified status of transmission.
    Member of        :  CExecuteFunc
    Friend of        :      -
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  06.11.2002
    Modifications    :  Anish kumar
						30.12.05 moved to CExecuteFunc ,refer CEx members
******************************************************************************/
UINT unKeyHandlerProc(LPVOID /*pParam*/)
{
    return 0;
}
/******************************************************************************
    Function Name    :  unErrorHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined error 
                        handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        user-defined error handler function.
                        - User will be notified status of transmission.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  25.02.2003
    Modifications    :  
******************************************************************************/
UINT unErrorHandlerProc(LPVOID /*pParam*/)
{
    return 0;
}

/******************************************************************************
    Function Name    :  unDLLUnloadHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined DLL handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        user-defined DLL unload handler function.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  22.07.2004
    Modifications    :  
******************************************************************************/
UINT unDLLUnloadHandlerProc(LPVOID /*pParam*/)
{
    return 0;
}

/******************************************************************************
    Function Name    :  unDLLloadHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined DLL handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        user-defined DLL load handler function.
                        - User will be notified status of transmission.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Ravikumar Patil
    Date Created     :  10.03.2003
    Modifications    :  Amitesh Bharti on 22.07.2004
                        Renamed the function as unDLLloadHandlerProc as the DLL
                        unload functionality got seprated
******************************************************************************/


UINT unDLLloadHandlerProc(LPVOID /*pParam*/)
{
    return 0;
}

/******************************************************************************
    Function Name    :  unReadNodeMsgHandlerBuffer
    Input(s)         :  pParam - Typecasted address CExecuteFunc object
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        reading of CExecutFunc's buffer
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  22.12.2005
    Modification By  :  Anish Kumar                                                      
    Modification on  :  29.01.07, Removed memory leak due to msg handler thread 
						termination 
******************************************************************************/
UINT unReadNodeMsgHandlerBuffer(LPVOID /*pParam*/)
{
    return 0;
}

/*************************************************************************
    Function Name    :  TimerProc
    Input(s)         : 
    Output           :  
    Functionality    :  Timer call back function 
    Member of        :  Global timer thread function
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
**************************************************************************/
void CALLBACK TimerProc(UINT /*m_unTimerId*/, UINT /*uMsg*/, DWORD* /*dwUser*/,
						                    DWORD* /*dw1*/, DWORD* /*dw2*/)
{
	//CExecuteManager::ouGetExecuteManager().vManageTimerSetEvent();
	//omMainTimerEvent.SetEvent();	
}

/*************************************************************************
    Function Name    : unTimerHandlerProc
    Input(s)         : PSTIMERHANDLER
    Output           :  
    Functionality    : Execute the task assigned to each timer
    Member of        :  Global timer handler
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
**************************************************************************/

UINT unTimerHandlerProc(LPVOID /*pParam*/)
{
			return 0;
}


/*************************************************************************
    Function Name    : CalcTimersExecTime
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        : 
    Author(s)        :  Anish kumar
    Date Created     :  
**************************************************************************/

UINT CalcTimersExecTime(LPVOID /*pParam*/)
{
	return 0;
}
/*************************************************************************
    Function Name    : unGetMsgIDFromName
    Input(s)         : Message name with ID attached in it MsgName[0xMsgID]
    Output           :  
    Functionality    : 
    Member of        : 
    Author(s)        :  Anish kumar
    Date Created     :  
**************************************************************************/
UINT unGetMsgIDFromName(CString omMsgName)
{
	CString omStrMsgID;
	UINT unMsgID = (UINT)-1;
	CHAR* pcStopStr = NULL;
	int nIndex = omMsgName.ReverseFind(defMSGID_EXTENDED);
	if(nIndex != -1)
	{
		int nLength = omMsgName.GetLength(); 
		omStrMsgID = omMsgName.Mid(nIndex+1,nLength-1);
		unMsgID = strtol((LPCTSTR )omStrMsgID,&pcStopStr,16);
	}
	return unMsgID;
}