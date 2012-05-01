/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  ReadCanMsg.cpp
  Description   :
  $Log:   X:/Archive/Sources/DIL_J1939/ReadCanMsg.cpv  $

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "DIL_J1939_stdafx.h"
#include "J1939_UtilityFuncs.h"
#include "ReadCanMsg.h"
#include "NetworkMgmt.h"

UINT g_unCount = 0;

CReadCanMsg::CReadCanMsg(void)
{
    m_omHandleToNodeMgrMap.RemoveAll();
    // First create a default event.
    m_ahActionEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (NULL != m_ahActionEvent[0])
    {
        m_nEvents = 1;
    }
    else
    {
        m_nEvents = 0;
    }

    vReset(); // Reset rest of the events
}

CReadCanMsg::~CReadCanMsg(void)
{
    if (NULL != m_ahActionEvent[0])
    {
        CloseHandle(m_ahActionEvent[0]);
        m_ahActionEvent[0] = NULL;
        m_nEvents = 0;
    }
}

void CReadCanMsg::vReset(void)
{
    // Initialise to null all the events but the first one.
    for (int i = 1; i < m_nEvents; i++)
    {
        m_ahActionEvent[i] = NULL;
    }

    m_nEvents = 1;
}

HRESULT CReadCanMsg::AddEventHandle(HANDLE hHandle, BYTE byNodeMgrIndex)
{
    HRESULT Result = S_FALSE;
    // The thread should be inactive so long as the updation operation is
    // going on.
    BOOL bInaction = m_ouThreadUtil.bTransitToInaction();

    if (bInaction)
    {
        if ((NULL != hHandle) && (m_nEvents < DEF_MAX_SIMULATED_NODE))
        {
            m_ahActionEvent[m_nEvents++] = hHandle;
            m_omHandleToNodeMgrMap.SetAt(hHandle, byNodeMgrIndex);
            Result = S_OK;
        }
    }

    bInaction = m_ouThreadUtil.bTransitToActiveState(); // End of inaction
    return Result;
}

BOOL CReadCanMsg::bDeleteEventHandle(HANDLE handle)
{
    BOOL bResult = FALSE;
    // The thread should be inactive so long as the updation operation is
    // going on.
    bResult = m_ouThreadUtil.bTransitToInaction();

    if (bResult)
    {
        /* Go ahead iff there is at least one client handle.*/
        if (m_nEvents > 1)
        {
            ASSERT(handle != NULL); // Not expected to be null
            //Find the index of the handle
            int Index = 0;
            BOOL bFound = FALSE;

            while ((Index < m_nEvents) && (bFound == FALSE))
            {
                if (handle == m_ahActionEvent[Index])
                {
                    bFound = TRUE;
                }
                else
                {
                    Index++;
                }
            }

            if (bFound)
            {
                /* A hole will be created. So make the list contiguous by moving slots
                down. If its the last slot of the array, then no movement takes place*/
                m_omHandleToNodeMgrMap.RemoveKey(m_ahActionEvent[Index]);
                // Removing an entry means that a hole will be created. This
                // means the last event handle can be directly copied onto the
                // hole and the last entry can be removed.
                m_ahActionEvent[Index] = m_ahActionEvent[m_nEvents - 1];
                m_ahActionEvent[m_nEvents - 1] = NULL;
                m_nEvents--;
                bResult = TRUE;
            }
        }
    }

    bResult = m_ouThreadUtil.bTransitToActiveState(); // End of inaction
    return bResult;
}
DWORD WINAPI ReadDILCANMsg(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    if (pThreadParam != NULL)
    {
        CReadCanMsg* pCurrObj = (CReadCanMsg*) pThreadParam->m_pBuffer;

        if (pCurrObj != NULL)
        {
            pThreadParam->m_unActionCode = INVOKE_FUNCTION; // Set default action
            // Create the action event. In this case this will be used solely for
            // thread exit procedure. The first entry will be used.
            pThreadParam->m_hActionEvent = pCurrObj->m_ahActionEvent[0];
            DWORD dwWaitRet;
            BYTE byHIndex;
            bool bLoopON = true;

            while (bLoopON)
            {
                dwWaitRet = WaitForMultipleObjects(pCurrObj->m_nEvents,
                                                   pCurrObj->m_ahActionEvent, FALSE, INFINITE);
                ///// TEMP : BEGIN
                DWORD dwLLimit = WAIT_OBJECT_0;
                DWORD dwULimit = WAIT_OBJECT_0 + pCurrObj->m_nEvents - 1;
                DWORD dwLLError = WAIT_ABANDONED_0;
                DWORD dwULError = WAIT_ABANDONED_0 + pCurrObj->m_nEvents - 1;

                if ((dwWaitRet >= dwLLimit) && (dwWaitRet <= dwULimit))
                {
                    switch (pThreadParam->m_unActionCode)
                    {
                        case INVOKE_FUNCTION:
                        {
                            //Get the handle's index and pass it
                            byHIndex = (BYTE)(dwWaitRet - WAIT_OBJECT_0);
                            HANDLE hHandleSet = pCurrObj->m_ahActionEvent[byHIndex];
                            BYTE byNodeIndex;

                            if( pCurrObj->m_omHandleToNodeMgrMap.Lookup(hHandleSet, byNodeIndex))
                            {
                                //vRetrieveDataFromBuffer to read from that buffer
                                pCurrObj->vRetrieveDataFromBuffer(byNodeIndex);
                            }

                            //BOOL Result = ResetEvent(hHandleSet);
                            ResetEvent(hHandleSet);
                        }
                        break;

                        case EXIT_THREAD:
                        {
                            bLoopON = false;
                        }
                        break;

                        case INACTION:
                        {
                            // Signal the owner
                            SetEvent(pThreadParam->m_hThread2Owner);
                            Sleep(0);
                            // Wait until owner signals back.
                            WaitForSingleObject(pThreadParam->m_hOwner2Thread, INFINITE);
                            // Signal the owner
                            SetEvent(pThreadParam->m_hThread2Owner);
                            Sleep(0);
                        }
                        break;

                        case CREATE_TIME_MAP:
                        default:
                            break;
                    }
                }
                else if ((dwWaitRet >= dwLLError) && (dwWaitRet <= dwULError))
                {
                    TRACE(_T("Abandoned... %X %d\n"), dwWaitRet, g_unCount++);
                }
                else if ( dwWaitRet == WAIT_TIMEOUT)
                {
                    TRACE(_T("ReadDILCANMsg->WAIT_TIMEOUT %d\n"), g_unCount++);
                }
                else if (dwWaitRet == WAIT_FAILED)
                {
                    TRACE(_T("WAIT_FAILED... %X %d\n"), GetLastError(), g_unCount++);
                }

                ///// TEMP : END
            }

            SetEvent(pThreadParam->hGetExitNotifyEvent());
        }
    }

    return 0;
}

void CReadCanMsg::vRetrieveDataFromBuffer(BYTE byIndex)
{
    //Get the node manager and indicate to read data
    CNodeConManager* pouNodeManager =
        CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(byIndex);

    if (pouNodeManager != NULL)
    {
        pouNodeManager->vReadCANdataBuffer();
    }
}

void CReadCanMsg::vDoInit(void)
{
    m_ouThreadUtil.m_pBuffer = (LPVOID) this;
    BOOL Result = m_ouThreadUtil.bStartThread(ReadDILCANMsg);
    ASSERT(Result);
}

void CReadCanMsg::vDoExit(void)
{
    //BOOL Result = m_ouThreadUtil.bTerminateThread();
    m_ouThreadUtil.bTerminateThread();
}
