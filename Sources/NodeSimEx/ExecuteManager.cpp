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
 * \file      ExecuteManager.cpp
 * \brief     This is a singleton class which takes care of creating and destroying 
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This is a singleton class which takes care of creating and destroying 
 */

#include "NodeSimEx_stdafx.h"
#include "ExecuteManager.h" 
#include "ExecuteFunc.h"
#include "Export_UserDllCAN.h"
#include "SimSysManager.h"
#include "GlobalObj.h"

//flag for exiting from thread
extern BOOL g_bReadDllMsg;
//read dll msg thread pointer

extern UINT unReadDllMsgBuffer(LPVOID pParam);

CExecuteManager* CExecuteManager::sm_pouManager[BUS_TOTAL]= {NULL};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/***********************************************************************
    Function Name    :  CExecuteManager
    Input(s)         : 
    Output           :  
    Functionality    :  Constructor,start the read msg buffer thread for 
						global message buffer,start the timer for timer
						handlers
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
************************************************************************/
CExecuteManager::CExecuteManager(ETYPE_BUS eBus):
				 m_psFirstNodeObject(NULL),
				 m_psLastNodeObject(NULL),
                 m_odSetResetTimer(eBus)
{
    m_eBus = eBus;
    //for read dll msg thread
    g_bReadDllMsg = TRUE;
    //start readDllMsg thread
    m_pouBuildProgram = new CBuildProgram(m_eBus);
    InitializeCriticalSection(&m_CritSectPsNodeObject);
    m_omDllMsgEvent.SetEvent();
}

/************************************************************************
    Function Name    :  vStartDllReadThread
    Input(s)         : 
    Output           :  
    Functionality    :  start the read msg buffer thread for 
						global message buffer & start Multimedia timer
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  23.02.05
*************************************************************************/
void CExecuteManager:: vStartDllReadThread()
{
	m_pomReadDllThrd = AfxBeginThread(unReadDllMsgBuffer, this);
}

/************************************************************************
    Function Name    :  ~CExecuteManager
    Input(s)         : 
    Output           :  
    Functionality    :  stop the read msg buffer thread for 
						global message buffer
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
	Modification by  :  Anish kumar
    Date             :  17.11.06,moved code m_odSetResetTimer.vStopTimer() 
						after waiting for the timer timer thread to exit.
						Removed timer stop call 
	Modification by  :  Anish kumar
    Date             :  29.01.07,moved code g_bReadDllMsg=FALSE 
						in mainframe class on destroy(to avoid mem leak)
*************************************************************************/
CExecuteManager::~CExecuteManager()
{
	//stop the ReadDllMsg thread
	//g_bReadDllMsg=FALSE;
	DWORD dwResult=WaitForSingleObject(m_omDllMsgEvent,500);
	if( dwResult == WAIT_TIMEOUT )
	{
		TerminateThread(m_pomReadDllThrd->m_hThread,0);
	}
	DeleteCriticalSection(&m_CritSectPsNodeObject);
	if(m_pouBuildProgram!=NULL)
	{
		delete m_pouBuildProgram;
		m_pouBuildProgram=NULL;
	}

}
/************************************************************************
    Function Name    :  hReturnDllHandle
    Input(s)         :  Node name
    Output           :  Handle of DLL of the node
    Functionality    :  Return the handle of dLL of the node if found else 
						return NULL
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*************************************************************************/

const HMODULE CExecuteManager::hReturnDllHandle(const CString omStrNodeName)
{
   PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
   while(psTempNodeObject!=NULL)
   {
	   if(omStrNodeName==psTempNodeObject->om_NodeName)
	   {
		   return  psTempNodeObject->m_psExecuteFunc->hGetDllHandle();
	   }
	    psTempNodeObject=psTempNodeObject->m_psNextNode;
   }
   return NULL;

}
/*************************************************************************
    Function Name    :  ouGetExecuteManager
    Input(s)         : 
    Output           :  The only CExecuteManager object
    Functionality    :  It returns the only CExecuteManager object created
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
**************************************************************************/
CExecuteManager& CExecuteManager::ouGetExecuteManager(ETYPE_BUS eBus)
{
    if (sm_pouManager[eBus] == NULL)
    {
        sm_pouManager[eBus] = new CExecuteManager(eBus);
    }
    return *(sm_pouManager[eBus]);
}
void CExecuteManager::vClearObj(ETYPE_BUS eBus)
{
    if (sm_pouManager[eBus] != NULL)
    {
        delete sm_pouManager[eBus];
        sm_pouManager[eBus] = NULL;
    }
}
/*************************************************************************
    Function Name    :  CExecuteManager
    Input(s)         :  Node name
    Output           :  BOOL
    Functionality    :  It deletes the node structure from the node list 
						at unloading of DLL
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
**************************************************************************/
BOOL CExecuteManager::vDeleteNode(const CString omStrNodeName)
{
	 BOOL bReturn=FALSE;
	 PSNODEOBJECT psPrevNodeObject,psCurrNodeObject;
	 if(m_psFirstNodeObject==NULL)
	 {
		 bReturn= FALSE;
	 }

	 else if(!omStrNodeName.CompareNoCase(m_psFirstNodeObject->om_NodeName))
	 {
		 if(m_psFirstNodeObject==m_psLastNodeObject)
		 {
			 m_psLastNodeObject=NULL;
		 }
		psCurrNodeObject=m_psFirstNodeObject;
		m_psFirstNodeObject=m_psFirstNodeObject->m_psNextNode;
		if(psCurrNodeObject->m_psExecuteFunc!=NULL)
		{
			EnterCriticalSection(&m_CritSectPsNodeObject);
			delete psCurrNodeObject->m_psExecuteFunc;
			psCurrNodeObject->m_psExecuteFunc=NULL;
			LeaveCriticalSection(&m_CritSectPsNodeObject);
		}
		delete psCurrNodeObject;
		psCurrNodeObject=NULL;
		bReturn=TRUE;
	 }
	 else
	 {
		 psPrevNodeObject=NULL;
		 psCurrNodeObject=m_psFirstNodeObject;
		 while(psCurrNodeObject!=NULL && omStrNodeName.CompareNoCase
			                           (psCurrNodeObject->om_NodeName))
		 {
			 psPrevNodeObject=psCurrNodeObject;
			 psCurrNodeObject=psCurrNodeObject->m_psNextNode;
		 }
		 if(psCurrNodeObject==NULL)
		 {
			 bReturn= FALSE;
		 }
		 else
		 {
			 psPrevNodeObject->m_psNextNode=psCurrNodeObject->m_psNextNode;
			 if(m_psLastNodeObject==psCurrNodeObject)
			 {
				 m_psLastNodeObject=psPrevNodeObject;
			 }
		    if(psCurrNodeObject->m_psExecuteFunc!=NULL)
			{
				EnterCriticalSection(&m_CritSectPsNodeObject);
				delete psCurrNodeObject->m_psExecuteFunc;
				psCurrNodeObject->m_psExecuteFunc=NULL;
				LeaveCriticalSection(&m_CritSectPsNodeObject);
			}
			 EnterCriticalSection(&m_CritSectPsNodeObject);
			 delete psCurrNodeObject;
			 psCurrNodeObject=NULL;
			 LeaveCriticalSection(&m_CritSectPsNodeObject);
			 bReturn=TRUE;
		 }
	
	 }
	 if(m_psFirstNodeObject==NULL)
	 {
		CSimSysManager::ouGetSimSysManager(m_eBus).
            ouGetFlags().vSetFlagStatus( H_DLLLOADED, FALSE );
	 }
	return bReturn;

	
}

/****************************************************************************************
    Function Name    :  vAddNode
    Input(s)         :	Node info structure and corressponding object
    Output           :  void
    Functionality    :  It add the NodeObject structure in the list at the time of 
						loading a DLL
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
/****************************************************************************************/
void CExecuteManager:: vAddNode (const PSNODEINFO psNodeInfo,CExecuteFunc* pExecuteFunc)
{
   PSNODEOBJECT psNodeObject=new SNODEOBJECT;
   if(psNodeObject!=NULL)
   {
	   psNodeObject->om_NodeName=psNodeInfo->m_omStrNodeName;
	   psNodeObject->m_psExecuteFunc=pExecuteFunc;
	   psNodeObject->m_psNextNode=NULL;
	   //add to the last with last structure pointer equal to NULL
	   if(m_psFirstNodeObject==NULL)
	   {
		   m_psFirstNodeObject=psNodeObject;
		   m_psLastNodeObject=psNodeObject;
	   }
	   else
	   {
		   m_psLastNodeObject->m_psNextNode=psNodeObject;
		   m_psLastNodeObject=psNodeObject;
	   }
	   pExecuteFunc->vSetNodeInfo(psNodeInfo);
	   pExecuteFunc->vCopyHandlersArrayToUI(psNodeInfo);
	   bDllLoaded=TRUE;
   }
   
}

