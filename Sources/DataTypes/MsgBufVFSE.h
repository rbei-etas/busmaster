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
 * \file      MsgBufVFSE.h
 * \brief     Defines a variant of circular queue class in FSE.
 * \author    Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines a variant of circular queue class in FSE.
 */

#pragma once

/* C++ includes */
#include <map>

/* Project includes */
#include "BaseMsgBufAll.h"
#include "include/Utils_macro.h"
#include "include/Error.h"

typedef map<__int64, int> CSlotIndexMapType;
const int TOTAL_SIZE_APP_BUFFER       = 200;
const int MAX_MCNET_DATA_SIZE   = 0x7FFF;

template <typename SMSGBUFFER>
class CMsgBufVFSE: public CBaseMsgBufFSE<SMSGBUFFER>
{
protected:
    SMSGBUFFER* m_pasMsgBuffer;
    CRITICAL_SECTION m_CritSectionForGB;
    int m_nIndexRead, m_nIndexWrite, m_nMsgCount;
    int m_nBufferSize, m_nMsgDataSize;
    HANDLE m_hNotifyingEvent;

public:
    CMsgBufVFSE();
    ~CMsgBufVFSE();

    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer);
    HRESULT WriteIntoBuffer(SMSGBUFFER* psMsgBuffer);
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, __int64 nSlotId);
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, int nIndex);
    HRESULT WriteIntoBuffer(const SMSGBUFFER* psMsgBuffer, __int64 nSlotId, int& nIndex);
    int GetMsgCount(void) const;
    void vClearMessageBuffer(void);
    HANDLE hGetNotifyingEvent(void) const;
    int nSetBufferMsgSize(int nMsgDataSize);
    void vDoSortBuffer(int nField,bool bAscending);
    void nGetMapIndexAtID(int nIndex,__int64& nMapIndex);
private:
    BYTE* m_pbyTempData;
    CSlotIndexMapType m_stlIdIndexMap;
    void vCopyMsg(SMSGBUFFER* psDestMsg, const SMSGBUFFER* psSrcMsg);
    void vDoSortIndexMapArray();
};

/**
 * \brief Constructor
 *
 * Initializes buffer with minimum memory.
 */
template <typename SMSGBUFFER>
CMsgBufVFSE<SMSGBUFFER>::CMsgBufVFSE()
{
    m_nBufferSize  = TOTAL_SIZE_APP_BUFFER;
    m_nMsgDataSize = MAX_MCNET_DATA_SIZE;
    m_pasMsgBuffer = new SMSGBUFFER[TOTAL_SIZE_APP_BUFFER];

    for (int i = 0; i < TOTAL_SIZE_APP_BUFFER; i++)
    {
        m_pasMsgBuffer[i].vInitialize(m_nMsgDataSize);
    }

    vClearMessageBuffer(); // Clear the message buffer
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    // For raw data bytes. It should be equal to the size of SMSGBUFFER
    m_pbyTempData = new BYTE[MAX_MSG_LEN_J1939];
}

/**
 * \brief Destructor
 *
 * Clears buffer.
 */
template <typename SMSGBUFFER>
CMsgBufVFSE<SMSGBUFFER>::~CMsgBufVFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = NULL;

    if (m_pasMsgBuffer != NULL)
    {
        delete[] m_pasMsgBuffer;
        m_pasMsgBuffer = NULL;
    }

    DELETE_ARRAY(m_pbyTempData);
    DeleteCriticalSection(&m_CritSectionForGB);
}

/**
 * \brief Set Buffer Message Size
 *
 * User can set the buffer size.
 */
template <typename SMSGBUFFER>
int CMsgBufVFSE<SMSGBUFFER>::nSetBufferMsgSize(int nMsgDataSize)
{
    int nReturn = -1;
    vClearMessageBuffer();

    for (int i = 0; i < TOTAL_SIZE_APP_BUFFER; i++)
    {
        m_pasMsgBuffer[i].vInitialize(nMsgDataSize);
    }

    m_nMsgDataSize = nMsgDataSize;
    nReturn = 0;
    m_pbyTempData = new BYTE[nMsgDataSize];
    return nReturn;
}

/**
 * \brief Clear Message Buffer
 *
 * Clears the data in the buffer.
 */
template <typename SMSGBUFFER>
void CMsgBufVFSE<SMSGBUFFER>::vClearMessageBuffer(void)
{
    //memset((BYTE *) m_asMsgBuffer, 0, SIZE_APP_BUFFER * m_nMsgSize);
    for( int i = 0; i < m_nBufferSize; i++)
    {
        m_pasMsgBuffer[i].vClear();
    }

    m_stlIdIndexMap.clear();
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
}

/**
 * \brief Read Buffer
 *
 * Reads msg into SMSGBUFFER structure from the index nIndex.
 */
template <typename SMSGBUFFER>
HRESULT CMsgBufVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* psMsg, int nIndex)
{
    HRESULT nResult = S_OK;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > TOTAL_SIZE_APP_BUFFER));
#endif
    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else if (nIndex >= TOTAL_SIZE_APP_BUFFER)
    {
        nResult = ERR_INVALID_INDEX;
    }
    else
    {
        vCopyMsg(psMsg, &(m_pasMsgBuffer[nIndex]));
    }

    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

/**
 * \brief Read From Buffer
 *
 * Reads msg with ID nSlotId into SMSGBUFFER structure.
 */
