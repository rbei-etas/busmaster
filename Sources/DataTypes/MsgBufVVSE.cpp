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
 * \file      MsgBufVVSE.cpp
 * \brief     Implements the variant of circular queue class in VSE.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implements the variant of circular queue class in VSE.
 */

// Insert your headers here
#include "Datatypes_stdafx.h"
#include "MsgBufVVSE.h"
//Error definition
#include "include/Error.h"



//type
#define DATA_LEN   WORD
#define TYPE       BYTE
#define MAX_BUFFER_SIZE 200000
#define MIN_BUFFER_SIZE 50000
// Const variables
const size_t DATA_LEN_SIZE = sizeof(DATA_LEN);
const size_t TYPE_SIZE     = sizeof(TYPE);
//length
#define HEADER_LEN 3

//offset
#define TYPE_OFFSET     0
#define MSGLEN_OFFSET   1


/**********************************************************************************
Function Name   :   CMsgBufVVSE()
Input           :
Output          :
Functionality   :   Initializes buffer with minimum memory.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
CMsgBufVVSE::CMsgBufVVSE(void)
{
    m_nBufferSize = MIN_BUFFER_SIZE;
    m_pbyMsgBuffer = new BYTE[MIN_BUFFER_SIZE];// allocate memory first
    vClearMessageBuffer(); // Clear the message buffer
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

//Destructor deallocates memory
CMsgBufVVSE::~CMsgBufVVSE(void)
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = NULL;

    if (m_pbyMsgBuffer != NULL)
    {
        delete[] m_pbyMsgBuffer;
        m_pbyMsgBuffer = NULL;
    }

    DeleteCriticalSection(&m_CritSectionForGB);
}
/**********************************************************************************
Function Name   :   vClearMessageBuffer()
Input           :
Output          :
Functionality   :   Clears msg buffer and initializes all variables.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
void CMsgBufVVSE::vClearMessageBuffer(void)
{
    memset(m_pbyMsgBuffer, 0, m_nBufferSize);
    m_nIndexWrite   = 0;
    m_nMsgCount     = 0;
    m_nStartIndex   = 0;
    m_nReadIndexTmp = 0;
    m_nTmpMsgCount  = 0;
}

/**********************************************************************************
Function Name   :   nSetStartPos
Input           :   Entry Number - nIndex
Output          :   CALL_SUCCESS for success.
                    ERR_INVALID_INDEX for invalid index.
Functionality   :   Interface function. Sets the start pos to the 'nIndex'th entry.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   10/11/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVVSE::SetStartPos(int nEntry)
{
    HRESULT Result = CALL_SUCCESS;

    if (nEntry < 0 || nEntry > m_nMsgCount)
    {
        Result = ERR_INVALID_INDEX;
    }
    else
    {
        m_nReadIndexTmp = m_nStartIndex;

        for (int i = 0; i < nEntry; i++)
        {
            nAdvanceReadIndex(m_nReadIndexTmp);
            m_nTmpMsgCount++;
        }
    }

    return Result;
}
/**********************************************************************************
Function Name   :   ReadEntry
Input           :   INDEX, bSetNextIndexStartPos.
Output          :   CALL_SUCCESS for success.
                    ERR_READ_MEMORY_SHORT when caller specifies size of the msg
                    less than required size.
                    ERR_INVALID_INDEX for invalid index.
Functionality   :   Interface function. Reads the entry from the given index and
                    user will have an option set the next entry start pos.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVVSE::ReadEntry(int& nType, BYTE* pbyMsg, int& nSize, int nEntry, BOOL bSetNextIndexStartPos)
{
    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);
    HRESULT Result = CALL_SUCCESS;

    if ((nEntry < 0) || ((nEntry + m_nTmpMsgCount) > m_nMsgCount))
    {
        Result = ERR_INVALID_INDEX;
    }
    else
    {
        int nTmpIndex = m_nReadIndexTmp;

        for (int i = 0; i < nEntry; i++)
        {
            nAdvanceReadIndex(nTmpIndex);
        }

        Result = ReadBuffer(nType, pbyMsg, nSize, nTmpIndex);

        if ((Result == CALL_SUCCESS) && (bSetNextIndexStartPos == TRUE))
        {
            m_nReadIndexTmp = nTmpIndex;
            m_nTmpMsgCount += nEntry;
        }
    }

    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);
    return Result;
}

/**********************************************************************************
Function Name   :   WriteIntoBuffer()
Input           :   TYPE, SIZE, pointer to msg.
Output          :   CALL_SUCCESS for success.
                    ERR_WRITE_MSG_TOO_LARGE when caller specifies size of the msg
                    more than buffer size.
Functionality   :   Interface function. Writes msg into circular function.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVVSE::WriteIntoBuffer(INT nType, BYTE* pbyMsg, INT nSize)
{
    int nResult = CALL_SUCCESS;
#ifdef _DEBUG
    ASSERT(pbyMsg != NULL);
    ASSERT(!(m_nIndexWrite > m_nBufferSize));
#endif
    EnterCriticalSection(&m_CritSectionForGB);

    // Check the size of the message
    if (nSize > m_nBufferSize)
    {
        nResult = ERR_WRITE_MSG_TOO_LARGE;
    }
    else
    {
        nResult = nHandleStartIndex(nSize + HEADER_LEN);//helper function to handle buffer overrun
        nWriteBuffer(nType, pbyMsg, nSize);//helper function to write buffer
        ++m_nMsgCount;
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

int CMsgBufVVSE::GetMsgCount(void) const
{
    return m_nMsgCount;
}

HANDLE CMsgBufVVSE::hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

/**********************************************************************************
Function Name   :   nSetBufferSize()
Output          :   CALL_SUCCESS for success.
                    WARN_BUFFER_SIZE_MIN_ASSUMED when caller specifies less than
                    minimum buffer size.
                    WARN_BUFFER_SIZE_MAX_ASSUMED when caller specifies more than
                    maximum buffer size.
Functionality   :   Interface function. Caller can set the buffer size.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/

int CMsgBufVVSE::nSetBufferSize(int& nSize)
{
    int nResult = CALL_SUCCESS;

    if (nSize < MIN_BUFFER_SIZE)
    {
        nSize = MIN_BUFFER_SIZE;
        nResult =  WARN_BUFFER_SIZE_MIN_ASSUMED;
    }
    else if (nSize > MAX_BUFFER_SIZE)
    {
        nSize = MIN_BUFFER_SIZE;
        nResult = WARN_BUFFER_SIZE_MAX_ASSUMED;
    }

    EnterCriticalSection(&m_CritSectionForGB);

    if (m_pbyMsgBuffer != NULL)
    {
        delete[] m_pbyMsgBuffer;
    }

    m_nBufferSize = nSize;
    m_pbyMsgBuffer = new BYTE[nSize];

    if (m_pbyMsgBuffer == NULL)
    {
        nResult = ERR_CREATE_MEMORY_FAIL;
        m_nBufferSize = 0;
    }
    else
    {
        vClearMessageBuffer();
    }

    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}
/**********************************************************************************
Function Name   :   AdvanceToNextMsg()
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
                    if allocated memory by the caller is not enough.
Functionality   :   Interface function. Current msg is skipped.
                    Caller can use this function when wishes to skip the
                    current msg due to memory constraint.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVVSE::AdvanceToNextMsg()
{
    HRESULT Result = CALL_SUCCESS;

    if (m_nMsgCount == 0)
    {
        Result = EMPTY_APP_BUFFER;
    }
    else
    {
        nAdvanceReadIndex(m_nStartIndex);//Helper function to advance the read index
        //to the next msg.
        m_nMsgCount--;
    }

    return Result;
}
/**********************************************************************************
Function Name   :   nAdvanceReadIndex(int& nIndex)
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
                    if allocated memory by the caller is not enough.
Functionality   :   Helper function. Advances the nIndex to the next entry
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVVSE::nAdvanceReadIndex(int& nReadIndex)
{
    int nResult = CALL_SUCCESS;
    static BYTE abyHeader[HEADER_LEN] = {0};
    nGetCurrMsgHeader(nReadIndex, abyHeader); // Get current msg header TYPE, MSG LENGTH
    int nMsgLen = 0;
    memcpy(&nMsgLen, abyHeader + MSGLEN_OFFSET, DATA_LEN_SIZE);

    if ((nReadIndex + HEADER_LEN + nMsgLen) < m_nBufferSize)
    {
        nReadIndex += (HEADER_LEN + nMsgLen);
    }
    else
    {
        nReadIndex = (nReadIndex + HEADER_LEN + nMsgLen) - m_nBufferSize;
    }

    return nResult;
}
/**********************************************************************************
Function Name   :   nWriteBuffer()
Input(s)        :   TYPE, SIZE, pointer to the message.
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   Helper function. Current msg is written into the
                    circular buffer.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVVSE::nWriteBuffer(INT nType, BYTE* pbyMsg, INT nSize)
{
    int nResult = CALL_SUCCESS;
    static BYTE abyHeader[HEADER_LEN] = {0};
    nResult = nConstructHeader(nType, nSize, abyHeader);//Helper function to construct header from TYPE, MSG LENGTH
    ASSERT(nResult == CALL_SUCCESS);

    if ((m_nIndexWrite + HEADER_LEN + nSize) <= m_nBufferSize)
    {
        //Write header
        memcpy(m_pbyMsgBuffer + m_nIndexWrite, abyHeader, HEADER_LEN);
        //Write msg
        memcpy(m_pbyMsgBuffer + m_nIndexWrite + HEADER_LEN, pbyMsg, nSize);
        m_nIndexWrite += (HEADER_LEN + nSize);
    }
    else if (m_nIndexWrite + HEADER_LEN <= m_nBufferSize)
    {
        memcpy(m_pbyMsgBuffer + m_nIndexWrite, abyHeader, HEADER_LEN);
        // Write msg
        memcpy(m_pbyMsgBuffer + m_nIndexWrite + HEADER_LEN, pbyMsg, (m_nBufferSize - m_nIndexWrite - HEADER_LEN));
        memcpy(m_pbyMsgBuffer, pbyMsg +(m_nBufferSize - m_nIndexWrite - HEADER_LEN), nSize - (m_nBufferSize - m_nIndexWrite - HEADER_LEN));
        m_nIndexWrite = (nSize - (m_nBufferSize - m_nIndexWrite - HEADER_LEN));
    }
    else
    {
        //write the header
        memcpy(m_pbyMsgBuffer + m_nIndexWrite, abyHeader, (m_nBufferSize - m_nIndexWrite));
        memcpy(m_pbyMsgBuffer, abyHeader + (m_nBufferSize - m_nIndexWrite), HEADER_LEN - (m_nBufferSize - m_nIndexWrite));
        //write the msg
        memcpy(m_pbyMsgBuffer + (HEADER_LEN - (m_nBufferSize - m_nIndexWrite)), pbyMsg, nSize);
        m_nIndexWrite = (HEADER_LEN - (m_nBufferSize - m_nIndexWrite)) + nSize;
    }

    return nResult;
}

/**********************************************************************************
Function Name   :   ReadBuffer()
Input(s)        :   TYPE, SIZE, pointer to the byte where MSG will be stored.
Output          :   CALL_SUCCESS for success. ERR_READ_MEMORY_SHORT
                    if allocated memory by the caller is not enough.
Functionality   :   Helper function. Current msg pointed by nIndex
                    is read.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVVSE::ReadBuffer(INT& nType, BYTE* pbyMsg, INT& nSize, INT& nIndex)
{
    HRESULT Return = CALL_SUCCESS;
    BYTE abyHeader[HEADER_LEN];
    Return = nGetCurrMsgHeader(nIndex, abyHeader);
    // It is expected to return CALL_SUCCESS
    ASSERT(Return == CALL_SUCCESS);
    int nMsgLen = 0;
    memcpy(&nType, (abyHeader + TYPE_OFFSET), TYPE_SIZE);
    memcpy(&nMsgLen, (abyHeader + MSGLEN_OFFSET), DATA_LEN_SIZE);

    if (nMsgLen > nSize)
    {
        Return = ERR_READ_MEMORY_SHORT;
        nSize   = nMsgLen - nSize;
    }
    else
    {
        if ((nIndex + HEADER_LEN + nMsgLen) <= m_nBufferSize)
        {
            memcpy(pbyMsg, (m_pbyMsgBuffer + nIndex + HEADER_LEN), nMsgLen);
            nIndex += (HEADER_LEN + nMsgLen);
        }
        else if ((nIndex + HEADER_LEN) <= m_nBufferSize)
        {
            memcpy(pbyMsg, (m_pbyMsgBuffer + nIndex + HEADER_LEN), (m_nBufferSize - nIndex - HEADER_LEN));
            memcpy((pbyMsg + (m_nBufferSize - nIndex - HEADER_LEN)), m_pbyMsgBuffer, (nMsgLen - (m_nBufferSize - nIndex - HEADER_LEN)));
            nIndex = nMsgLen - (m_nBufferSize - nIndex - HEADER_LEN);
        }
        else
        {
            memcpy(pbyMsg, (m_pbyMsgBuffer + (nIndex + HEADER_LEN - m_nBufferSize)), nMsgLen);
            nIndex = (nIndex + HEADER_LEN - m_nBufferSize) +  nMsgLen;
        }

        nSize = nMsgLen;
    }

    return Return;
}
/**********************************************************************************
Function Name   :   nConstructHeader()
Input(s)        :   TYPE, SIZE, Array of byte where header will be stored.
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   Header is constructed with TYPE & SIZE.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVVSE::nConstructHeader(INT nType, INT nSize, BYTE* pbyHeader)
{
    memcpy(pbyHeader + TYPE_OFFSET, &nType, TYPE_SIZE);
    memcpy(pbyHeader + MSGLEN_OFFSET, &nSize, DATA_LEN_SIZE);
    return CALL_SUCCESS;
}
/**********************************************************************************
Function Name   :   nGetCurrMsgHeader(int nIndex, BYTE* pbyHeader)
Input(s)        :   Array of byte where header will be stored.
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   header of the msg at the 'nIndex' is retrieved.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVVSE::nGetCurrMsgHeader(int nIndex, BYTE* pbyHeader)
{
    if ((nIndex + HEADER_LEN) <= m_nBufferSize)
    {
        memcpy(pbyHeader, (m_pbyMsgBuffer + nIndex), HEADER_LEN);
    }
    else
    {
        memcpy(pbyHeader, (m_pbyMsgBuffer + nIndex), m_nBufferSize - nIndex);
        memcpy(pbyHeader + (m_nBufferSize - nIndex), m_pbyMsgBuffer, (nIndex + HEADER_LEN) - m_nBufferSize);
    }

    return CALL_SUCCESS;
}

/**********************************************************************************
Function Name   :   nHandleStartIndex()
Input(s)        :   -
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   If buffer overrun criteria is met, start index
                    is advanced until it solves buffer overrun.
Member of       :   CMsgBufVVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVVSE::nHandleStartIndex(int nSize)
{
    int nResult = CALL_SUCCESS;

    if (m_nStartIndex < m_nIndexWrite)
    {
        if ((m_nStartIndex + m_nBufferSize) <=  (m_nIndexWrite + nSize))
        {
            TRACE("<start>%d<read>%d<write>%d<count>%d<tmpCount>%d\n",m_nStartIndex, m_nReadIndexTmp, m_nIndexWrite, m_nMsgCount, m_nTmpMsgCount);
            nAdvanceReadIndex(m_nStartIndex);
            m_nMsgCount--;
            m_nReadIndexTmp = m_nStartIndex;
            m_nTmpMsgCount = 0;
            TRACE("<start>%d<read>%d<write>%d<count>%d<tmpCount>%d\n",m_nStartIndex, m_nReadIndexTmp, m_nIndexWrite, m_nMsgCount, m_nTmpMsgCount);
            nHandleStartIndex(nSize);
        }
    }
    else if (m_nStartIndex > m_nIndexWrite)
    {
        if ((m_nStartIndex) <=  (m_nIndexWrite + nSize))
        {
            TRACE("<start>%d<read>%d<write>%d<count>%d<tmpCount>%d\n",m_nStartIndex, m_nReadIndexTmp, m_nIndexWrite, m_nMsgCount, m_nTmpMsgCount);
            nAdvanceReadIndex(m_nStartIndex);
            m_nMsgCount--;
            m_nReadIndexTmp = m_nStartIndex;
            m_nTmpMsgCount = 0;
            TRACE("<start>%d<read>%d<write>%d<count>%d<tmpCount>%d\n",m_nStartIndex, m_nReadIndexTmp, m_nIndexWrite, m_nMsgCount, m_nTmpMsgCount);
            nHandleStartIndex(nSize);
        }
    }
    else
    {
        if (m_nMsgCount != 0)
        {
            ASSERT(FALSE);
        }
    }

    TRACE("handled<start>%d<read>%d<write>%d<count>%d<tmpCount>%d\n",m_nStartIndex, m_nReadIndexTmp, m_nIndexWrite, m_nMsgCount, m_nTmpMsgCount);
    return nResult;
}