/***************************************************************************************
    Function Name    :  vEnableDisableAllHandlers
    Input(s)         :  bool enable/disable 
    Output           :  
    Functionality    :  enable/disable all handlers of all the nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
****************************************************************************************/
void CExecuteManager::vEnableDisableAllHandlers(BOOL bState)
{
	CWaitCursor omWait;
	if( bState==TRUE )
	{
		//start the timer threads for each node
		vManageTimerThreads();
		vStartTimer();
	}
    CFlags &ouFlag =  CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags();
    ouFlag.vSetFlagStatus(H_ALL_HANDLER, bState);
    ouFlag.vSetFlagStatus(H_MSGHANDLERBUTTON, bState);
    ouFlag.vSetFlagStatus(H_KEY_HANDLER_ON, bState);
    ouFlag.vSetFlagStatus(H_TIMERBUTTON, bState);
    ouFlag.vSetFlagStatus(H_ERROR_HANDLER, bState);
	PSNODEOBJECT psNodeObject=m_psFirstNodeObject;
	while(psNodeObject!=NULL)
	{
		psNodeObject->m_psExecuteFunc->bActivateDeactivateHandlers(bState);
		psNodeObject=psNodeObject->m_psNextNode;
	}
	vManageTimerThreads();
    vUpdateHandlerDetailsInDetView();

	//Update the All Handlers button text
	CSimSysDetView* pSimSysDetView = 
		CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
	if (NULL != pSimSysDetView)
	{
		pSimSysDetView->vChangeEDAllHanButtonText(bState);
	}
}

/***************************************************************************************
    Function Name    :  vEnableDisableAllTimers
    Input(s)         :  bool enable/disable 
    Output           :  
    Functionality    :  enable/disable all handlers of all the nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  07.06.06
****************************************************************************************/
void CExecuteManager::vEnableDisableAllTimers(BOOL bState)
{
	//CWaitCursor omWait;
	if( bState==TRUE )
	{
		//start the timer threads for each node
		vManageTimerThreads();
		vStartTimer();
	}
    CSimSysManager::
        ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_TIMERBUTTON, bState);
	PSNODEOBJECT psNodeObject=m_psFirstNodeObject;
	while(psNodeObject!=NULL)
	{
		psNodeObject->m_psExecuteFunc->vEnableDisableAllTimers(bState);
		psNodeObject = psNodeObject->m_psNextNode;
	}
	vManageTimerThreads();
    vUpdateHandlerDetailsInDetView();					
}

/***************************************************************************************
    Function Name    :  vEnableDisableNodeHandlers
    Input(s)         :  PSNODEINFO,bool enable/disable 
    Output           :  
    Functionality    :  enable/disable all handlers of the node whose NodeInfo 
						is passed as parameter
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
****************************************************************************************/
void CExecuteManager::vEnableDisableNodeHandlers(const PSNODEINFO psNodeInfo,BOOL bState)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	BOOL bFound=FALSE;
	while((!bFound) && (psTempNodeObject != NULL))
		{
			 if(psNodeInfo->m_omStrNodeName==psTempNodeObject->om_NodeName)
			 {
				bFound=TRUE;
			 }
			 else
			 {
				 psTempNodeObject=psTempNodeObject->m_psNextNode;
			 }
		}
	if(psTempNodeObject!=NULL)
	{
        vManageTimerThreads();
		psTempNodeObject->m_psExecuteFunc->bActivateDeactivateHandlers(bState);
	}
   	
}
/**************************************************************************************
    Function Name    :  vEnableNodeKeyHandler
    Input(s)         :  PSNODEINFO,bool enable/disable 
    Output           :  
    Functionality    :  Enable disable key handlers of the node whose NOdeInfo is
	                    passed as parameter
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vEnableNodeKeyHandler(const PSNODEINFO psNodeInfo,BOOL bState)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	BOOL bFound=FALSE;
	while(!bFound&&psTempNodeObject!=NULL)
		{
			 if(!psNodeInfo->m_omStrNodeName.CompareNoCase(psTempNodeObject->om_NodeName))
			 {
				bFound=TRUE;
			 }
			 else
			 {
				 psTempNodeObject=psTempNodeObject->m_psNextNode;
			 }
		}
	if(bFound)
	{
		psTempNodeObject->m_psExecuteFunc->bEnableDisableKeyHandlers(bState);
	}


}

/**************************************************************************************
    Function Name    :  vEnableAllKeyHandler
    Input(s)         :  bool enable/disable 
    Output           :  
    Functionality    :  enable/disable all key handlers of all the nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vEnableAllKeyHandler(BOOL bState)
{
	CWaitCursor omWait;
    CSimSysManager::
        ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_KEY_HANDLER_ON, bState);
	PSNODEOBJECT psNodeObject=m_psFirstNodeObject;
	while( psNodeObject != NULL )
	{
			psNodeObject->m_psExecuteFunc->bEnableDisableKeyHandlers(bState); 
			psNodeObject=psNodeObject->m_psNextNode;
	}    
    vUpdateHandlerDetailsInDetView();
}

/****************************************************************************************
    Function Name    :  vEnableNodeMessageHandler
    Input(s)         :  PSNODEINFO,bool enable/disable 
    Output           :  
    Functionality    :  Enable disable messaege handlers of the node whose NOdeInfo is
	                    passed as parameter
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*****************************************************************************************/
void CExecuteManager::vEnableNodeMessageHandler(const PSNODEINFO psNodeInfo,BOOL bState)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    BOOL bFound=FALSE;
	while(!bFound&&psTempNodeObject!=NULL)
		{
			 if(!psNodeInfo->m_omStrNodeName.CompareNoCase(psTempNodeObject->om_NodeName))
			 {
				bFound=TRUE;
			 }
			 else
			 {
				 psTempNodeObject=psTempNodeObject->m_psNextNode;
			 }
		}
	if(psTempNodeObject!=NULL)
	{
		psTempNodeObject->m_psExecuteFunc->bEnableDisableMsgHandlers(bState);
	}
}

/****************************************************************************************
    Function Name    :  vEnableAllMessageHandler
    Input(s)         :  bool enable/disable 
    Output           :  
    Functionality    :  enable/disable all message handlers of all the nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*****************************************************************************************/
void CExecuteManager::vEnableAllMessageHandler(BOOL bState)
{
    CWaitCursor omWait;
    CSimSysManager::
        ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_MSGHANDLERBUTTON, bState);
	PSNODEOBJECT psNodeObject=m_psFirstNodeObject;
	while(psNodeObject!=NULL)
	{
		psNodeObject->m_psExecuteFunc->bEnableDisableMsgHandlers(bState); 
		psNodeObject=psNodeObject->m_psNextNode;
	}
    vUpdateHandlerDetailsInDetView();
}


/****************************************************************************************
    Function Name    :  vEnableNodeErrorHandler
    Input(s)         :  PSNODEINFO,bool enable/disable 
    Output           :  
    Functionality    :  Enable disable error handlers of the node whose NOdeInfo is
	                    passed as parameter
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*****************************************************************************************/
void CExecuteManager::vEnableNodeErrorHandler(const PSNODEINFO psNodeInfo,BOOL bState)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    BOOL bFound=FALSE;
	while(!bFound&&psTempNodeObject!=NULL)
		{
			 if(!psNodeInfo->m_omStrNodeName.CompareNoCase(psTempNodeObject->om_NodeName))
			 {
				bFound=TRUE;
			 }
			 else
			 {
				 psTempNodeObject=psTempNodeObject->m_psNextNode;
			 }
		}
	if(psTempNodeObject)
	{
		psTempNodeObject->m_psExecuteFunc->bEnableDisableErrorHandlers(bState);
	}
}

