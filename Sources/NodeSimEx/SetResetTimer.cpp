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
 * \file      SetResetTimer.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
// Include all standard header files
#include "NodeSimEx_stdafx.h"
// CSetResetTimer class definition
#include "SetResetTimer.h"
//accessin manager class object
#include "ExecuteManager.h"
#include "SimSysManager.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEvent omMainTimerEvent;
//Timer callback for timer handlers
//Bus Type
ETYPE_BUS CSetResetTimer::sm_eBus = CAN;
void CALLBACK TimerProc(UINT uTimerID, UINT uMsg, DWORD* dwUser,
                        DWORD* dw1, DWORD* dw2);

UINT CalcTimersExecTime(LPVOID pParam);

/******************************************************************************/
/*  Function Name    :  CSetResetTimer                                        */
/*  Input(s)         : pointer to timer structure and pointer to CWnd class   */
/*  Output           :                                                        */
/*  Functionality    :  This is default constructor. This will initialise     */
/*                      pointer to structure m_asTimer with address passed    */
/*                      as parameter                                          */
/*                                                                            */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2002                                            */
/******************************************************************************/
CSetResetTimer::CSetResetTimer(ETYPE_BUS eBus)
{
    m_psFirstNodeTimerList = nullptr;
    m_psLastNodeTimerList = nullptr;
    m_psCalTimerThreadStruct = nullptr;
    m_unTimerID = 0;
    m_eBus = eBus;
}


