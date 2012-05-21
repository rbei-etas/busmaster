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
 * \file      MsgBufCANVFSE.h
 * \brief     Defines and implements the template class for circular queue
 * \author    Ratnadip Choudhury, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines and implements the template class for circular queue
 */

#pragma once

/* Project includes */
#include "include/Error.h"
#include "BaseMsgBufAll.h"

const int SIZE_APP_CAN_BUFFER       = 5000;

/**
 * \brief Message Buffer CAN Template Class
 *
 * This is the concrete template class of a circular queue where each entry is
 * of fixed size. Implemented as a template class so as to cater to any data type.
 * Here SMSGBUFFER is the data type in operation.
 */
template <typename SMSGBUFFER>
class CMsgBufCANVFSE
{
protected:

    SMSGBUFFER m_asMsgBuffer[SIZE_APP_CAN_BUFFER]; // The data buffer
    CRITICAL_SECTION m_CritSectionForGB;       // To make it thread safe

    int m_nIndexRead;                          // Current read index
    int m_nIndexWrite;                         // Current write index
    int m_nMsgCount;                           // Number of message entries
    int m_nMsgSize;                            /* At the beginning we need to
    store size of a message entry. This information will be used frequently */
    HANDLE m_hNotifyingEvent;                  // Event to be signalled when
    // there is at least one message

public:
    // Short explanation on each member function is present in the base class.
    // That's why information are not repeated unnecessarily.

    CMsgBufCANVFSE();
    ~CMsgBufCANVFSE();

    int ReadFromBuffer(SMSGBUFFER* psMsgBuffer,__int64 nSlotId);
    int ReadFromBuffer(SMSGBUFFER* psMsgBuffer,int nIndex);
    int WriteIntoBuffer(const SMSGBUFFER* psMsg, __int64 nSlotId, int& nIndex);
    int WriteIntoBuffer(const SMSGBUFFER* psMsg);
    int GetBufferLength(void) const;
    void vClearMessageBuffer(void);
    HANDLE hGetNotifyingEvent(void) const;
    void vDoSortBuffer(int nField,bool bAscending);
    void vDoSortIndexMapArray();
    void nGetMapIndexAtID(int nIndex,__int64& nMapIndex);

private:
    CMap<__int64, __int64, int, int> m_omIdIndexMap;
};

/**
 * \brief Constructor
 *
 * Standard constructor
 */
template <typename SMSGBUFFER>
CMsgBufCANVFSE<SMSGBUFFER>::CMsgBufCANVFSE()
{
    m_nMsgSize = sizeof(SMSGBUFFER);
    vClearMessageBuffer();
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

/**
 * \brief Destructor
 *
 * Destructor
 */
template <typename SMSGBUFFER>
CMsgBufCANVFSE<SMSGBUFFER>::~CMsgBufCANVFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = NULL;
    DeleteCriticalSection(&m_CritSectionForGB);
}

/**
 * \brief Clear Message Buffer
 *
 * Clears the message buffer and resets the indices.
 */
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vClearMessageBuffer(void)
{
    memset((BYTE*) m_asMsgBuffer, 0, SIZE_APP_CAN_BUFFER * m_nMsgSize);
    m_omIdIndexMap.RemoveAll();
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
}

/**
 * \brief      Read From Buffer
 * \param[out] psMsg The target message entry.
 * \return     EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
 *
 * Reads a message entry and advances the read index. On
 * successful reading operation the present entry is
 * invalidated to make room for a new entry.
 */
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* psMsg, int nIndex)
{
    HRESULT nResult = S_OK;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > SIZE_APP_CAN_BUFFER));
#endif
    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else if (nIndex >= SIZE_APP_CAN_BUFFER)
    {
        nResult = ERR_INVALID_INDEX;
    }
    else
    {
        INT TempIndex = m_nIndexRead + nIndex;

        if (m_nIndexRead + nIndex >= SIZE_APP_CAN_BUFFER)
        {
            TempIndex -= SIZE_APP_CAN_BUFFER;
        }

        *psMsg = m_asMsgBuffer[TempIndex];
    }

    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

/**
 * \brief Read From Buffer
 *
 * Reads from buffer.
 */
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* psMsg, __int64 nSlotId)
{
    HRESULT nResult = CALL_SUCCESS;
    int nIndex;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > SIZE_APP_CAN_BUFFER));