/****************************************************************************************
    Function Name    :  vEnableAllErrorHandler
    Input(s)         :  bool enable/disable 
    Output           :  
    Functionality    :  enable/disable all error handlers of all the nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*****************************************************************************************/
void CExecuteManager::vEnableAllErrorHandler(BOOL bState)
{
    CWaitCursor omWait;
    CSimSysManager::
        ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_ERROR_HANDLER, bState);
	PSNODEOBJECT psNodeObject=m_psFirstNodeObject;
	while(psNodeObject!=NULL)
	{
		psNodeObject->m_psExecuteFunc->bEnableDisableErrorHandlers(bState);
		psNodeObject=psNodeObject->m_psNextNode;
	}
    vUpdateHandlerDetailsInDetView();
}

/****************************************************************************************
    Function Name    :  vEnableNodeTimerHandler
    Input(s)         :  PSNODEINFO,bool enable/disable 
    Output           :  
    Functionality    :  Enable disable timer handlers of the node whose NOdeInfo is
	                    passed as parameter
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*****************************************************************************************/
void CExecuteManager::vEnableNodeTimerHandler(const PSNODEINFO psNodeInfo,BOOL bState)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    BOOL bFound=FALSE;
	while((!bFound) && (psTempNodeObject!=NULL))
		{
			 if(!psNodeInfo->m_omStrNodeName.CompareNoCase
				              (psTempNodeObject->om_NodeName))
			 {
				bFound=TRUE;
			 }
			 else
			 {
				 psTempNodeObject=psTempNodeObject->m_psNextNode;
			 }
		}
	if(bFound)
	{
		psTempNodeObject->m_psExecuteFunc->vEnableDisableAllTimers(bState);
	}
}


/**************************************************************************************
    Function Name    :  vManageOnKeyHandler
    Input(s)         :  key pressed
    Output           :  
    Functionality    :  Calls all key handlers for a key
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vManageOnKeyHandler(UCHAR ucKey)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	while(psTempNodeObject!=NULL)
	{
		if(psTempNodeObject->m_psExecuteFunc->bGetFlagStatus(EXKEY_HANDLER) == TRUE)
		{
			psTempNodeObject->m_psExecuteFunc->vExecuteOnKeyHandler(ucKey);
		}
		psTempNodeObject=psTempNodeObject->m_psNextNode;
	}
}

/***************************************************************************************
    Function Name    :  vManageOnMessageHandler
    Input(s)         :  Message structure
    Output           :  
    Functionality    :  Calls all Message handlers for a Message
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vManageOnMessageHandlerCAN(STCAN_MSG sRxMsgInfo, DWORD dwClientId)
{
    EnterCriticalSection(&m_CritSectPsNodeObject);
	PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    while(psTempNodeObject != NULL)
    {
	    if(psTempNodeObject->m_psExecuteFunc->bGetFlagStatus(EXMSG_HANDLER) == TRUE)
	    {
            
            sNODEINFO sNodeInfo(m_eBus);
            psTempNodeObject->m_psExecuteFunc->vGetNodeInfo(sNodeInfo);
            if (sNodeInfo.m_dwClientId == dwClientId)
            {
	            psTempNodeObject->m_psExecuteFunc->vWriteInQMsg(sRxMsgInfo);
            }            
	    }
	    psTempNodeObject = psTempNodeObject->m_psNextNode;
    }
    LeaveCriticalSection(&m_CritSectPsNodeObject);	
}


/**************************************************************************************
    Function Name    :  vManageOnMessageHandler
    Input(s)         :  Error Message structure and code
    Output           :  
    Functionality    :  Calls all Error handlers for a Error
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vManageOnErrorHandlerCAN(eERROR_STATE eErrorCode,SCAN_ERR sErrorVal, DWORD dwClientId)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	while(psTempNodeObject!=NULL)
    {
        sNODEINFO sNodeInfo(m_eBus);
        psTempNodeObject->m_psExecuteFunc->vGetNodeInfo(sNodeInfo);
        if (sNodeInfo.m_dwClientId == dwClientId)
        {
	        if(psTempNodeObject->m_psExecuteFunc->bGetFlagStatus(EXERROR_HANDLER) == TRUE)
	        {
                
	                psTempNodeObject->m_psExecuteFunc->vExecuteOnErrorHandler( eErrorCode,
																	        sErrorVal);
                
	        }
        }
	    psTempNodeObject=psTempNodeObject->m_psNextNode;
   }		
}
/**************************************************************************************
    Function Name    :  vManageOnDataConfHandler
    Input(s)         :  
    Output           :  
    Functionality    :  Calls all key handlers for a key
    Member of        :  CExecuteManager
    Author(s)        :  Pradeep Kadoor
    Date Created     :  01.01.2010
***************************************************************************************/
void CExecuteManager::vManageOnDataConfHandlerJ1939(DWORD dwClientId, UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess)
{
	PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    BOOL bContinue = TRUE;
	while((psTempNodeObject != NULL) && (bContinue == TRUE))
	{
        //Since shRemoteLc indicates Receiver, compare the node LCN with it
        sNODEINFO sNodeInfo(J1939);
        psTempNodeObject->m_psExecuteFunc->vGetNodeInfo(sNodeInfo);
        if(dwClientId == sNodeInfo.m_dwClientId)
		{
            psTempNodeObject->m_psExecuteFunc->vExecuteOnDataConfHandlerJ1939(unPGN, bySrc, byDest, bSuccess);
            bContinue = FALSE;
		}
		psTempNodeObject=psTempNodeObject->m_psNextNode;
	}
}
/**************************************************************************************
    Function Name    :  vManageOnDataConfHandler
    Input(s)         :  
    Output           :  
    Functionality    :  Calls all key handlers for a key
    Member of        :  CExecuteManager
    Author(s)        :  Pradeep Kadoor
    Date Created     :  01.01.2010
***************************************************************************************/
void CExecuteManager::vManageOnAddressClaimHandlerJ1939(DWORD dwClientId, BYTE byAddress)
{
	PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    BOOL bContinue = TRUE;
	while((psTempNodeObject != NULL) && (bContinue == TRUE))
	{
        //Since shRemoteLc indicates Receiver, compare the node LCN with it
        sNODEINFO sNodeInfo(J1939);
        psTempNodeObject->m_psExecuteFunc->vGetNodeInfo(sNodeInfo);
        if(dwClientId == sNodeInfo.m_dwClientId)
		{
            sNodeInfo.m_byPrefAddress = byAddress;
            CSimSysDetView* pSimSysDetView = NULL;
            pSimSysDetView = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
            if (NULL != pSimSysDetView)
            {
                pSimSysDetView->vSetNodeAddress(byAddress);
            }
            psTempNodeObject->m_psExecuteFunc->vSetNodeInfo(&sNodeInfo);
            psTempNodeObject->m_psExecuteFunc->vExecuteOnAddressClaimHandlerJ1939(byAddress);
            bContinue = FALSE;
		}
		psTempNodeObject=psTempNodeObject->m_psNextNode;
	}
}
/******************************************************************************/
/*  Function Name    :  bExecuteDLLBuildLoad                                  */
/*  Input(s)         :  NOdeInfo structure list from UI                       */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Called by $$$$$$$$$$ when user want to build and load */
/*                      the DLL. If DLL is already loaded, it will be unloaded*/
/*                      and all user defined handler and flags will be reset. */
/*                                                                            */
/*  Member of        :  CExecuteManager                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  27.12.05                                              */    
/*  Modification By  :                                                        */
/*  Modification on  :														  */
/*                                                                            */
/******************************************************************************/
BOOL CExecuteManager::bExecuteDllBuildLoad(PSNODEINFO psNodeInfo) 
{
   CStringArray omStrArray;
   //CWaitCursor omWait;
   BOOL bFreeLibrary = TRUE;
   BOOL bReturn=   FALSE;
   
   if(psNodeInfo!=NULL)    
   {
    
       if (!psNodeInfo->m_omStrFileName.IsEmpty())
       {
          // If the object is not created, create it.
           if(m_pouBuildProgram==NULL)
           {
               m_pouBuildProgram = new CBuildProgram(m_eBus);
           }
           if(m_pouBuildProgram!=NULL)
           {
			   PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
               if(psNodeInfo->m_bIsDllLoaded)
               {
				   BOOL bFound=FALSE;
				   while(psTempNodeObject!=NULL&&!bFound)
				   {
					   if(!psNodeInfo->m_omStrNodeName.CompareNoCase
						   (psTempNodeObject->om_NodeName))
					   {
						   bFound=TRUE;
					   }
					   else
					   {
							psTempNodeObject=psTempNodeObject->m_psNextNode;
					   }
				   }
				   if(psTempNodeObject!=NULL)
				   {
					  bFreeLibrary= psTempNodeObject->m_psExecuteFunc->bUnloadDll();
					  if(bFreeLibrary)
					  {
//						  m_odSetResetTimer.
//							  vDeleteNodeTimerList(psTempNodeObject->om_NodeName);
						  vDeleteNode(psNodeInfo->m_omStrNodeName);
					  }
				      
				   }
	
               }
               // Check if DLL is loaded after being build.
               BOOL bDLLLoaded = FALSE;
               // Call the function to build and load only if resource for last
               // loaded file is free 
               if(bFreeLibrary == TRUE )
               {
                    bDLLLoaded =
                      m_pouBuildProgram->bBuildProgram( psNodeInfo,TRUE);
               }
               else
               {
                   CString omStrMessage = STR_EMPTY;
                   omStrMessage  = _T("Unable to free resources for loaded ");
                   omStrMessage += 
					   psTempNodeObject->m_psExecuteFunc->omGetPrevFileName();
                   omStrMessage += _T(" file");
				   omStrArray.Add(omStrMessage);
				   m_pouBuildProgram->bAddString(omStrArray);
				   bReturn=FALSE;
               }
			   


               // all handler function structure with function pointer and other
               // informations.
               if(bDLLLoaded == TRUE)
               {
                    HMODULE hModuleHandle  = NULL;
                    CString omStrDLLName = STR_EMPTY;
                    INT nIndex           = 0;
                    BOOL bInitStruct     = FALSE;
					hModuleHandle=psNodeInfo->m_hModuleHandle;

   					omStrDLLName=psNodeInfo->m_omStrFileName;										


					 // Get the DLL name from source file name by replacing .c
                    // with .DLL                                       
                    nIndex       = omStrDLLName.ReverseFind('.');
                    if(nIndex!=-1)
                    {
                        omStrDLLName  = omStrDLLName.Left(nIndex);
                        omStrDLLName += defDOT_DLL;
                    }
					
                    CExecuteFunc* m_pouExecuteFunc = new CExecuteFunc(m_eBus, omStrDLLName);
                    // check for successfull memory allocation
                    if(m_pouExecuteFunc != NULL)
                    {
						m_pouExecuteFunc->vSetDllHandle(hModuleHandle);
						m_pouExecuteFunc->vInitialiseInterfaceFnPtrs(hModuleHandle);
                        m_pouExecuteFunc->vSetNodeInfo(psNodeInfo);
                        bInitStruct = m_pouExecuteFunc->bInitStruct(omStrArray);
                        // If the initialisation of any one structure fails,
                        // prompt a message.
                        if(bInitStruct == FALSE )
                        {
                            bFreeLibrary = FreeLibrary(m_pouExecuteFunc->
								                          hGetDllHandle());
                            CString omStrErrorMsg = defINIT_HANDLER_STRUCT_MSG;
                            if(bFreeLibrary == TRUE )
                            {
                                omStrErrorMsg += defDLL_UNLOADED_MSG;
                                omStrErrorMsg.Replace(_T("FILENAME"),omStrDLLName);
                               
                            }
							omStrArray.Add(omStrErrorMsg);
                            m_pouBuildProgram->bAddString(omStrArray);
                            psNodeInfo->m_bIsDllLoaded=FALSE;
							bReturn =FALSE;
                        }
                        else
                        {	
							psNodeInfo->m_bIsDllLoaded=TRUE;
                            vAddNode(psNodeInfo,m_pouExecuteFunc);
                            m_pouExecuteFunc->vExecuteOnDLLHandler(DLL_LOAD);
							PSTIMERHANDLERLIST psTimerListPtr=
								       m_pouExecuteFunc->psGetTimerListPtr();
						//Restore timer status as it was before B&L
							if(psTempNodeObject!=NULL)
							{
								m_odSetResetTimer.vRestoreTimerStatus(psNodeInfo->
									m_omStrNodeName,psTimerListPtr);
								m_odSetResetTimer.
									vDeleteNodeTimerList(psNodeInfo->m_omStrNodeName);
							}

							m_odSetResetTimer.vAddNewNodeTimerList(psNodeInfo->
								m_omStrNodeName,psTimerListPtr);
						
							m_pouExecuteFunc->vStartTimerThreads();
						//Restore handler's status
							if(psNodeInfo->m_bIsAllHandlersEnabled)
							{
								m_pouExecuteFunc->bActivateDeactivateHandlers(TRUE);
							}
							else 
							{
								if(psNodeInfo->m_bErrorHandlersEnabled)
								{
									m_pouExecuteFunc->bEnableDisableErrorHandlers(TRUE);
								}
								if(psNodeInfo->m_bKeyHandlersEnabled)
								{
									m_pouExecuteFunc->bEnableDisableKeyHandlers(TRUE);
								}
								if(psNodeInfo->m_bMsgHandlersEnabled)
								{
									m_pouExecuteFunc->bEnableDisableMsgHandlers(TRUE);
								}
							}
                            
                            psNodeInfo->bStartThreadProc();
							bReturn =TRUE;
                        }
                    }
               }
			   if(FALSE==bReturn&&psTempNodeObject!=NULL)
			   {
				   m_odSetResetTimer.vDeleteNodeTimerList
					                 (psNodeInfo->m_omStrNodeName);
			   }
           
           }
           else
           {			    
			    omStrArray.Add(defOBJECT_BUILDPROGRAM_MSG);
			    m_pouBuildProgram->bAddString(omStrArray);
				bReturn =FALSE;
               
           }
       }  
  }

  // Create the thread for the timer if a timer is present
  if (psNodeInfo->m_omStrArrayTimerHandlers.GetSize() > 0)
  {
	vStartTimer();
  }

  m_odSetResetTimer.DestroyWindow();
  return bReturn;
}



