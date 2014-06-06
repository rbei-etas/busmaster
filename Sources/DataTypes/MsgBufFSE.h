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
 * \file      MsgBufFSE.h
 * \brief     Defines and implements the template class for circular queue
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines and implements the template class for circular queue
 */

#pragma once

/* See http://stackoverflow.com/questions/3051992/compiler-warning-at-c-template-base-class for why this is disabled. */
#pragma warning(disable:4505)

#include "include/Error.h"
#include "BaseMsgBufAll.h"

const int SIZE_APP_BUFFER = 20000;

/**
 * This is the concrete template class of a circular queue where each entry is
 * of fixed size. Implemented as a template class so as to cater to any data type.
 * Here SMSGBUFFER is the data type in operation.
 */
template <typename SMSGBUFFER>
class CMsgBufFSE : public CBaseMsgBufFSE<SMSGBUFFER>
{
public:
    CMsgBufFSE();
    ~CMsgBufFSE();

    /**
     * Reads a message entry and advances the read index. On
     * successful reading operation the present entry is
     * invalidated to make room for a new entry.
     *
     * @param[out] psMsgBuffer The target message entry.
     * @return EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
     */
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer);

    /**
     * To read an entry from the circular queue
     */
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, __int64 nSlotId);

    /**
     * To read an entry from the circular queue
     */
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, int nIndex);

    /**
     * Writes a message entry and advances the write index.
     *
     * @param[in] psMsgBuffer The source message entry.
     * @return ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
     */
    HRESULT WriteIntoBuffer(SMSGBUFFER* psMsgBuffer);

    /**
     * To write an entry into the circular queue
     */
    HRESULT WriteIntoBuffer(const SMSGBUFFER* psMsgBuffer, __int64 nSlotId, int& nIndex);

    /**
     * Returns the number of unread entries in the queue.
     *
     * @return Number of message entries (int)
     */
    int GetMsgCount(void) const;

    /**
     * To set the current queue length
     */
    int nSetBufferMsgSize(int nMsgDataSize);

    /**
     * Clears the message buffer and resets the indices.
     */
    void vClearMessageBuffer(void);

    /**
     * Returns handle of the event that gets signalled when
     * a message entry is added.
     *
     * @return The notifying event handle (HANDLE)
     */
    HANDLE hGetNotifyingEvent(void) const;

protected:
    /** The data buffer */
    SMSGBUFFER m_asMsgBuffer[SIZE_APP_BUFFER];
    
    /** To make it thread safe */
    CRITICAL_SECTION m_CritSectionForGB;

    /** Current read index */
    int m_nIndexRead;

    /** Current write index */
    int m_nIndexWrite;

    /** Number of message entries */
    int m_nMsgCount;

    /**
     * At the beginning we need to store size of a message entry.
     * This information will be used frequently
     */
    int m_nMsgSize;

    /** Event to be signalled when there is at least one message */
    HANDLE m_hNotifyingEvent;
};

template <typename SMSGBUFFER>
CMsgBufFSE<SMSGBUFFER>::CMsgBufFSE()
{
    m_nMsgSize = sizeof(SMSGBUFFER);
    vClearMessageBuffer();
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

template <typename SMSGBUFFER>
CMsgBufFSE<SMSGBUFFER>::~CMsgBufFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = nullptr;
    DeleteCriticalSection(&m_CritSectionForGB);
}

template <typename SMSGBUFFER> void CMsgBufFSE<SMSGBUFFER>::
vClearMessageBuffer(void)
{
    memset((BYTE*) m_asMsgBuffer, 0, SIZE_APP_BUFFER * m_nMsgSize);
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
}

template <typename SMSGBUFFER> HRESULT CMsgBufFSE<SMSGBUFFER>::ReadFromBuffer(
    SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != nullptr);
    ASSERT(!(m_nIndexRead > SIZE_APP_BUFFER));
#endif

    /* Lock the buffer */
    EnterCriticalSection(&m_CritSectionForGB);

    /* Check entry indexed by m_nIndexRead. If this particular entry */
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else
    {
        /* Copy the current entry and advance the read index by one. */
        memcpy(psMsg, &(m_asMsgBuffer[m_nIndexRead++]), m_nMsgSize);
        if (m_nIndexRead == SIZE_APP_BUFFER)
        {
            m_nIndexRead = 0;
        }
        /* Total number of to-be-read entries decremented by 1. */
        --m_nMsgCount;
    }

    /* Unlock the buffer */
    LeaveCriticalSection(&m_CritSectionForGB);

    return nResult;
}

template <typename SMSGBUFFER> HRESULT CMsgBufFSE<SMSGBUFFER>::WriteIntoBuffer(
    SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != nullptr);
    ASSERT(!(m_nIndexWrite > SIZE_APP_BUFFER));
#endif

    /*  Lock the buffer */
    EnterCriticalSection(&m_CritSectionForGB);

    /* Check for buffer overflow */
    if (m_nMsgCount == SIZE_APP_BUFFER)
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        /* Write the source entry and advance the write index by one. */
        memcpy (&(m_asMsgBuffer[m_nIndexWrite++]), psMsg, m_nMsgSize);
        if (m_nIndexWrite == SIZE_APP_BUFFER)
        {
            m_nIndexWrite = 0;
        }

        /* Total number of to-be-read entries incremented by 1. */
        ++m_nMsgCount;

        /* Notify addition of an entry. */
        SetEvent(m_hNotifyingEvent);
    }

    /* Unlock the buffer */
    LeaveCriticalSection(&m_CritSectionForGB);

    return nResult;
}

template <typename SMSGBUFFER> int CMsgBufFSE<SMSGBUFFER>::
GetMsgCount(void) const
{
    return m_nMsgCount;
}

template <typename SMSGBUFFER> HANDLE CMsgBufFSE<SMSGBUFFER>::
hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* /*psMsgBuffer*/, __int64 /*nSlotId*/)
{
    return ERR_NOT_SUPPORTED;
}

template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* /*psMsgBuffer*/, int /*nIndex*/)
{
    return ERR_NOT_SUPPORTED;
}

template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* /*psMsgBuffer*/,
        __int64 /*nSlotId*/, int& /*nIndex*/)
{
    return ERR_NOT_SUPPORTED;
}

template <typename SMSGBUFFER>
int CMsgBufFSE<SMSGBUFFER>::nSetBufferMsgSize(int /*nMsgDataSize*/)
{
    return ERR_NOT_SUPPORTED;
}
