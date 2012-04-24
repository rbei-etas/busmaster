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
 * \file      HandlerFunc.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "HandlerFunc.h"
#include "ExecuteManager.h"
#include "AppServicesCAN.h"

CRITICAL_SECTION g_omDllMsgCritiSec;

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
UINT unKeyHandlerProc(LPVOID pParam)
{
    if (pParam != NULL)
    {
        PSEXECUTE_KEY_HANDLER psExecuteKeyHandler =
                                        (PSEXECUTE_KEY_HANDLER) pParam;
		// There is no memory allocation is this thread. So initialise it to NULL
		 psExecuteKeyHandler->m_pCExecuteFunc->m_asUtilThread[defKEY_HANDLER_THREAD].
			                                                       m_pvThread = NULL;
		// Reset the event signaled
        psExecuteKeyHandler->m_pCExecuteFunc->
			m_aomState[defKEY_HANDLER_THREAD ].ResetEvent();

        try
        {
            psExecuteKeyHandler->
                m_pFKeyHandler(psExecuteKeyHandler->m_ucKeyValue);
        }
        catch(...)
        {
            CHAR acError[256];
            sprintf_s( acError,
                     defSTR_ERROR_IN_KEY_PROG,
                     psExecuteKeyHandler->m_ucKeyValue );
            // Display the error information in the Trace window
            gbSendStrToTrace(acError);
        }
        
 //       psExecuteKeyHandler = NULL;
		psExecuteKeyHandler->m_pCExecuteFunc->
        m_asUtilThread[defKEY_HANDLER_THREAD].m_pvThread = NULL; 
		 // Reinitialise the Thread handle before terminating it.
		psExecuteKeyHandler->m_pCExecuteFunc->
		m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread = NULL;
		// Set the event to indicate termination of this thread.
		psExecuteKeyHandler->m_pCExecuteFunc->
		m_aomState[defKEY_HANDLER_THREAD].SetEvent();
		delete psExecuteKeyHandler;
    }
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
UINT unErrorHandlerProc(LPVOID pParam)
{
    if (pParam != NULL)
    {
        PSERRORHANDLER pErrorHandler = (PSERRORHANDLER) pParam; //ell2kor
		// There is no memory allocation is this thread. So initialise it to NULL
		pErrorHandler->m_pCExecuteFunc->
			m_asUtilThread[defERROR_HANDLER_THREAD].m_pvThread = NULL;
		// Reset the event signaled
		pErrorHandler->m_pCExecuteFunc->
			m_aomState[defERROR_HANDLER_THREAD].ResetEvent();
        try
        {
            pErrorHandler->m_pFErrorHandlers(pErrorHandler->sErrorVal); //ell2kor
        }
        catch(...)
        {
            // Display the error information in the Trace window
            gbSendStrToTrace(defSTR_ERROR_IN_ERR_PROG);
        } 
		// Reinitialise the Thread handle before terminating it.
		pErrorHandler->m_pCExecuteFunc->
		    m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread = NULL;
		// Set the event to indicate termination of this thread.
		pErrorHandler->m_pCExecuteFunc->
			m_aomState[defERROR_HANDLER_THREAD].SetEvent();
    }
    return 0;
}

/******************************************************************************
    Function Name    :  unEventHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined event 
                        handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        user-defined event handler function.
                        - User will be notified status of transmission.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Pradeep Kadoor
    Date Created     :  08.02.2011
    Modifications    :  
******************************************************************************/
UINT unEventHandlerProc(LPVOID pParam)
{    
    if (pParam != NULL)
    {
        PSEVENTHANDLER pEventHandler = (PSEVENTHANDLER) pParam; //ell2kor
		// There is no memory allocation is this thread. So initialise it to NULL
		pEventHandler->m_pCExecuteFunc->
			m_asUtilThread[defEVENT_HANDLER_THREAD].m_pvThread = NULL;
		// Reset the event signaled
		pEventHandler->m_pCExecuteFunc->
			m_aomState[defEVENT_HANDLER_THREAD].ResetEvent();
        try
        {
            if (pEventHandler->m_byEventType == 0x0)
            {
                pEventHandler->m_pFEventHandlers(pEventHandler->m_unPGN, pEventHandler->m_bySrc, 
                                                 pEventHandler->m_byDest, pEventHandler->m_bSuccess);
            }
            else if (pEventHandler->m_byEventType == 0x1)
            {
                pEventHandler->m_pFEventHandlers(pEventHandler->m_bySrc);
            }
        }
        catch(...)
        {
            // Display the error information in the Trace window
            gbSendStrToTrace(defSTR_ERROR_IN_EVENT_PROG);
        } 
		// Reinitialise the Thread handle before terminating it.
		pEventHandler->m_pCExecuteFunc->
		    m_asUtilThread[defEVENT_HANDLER_THREAD].m_hThread = NULL;
		// Set the event to indicate termination of this thread.
		pEventHandler->m_pCExecuteFunc->
			m_aomState[defEVENT_HANDLER_THREAD].SetEvent();
    }
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
UINT unDLLUnloadHandlerProc(LPVOID pParam)
{    
    if (pParam != NULL)
    {
        PSEXECUTE_LOAD_HANDLER psExecuteUnLoadHandler=(PSEXECUTE_LOAD_HANDLER) pParam;
		// Reset the event signaled
		psExecuteUnLoadHandler->m_pCExecuteFunc->
				m_aomState[defDLL_UNLOAD_HANDLER_THREAD].ResetEvent();
		if(psExecuteUnLoadHandler->m_pCExecuteFunc!=NULL)
		{
			try
			{
				psExecuteUnLoadHandler->pFDllHandler();
			}
			catch(...)
			{
				// Display the error information in the Trace window
				gbSendStrToTrace(defSTR_ERROR_IN_DLL_UNLOAD);
			}
			// There is no memory allocation is this thread. So initialise it to NULL
			psExecuteUnLoadHandler->m_pCExecuteFunc->
				m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_pvThread = NULL;
			// Reinitialise the Thread handle before terminating it.
			psExecuteUnLoadHandler->m_pCExecuteFunc->
				m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_hThread = NULL;
			// Set the event to indicate termination of this thread.
			psExecuteUnLoadHandler->m_pCExecuteFunc->
				m_aomState[defDLL_UNLOAD_HANDLER_THREAD].SetEvent();
			delete psExecuteUnLoadHandler;
			psExecuteUnLoadHandler=NULL;
		}
    }
   
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


UINT unDLLloadHandlerProc(LPVOID pParam)
{    
    if (pParam != NULL)
    {
        PSEXECUTE_LOAD_HANDLER psExecuteLoadHandler=(PSEXECUTE_LOAD_HANDLER) pParam;
		// Reset the event signaled
		if(psExecuteLoadHandler->m_pCExecuteFunc!=NULL)
		{
			psExecuteLoadHandler->m_pCExecuteFunc->
				m_aomState[defDLL_LOAD_HANDLER_THREAD].ResetEvent();
			try
			{
				psExecuteLoadHandler->pFDllHandler();
			}
			catch(...)
			{
				// Display the error information in the Trace window
				gbSendStrToTrace(defSTR_ERROR_IN_DLL_LOAD);
			} 
			// There is no memory allocation is this thread. So initialise it to NULL
			psExecuteLoadHandler->m_pCExecuteFunc->
				m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_pvThread = NULL;
			// Reinitialise the Thread handle before terminating it.
			psExecuteLoadHandler->m_pCExecuteFunc->
				m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_hThread = NULL;
			// Set the event to indicate termination of this thread.
			psExecuteLoadHandler->m_pCExecuteFunc->
				m_aomState[defDLL_LOAD_HANDLER_THREAD].SetEvent();
			delete psExecuteLoadHandler;
			psExecuteLoadHandler=NULL;
		}
    }
	 	
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
UINT unReadNodeMsgHandlerBuffer(LPVOID pParam)
{
	if(pParam != NULL)
	{
		CExecuteFunc* pCExecuteFunc=(CExecuteFunc*)pParam;
		pCExecuteFunc->m_aomState[defMSG_HANDLER_THREAD].ResetEvent();
		//if dll is unloaded exit loop and end thread
		//BOOL bDllLoaded=pCExecuteFunc->bIsDllLoaded();
		while(pCExecuteFunc->bIsDllLoaded())
		{
			WaitForSingleObject( pCExecuteFunc->m_omReadFromQEvent,INFINITE);
			//wait for event set by write thread
			pCExecuteFunc->m_omReadFromQEvent.ResetEvent();
			UINT unMsgCnt=pCExecuteFunc->unGetBufferMsgCnt();
			//if buffer is empty wait for read event
			while( unMsgCnt>0 && pCExecuteFunc->bIsDllLoaded() &&
				                   !(pCExecuteFunc->m_bStopMsgHandlers) )
			{
				STCAN_MSG sCanMsg=pCExecuteFunc->sReadFromQMsg();
				pCExecuteFunc->vExecuteOnMessageHandlerCAN(sCanMsg);
				unMsgCnt=pCExecuteFunc->unGetBufferMsgCnt();	
			}
		}
		pCExecuteFunc->m_aomState[defMSG_HANDLER_THREAD].SetEvent( );
	}
	return 0;
}

/******************************************************************************
    Function Name    :  unReadDllMsgBuffer
    Input(s)         :  pParam - Typecasted address CExecuteFunc object
    Output           :  Zero
    Functionality    :  This is a thread control function to process 
                        reading of CExecutemanger's buffer
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  22.12.2005
    Modifications    :  Anish kumar
					 :  29.01.07,changes the code to avoid mem leak
******************************************************************************/
const int SIZE_OF_SDLL_MSG=sizeof(SDLL_MSG);
UINT unReadDllMsgBuffer(LPVOID pParam)
{
    CExecuteManager* pExecMngr = (CExecuteManager*)pParam;
	while(g_bReadDllMsg)
	{
		pExecMngr->m_omDllMsgEvent.ResetEvent();
		if(g_unDllMsgCnt>0)
		{
			SDLL_MSG sDllMessages;
			EnterCriticalSection(&g_omDllMsgCritiSec);
			memcpy(&sDllMessages,&(g_asDllMessages[g_unReadDllMsg]),SIZE_OF_SDLL_MSG);
			g_unDllMsgCnt--;
			g_unReadDllMsg=(g_unReadDllMsg+1)%defMAX_HMODULE_MSG;
			LeaveCriticalSection(&g_omDllMsgCritiSec);
            CExecuteManager::ouGetExecuteManager(pExecMngr->m_eBus).vManageDllMessageHandler(sDllMessages);
			Sleep(0);
		}
		else 
		{
			Sleep(50);
		}
	}
	pExecMngr->m_omDllMsgEvent.SetEvent();
	return 0;
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

UINT unTimerHandlerProc(LPVOID pParam)
{    
	PSTIMERHANDLER psTimerStruct=(PSTIMERHANDLER)pParam;
	if(psTimerStruct!=NULL)
		while(psTimerStruct->hDllHandle)
		{
			WaitForSingleObject(psTimerStruct->omWaitEvent,INFINITE);
			if(psTimerStruct->bTimerSelected)
			{
				psTimerStruct->omWaitEvent.ResetEvent();
				
				if( psTimerStruct->pFTimerHandler!=NULL &&
					              psTimerStruct->bTimerSelected&&
								  psTimerStruct->hDllHandle )
				{
					try
					{
						psTimerStruct->pFTimerHandler();
					}
					catch(...)
					{
						CHAR acError[256];
						sprintf_s( acError,
							defSTR_ERROR_IN_TIMER_PROG,
							psTimerStruct->omStrTimerHandlerName );
						// Display the error information in the Trace window
						gbSendStrToTrace(acError);
						
					}
					 psTimerStruct->unCurrTime=0;
                     if(psTimerStruct->bTimerType==TRUE)
                     {
                         psTimerStruct->bTimerSelected=FALSE;
                     }
				}
				
			}
			
		}
        psTimerStruct->pomThreadHandle=NULL;
		return 0;
}