/***************************************************************************************
    Function Name    :  bExecuteDllBuild
    Input(s)         :  nodeinfo structure list from UI
    Output           :  BOOL
    Functionality    :  compile the file attached to each node of nodelist
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05

***************************************************************************************/
BOOL CExecuteManager::bExecuteDllBuild(PSNODEINFO psNodeInfo)
{
	CStringArray omStrArray;
	CWaitCursor omWait;
	BOOL bReturn=FALSE;
	if(psNodeInfo!=NULL)
	{
		if(!psNodeInfo->m_omStrFileName.IsEmpty())
		{

			if(m_pouBuildProgram==NULL)
			{
				m_pouBuildProgram = new CBuildProgram(m_eBus);
			}
			if(m_pouBuildProgram!=NULL)
			{
				bReturn=m_pouBuildProgram->bBuildProgram(psNodeInfo,FALSE);
			}
			else
			{
				omStrArray.Add(defOBJECT_BUILDPROGRAM_MSG);
				m_pouBuildProgram->bAddString(omStrArray);
				bReturn=FALSE;
			}
		}
	}

	return bReturn;
}

/***************************************************************************************
    Function Name    :  bExecuteDllUnload
    Input(s)         :  nodeinfo structure list from UI
    Output           :  BOOL
    Functionality    :  It unload the DLLs of the list 
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/

BOOL CExecuteManager::bExecuteDllUnload(PSNODEINFO psNodeInfo)
{
	BOOL bReturn=FALSE;
	CStringArray omStrArray;
	CWaitCursor omWait;
	if(psNodeInfo!=NULL)    
	{
		BOOL bFreeLibrary;
		CString omStrMessage;
		PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
		if(psNodeInfo->m_bIsDllLoaded)
		{
			
			BOOL bFound=FALSE;
			while(psTempNodeObject!=NULL&&!bFound)
			{
				if(!psNodeInfo->m_omStrNodeName.CompareNoCase
					               (psTempNodeObject->om_NodeName))
				{
					bFound=TRUE;
				}
				else
				{
					psTempNodeObject=psTempNodeObject->m_psNextNode;
				}
			}
			if(bFound&&psTempNodeObject->m_psExecuteFunc!=NULL)
			{
				 bFreeLibrary= psTempNodeObject->m_psExecuteFunc->bUnloadDll();
				 m_odSetResetTimer.vDeleteNodeTimerList(psTempNodeObject->
					                                             om_NodeName);
				// check if FreeLibrary is successfull unloaded. 
				// Give failure or success message
				if(bFreeLibrary == FALSE)
				{
					omStrMessage  = _T("Unable to unload ");
					omStrMessage += psNodeInfo->m_omStrDllName;
					omStrArray.Add(omStrMessage);
					m_pouBuildProgram->bAddString(omStrArray);
					bReturn=FALSE;
				}
				else
				{
					psTempNodeObject->m_psExecuteFunc->vSetDllHandle(NULL);
					omStrMessage  = _T("Successfully unloaded ");
					omStrMessage += psNodeInfo->m_omStrDllName;
					omStrArray.Add(omStrMessage);
					m_pouBuildProgram->bAddString(omStrArray);
					psNodeInfo->m_bIsDllLoaded=FALSE;
                    psNodeInfo->bTerminateThreadProc();
					vDeleteNode(psNodeInfo->m_omStrNodeName);
					bReturn=TRUE;
				}
			}
			
		}
	}
	m_odSetResetTimer.DestroyWindow();
	return bReturn;
}

/***************************************************************************************
    Function Name    :  bExecuteDllLoad
    Input(s)         :  pointer to SNODEINFO
    Output           :  BOOL
    Functionality    :  It calls the load/unload handler of the node whose NOdeInfo
						structure is passed to it.
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/

BOOL CExecuteManager::bExecuteDllLoad(PSNODEINFO psNodeInfo)
{
	BOOL bFreeLibrary = TRUE;
	BOOL bReturn=FALSE;
	CStringArray omStrArray;
	BOOL bVersion			= FALSE; //ell2kor
    int	iMajorVer ;
    int  iMinorVer ;
	CWaitCursor omWait;
	if(psNodeInfo!=NULL)    
	{
		if(psNodeInfo->m_bIsDllLoaded)
		{
			PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
			BOOL bFound=FALSE;
			while(psTempNodeObject!=NULL&&!bFound)
			{
				if(!psNodeInfo->m_omStrNodeName.CompareNoCase
					(psTempNodeObject->om_NodeName))
				{
					bFound=TRUE;
				}
				else
				{
					psTempNodeObject=psTempNodeObject->m_psNextNode;
				}
			}
			if(psTempNodeObject!=NULL)
			{
				bFreeLibrary= psTempNodeObject->m_psExecuteFunc->bUnloadDll();
				if(bFreeLibrary)
				{
					m_odSetResetTimer.
						vDeleteNodeTimerList(psTempNodeObject->om_NodeName);
					vDeleteNode(psNodeInfo->m_omStrNodeName);
				}
				
			}
		}
		CString omStrDLLName=psNodeInfo->m_omStrDllName;
		// Get the DLL name from source file name by replacing .c
		// with .DLL                                       
		int nIndex  = omStrDLLName.ReverseFind('.');
		if(nIndex!=-1)
		{
			omStrDLLName  = omStrDLLName.Left(nIndex);
			omStrDLLName += defDOT_DLL;
		}
		//save dll name in nodeinfo structure
        psNodeInfo->m_omStrDllName=omStrDLLName;
        HINSTANCE hModuleHandle = LoadLibrary(omStrDLLName);
        if(hModuleHandle != NULL)
        {
            //Get the function pointer of the bGetProgramVersion() 
            PFGET_PRG_VER pFGetPrgVer = (PFGET_PRG_VER)
                                        GetProcAddress( hModuleHandle,
                                        defNAME_FUNC_GET_PRG_VER );
            if( pFGetPrgVer != NULL )
            {
                bVersion = pFGetPrgVer(&iMajorVer,&iMinorVer,hModuleHandle);
                if( bVersion == TRUE )
                {
                    if( iMajorVer != defMAJOR_VERSION &&
                        iMinorVer != defMINOR_VERSION )
                    {
                        CString omStrErrMsg ;
                        omStrErrMsg.Format( defUNSUPPORTED_VER,
                                            iMajorVer, iMinorVer );
                        bFreeLibrary = FALSE ;
                        if( FreeLibrary(hModuleHandle) != 0 )
                            hModuleHandle = NULL;
                        //AfxMessageBox( omStrErrMsg );
                        omStrArray.Add(omStrErrMsg);
                        m_pouBuildProgram->bAddString(omStrArray);
                        /*if (CGlobalObj::g_podNodeToDllMap != NULL)
                        {
                            CGlobalObj::g_podNodeToDllMap->RemoveKey(psNodeInfo->m_omStrDllName);
                        }*/
                    }
                    else
                    {
                        /*if (CGlobalObj::g_podNodeToDllMap != NULL)
                        {
                            CGlobalObj::g_podNodeToDllMap->operator [](psNodeInfo->m_omStrDllName) = hModuleHandle;
                            ::PostMessage(CGlobalObj::hWmdMDIParentFrame,WM_LOAD_UNLOAD, 1, 0);
                        }*/
                    }
                }
            }
            else
            {
                CString omStrErrMsg ;
                omStrErrMsg.Format( defOLD_VER, omStrDLLName );
                bFreeLibrary = FALSE ;
                if( FreeLibrary(hModuleHandle) != 0 )
                {
                    hModuleHandle = NULL;
                }
                //AfxMessageBox(omStrErrMsg);
                omStrArray.Add(omStrErrMsg);
                m_pouBuildProgram->bAddString(omStrArray);
            }
        }
		if(hModuleHandle!=NULL && bFreeLibrary == TRUE)
		{
			CExecuteFunc* m_pouExecuteFunc = new CExecuteFunc(m_eBus, omStrDLLName);
			// check for successfull memory allocation
			if(m_pouExecuteFunc != NULL)
			{
				m_pouExecuteFunc->vSetDllHandle(hModuleHandle);
				m_pouExecuteFunc->vInitialiseInterfaceFnPtrs(hModuleHandle);
                m_pouExecuteFunc->vSetNodeInfo(psNodeInfo);
				BOOL bInitStruct = m_pouExecuteFunc->bInitStruct(omStrArray);
				// If the initialisation of any one structure fails,
				// prompt a message.
				if(bInitStruct == FALSE )
				{
					BOOL bFreeLibrary = FreeLibrary(hModuleHandle);
					CString omStrErrorMsg = defINIT_HANDLER_STRUCT_MSG;
					if(bFreeLibrary == TRUE )
					{
						omStrErrorMsg += defDLL_UNLOADED_MSG;
						omStrErrorMsg.Replace(_T("FILENAME"),omStrDLLName);
						// m_pouExecuteFunc->hGetDllHandle() = NULL;
					}
					omStrArray.Add(omStrErrorMsg);
					m_pouBuildProgram->bAddString(omStrArray);
					bReturn=FALSE;
				}
				else
				{
					CString omStrErrorMsg = _T(" ");
					psNodeInfo->m_bIsDllLoaded=TRUE;
					omStrArray.Add(omStrDLLName + _T(" loaded successfully"));
					m_pouBuildProgram->bAddString(omStrArray);
					CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().
                                           vSetFlagStatus( H_DLLLOADED, TRUE );
					vAddNode(psNodeInfo,m_pouExecuteFunc);
					m_pouExecuteFunc->vExecuteOnDLLHandler(DLL_LOAD);
					//vRestoreTimerStatus(psTimerStruct);
					 // Set the flag for DLL loaded.
                    psNodeInfo->bStartThreadProc();
					PSTIMERHANDLERLIST psTimerListPtr=
								       m_pouExecuteFunc->psGetTimerListPtr();
					m_odSetResetTimer.vAddNewNodeTimerList(psNodeInfo->
								m_omStrNodeName,psTimerListPtr);
					bReturn=TRUE;
				}
			}
		}
		
	}

	// Create the thread for the timer if a timer is present
	if (psNodeInfo->m_omStrArrayTimerHandlers.GetSize() > 0)
	{
		vStartTimer();
	}

	m_odSetResetTimer.DestroyWindow();
	return bReturn;
}