/******************************************************************************/
/*  Function Name    :  vSetResetAllTimers                                    */
/*  Input(s)         :  BOOL                                                  */
/*  Output           :  void                                                  */
/*  Functionality    :  This function will be called to Set/Reset all valid   */
/*                      timers in the user dll. This will change the global   */
/*                      structure and will start/stop timers using the func   */
/*                      vStartStopTimers().                                   */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  04.02.2004                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   23.12.05,populated the timer list box with selected  */
/*                       node's timer list                                    */
/******************************************************************************/
void CSetResetTimer::vSetResetAllTimers(CString om_StrNode,BOOL bEnable)
{
    PSNODETIMERLIST ps_CurrNodeTrLst=m_psFirstNodeTimerList;
    BOOL bFound=FALSE;
    while(ps_CurrNodeTrLst!=nullptr&&!bFound)
    {
        if(om_StrNode==ps_CurrNodeTrLst->omNodeName)
        {
            bFound=TRUE;
        }
        else
        {
            ps_CurrNodeTrLst=ps_CurrNodeTrLst->psNextTimerListPtr;
        }
    }
    if(ps_CurrNodeTrLst!=nullptr)
    {
        PSTIMERHANDLERLIST psTimerLst=ps_CurrNodeTrLst->psTimerListPtr;
        while(psTimerLst!=nullptr)
        {
            psTimerLst->sTimerHandler.bTimerSelected=bEnable;
            psTimerLst->sTimerHandler.unCurrTime=0;
            psTimerLst=psTimerLst->psNextTimer;
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vAddNewNodeTimerList                                  */
/*  Input(s)         :  NodeName,timer list of node                           */
/*  Output           :                                                        */
/*  Functionality    :  This function adds a node's timer list to local list  */
/*                      of timer and node structure                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  14.01.05                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
const int SIZE_TIMER_STRUCT = sizeof(STIMERHANDLER);
void CSetResetTimer::vAddNewNodeTimerList(CString om_NodeName,
        PSTIMERHANDLERLIST psTimerStrList)
{
    //Make a local timer list for the node
    PSTIMERHANDLERLIST psFirstIndex=nullptr;
    PSTIMERHANDLERLIST psLastIndex=nullptr;
    PSTIMERHANDLERLIST ps_CurrTimer=psTimerStrList;
    //copy the timer list
    while(ps_CurrTimer!=nullptr)
    {
        PSTIMERHANDLERLIST psTimerLst=new STIMERHANDLERLIST;
        psTimerLst->psNextTimer=nullptr;

        psTimerLst->sTimerHandler.omStrTimerHandlerName =
            ps_CurrTimer->sTimerHandler.omStrTimerHandlerName ;
        psTimerLst->sTimerHandler.bTimerType =
            ps_CurrTimer->sTimerHandler.bTimerType;
        psTimerLst->sTimerHandler.bTimerSelected =
            ps_CurrTimer->sTimerHandler.bTimerSelected;
        psTimerLst->sTimerHandler.unTimerVal =
            ps_CurrTimer->sTimerHandler.unTimerVal;
        psTimerLst->sTimerHandler.pFTimerHandler =
            ps_CurrTimer->sTimerHandler.pFTimerHandler;
        psTimerLst->sTimerHandler.unTimerID =
            ps_CurrTimer->sTimerHandler.unTimerID;
        psTimerLst->sTimerHandler.unCurrTime =
            ps_CurrTimer->sTimerHandler.unCurrTime;
        psTimerLst->sTimerHandler.hDllHandle =
            ps_CurrTimer->sTimerHandler.hDllHandle;
        psTimerLst->sTimerHandler.pomThreadHandle =
            ps_CurrTimer->sTimerHandler.pomThreadHandle;




        if(psFirstIndex!=nullptr)
        {
            psLastIndex->psNextTimer=psTimerLst;
            psLastIndex=psTimerLst;
        }
        else
        {
            psFirstIndex=psTimerLst;
            psLastIndex=psTimerLst;
        }
        ps_CurrTimer=ps_CurrTimer->psNextTimer;
    }
    //After the node's timer list is prepared add it to node timer list
    PSNODETIMERLIST psNodeTimer=new SNODETIMERLIST;
    if(psNodeTimer!=nullptr)
    {
        psNodeTimer->omNodeName=om_NodeName;
        //Assign the first pointer of the list to NodeTimer structure
        psNodeTimer->psTimerListPtr=psFirstIndex;
        psNodeTimer->psNextTimerListPtr=nullptr;
        //Add to NodeTimer List
        if(m_psFirstNodeTimerList!=nullptr)
        {
            m_psLastNodeTimerList->psNextTimerListPtr=psNodeTimer;
            m_psLastNodeTimerList=psNodeTimer;
        }
        else
        {
            m_psFirstNodeTimerList=psNodeTimer;
            m_psLastNodeTimerList=m_psFirstNodeTimerList;
        }
    }
}


/******************************************************************************/
/*  Function Name    :  vDeleteNodeTimerList                                  */
/*  Input(s)         :  NodeName                                              */
/*  Output           :                                                        */
/*  Functionality    :  This function deletes a node's timer list of local    */
/*                      list of timer and node structure                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  14.01.05                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CSetResetTimer::vDeleteNodeTimerList(CString om_NodeName)
{
    if(m_psFirstNodeTimerList!=nullptr)
    {
        PSNODETIMERLIST psPrevNodeList,psCurrNodeList;
        //If it is first structure
        if(om_NodeName==m_psFirstNodeTimerList->omNodeName)
        {
            psCurrNodeList=m_psFirstNodeTimerList;
            if(m_psFirstNodeTimerList==m_psLastNodeTimerList)
            {
                m_psLastNodeTimerList=nullptr;
            }
            m_psFirstNodeTimerList=m_psFirstNodeTimerList->psNextTimerListPtr;
            if(psCurrNodeList->psTimerListPtr!=nullptr)
            {
                vDeleteTimerListOfNode(psCurrNodeList->psTimerListPtr);
            }
            delete psCurrNodeList;
            psCurrNodeList=nullptr;
        }
        else
        {
            psPrevNodeList=nullptr;
            psCurrNodeList=m_psFirstNodeTimerList;
            while(psCurrNodeList!=nullptr && om_NodeName!=psCurrNodeList->omNodeName)
            {
                psPrevNodeList=psCurrNodeList;
                psCurrNodeList=psCurrNodeList->psNextTimerListPtr;
            }
            if(psCurrNodeList!=nullptr)
            {
                if(nullptr != psPrevNodeList)
                {
                    psPrevNodeList->psNextTimerListPtr=psCurrNodeList->psNextTimerListPtr;
                }
                if(psCurrNodeList==m_psLastNodeTimerList)
                {
                    m_psLastNodeTimerList=psPrevNodeList;
                }
                if(psCurrNodeList->psTimerListPtr!=nullptr)
                {
                    vDeleteTimerListOfNode(psCurrNodeList->psTimerListPtr);
                }
                delete psCurrNodeList;
                psCurrNodeList=nullptr;
            }

        }
    }
}

/*************************************************************************
    Function Name    :  vStartTimer
    Input(s)         :
    Output           :
    Functionality    :  Starts the only multimedia timer for all timers
    Member of        :  CSetResetTimer
    Author(s)        :  Anish kumar
    Date Created     :  14.01.06
**************************************************************************/
void CSetResetTimer::vStartTimer()
{
    omMainTimerEvent.ResetEvent();
    if(m_psCalTimerThreadStruct == nullptr)
    {
        m_psCalTimerThreadStruct = new SCALCEXECTIMERTHREAD;
        m_psCalTimerThreadStruct->m_pomThreadPtr=nullptr;
    }
    if(m_psCalTimerThreadStruct->m_pomThreadPtr == nullptr)
    {
        CSetResetTimer::sm_eBus = m_eBus;
        m_psCalTimerThreadStruct->m_bThreadStop = FALSE;
        m_psCalTimerThreadStruct->m_omExitThreadEvent.ResetEvent();
        m_psCalTimerThreadStruct->m_eBus = m_eBus;
        m_psCalTimerThreadStruct->m_pomThreadPtr
            = AfxBeginThread(CalcTimersExecTime,m_psCalTimerThreadStruct);
    }
    if(m_unTimerID == 0)
    {

        m_unTimerID = timeSetEvent(1, 0, (LPTIMECALLBACK)TimerProc, 0,
                                   TIME_CALLBACK_FUNCTION|TIME_PERIODIC );
    }
}



/*************************************************************************
    Function Name    :  vStopTimer
    Input(s)         :
    Output           :
    Functionality    :  Stop the multimedia timer
    Member of        :  CSetResetTimer
    Author(s)        :  Anish kumar
    Date Created     :  14.01.06
    Modification By  :  Anish Kumar
    Modification on  :  20.11.06,set the event for timerproc before
                        terminating it
**************************************************************************/
void CSetResetTimer::vStopTimer()
{
    if(m_psCalTimerThreadStruct != nullptr)
    {
        m_psCalTimerThreadStruct->m_bThreadStop = TRUE;
        omMainTimerEvent.SetEvent();
        WaitForSingleObject(m_psCalTimerThreadStruct->m_omExitThreadEvent,20);
        timeKillEvent(m_unTimerID);
        m_unTimerID = 0;
        if(m_psCalTimerThreadStruct->m_pomThreadPtr != nullptr)
        {
            TerminateThread(m_psCalTimerThreadStruct->m_pomThreadPtr->m_hThread,0);
            m_psCalTimerThreadStruct->m_pomThreadPtr=nullptr;
        }
        delete m_psCalTimerThreadStruct;
        m_psCalTimerThreadStruct=nullptr;
    }
}

/******************************************************************************/
/*  Function Name    :  vDeleteTimerListOfNode                                */
/*  Input(s)         :  Tmer List pointer                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function deletes a node's timer list             */
/*  Member of        :  CSetResetTimer                                        */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  07.02.06                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CSetResetTimer::vDeleteTimerListOfNode(PSTIMERHANDLERLIST psListOfTimer)
{
    if(psListOfTimer!=nullptr)
    {
        PSTIMERHANDLERLIST psCurrTimerLst;
        while(psListOfTimer!=nullptr)
        {
            psCurrTimerLst=psListOfTimer;
            psListOfTimer=psListOfTimer->psNextTimer;
            delete psCurrTimerLst;
            psCurrTimerLst=nullptr;
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vRestoreTimerStatus                                   */
/*  Input(s)         :  Tmer List pointer,Node Namme                          */
/*  Output           :                                                        */
/*  Functionality    :  This function Restore a node's timer status on B&L    */
/*  Member of        :  CSetResetTimer                                        */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  24.02.06                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CSetResetTimer::vRestoreTimerStatus(CString omNodeName,
        PSTIMERHANDLERLIST psTimerListPtr)
{
    if(m_psFirstNodeTimerList!=nullptr)
    {
        PSNODETIMERLIST psCurrNodeTimerLst=m_psFirstNodeTimerList;
        BOOL bFound=FALSE;
        while(psCurrNodeTimerLst!=nullptr&&!bFound)
        {
            if(omNodeName==psCurrNodeTimerLst->omNodeName)
            {
                bFound=TRUE;
            }
            else
            {
                psCurrNodeTimerLst=psCurrNodeTimerLst->psNextTimerListPtr;
            }
        }
        if(bFound)
        {
            PSTIMERHANDLERLIST psCurrTimerLst=psCurrNodeTimerLst->psTimerListPtr;
            while(psCurrTimerLst!=nullptr)
            {
                PSTIMERHANDLERLIST psLocalTLst=psTimerListPtr;
                bFound=FALSE;
                while(psLocalTLst!=nullptr&&!bFound)
                {
                    if(psLocalTLst->sTimerHandler.omStrTimerHandlerName==
                            psCurrTimerLst->sTimerHandler.omStrTimerHandlerName)
                    {
                        psLocalTLst->sTimerHandler.bTimerSelected=
                            psCurrTimerLst->sTimerHandler.bTimerSelected;
                        psLocalTLst->sTimerHandler.bTimerType=
                            psCurrTimerLst->sTimerHandler.bTimerType;
                        bFound=TRUE;
                    }
                    else
                    {
                        psLocalTLst=psLocalTLst->psNextTimer;
                    }
                }
                psCurrTimerLst=psCurrTimerLst->psNextTimer;
            }
        }
    }
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

UINT CalcTimersExecTime(LPVOID pParam)
{
    PSCALCEXECTIMERTHREAD psThreadInfo = (PSCALCEXECTIMERTHREAD)pParam;
    while( !psThreadInfo->m_bThreadStop )
    {
        WaitForSingleObject(omMainTimerEvent,INFINITE);
        omMainTimerEvent.ResetEvent();
        if(!psThreadInfo->m_bThreadStop)
        {
            CExecuteManager::ouGetExecuteManager(psThreadInfo->m_eBus).vManageTimerExecution(); //Added for issue #356
        }
    }
    psThreadInfo->m_pomThreadPtr = nullptr;
    psThreadInfo->m_omExitThreadEvent.SetEvent();
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
    omMainTimerEvent.SetEvent();
}