#endif
    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else
    {
        if (m_omIdIndexMap.Lookup(nSlotId, nIndex))
        {
            *psMsg = m_asMsgBuffer[nIndex];
        }
        else
        {
            nResult = ERR_INVALID_SLOT;
        }
    }

    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

/**
 * \brief     Write Into Buffer
 * \param[in] psMsg The source message entry.
 * \return    ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
 *
 * Writes a message entry and advances the write index.
 */
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* psMsg,
        __int64 nSlotId, int& nIndex)

{
    int nResult = CALL_SUCCESS;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > SIZE_APP_CAN_BUFFER));
#endif
    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_CAN_BUFFER) // Check for buffer overflow
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        if (m_omIdIndexMap.Lookup(nSlotId, nIndex))
        {
            m_asMsgBuffer[nIndex] = *psMsg;
        }
        else
        {
            nIndex = m_nMsgCount;
            m_asMsgBuffer[m_nMsgCount] = *psMsg;
            m_omIdIndexMap[nSlotId] = m_nMsgCount;
            ++m_nMsgCount;
        }

        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer
    return nResult;
}

/**
 * \brief     Write Into Buffer
 * \param[in] psMsg The source message entry.
 * \return    ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
 *
 * Writes a message entry and advances the write index.
 */
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > SIZE_APP_CAN_BUFFER));
#endif
    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_CAN_BUFFER) // Check for buffer overflow
    {
        m_asMsgBuffer[m_nIndexRead] = *psMsg;
        m_nIndexRead++;

        if (m_nIndexRead == SIZE_APP_CAN_BUFFER)
        {
            m_nIndexRead = 0;
        }

        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        m_asMsgBuffer[m_nMsgCount] = *psMsg;
        ++m_nMsgCount;
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer
    return nResult;
}

/**
 * \brief  Get Buffer Length
 * \return Number of message entries (int)
 *
 * Returns the number of unread entries in the queue.
 */
template <typename SMSGBUFFER> int CMsgBufCANVFSE<SMSGBUFFER>::
GetBufferLength(void) const
{
    return m_nMsgCount;
}

/**
 * \brief  Get Notifying Event
 * \return The notifying event handle (HANDLE)
 *
 * Returns handle of the event that gets signalled when
 * a message entry is added.
 */
template <typename SMSGBUFFER> HANDLE CMsgBufCANVFSE<SMSGBUFFER>::
hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

/**
 * \brief     Do Sort Buffer
 * \param[in] nField The field to be used as the sorting key.
 *
 * Reorders the list according to the sorting key specified.
 */
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vDoSortBuffer(int nField,bool bAscending)
{
    SMSGBUFFER::vSetSortField(nField);
    SMSGBUFFER::vSetSortAscending(bAscending);
    qsort((void*) m_asMsgBuffer, (size_t) GetBufferLength(),
          sizeof(SMSGBUFFER), SMSGBUFFER::DoCompareIndiv);
    //After sorting Start index has to be reset
    m_nIndexRead = 0;

    //in case of Append Mode the count will be 0
    if(m_omIdIndexMap.GetCount() ==0)
    {
        return;
    }

    vDoSortIndexMapArray();
}

/**
 * \brief Do Sort Index Map Array
 *
 * Reorders the Index Map Array according to the order specified.
 */
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vDoSortIndexMapArray()
{
    for(int nCnt = 0; nCnt< m_omIdIndexMap.GetCount(); nCnt++)
    {
        __int64 nSlotID = SMSGBUFFER::GetSlotID(m_asMsgBuffer[nCnt]);
        m_omIdIndexMap.SetAt(nSlotID, nCnt);
    }
}

/**
 * \brief      Get Map Index At ID
 * \param[in]  nIndex The Index at which the SlotID needs to be picked from.
 * \param[out] nMapIndex Map Index
 *
 * Returns the Slot ID of the index specified in m_omIdIndexMap.
 */
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
nGetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    POSITION pos = m_omIdIndexMap.GetStartPosition();
    int nLocalIndex=0;
    __int64 nSlotID;

    while( pos != NULL )
    {
        m_omIdIndexMap.GetNextAssoc(pos, nSlotID, nLocalIndex);

        if(nIndex == nLocalIndex)
        {
            nMapIndex = nSlotID;
            return;
        }
    }
}