/***************************************************************************************
    Function Name    :  vExSetNodeInfo
    Input(s)         :  node name and its NodeInfo structure
    Output           :  BOOL
    Functionality    :  find the node and call its NodeInfo modification function
						when the user change the data from UI
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vExSetNodeInfo(const CString omStrNodeName ,
									 const PSNODEINFO psNodeInf)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	BOOL bFound=FALSE;
	while(!bFound&&psTempNodeObject!=NULL)
		{
			 if(!omStrNodeName.CompareNoCase(psTempNodeObject->om_NodeName))
			 {
				bFound=TRUE;
			 }
			 else
			 {
				 psTempNodeObject=psTempNodeObject->m_psNextNode;
			 }
		}
	if(bFound)
	{
		psTempNodeObject->m_psExecuteFunc->vSetNodeInfo(psNodeInf);
	}
   
}

/***************************************************************************************
    Function Name    :  vManageTimerThreads
    Input(s)         : 
    Output           :  
    Functionality    :  This function is called each time a DLL is loaded to
						start the timers of the nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vManageTimerThreads()
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	while(psTempNodeObject!=NULL)
	{
	  BOOL bTimerThrdStrtd=psTempNodeObject->m_psExecuteFunc->
						               bIsTimerThreadStarted();
	  if(bTimerThrdStrtd==FALSE)
	  {
			psTempNodeObject->m_psExecuteFunc->vStartTimerThreads();
	  }
	  psTempNodeObject=psTempNodeObject->m_psNextNode;
   }		
}

/***************************************************************************************
    Function Name    :  psGetNodeObjectList
    Input(s)         : 
    Output           :  Pointer to first node of nodeobject list
    Functionality    :  Returns the pointer to NodeObject list
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/

const PSNODEOBJECT CExecuteManager::psGetNodeObjectList()
{
	return  m_psFirstNodeObject;
}

/***************************************************************************************
    Function Name    :  pmGetNodeObject
    Input(s)         :  Handle to a DLL
    Output           :  CExecute func pointer
    Functionality    :  It takes handle to a dll and returns the node object to 
	                    which it associated
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/

CExecuteFunc* CExecuteManager::pmGetNodeObject(HMODULE hModule)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	while(psTempNodeObject!=NULL)
	{
		 HMODULE h_Module=psTempNodeObject->m_psExecuteFunc->hGetDllHandle();
		 if(hModule==h_Module)
		 {
			 return psTempNodeObject->m_psExecuteFunc;
		 }
		 else
		 {
			 psTempNodeObject=psTempNodeObject->m_psNextNode;
		 }
	}	
	return NULL;
}


/***************************************************************************************
    Function Name    :  vManageDllMessageHandler
    Input(s)         :  The structure which contain message structure with DLl
	                    handle from which it has been transmitted.
    Output           :  
    Functionality    :  It takes the SDLL_MSG Structure and if the node's message 
						handler is enabled it writes in its queue.
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vManageDllMessageHandler(SDLL_MSG sDllMessages)
{
	PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
	while( (psTempNodeObject != NULL) && (m_psFirstNodeObject != NULL) )
	{
		if( psTempNodeObject->m_psExecuteFunc != NULL )
		{
			HMODULE h_Module=psTempNodeObject->m_psExecuteFunc->hGetDllHandle();
			if( h_Module != sDllMessages.h_DllHandle)
			{
				//if msg handler is enable then write into buffer
				BOOL bMsgFlag = psTempNodeObject->m_psExecuteFunc->
                                bGetFlagStatus(EXMSG_HANDLER);
				if(bMsgFlag)
				{
                    STCAN_MSG* psRxMsgInfo = (STCAN_MSG*)sDllMessages.sRxMsg;
					psTempNodeObject->m_psExecuteFunc->vWriteInQMsg(*psRxMsgInfo);
				}
			}
		}
		psTempNodeObject=psTempNodeObject->m_psNextNode;
	}	
		
}

/******************************************************************************/
/*  Function Name    :  vSetResetOnTimerHandler                               */    
/*  Input(s)         :													      */    
/*  Output           :                                                        */    
/*  Functionality    :  This function will invoke timer handler dialog and    */
/*                      passthe information stored in the list of structure   */
/*                      to the dialog class. After dialog is close with OK    */
/*                      timers will be set.                                   */     
/*  Member of        :  CExecuteManager                                       */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Anish Kumar                                           */    
/*  Date Created     :  14.01.2006                                            */    
/******************************************************************************/
VOID CExecuteManager::vSetResetOnTimerHandler(const CStringArray& omSysNode,
											          BOOL bFromSimSys)
{
    m_odSetResetTimer.vSetSimSysNodeArray(omSysNode,bFromSimSys);
	vManageTimerThreads();
	m_odSetResetTimer.DestroyWindow();
    // A modelless dialog is created.
    BOOL bIsTimerDialogCreated  = CSimSysManager::ouGetSimSysManager(m_eBus).
                                  ouGetFlags().nGetFlagStatus(H_TIMERBUTTON);
	if(FALSE == bIsTimerDialogCreated)
	{
		BOOL bDlgReturn = m_odSetResetTimer.Create(IDD_DLG_SET_RESET_TIMER, 
				                                                    NULL);
		if(bDlgReturn != 0)
		{
			m_odSetResetTimer.CenterWindow();
			m_odSetResetTimer.ShowWindow(SW_SHOW);
			CSimSysManager::ouGetSimSysManager(m_eBus).
                ouGetFlags().vSetFlagStatus(H_TIMERBUTTON,TRUE);
		}   
	}
}

