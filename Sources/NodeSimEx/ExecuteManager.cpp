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
#include <array>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//flag for exiting from thread
extern BOOL g_bReadDllMsg;
//read dll msg thread pointer

extern UINT unReadDllMsgBuffer(LPVOID pParam);

CExecuteManager* CExecuteManager::sm_pouManager[BUS_TOTAL]= {nullptr};


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
    m_psFirstNodeObject(nullptr),
    m_psLastNodeObject(nullptr),
    m_odSetResetTimer(eBus)
{
    m_eBusStatus = BUS_DISCONNECTED;

    m_eBus = eBus;
    memset(arrhDirNotify, 0, sizeof(arrhDirNotify));
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
    if(m_pouBuildProgram!=nullptr)
    {
        delete m_pouBuildProgram;
        m_pouBuildProgram=nullptr;
    }

}
/************************************************************************
    Function Name    :  hReturnDllHandle
    Input(s)         :  Node name
    Output           :  Handle of DLL of the node
    Functionality    :  Return the handle of dLL of the node if found else
                        return nullptr
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
*************************************************************************/

const HMODULE CExecuteManager::hReturnDllHandle(const CString omStrNodeName)
{
    PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    while(psTempNodeObject!=nullptr)
    {
        if(omStrNodeName==psTempNodeObject->om_NodeName)
        {
            return  psTempNodeObject->m_psExecuteFunc->hGetDllHandle();
        }
        psTempNodeObject=psTempNodeObject->m_psNextNode;
    }
    return nullptr;

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
    if (sm_pouManager[eBus] == nullptr)
    {
        sm_pouManager[eBus] = new CExecuteManager(eBus);
    }
    return *(sm_pouManager[eBus]);
}
void CExecuteManager::vClearObj(ETYPE_BUS eBus)
{
    if (sm_pouManager[eBus] != nullptr)
    {
        delete sm_pouManager[eBus];
        sm_pouManager[eBus] = nullptr;
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
    if(m_psFirstNodeObject==nullptr)
    {
        bReturn= FALSE;
    }

    else if(!omStrNodeName.CompareNoCase(m_psFirstNodeObject->om_NodeName))
    {
        if(m_psFirstNodeObject==m_psLastNodeObject)
        {
            m_psLastNodeObject=nullptr;
        }
        psCurrNodeObject=m_psFirstNodeObject;
        m_psFirstNodeObject=m_psFirstNodeObject->m_psNextNode;
        if(psCurrNodeObject->m_psExecuteFunc!=nullptr)
        {
            EnterCriticalSection(&m_CritSectPsNodeObject);
            delete psCurrNodeObject->m_psExecuteFunc;
            psCurrNodeObject->m_psExecuteFunc=nullptr;
            LeaveCriticalSection(&m_CritSectPsNodeObject);
        }
        delete psCurrNodeObject;
        psCurrNodeObject=nullptr;
        bReturn=TRUE;
    }
    else
    {
        psPrevNodeObject=nullptr;
        psCurrNodeObject=m_psFirstNodeObject;
        while(psCurrNodeObject!=nullptr && omStrNodeName.CompareNoCase
                (psCurrNodeObject->om_NodeName))
        {
            psPrevNodeObject=psCurrNodeObject;
            psCurrNodeObject=psCurrNodeObject->m_psNextNode;
        }
        if(psCurrNodeObject==nullptr)
        {
            bReturn= FALSE;
        }
        else
        {
			if(nullptr != psPrevNodeObject)
			{
            psPrevNodeObject->m_psNextNode=psCurrNodeObject->m_psNextNode;
			}
            if(m_psLastNodeObject==psCurrNodeObject)
            {
                m_psLastNodeObject=psPrevNodeObject;
            }
            if(psCurrNodeObject->m_psExecuteFunc!=nullptr)
            {
                EnterCriticalSection(&m_CritSectPsNodeObject);
                delete psCurrNodeObject->m_psExecuteFunc;
                psCurrNodeObject->m_psExecuteFunc=nullptr;
                LeaveCriticalSection(&m_CritSectPsNodeObject);
            }
            EnterCriticalSection(&m_CritSectPsNodeObject);
            delete psCurrNodeObject;
            psCurrNodeObject=nullptr;
            LeaveCriticalSection(&m_CritSectPsNodeObject);
            bReturn=TRUE;
        }

    }
    return bReturn;


}

/****************************************************************************************
    Function Name    :  vAddNode
    Input(s)         :  Node info structure and corressponding object
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
    if(psNodeObject!=nullptr)
    {
        psNodeObject->om_NodeName=psNodeInfo->m_omStrNodeName;
        psNodeObject->m_psExecuteFunc=pExecuteFunc;
        psNodeObject->m_psNextNode=nullptr;
        //add to the last with last structure pointer equal to nullptr
        if(m_psFirstNodeObject==nullptr)
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
    PSNODEOBJECT psNodeObject=m_psFirstNodeObject;
    while(psNodeObject!=nullptr)
    {
        psNodeObject->m_psExecuteFunc->vEnableDisableAllTimers(bState);
        psNodeObject = psNodeObject->m_psNextNode;
    }
    vManageTimerThreads();
}


/**************************************************************************************
    Function Name    :  vManageOnKeyHandler
    Input(s)         :  key pressed
    Output           :
    Functionality    :  Calls all key handlers for a key
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
    Modification     :  Robin G.K.
                     :  21.10.14, Removed dependancy on unused EXKEY_HANDLER flag.
***************************************************************************************/
void CExecuteManager::vManageOnKeyHandler(UCHAR ucKey)
{
    PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    while(psTempNodeObject!=nullptr)
    {
            psTempNodeObject->m_psExecuteFunc->vExecuteOnKeyHandler(ucKey);
        psTempNodeObject=psTempNodeObject->m_psNextNode;
    }
}

void CExecuteManager::vManageBusEventHandler(eBUSEVEHANDLER eBusEvent)
{
    m_eBusStatus =BUS_DISCONNECTED;
    if ( BUS_CONNECT == eBusEvent )
    {
        m_eBusStatus = BUS_CONNECTED;
    }

    PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    while(psTempNodeObject!=nullptr)
    {
        psTempNodeObject->m_psExecuteFunc->vExecuteOnBusEventHandler(eBusEvent);
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
void CExecuteManager::vManageOnMessageHandlerCAN_(PSTCAN_TIME_MSG sRxMsgInfo, DWORD& dwClientId)
{
    int i = 0;
    EnterCriticalSection(&m_CritSectPsNodeObject);
    PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    while(psTempNodeObject != nullptr)
    {
            if (psTempNodeObject->m_psExecuteFunc->dwGetNodeClientId() == dwClientId)
            {
                psTempNodeObject->m_psExecuteFunc->vWriteInQMsg(*sRxMsgInfo);
            }

        psTempNodeObject = psTempNodeObject->m_psNextNode;
    }
    LeaveCriticalSection(&m_CritSectPsNodeObject);
}

/***************************************************************************************
    Function Name    :  vManageOnMessageHandlerLIN
    Input(s)         :  Message structure
    Output           :
    Functionality    :  Calls all Message handlers for a Message
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***************************************************************************************/
void CExecuteManager::vManageOnMessageHandlerLIN(PSTLIN_TIME_MSG sRxMsgInfo, DWORD& dwClientId)
{
    EnterCriticalSection(&m_CritSectPsNodeObject);
    PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    while(psTempNodeObject != nullptr)
    {
        
            if (psTempNodeObject->m_psExecuteFunc->dwGetNodeClientId() == dwClientId)
            {
                psTempNodeObject->m_psExecuteFunc->vWriteInQMsgLIN(*sRxMsgInfo);
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
    while(psTempNodeObject!=nullptr)
    {
        if (psTempNodeObject->m_psExecuteFunc->dwGetNodeClientId() == dwClientId)
        {

                psTempNodeObject->m_psExecuteFunc->vExecuteOnErrorHandler( eErrorCode,
                        sErrorVal);

        }
        psTempNodeObject=psTempNodeObject->m_psNextNode;
    }
}

/**************************************************************************************
    Function Name    :  vManageOnErrorHandlerLIN
    Input(s)         :  Error Message structure and code
    Output           :
    Functionality    :  Calls all Error handlers for a Error
    Member of        :
    Author(s)        :
    Date Created     :
***************************************************************************************/
void CExecuteManager::vManageOnErrorHandlerLIN(SERROR_INFO_LIN ouErrorInfo, DWORD dwClientId)
{
    PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
    while(psTempNodeObject!=nullptr)
    {
        if (psTempNodeObject->m_psExecuteFunc->dwGetNodeClientId() == dwClientId)
        {
                psTempNodeObject->m_psExecuteFunc->vExecuteOnErrorHandlerLIN( ouErrorInfo);

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
    while((psTempNodeObject != nullptr) && (bContinue == TRUE))
    {
        //Since shRemoteLc indicates Receiver, compare the node LCN with it
        if(dwClientId == psTempNodeObject->m_psExecuteFunc->dwGetNodeClientId())
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
    while((psTempNodeObject != nullptr) && (bContinue == TRUE))
    {
        //Since shRemoteLc indicates Receiver, compare the node LCN with it
        sNODEINFO sNodeInfo(J1939);
        psTempNodeObject->m_psExecuteFunc->vGetNodeInfo(sNodeInfo);
        if(dwClientId == sNodeInfo.m_dwClientId)
        {
            sNodeInfo.m_byPrefAddress = byAddress;
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
/*  Modification on  :                                                        */
/*                                                                            */
/******************************************************************************/
BOOL CExecuteManager::bExecuteDllBuildLoad(PSNODEINFO psNodeInfo, BOOL bDisplaySuccessful)
{
    CStringArray omStrArray;
    //CWaitCursor omWait;
    BOOL bFreeLibrary = TRUE;
    BOOL bReturn=   FALSE;

    if(psNodeInfo!=nullptr)
    {

        if (!psNodeInfo->m_omStrCFileName.IsEmpty())
        {
            // If the object is not created, create it.
            if(m_pouBuildProgram==nullptr)
            {
                m_pouBuildProgram = new CBuildProgram(m_eBus);
            }
            if(m_pouBuildProgram!=nullptr)
            {
                PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
                if(psNodeInfo->m_bIsNodeEnabled)
                {
                    BOOL bFound=FALSE;
                    while(psTempNodeObject!=nullptr&&!bFound)
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
                    if(psTempNodeObject!=nullptr)
                    {
                        bFreeLibrary= psTempNodeObject->m_psExecuteFunc->bUnloadDll();
                        if(bFreeLibrary)
                        {
                            //                        m_odSetResetTimer.
                            //                            vDeleteNodeTimerList(psTempNodeObject->om_NodeName);
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
                        m_pouBuildProgram->bBuildProgram( psNodeInfo,TRUE,bDisplaySuccessful);
                }
                else
                {
                    CString omStrMessage = "";
                    omStrMessage  = "Unable to free resources for loaded ";
                    omStrMessage +=
                        psTempNodeObject->m_psExecuteFunc->omGetPrevFileName();
                    omStrMessage += " file";
                    omStrArray.Add(omStrMessage);
                    m_pouBuildProgram->bAddString(omStrArray);
                    bReturn=FALSE;
                }



                // all handler function structure with function pointer and other
                // informations.
                if(bDLLLoaded == TRUE)
                {
                    HMODULE hModuleHandle  = nullptr;
                    CString omStrDLLName = "";
                    INT nIndex           = 0;
                    BOOL bInitStruct     = FALSE;
                    hModuleHandle=psNodeInfo->m_hModuleHandle;

                    omStrDLLName=psNodeInfo->m_omStrCFileName;


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
                    if(m_pouExecuteFunc != nullptr)
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
                                omStrErrorMsg.Replace("FILENAME",omStrDLLName);

                            }
                            omStrArray.Add(omStrErrorMsg);
                            m_pouBuildProgram->bAddString(omStrArray);
                            psNodeInfo->m_bIsNodeEnabled=FALSE;
                            bReturn =FALSE;
                        }
                        else
                        {
                            psNodeInfo->m_bIsNodeEnabled=TRUE;
                            vAddNode(psNodeInfo,m_pouExecuteFunc);
                            m_pouExecuteFunc->vExecuteOnDLLHandler(DLL_LOAD);
                            PSTIMERHANDLERLIST psTimerListPtr=
                                m_pouExecuteFunc->psGetTimerListPtr();
                            //Restore timer status as it was before B&L
                            if(psTempNodeObject!=nullptr)
                            {
                                m_odSetResetTimer.vRestoreTimerStatus(psNodeInfo->
                                                                      m_omStrNodeName,psTimerListPtr);
                                m_odSetResetTimer.
                                vDeleteNodeTimerList(psNodeInfo->m_omStrNodeName);
                            }

                            m_odSetResetTimer.vAddNewNodeTimerList(psNodeInfo->
                                                                   m_omStrNodeName,psTimerListPtr);

                            m_pouExecuteFunc->vStartTimerThreads();
                            m_pouExecuteFunc->vEnableDisableAllTimers(TRUE);

                            psNodeInfo->bStartThreadProc();
                            bReturn =TRUE;
                        }
                    }
                }
                if(FALSE==bReturn&&psTempNodeObject!=nullptr)
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
BOOL CExecuteManager::bExecuteDllBuild(PSNODEINFO psNodeInfo, BOOL bDisplaySuccessful)
{
    CStringArray omStrArray;
    CWaitCursor omWait;
    BOOL bReturn=FALSE;
    if(psNodeInfo!=nullptr)
    {
        if(!psNodeInfo->m_omStrCFileName.IsEmpty())
        {

            if(m_pouBuildProgram==nullptr)
            {
                m_pouBuildProgram = new CBuildProgram(m_eBus);
            }
            if(m_pouBuildProgram!=nullptr)
            {
                bReturn=m_pouBuildProgram->bBuildProgram(psNodeInfo,FALSE,bDisplaySuccessful);
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
    BOOL bReturn=TRUE;
    CStringArray omStrArray;
    CWaitCursor omWait;
    if(psNodeInfo!=nullptr)
    {
        BOOL bFreeLibrary;
        CString omStrMessage;
        PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
        if(psNodeInfo->m_bIsNodeEnabled && !psNodeInfo->m_omStrDllName.IsEmpty())
        {

            BOOL bFound=FALSE;
            while(psTempNodeObject!=nullptr&&!bFound)
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
            if(bFound&&psTempNodeObject->m_psExecuteFunc!=nullptr)
            {
                bFreeLibrary= psTempNodeObject->m_psExecuteFunc->bUnloadDll();
                m_odSetResetTimer.vDeleteNodeTimerList(psTempNodeObject->
                                                       om_NodeName);
                // check if FreeLibrary is successfull unloaded.
                // Give failure or success message
                if(bFreeLibrary == FALSE)
                {
                    omStrMessage  = "Unable to unload ";
                    omStrMessage += psNodeInfo->m_omStrDllName;
                    omStrArray.Add(omStrMessage);
                    m_pouBuildProgram->bAddString(omStrArray);
                    bReturn=FALSE;
                }
                else
                {
                    psTempNodeObject->m_psExecuteFunc->vSetDllHandle(nullptr);
                    psNodeInfo->bTerminateThreadProc();
                    vDeleteNode(psNodeInfo->m_omStrNodeName);
                    bReturn=TRUE;
                }
            }

        }
    }
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

BOOL CExecuteManager::bExecuteDllLoad(PSNODEINFO psNodeInfo, BOOL bDisplaySuccessful)
{
    BOOL bFreeLibrary = TRUE;
    BOOL bReturn=FALSE;
    CStringArray omStrArray;
    BOOL bVersion           = FALSE; //ell2kor
    int iMajorVer ;
    int  iMinorVer ;
    CWaitCursor omWait;
    if(psNodeInfo!=nullptr)
    {
        if(psNodeInfo->m_bIsNodeEnabled)
        {
            PSNODEOBJECT psTempNodeObject=m_psFirstNodeObject;
            BOOL bFound=FALSE;
            while(psTempNodeObject!=nullptr&&!bFound)
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
            if(psTempNodeObject!=nullptr)
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
        if(hModuleHandle != nullptr)
        {
            //Get the function pointer of the bGetProgramVersion()
            PFGET_PRG_VER pFGetPrgVer = (PFGET_PRG_VER)
                                        GetProcAddress( hModuleHandle,
                                                defNAME_FUNC_GET_PRG_VER );
            if( pFGetPrgVer != nullptr )
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
                        {
                            hModuleHandle = nullptr;
                        }
                        //AfxMessageBox( omStrErrMsg );
                        omStrArray.Add(omStrErrMsg);
                        m_pouBuildProgram->bAddString(omStrArray);
                        /*if (CGlobalObj::g_podNodeToDllMap != nullptr)
                        {
                            CGlobalObj::g_podNodeToDllMap->RemoveKey(psNodeInfo->m_omStrDllName);
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
                    hModuleHandle = nullptr;
                }
                //AfxMessageBox(omStrErrMsg);
                omStrArray.Add(omStrErrMsg);
                m_pouBuildProgram->bAddString(omStrArray);
            }
        }
        if(hModuleHandle!=nullptr && bFreeLibrary == TRUE)
        {
            CExecuteFunc* m_pouExecuteFunc = new CExecuteFunc(m_eBus, omStrDLLName);
            // check for successfull memory allocation
            if(m_pouExecuteFunc != nullptr)
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
                        omStrErrorMsg.Replace("FILENAME",omStrDLLName);
                        // m_pouExecuteFunc->hGetDllHandle() = nullptr;
                    }
                    omStrArray.Add(omStrErrorMsg);
                    m_pouBuildProgram->bAddString(omStrArray);
                    bReturn=FALSE;
                }
                else
                {
                    CString omStrErrorMsg = " ";
                    psNodeInfo->m_bIsNodeEnabled=TRUE;
                    if(bDisplaySuccessful)
                    {
                        omStrArray.Add(omStrDLLName + " loaded successfully");
                        m_pouBuildProgram->bAddString(omStrArray);
                    }

                    vAddNode(psNodeInfo,m_pouExecuteFunc);
                    m_pouExecuteFunc->vExecuteOnDLLHandler(DLL_LOAD);
                    PSTIMERHANDLERLIST psTimerListPtr=
                        m_pouExecuteFunc->psGetTimerListPtr();
                    m_odSetResetTimer.vAddNewNodeTimerList(psNodeInfo->
                                                           m_omStrNodeName,psTimerListPtr);

                    m_pouExecuteFunc->vStartTimerThreads();
                    m_pouExecuteFunc->vEnableDisableAllTimers(TRUE);

                    psNodeInfo->bStartThreadProc();
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
    while(!bFound&&psTempNodeObject!=nullptr)
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
    while(psTempNodeObject!=nullptr)
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
    while(psTempNodeObject!=nullptr)
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
    return nullptr;
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
    while( (psTempNodeObject != nullptr) && (m_psFirstNodeObject != nullptr) )
    {
        if( psTempNodeObject->m_psExecuteFunc != nullptr )
        {
            HMODULE h_Module=psTempNodeObject->m_psExecuteFunc->hGetDllHandle();
            if( h_Module != sDllMessages.h_DllHandle)
            {
                // Write into buffer.
                STCAN_TIME_MSG* psRxMsgInfo = (STCAN_TIME_MSG*)sDllMessages.sRxMsg;
                psTempNodeObject->m_psExecuteFunc->vWriteInQMsg(*psRxMsgInfo);

            }
        }
        psTempNodeObject=psTempNodeObject->m_psNextNode;
    }

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
    if(m_psFirstNodeObject!=nullptr)
    {
        while(m_psFirstNodeObject!=nullptr)
        {
            psCurrNodeObject=m_psFirstNodeObject;
            if(psCurrNodeObject->m_psExecuteFunc!=nullptr)
            {
                psCurrNodeObject->m_psExecuteFunc->bUnloadDll();
                EnterCriticalSection(&m_CritSectPsNodeObject);
                delete psCurrNodeObject->m_psExecuteFunc;
                psCurrNodeObject->m_psExecuteFunc = nullptr;
                LeaveCriticalSection(&m_CritSectPsNodeObject);
            }
            m_odSetResetTimer.vDeleteNodeTimerList(psCurrNodeObject->
                                                   om_NodeName);
            m_psFirstNodeObject=m_psFirstNodeObject->m_psNextNode;
            EnterCriticalSection(&m_CritSectPsNodeObject);
            delete psCurrNodeObject;
            psCurrNodeObject=nullptr;
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
    while(psCurrSRsetNodeTmrs!=nullptr)
    {
        psCurrNodeObject=m_psFirstNodeObject;
        BOOL bFound=FALSE;
        while(!bFound&&psCurrNodeObject!=nullptr)
        {
            if(!psCurrNodeObject->om_NodeName.CompareNoCase
                    (psCurrSRsetNodeTmrs->omNodeName))
            {
                bFound=TRUE;
            }
            else
            {
                (psCurrNodeObject=psCurrNodeObject->m_psNextNode);
            }
        }
        if(psCurrNodeObject!=nullptr)
        {
            PSTIMERHANDLERLIST psNodeTimerList,psRSetTimerList;
            psNodeTimerList=psCurrNodeObject->m_psExecuteFunc->
                            psGetTimerListPtr();
            psRSetTimerList=psCurrSRsetNodeTmrs->psTimerListPtr;
            while(psRSetTimerList!=nullptr)
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
    while(psCurrSRsetNodeTmrs!=nullptr)
    {
        psCurrNodeObject=m_psFirstNodeObject;
        BOOL bFound=FALSE;
        while(psCurrNodeObject!=nullptr&&!bFound)
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
        if(psCurrNodeObject!=nullptr)
        {
            PSTIMERHANDLERLIST psNodeTimerList,psRSetTimerList;
            psNodeTimerList=psCurrNodeObject->m_psExecuteFunc->
                            psGetTimerListPtr();
            psRSetTimerList=psCurrSRsetNodeTmrs->psTimerListPtr;
            while(psRSetTimerList!=nullptr)
            {
                //              memcpy(&psNodeTimerList->sTimerHandler,
                //                     &psRSetTimerList->sTimerHandler,
                //                     SIZE_TIMER_STRUCT);
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
    while(psCurrNodeObject!=nullptr&&!bFound)
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
    if(psCurrNodeObject!=nullptr)
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
    if(m_pouBuildProgram!=nullptr)
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
    while( psTempNode != nullptr )
    {
        PSTIMERHANDLERLIST psNodeTimerList = psTempNode->
                                             m_psExecuteFunc->psGetTimerListPtr();
        while( psNodeTimerList != nullptr )
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
BOOL CExecuteManager::bDLLBuildAll()
{
    BOOL bReturn = FALSE;
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CString omStrFileName = "";
    int nNodeCount = pSimSysNodeInfo->m_unNumberOfNodesAdded;
    CStringArray omStrBuildFiles;
    omStrBuildFiles.RemoveAll();
    vClearOutputWnd();

    PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;
    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_DLL)
        {
            pTempNode->m_sNodeInfo.m_eNodeState = NODE_BUILD_SUCCESS;
        }
        else
        {
            pTempNode->m_sNodeInfo.m_eNodeState = NODE_NOT_BUILT;
        }
        if(pTempNode->m_sNodeInfo.m_omStrCFileName != "" &&
                pTempNode->m_sNodeInfo.m_bIsNodeEnabled==TRUE)
        {
            //Build all modified and Enabled nodes
            bSuccess =
                bExecuteDllBuild(&pTempNode->m_sNodeInfo,FALSE);
            if(!bSuccess)
            {
                nFailure++;
                omStrBuildFiles.Add(pTempNode->m_sNodeInfo.m_omStrCFileName);
            }

        }
        pTempNode = pTempNode->m_psNextNode;
    }

    if((nFailure == 0) || (nFailure != nNodeCount))
    {
        // if the build is successfull atleast for one.

        // to indicate to the tree view about the new dlls built.
        CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();

        if(psSimSysTree != nullptr)
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
    Function Name    :  bDllUnloadAll
    Input(s)         :
    Output           :
    Functionality    :  Unload all node loaded files
    Member of        :  CExecuteManager
    Author(s)        :  Anish kumar
    Date Created     :
***************************************************************************************/
BOOL CExecuteManager::bDllUnloadAll(CStringArray* pomStrErrorFiles)
{
    BOOL bReturn = FALSE;
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    int nNodeCount = 0;

    CStringArray omStrUnLoadFiles;
    pomStrErrorFiles->RemoveAll();

    vStopTimer();
    vClearOutputWnd();
    if(pSimSysNodeInfo != nullptr)
    {
        nNodeCount += pSimSysNodeInfo->m_unNumberOfNodesAdded;
        PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;
        while(pTempNode != nullptr)
        {
            if(pTempNode->m_sNodeInfo.m_bIsNodeEnabled && !pTempNode->m_sNodeInfo.m_omStrDllName.IsEmpty())  // if any dll is loaded
            {
                bSuccess = bExecuteDllUnload(&pTempNode->m_sNodeInfo);
                if(!bSuccess)
                {
                    nFailure++;
                    pomStrErrorFiles->Add(pTempNode->m_sNodeInfo.m_omStrDllName);
                }
            }
            pTempNode = pTempNode->m_psNextNode;
        }
    }


    if((nFailure == 0) || (nFailure != nNodeCount))
    {
        // if the unload is successfull atleast for one dll.

        // to indicate to the tree view about the dlls unloaded.
        CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
        if(psSimSysTree != nullptr)
        {
            psSimSysTree->bPopulateTree();
        }

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
    CExecuteFunc* Result = nullptr;
    PSNODEOBJECT psTempNodeObject = m_psFirstNodeObject;
    while((psTempNodeObject != nullptr)
            && (psTempNodeObject->m_psExecuteFunc != nullptr))
    {
        if (psTempNodeObject->m_psExecuteFunc->dwGetNodeClientId() == dwClient)
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
BOOL CExecuteManager::bDLLBuildLoadAll(CStringArray* pomStrErrorFiles)
{
    BOOL bReturn = FALSE;
    BOOL bSuccess = FALSE;
    int nFailure=0;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    int nNodeCount = pSimSysNodeInfo->m_unNumberOfNodesAdded;

    vStopTimer();
    vClearOutputWnd();
    if (CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate != nullptr)
    {
        CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->SaveAllModified();
    }


    pomStrErrorFiles->RemoveAll();



    PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;

    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo.m_omStrCFileName != "")
        {
            bSuccess =
                bExecuteDllBuildLoad(&pTempNode->m_sNodeInfo,TRUE);
            if(!bSuccess)
            {
                nFailure++;
                pomStrErrorFiles->Add(pTempNode->m_sNodeInfo.m_omStrCFileName);
            }
        }
        pTempNode = pTempNode->m_psNextNode;
    }
    if((nFailure == 0) || (nFailure != nNodeCount))
    {
        // if the build n load is successfull atleast for one.

        // to indicate to the tree view about the dlls loaded.
        CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
        if(psSimSysTree != nullptr)
        {
            psSimSysTree->bPopulateTree();
        }

    }
    if(nFailure == 0)
    {
        bReturn= TRUE;
    }

    vStartTimer();
    return bReturn;
}

/***************************************************************************************
    Function Name    :  bDLLBuildLoadAllEnabled
    Input(s)         :
    Output           :  BOOL
    Functionality    :  Build and load all node files that are enabled
    Member of        :  CExecuteManager
    Author(s)        :  Robin G.K.
    Date Created     :  21.10.14
***************************************************************************************/
BOOL CExecuteManager::bDLLBuildLoadAllEnabled()
{
    BOOL bReturn = FALSE;
    BOOL bSuccess = TRUE;
    CStringArray omStrBuildFiles;
    int nFailure=0;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    int nNodeCount = pSimSysNodeInfo->m_unNumberOfNodesAdded;

    vStopTimer();
    vClearOutputWnd();
    if (CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate != nullptr)
    {
        CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->SaveAllModified();
    }
    omStrBuildFiles.RemoveAll();
    PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;

    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_C_CPP &&
                pTempNode->m_sNodeInfo.m_omStrCFileName != "" && pTempNode->m_sNodeInfo.m_bIsNodeEnabled == true
                && pTempNode->m_sNodeInfo.m_eNodeState != NODE_BUILD_SUCCESS)
        {
            //Build all modified and Enabled nodes
            bSuccess =
                bExecuteDllBuild(&pTempNode->m_sNodeInfo,FALSE);
            if(!bSuccess)
            {
                nFailure++;
                omStrBuildFiles.Add(pTempNode->m_sNodeInfo.m_omStrCFileName);
            }
        }
        pTempNode = pTempNode->m_psNextNode;
    }


    if(nFailure==0)
    {
        omStrBuildFiles.RemoveAll();
        pTempNode = pSimSysNodeInfo->m_psNodesList;
        while(pTempNode != nullptr)
        {
            if((pTempNode->m_sNodeInfo.m_omStrCFileName != ""
                    || pTempNode->m_sNodeInfo.m_omStrDllName != "")
                    && pTempNode->m_sNodeInfo.m_bIsNodeEnabled == true) //Build all nodes except unloaded(or disabled) nodes.
            {
                bSuccess =
                    bExecuteDllLoad(&pTempNode->m_sNodeInfo,FALSE);
                if(!bSuccess)
                {
                    nFailure++;
                    omStrBuildFiles.Add(pTempNode->m_sNodeInfo.m_omStrCFileName);
                    pTempNode->m_sNodeInfo.m_eNodeState = NODE_BUILD_FAIL;
                }
                else
                {
                    pTempNode->m_sNodeInfo.m_eNodeState = NODE_BUILD_SUCCESS;
                }
            }
            pTempNode = pTempNode->m_psNextNode;
        }

    }
    CSimSysTreeView* psSimSysTree = CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
    if(psSimSysTree != nullptr)
    {
        psSimSysTree->bPopulateTree();
    }

    vStartTimer();

    if(nFailure == 0)
    {
        bReturn= TRUE;
    }
    return bReturn;
}

/***************************************************************************************
    Function Name    :  bDLLUnloadAllEnabled
    Input(s)         :
    Output           :  BOOL
    Functionality    :  Unload enabled nodes.
    Member of        :  CExecuteManager
    Author(s)        :  Robin G.K.
    Date Created     :  21.10.14
***************************************************************************************/

BOOL CExecuteManager::bDLLUnloadAllEnabled()
{
    BOOL bReturn = FALSE;
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    int nNodeCount = 0;

    CStringArray omStrUnLoadFiles;
    omStrUnLoadFiles.RemoveAll();

    vStopTimer();
    vClearOutputWnd();

    PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;
    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo.m_bIsNodeEnabled)  // if any dll is loaded
        {
            bSuccess = bExecuteDllUnload(&pTempNode->m_sNodeInfo);
            if(!bSuccess)
            {
                nFailure++;
                omStrUnLoadFiles.Add(pTempNode->m_sNodeInfo.m_omStrDllName);
            }
        }
        pTempNode = pTempNode->m_psNextNode;
    }

    vStopTimer();


    if(nFailure == 0)
    {
        bReturn= TRUE;
    }
    return bReturn;
}
BOOL CExecuteManager::bIsExist(ETYPE_BUS eBus)
{
    return (sm_pouManager[eBus] != nullptr);
}

/***************************************************************************************
    Function Name    :  bDLLUnloadAllEnabled
    Input(s)         :
    Output           :
    Functionality    :  Unload enabled nodes.
    Member of        :  CExecuteManager
    Author(s)        :  Robin G.K.
    Date Created     :  21.10.14
***************************************************************************************/

void CExecuteManager::vSetNodebFileIsModified(std::string strFilePath)
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;
    while(pTempNode != nullptr && strFilePath.find(".cpp") != std::string::npos)
    {
        //Node having the given file name is set as modified.

        if(_stricmp(pTempNode->m_sNodeInfo.m_omStrCFileName,strFilePath.c_str()) == 0)
        {
            pTempNode->m_sNodeInfo.m_eNodeState = NODE_NOT_BUILT;
        }
        pTempNode = pTempNode->m_psNextNode;
    }
}
/************************************************************************
    Function Name    :  strGetFilePath
    Input(s)         :  HANDLE hDir
    Output           :  string
    Functionality    :  Returns file path corresponding to a handle value in
                        m_mapHandleFilePath
    Member of        :  CExecuteManager
    Author(s)        :  Robin G.K.
    Date Created     :  13.11.14
*************************************************************************/
std::string CExecuteManager::strGetFilePath(HANDLE hDir)
{
    return m_mapHandleFilePath.at(hDir);
}