template <typename SMSGBUFFER>
HRESULT CMsgBufVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* psMsg, __int64 nSlotId)
{
    HRESULT nResult = ERR_INVALID_SLOT;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > TOTAL_SIZE_APP_BUFFER));
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
        CSlotIndexMapType::iterator iter;
        iter = m_stlIdIndexMap.find(nSlotId);

        if (iter != m_stlIdIndexMap.end())
        {
            vCopyMsg(psMsg, &(m_pasMsgBuffer[iter->second]));
            nResult = CALL_SUCCESS;
        }
    }

    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

/**
 * \brief Write Into Buffer
 *
 * Writes msg with ID nSlotId from SMSGBUFFER structure into buffer return back the index
 */
template <typename SMSGBUFFER>
HRESULT CMsgBufVFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* psMsg, __int64 nSlotId, int& nIndex)
{
    int nResult = CALL_SUCCESS;
#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > TOTAL_SIZE_APP_BUFFER));
#endif
    EnterCriticalSection(&m_CritSectionForGB);

    //Check for buffer overflow
    if (m_nMsgCount == TOTAL_SIZE_APP_BUFFER)
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        CSlotIndexMapType::iterator iter;
        iter = m_stlIdIndexMap.find(nSlotId);

        if (iter != m_stlIdIndexMap.end())
        {
            nIndex = iter->second;
        }
        else
        {
            nIndex = m_nMsgCount;
            m_stlIdIndexMap.insert(pair<__int64, int>(nSlotId, nIndex));
            ++m_nMsgCount;
        }

        vCopyMsg(&(m_pasMsgBuffer[nIndex]), psMsg);
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

/**
 * \brief Get Message Count
 *
 * Gets the message count.
 */
template <typename SMSGBUFFER>
int CMsgBufVFSE<SMSGBUFFER>::GetMsgCount(void)const
{
    return m_nMsgCount;
}

/**
 * \brief Get Notifying Event
 *
 * Gets the notifying event.
 */
template <typename SMSGBUFFER>
HANDLE CMsgBufVFSE<SMSGBUFFER>::hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

/**
 * \brief Copy Message
 *
 * Helper function copies full data from one SMSGBUFFER structure into another structure.
 */
template <typename SMSGBUFFER>
void CMsgBufVFSE<SMSGBUFFER>::vCopyMsg(SMSGBUFFER* psDestMsg, const SMSGBUFFER* psSrcMsg)
{
    const SMSGBUFFER* psTempSrc = psSrcMsg;
    psTempSrc->vGetDataStream(m_pbyTempData);
    psDestMsg->vSetDataStream(m_pbyTempData);
}

/**
 * \brief     Do Sort Buffer
 * \param[in] nField The field to be used as the sorting key.
 *
 * Reorders the list according to the sorting key specified.
 */
template <typename SMSGBUFFER> void CMsgBufVFSE<SMSGBUFFER>::
vDoSortBuffer(int nField,bool bAscending)
{
    SMSGBUFFER::vSetSortField(nField);
    SMSGBUFFER::vSetSortAscending(bAscending);
    qsort((void*) m_pasMsgBuffer, (size_t) GetMsgCount(),
          sizeof(SMSGBUFFER), SMSGBUFFER::DoCompareIndiv);
    //After sorting Start index has to be reset
    m_nIndexRead = 0;

    if(m_stlIdIndexMap.size() ==0)
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
template <typename SMSGBUFFER> void CMsgBufVFSE<SMSGBUFFER>::
vDoSortIndexMapArray()
{
    UINT unSize = (UINT)m_stlIdIndexMap.size();
    m_stlIdIndexMap.clear();

    for(int nCnt = 0; (UINT)nCnt< unSize; nCnt++)
    {
        __int64 nSlotID = SMSGBUFFER::GetSlotID(m_pasMsgBuffer[nCnt]);
        m_stlIdIndexMap.insert(pair<__int64, int>(nSlotID, nCnt));
    }
}

/**
 * \brief     Get Map Index At ID
 * \param[in] nIndex The Index at which the SlotID needs to be pickef from.
 *
 * Returns the Slot ID of the index specified in m_omIdIndexMap.
 */
template <typename SMSGBUFFER> void CMsgBufVFSE<SMSGBUFFER>::
nGetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    /*CSlotIndexMapType::iterator iter;
    iter = find(m_stlIdIndexMap.begin(), m_stlIdIndexMap.end(), */
    for (CSlotIndexMapType::const_iterator it = m_stlIdIndexMap.begin();
            it != m_stlIdIndexMap.end(); ++it)
    {
        if (it->second == nIndex)
        {
            nMapIndex = it->first;
            break;
        }
    }
}

/**
 * \brief Read From Buffer
 *
 * Read the oldest message from circular queue.
 */
template <typename SMSGBUFFER>
HRESULT CMsgBufVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* /*psMsgBuffer*/)
{
    return ERR_NOT_SUPPORTED;
}

/**
 * \brief Write Into Buffer
 *
 * Write the message at the end of circular queue.
 */
template <typename SMSGBUFFER>
HRESULT CMsgBufVFSE<SMSGBUFFER>::WriteIntoBuffer(SMSGBUFFER* /*psMsgBuffer*/)
{
    return ERR_NOT_SUPPORTED;
}
