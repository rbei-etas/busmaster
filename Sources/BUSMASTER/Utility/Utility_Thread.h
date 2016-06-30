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
 * @brief Definition file for CPARAM_THREADPROC class.
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
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
    SUSPEND,
    IDLE
};
#endif

#define THREAD_WAIT_PERIOD       200
#define EXIT_CODE_ABNORMAL       2

class CPARAM_THREADPROC
{
public:
    /** Event on which the thread works on */
    HANDLE m_hActionEvent;

    /** Action to be carried out by the thread */
    UINT m_unActionCode;

    /** Other data buffer, if any */
    LPVOID m_pBuffer;

    /** Communication event from thread to owner */
    HANDLE m_hThread2Owner;

    /** Communication event from owner to thread */
    HANDLE m_hOwner2Thread;

    CPARAM_THREADPROC(void)
    {
        vInitialise();
        m_hNotifyExit = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        m_hThread2Owner = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        m_hOwner2Thread = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }

    ~CPARAM_THREADPROC(void)
    {
        CloseHandle(m_hNotifyExit);
        m_hNotifyExit = nullptr;

        CloseHandle(m_hThread2Owner);
        m_hThread2Owner = nullptr;

        CloseHandle(m_hOwner2Thread);
        m_hOwner2Thread = nullptr;
    }

    BOOL bStartThread(LPTHREAD_START_ROUTINE NewThreadProc);

    BOOL bStartThreadEx(LPTHREAD_START_ROUTINE NewThreadProc,
                        HANDLE hActionEvent, LPVOID pvBuffer);

    BOOL bTerminateThread(void);

    HANDLE hGetExitNotifyEvent(void);

    /**
     * Preemptive function. This returns ONLY
     * when the thread transits into the inactive state.
     */
    BOOL bTransitToInaction(void);

    /**
     * This function brings the thread back
     * into the previous state.
     */
    BOOL bTransitToActiveState(void);

    BOOL bForceTerminateThread();

protected:
    /** Event to notify when the thread exits */
    HANDLE m_hNotifyExit;

    /** Handle of the thread */
    HANDLE m_hThread;

    /** Previous action code. */
    UINT m_unPrevActionCode;

    void vInitialise(void);
};
