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
 * \file      Utility_Thread.h
 * \brief     Definition file for CPARAM_THREADPROC class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CPARAM_THREADPROC class.
 */

#pragma once

#if !defined _ETHREAD_ACTION_
#define _ETHREAD_ACTION_
enum eTHREAD_ACTION
{
    EXIT_THREAD = 0,
    INVOKE_FUNCTION,
    CREATE_TIME_MAP,
    INACTION,
    IDLE
};
#endif

#define THREAD_WAIT_PERIOD       200
#define EXIT_CODE_ABNORMAL       2


class CPARAM_THREADPROC
{
protected:
    HANDLE m_hNotifyExit;    // Event to notify when the thread exits
    HANDLE m_hThread;        // Handle of the thread

    UINT   m_unPrevActionCode; // Previous action code.

    void vInitialise(void);

public:
    HANDLE m_hActionEvent;   // Event on which the thread works on
    UINT m_unActionCode;     // Action to be carried out by the thread
    LPVOID m_pBuffer;        // Other data buffer, if any
    HANDLE m_hThread2Owner;  // Communication event from thread to owner
    HANDLE m_hOwner2Thread;  // Communication event from owner to thread

    CPARAM_THREADPROC(void)
    {
        vInitialise();
        m_hNotifyExit = CreateEvent(NULL, FALSE, FALSE, NULL);
        //ASSERT(NULL != m_hNotifyExit);
        m_hThread2Owner = CreateEvent(NULL, FALSE, FALSE, NULL);
        //ASSERT(NULL != m_hThread2Owner);
        m_hOwner2Thread = CreateEvent(NULL, FALSE, FALSE, NULL);
        //ASSERT(NULL != m_hOwner2Thread);
    }
    ~CPARAM_THREADPROC(void)
    {
        CloseHandle(m_hNotifyExit);
        m_hNotifyExit = NULL;
        CloseHandle(m_hThread2Owner);
        m_hThread2Owner = NULL;
        CloseHandle(m_hOwner2Thread);
        m_hOwner2Thread = NULL;
    }

    BOOL bStartThread(LPTHREAD_START_ROUTINE NewThreadProc);
    BOOL bStartThreadEx(LPTHREAD_START_ROUTINE NewThreadProc,
                        HANDLE hActionEvent, LPVOID pvBuffer);
    BOOL bTerminateThread(void);
    HANDLE hGetExitNotifyEvent(void);

    BOOL bTransitToInaction(void); // Preemptive function. This returns ONLY
    // when the thread transits into the inactive state.
    BOOL bTransitToActiveState(void); // This function brings the thread back
    // into the previous state.
};
