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
 * \file      MsgContainerBase.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "CommonClass_stdafx.h"
#include "MsgContainerBase.h"
#include "Utility/Utility_Thread.h"

/**
 * \brief Data Copy Thread Procedure
 *
 * Read thread for the messages from DIL buffer
 */
DWORD WINAPI DataCopyThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    if (pThreadParam == NULL)
    {
        // TBD
    }

    CMsgContainerBase* pCurrObj = (CMsgContainerBase*) pThreadParam->m_pBuffer;

    if (pCurrObj == NULL)
    {
        // TBD
    }

    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                // Retrieve message from the driver
                pCurrObj->vRetrieveDataFromBuffer();
                ResetEvent(pThreadParam->m_hActionEvent);
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            case CREATE_TIME_MAP:
            {
                pCurrObj->InitTimeParams();
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
                SetEvent(pThreadParam->m_hActionEvent);
            }
            break;

            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());
    //Sleep(0);
    return 0;
}


CMsgContainerBase::CMsgContainerBase()
{
    m_pRxMsgCallBack  = NULL;
}

CMsgContainerBase::~CMsgContainerBase()
{
    m_sDataCopyThread.bTerminateThread();
}

/**
 * \brief Set Receive Message Callback Pointer
 *
 * Save the call back function pointer from the User
 * for any new Rx/Tx message.
 */
void CMsgContainerBase::vSetRxMsgCallBkPtr(MSG_RX_CALL_BK pFuncPtr)
{
    m_pRxMsgCallBack = pFuncPtr;
}

/**
 * \brief Start Read Thread
 *
 * Start the read thread
 */
BOOL CMsgContainerBase:: bStartReadThread(HANDLE hActionEvent)
{
    BOOL bResult = TRUE;
    m_sDataCopyThread.m_pBuffer = this;
    m_sDataCopyThread.m_hActionEvent = hActionEvent;

    if ( !m_sDataCopyThread.bStartThread(DataCopyThreadProc) )
    {
        // Log error message
        bResult = FALSE;
    }

    return bResult;
}

/**
 * \brief Stop Read Thread
 *
 * Stop the read thread
 */
BOOL CMsgContainerBase:: bStopReadThread()
{
    BOOL bReturn = m_sDataCopyThread.bTerminateThread();
    return bReturn;
}

/**
 * \brief Copy String to TCHAR array
 *
 * String to TCHAR
 */
BOOL CMsgContainerBase::bCopyStringToTCHARArr( TCHAR acDesStr[], CString omSrc, int MaxDesLen )
{
    BOOL bReturn = TRUE;
    memset (acDesStr, _T('\0'), MaxDesLen);

    if (MaxDesLen <= omSrc.GetLength())
    {
        //One for null char
        omSrc = omSrc.Left(MaxDesLen - 1);
        bReturn = FALSE;
    }

    strcpy(acDesStr, omSrc.GetBuffer(MAX_PATH));
    return bReturn;
}