/******************************************************************************/
/*  Function Name    :  vDestroyTimerDialog                                   */
/*  Input(s)         :  void                                                  */
/*  Output           :  void                                                  */
/*  Functionality    :  This function is will called to Destroy Start/Stop    */
/*                      Timer Dialog. During Disconnect if the                */
/*                      Start/Stop Timer Dialog is visible                    */
/*  Member of        :  CExecuteManager                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  05.02.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CExecuteManager::vDestroyTimerDialog()
{
    m_odSetResetTimer.DestroyWindow();
}



/***************************************************************************************
    Function Name    :  vDeleteAllNode
    Input(s)         : 
    Output           :  
    Functionality    :  Unload and delete all nodes
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
                                                 
***************************************************************************************/
void CExecuteManager::vDeleteAllNode()
{ 
	 PSNODEOBJECT psCurrNodeObject;
	if(m_psFirstNodeObject!=NULL)
	{
		while(m_psFirstNodeObject!=NULL)
		{
			psCurrNodeObject=m_psFirstNodeObject;
			if(psCurrNodeObject->m_psExecuteFunc!=NULL)
			{
				psCurrNodeObject->m_psExecuteFunc->bUnloadDll();
				EnterCriticalSection(&m_CritSectPsNodeObject);
				delete psCurrNodeObject->m_psExecuteFunc;
				psCurrNodeObject->m_psExecuteFunc = NULL;
				LeaveCriticalSection(&m_CritSectPsNodeObject);
			}
			m_odSetResetTimer.vDeleteNodeTimerList(psCurrNodeObject->
				                                           om_NodeName);
			m_psFirstNodeObject=m_psFirstNodeObject->m_psNextNode;
			EnterCriticalSection(&m_CritSectPsNodeObject);
			delete psCurrNodeObject;
			psCurrNodeObject=NULL;
			LeaveCriticalSection(&m_CritSectPsNodeObject);
		}
	}
	m_odSetResetTimer.vStopTimer();
}

/***************************************************************************************
    Function Name    :  vCopyTimersInSetResetMemory
    Input(s)         : 
    Output           :  
    Functionality    :  Copy the timers infomation from each node to the CSetReset
						local timer memory
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
const int SIZE_TIMER_STRUCT = sizeof(STIMERHANDLER);
void CExecuteManager::vCopyTimersInSetResetMemory(PSNODETIMERLIST psSRsetTimers)
{
	PSNODETIMERLIST psCurrSRsetNodeTmrs=psSRsetTimers;
	PSNODEOBJECT psCurrNodeObject;
	while(psCurrSRsetNodeTmrs!=NULL)
	{
		psCurrNodeObject=m_psFirstNodeObject;
		BOOL bFound=FALSE;
		while(!bFound&&psCurrNodeObject!=NULL)
			{
				if(!psCurrNodeObject->om_NodeName.CompareNoCase
					              (psCurrSRsetNodeTmrs->omNodeName))
				{
					bFound=TRUE;
				}
				else(psCurrNodeObject=psCurrNodeObject->m_psNextNode);
			}
		if(psCurrNodeObject!=NULL)
		{
			PSTIMERHANDLERLIST psNodeTimerList,psRSetTimerList;
			psNodeTimerList=psCurrNodeObject->m_psExecuteFunc->
				                               psGetTimerListPtr();
			psRSetTimerList=psCurrSRsetNodeTmrs->psTimerListPtr;
			while(psRSetTimerList!=NULL)
			{
				/*memcpy(&psRSetTimerList->sTimerHandler,
					   &psNodeTimerList->sTimerHandler,
					   SIZE_TIMER_STRUCT);*/
				psRSetTimerList->sTimerHandler.bTimerSelected=
					psNodeTimerList->sTimerHandler.bTimerSelected;
				psRSetTimerList->sTimerHandler.bTimerType=
					psNodeTimerList->sTimerHandler.bTimerType;
				psRSetTimerList->sTimerHandler.unTimerVal=
					psNodeTimerList->sTimerHandler.unTimerVal;
				psRSetTimerList=psRSetTimerList->psNextTimer;
				psNodeTimerList=psNodeTimerList->psNextTimer;
			}
		}
		psCurrSRsetNodeTmrs=psCurrSRsetNodeTmrs->psNextTimerListPtr;
	}
}
 
/***************************************************************************************
    Function Name    :  vUpdateTimersFromUser
    Input(s)         : 
    Output           :  
    Functionality    :  Copy the timers infomation CSetReset to node's local
					    timer memory
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vUpdateTimersFromUser(PSNODETIMERLIST psSRsetTimers)
{
	PSNODETIMERLIST psCurrSRsetNodeTmrs=psSRsetTimers;
	PSNODEOBJECT psCurrNodeObject=m_psFirstNodeObject;
	while(psCurrSRsetNodeTmrs!=NULL)
	{
		psCurrNodeObject=m_psFirstNodeObject;
		BOOL bFound=FALSE;
		while(psCurrNodeObject!=NULL&&!bFound)
			{
				if(!psCurrNodeObject->om_NodeName.CompareNoCase
				            	(psCurrSRsetNodeTmrs->omNodeName))
				{
					bFound=TRUE;
				}
				else
				{
					psCurrNodeObject=psCurrNodeObject->m_psNextNode;
				}
			}
		if(psCurrNodeObject!=NULL)
		{
			PSTIMERHANDLERLIST psNodeTimerList,psRSetTimerList;
			psNodeTimerList=psCurrNodeObject->m_psExecuteFunc->
				                                psGetTimerListPtr();
			psRSetTimerList=psCurrSRsetNodeTmrs->psTimerListPtr;
			while(psRSetTimerList!=NULL)
			{
//				memcpy(&psNodeTimerList->sTimerHandler,
//					   &psRSetTimerList->sTimerHandler,
//					   SIZE_TIMER_STRUCT);
				psNodeTimerList->sTimerHandler.bTimerSelected=
					psRSetTimerList->sTimerHandler.bTimerSelected;
				psNodeTimerList->sTimerHandler.bTimerType=
					psRSetTimerList->sTimerHandler.bTimerType;
				psNodeTimerList->sTimerHandler.unTimerVal=
					psRSetTimerList->sTimerHandler.unTimerVal;

				psRSetTimerList=psRSetTimerList->psNextTimer;
				psNodeTimerList=psNodeTimerList->psNextTimer;
			}
		}
		psCurrSRsetNodeTmrs=psCurrSRsetNodeTmrs->psNextTimerListPtr;
	}
}

/***************************************************************************************
    Function Name    :  vSetResetNodeTimers
    Input(s)         : 
    Output           :  
    Functionality    :  Enable/disable all timers of a node in CSetReset 
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  18.01.06
***************************************************************************************/

void CExecuteManager::vSetResetNodeTimers(CExecuteFunc* m_pCexecuteFunc,
										                    BOOL bEnable)
{
	PSNODEOBJECT psCurrNodeObject=m_psFirstNodeObject;
	BOOL bFound=FALSE;
	while(psCurrNodeObject!=NULL&&!bFound)
	{
		if(psCurrNodeObject->m_psExecuteFunc==m_pCexecuteFunc)
		{
			bFound=TRUE;
		}
		else
		{
			psCurrNodeObject=psCurrNodeObject->m_psNextNode;
		}
	}
	if(psCurrNodeObject!=NULL)
		m_odSetResetTimer.vSetResetAllTimers(psCurrNodeObject->
		                                         om_NodeName,bEnable);
}

/***************************************************************************************
    Function Name    :  vClearOutputWnd
    Input(s)         : 
    Output           :  
    Functionality    :  Call the required functions to clear output window
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  18.01.06
***************************************************************************************/
void CExecuteManager::vClearOutputWnd()
{
	if(m_pouBuildProgram!=NULL)
	{
		m_pouBuildProgram->vClearArray();
	}
	
}

/***************************************************************************************
    Function Name    :  vStopTimer
    Input(s)         : 
    Output           :  
    Functionality    :  Stop multimedia timer
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  28.02.06
***************************************************************************************/
void CExecuteManager::vStopTimer()
{
	m_odSetResetTimer.vStopTimer();
}

/***************************************************************************************
    Function Name    :  vStartTimer
    Input(s)         : 
    Output           :  
    Functionality    :  Start multimedia Timer
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  28.02.06
***************************************************************************************/
void CExecuteManager::vStartTimer()
{
	m_odSetResetTimer.vStartTimer();
}

/***************************************************************************************
    Function Name    :  vManageTimerExecution
    Input(s)         : 
    Output           :  
    Functionality    :  Manage execution of timers
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  04.05.06
***************************************************************************************/
void CExecuteManager::vManageTimerExecution()
{
	PSNODEOBJECT psTempNode = m_psFirstNodeObject;
	while( psTempNode != NULL )
	{
		PSTIMERHANDLERLIST psNodeTimerList = psTempNode->
			                 m_psExecuteFunc->psGetTimerListPtr();
		while( psNodeTimerList != NULL )
		{
			if( psNodeTimerList->sTimerHandler.bTimerSelected )
			 {                                
                ++( psNodeTimerList->sTimerHandler.unCurrTime );

                 if( (psNodeTimerList->sTimerHandler.unCurrTime % 
					                  psNodeTimerList->sTimerHandler.unTimerVal==0) &&
									  psNodeTimerList->sTimerHandler.hDllHandle)
                 {

                     psNodeTimerList->sTimerHandler.omWaitEvent.SetEvent();

                 }

			 }
			psNodeTimerList = psNodeTimerList->psNextTimer;
		}
		psTempNode=psTempNode->m_psNextNode;
	}
}

/***************************************************************************************
    Function Name    :  bDLLBuildAll
    Input(s)         : 
    Output           :  
    Functionality    :  Build all node files
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  
***************************************************************************************/
BOOL CExecuteManager::bDLLBuildAll(CStringArray *pomStrErrorFiles)
{
	BOOL bReturn = FALSE;
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    PSSIMSYSINFO pTempSimsys = 
				CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
    CString omStrFileName = STR_EMPTY;
    int nNodeCount = 0;
    pomStrErrorFiles->RemoveAll();
	vClearOutputWnd();
	while(pTempSimsys != NULL)
	{
		nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
			if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
			{
				//TO find which all files are opened through Framewnd
				CMDIFrameWnd *pMainWnd = (CMDIFrameWnd*)CWnd::FromHandle(CGlobalObj::sm_hWndMDIParentFrame);
				if(pMainWnd != NULL)
				{
					//Get the active child wnd
					CWnd *pWndTemp = pMainWnd->GetActiveFrame();
					if(pWndTemp != NULL)
					{
						CWnd *pWnd = pWndTemp;
						do
						{
							CString omStrWndName;
							CString omStrFileName =pTempNode->m_sNodeInfo.m_omStrFileName;
							int nIndex = omStrFileName.ReverseFind(defCHAR_PATH_SEPRATER);
							int nLength = omStrFileName.GetLength();
							omStrFileName = omStrFileName.Right(nLength - nIndex -1);
							pWnd->GetWindowText(omStrWndName);
							//if the file is opened save it
							if(!(omStrFileName.Compare(omStrWndName)))
							{
								CDocument*(pDoc) = CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->OpenDocumentFile(pTempNode->
															m_sNodeInfo.m_omStrFileName);
								if(pDoc!=NULL)
								{
									// If file name is not empty generate new def file
									pDoc->OnSaveDocument(pTempNode->
										     m_sNodeInfo.m_omStrFileName);
								}
								break;
							}
							pWnd = pWnd->GetNextWindow();
						}
						while((pWndTemp != pWnd) && pWnd != NULL);


						bSuccess = bExecuteDllBuild(&pTempNode->m_sNodeInfo);
						if(!bSuccess)
						{
							nFailure++;
							pomStrErrorFiles->Add(pTempNode->m_sNodeInfo.m_omStrFileName);
						}

					}
				}
			}
			pTempNode = pTempNode->m_psNextNode;
		}
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}
	if((nFailure == 0) || (nFailure != nNodeCount))  
	{ // if the build is successfull atleast for one.

		// to indicate to the tree view about the new dlls built.
		CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
		if(psSimSysTree != NULL)
        {
			psSimSysTree->bPopulateTree();
        }
	}
	if(nFailure == 0)
	{
		bReturn= TRUE;
	}
	return bReturn;
}

/***************************************************************************************
    Function Name    :  bDllLoadAll
    Input(s)         : 
    Output           :  
    Functionality    :  Load all node files
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  
***************************************************************************************/
BOOL CExecuteManager::bDllLoadAll(CStringArray *pomStrErrorFiles)
{
	BOOL bReturn = FALSE;
	BOOL bSuccess = FALSE;
    int nFailure = 0;
    PSSIMSYSINFO pTempSimsys = 
				CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
	int nNodeCount = 0;

    pomStrErrorFiles->RemoveAll();

	vStopTimer();
    vClearOutputWnd();
    while(pTempSimsys != NULL)
	{
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_omStrDllName != "")
            {
                bSuccess = bExecuteDllLoad(&pTempNode->m_sNodeInfo);
                if(!bSuccess)
                {
                    nFailure++;
                    pomStrErrorFiles->Add(pTempNode->m_sNodeInfo.m_omStrDllName);
                }
                else
                {    // to make the status of the handlers disabled
                    pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE;
                }
                
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        pTempSimsys = pTempSimsys->m_psSimsysNext;
        
    }
    if((nFailure == 0) || (nFailure != nNodeCount))  
    {   // if the load is successfull atleast for one dll.
		
        // to indicate to the tree view about the dlls loaded.
        CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
        if(psSimSysTree != NULL)
            psSimSysTree->bPopulateTree();
        //Disable all the handlers
        CSimSysManager &ouSSysManager = CSimSysManager::ouGetSimSysManager(m_eBus);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_ALL_HANDLER, FALSE);                    
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_ERROR_HANDLER, FALSE);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_KEY_HANDLER_ON, FALSE);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_MSGHANDLERBUTTON, FALSE);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_TIMERBUTTON, FALSE);
    }
    
	vStartTimer();
	if(nFailure == 0)
	{
		bReturn= TRUE;
	}
	return bReturn;
}

/***************************************************************************************
    Function Name    :  bDllUnloadAll
    Input(s)         : 
    Output           :  
    Functionality    :  Unload all node loaded files
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  
***************************************************************************************/
BOOL CExecuteManager::bDllUnloadAll(CStringArray *pomStrErrorFiles)
{
	BOOL bReturn = FALSE;
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    PSSIMSYSINFO pTempSimsys = 
				CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
    int nNodeCount = 0;

    CStringArray omStrUnLoadFiles;
    pomStrErrorFiles->RemoveAll();

	vStopTimer();
	vClearOutputWnd();
    while(pTempSimsys != NULL)
    {
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)  // if any dll is loaded 
            {
                bSuccess = bExecuteDllUnload(&pTempNode->m_sNodeInfo);
                if(!bSuccess)
                {
                    nFailure++;
                    pomStrErrorFiles->Add(pTempNode->m_sNodeInfo.m_omStrDllName);
                }
                else
                {    // to make the status of the handlers disabled
                    pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = FALSE;
                    pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE;
                    
                }
                                
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        pTempSimsys = pTempSimsys->m_psSimsysNext;
        
    }
    if((nFailure == 0) || (nFailure != nNodeCount))  
    {   // if the unload is successfull atleast for one dll.
        
        // to indicate to the tree view about the dlls unloaded.
        CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
        if(psSimSysTree != NULL)
            psSimSysTree->bPopulateTree();
        //Disable all the handlers
        CSimSysManager &ouSSysManager = CSimSysManager::ouGetSimSysManager(m_eBus);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_ALL_HANDLER, FALSE);                 
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_ERROR_HANDLER, FALSE);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_KEY_HANDLER_ON, FALSE);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_MSGHANDLERBUTTON, FALSE);
        ouSSysManager.ouGetFlags().vSetFlagStatus(H_TIMERBUTTON, FALSE);
    }

	vStopTimer();

	if(nFailure == 0)
	{
		bReturn= TRUE;
	}
	return bReturn;

}
/***************************************************************************************
    Function Name    :  bDLLBuildLoadAll
    Input(s)         : 
    Output           :  
    Functionality    :  Build and load all node files
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  
***************************************************************************************/
CExecuteFunc* CExecuteManager::pouGetExecuteFunc(DWORD dwClient)
{
    CExecuteFunc* Result = NULL;
    PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    while((psTempNodeObject != NULL)
           && (psTempNodeObject->m_psExecuteFunc != NULL))
    {	   
        sNODEINFO sNodeInfo(m_eBus);
        psTempNodeObject->m_psExecuteFunc->vGetNodeInfo(sNodeInfo);
        if (sNodeInfo.m_dwClientId == dwClient)
        {
            Result = psTempNodeObject->m_psExecuteFunc;
            break;
        }
        psTempNodeObject = psTempNodeObject->m_psNextNode;
    }
    return Result;
}

/***************************************************************************************
    Function Name    :  bDLLBuildLoadAll
    Input(s)         : 
    Output           :  
    Functionality    :  Build and load all node files
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  
***************************************************************************************/
BOOL CExecuteManager::bDLLBuildLoadAll(CStringArray *pomStrErrorFiles)
{
	BOOL bReturn = FALSE;
	BOOL bSuccess = FALSE;
	int nFailure=0;
	PSSIMSYSINFO pTempSimsys = 
		CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
	vStopTimer();
	vClearOutputWnd();
    if (CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate != NULL)
    {
        CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->SaveAllModified();
    }
	int nNodeCount = 0;

	pomStrErrorFiles->RemoveAll();

	while(pTempSimsys != NULL)
	{
		nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;

		while(pTempNode != NULL)
		{
			if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
			{
					bSuccess = 
						bExecuteDllBuildLoad(&pTempNode->m_sNodeInfo);
					if(!bSuccess)
					{
						nFailure++;
						pomStrErrorFiles->Add(pTempNode->m_sNodeInfo.m_omStrFileName);
			         }
		}
			pTempNode = pTempNode->m_psNextNode;
		}
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}

	

    if((nFailure == 0) || (nFailure != nNodeCount))  
    {   // if the build n load is successfull atleast for one.

        // to indicate to the tree view about the dlls loaded.
        CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
        if(psSimSysTree != NULL)
        psSimSysTree->bPopulateTree();

    }
	if(nFailure == 0)
	{
		bReturn= TRUE;
	}

  vStartTimer();
  return bReturn;
}

BOOL CExecuteManager::bActivateDeactivateHandlers(bool bActive,HMODULE hModule)
{
    BOOL bDllFound = FALSE;
	CExecuteFunc* pmCEexecuteFunc = pmGetNodeObject(hModule);

    //If handler is found
    if(pmCEexecuteFunc!=NULL) 
    {
        if (bActive == TRUE)
        {
            vManageTimerThreads();
		    vStartTimer();
        }
        pmCEexecuteFunc->bActivateDeactivateHandlers(bActive);
		bDllFound =  TRUE;
    }
    return bDllFound;
}

void CExecuteManager::vUpdateHandlerDetailsInDetView()
{
    CSimSysDetView* pSimSysDetView = NULL;
    pSimSysDetView = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
    if (NULL != pSimSysDetView)
    {
        pSimSysDetView->vUpdateHandlerList();
    }
}
BOOL CExecuteManager::bIsExist(ETYPE_BUS eBus)
{
    return (sm_pouManager[eBus] != NULL);
}