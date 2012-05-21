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
 * \file      MsgBufVSE.cpp
 * \brief     Source file of concrete class for circular queue in VSE.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file of concrete class for circular queue in VSE.
 */


// Insert your headers here
//#include "CommonInclude.h"
#include "DataTypes_stdafx.h"
#include "MsgBufVSE.h"
//Error definition
#include "include/Error.h"


//type
#define DATA_LEN   WORD
#define TYPE       BYTE
#define MAX_BUFFER_SIZE 100000
#define MIN_BUFFER_SIZE 5000

// Const variables
const size_t DATA_LEN_SIZE = sizeof(DATA_LEN);
const size_t TYPE_SIZE     = sizeof(TYPE);
//length
const int HEADER_LEN = TYPE_SIZE + DATA_LEN_SIZE;

//offset
#define TYPE_OFFSET     0
#define MSGLEN_OFFSET   1


/**********************************************************************************
Function Name   :   CMsgBufVSE()
Input           :
Output          :
Functionality   :   Initializes buffer with minimum memory.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
CMsgBufVSE::CMsgBufVSE(void)
{
    m_nBufferSize = MIN_BUFFER_SIZE;
    m_pbyMsgBuffer = new BYTE[MIN_BUFFER_SIZE];// allocate memory first
    vClearMessageBuffer(); // Clear the message buffer
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

//Destructor deallocates memory
CMsgBufVSE::~CMsgBufVSE(void)
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
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
void CMsgBufVSE::vClearMessageBuffer(void)
{
    memset(m_pbyMsgBuffer, 0, m_nBufferSize);
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
    m_nMsgSkipped = 0;
}

/**********************************************************************************
Function Name   :   ReadFromBuffer()
Input           :   TYPE, SIZE, pointer to msg.
Output          :   CALL_SUCCESS for success.
                    ERR_READ_MEMORY_SHORT when caller specifies size of the msg
                    less than required size.
Functionality   :   Interface function. reads current msg from circular buffer.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVSE::ReadFromBuffer(INT& nType, BYTE* pbyMsg, INT& nSize)
{
    HRESULT Result = CALL_SUCCESS;
#ifdef _DEBUG
    ASSERT(pbyMsg != NULL);
    ASSERT(!(m_nIndexRead > m_nBufferSize));
#endif
    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    if (m_nMsgCount == 0)
    {
        Result = EMPTY_APP_BUFFER;
    }
    else
    {
        //call the Helper function to read buffer
        if ((Result = ReadBuffer(nType, pbyMsg, nSize)) == CALL_SUCCESS)
        {
            m_nMsgCount--;
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
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVSE::WriteIntoBuffer(INT nType, BYTE* pbyMsg, INT nSize)
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
        nResult = nHandleBufferOverrun(nSize + HEADER_LEN);//helper function to handle buffer overrun
        nWriteBuffer(nType, pbyMsg, nSize);//helper function to write buffer
        ++m_nMsgCount;
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB);
    return nResult;
}

int CMsgBufVSE::GetMsgCount(void) const
{
    return m_nMsgCount;
}

HANDLE CMsgBufVSE::hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

int CMsgBufVSE::GetBufferLength(void) const
{
    return m_nBufferSize;
}

/**********************************************************************************
Function Name   :   nSetBufferSize()
Output          :   CALL_SUCCESS for success.
                    WARN_BUFFER_SIZE_MIN_ASSUMED when caller specifies less than
                    minimum buffer size.
                    WARN_BUFFER_SIZE_MAX_ASSUMED when caller specifies more than
                    maximum buffer size.
Functionality   :   Interface function. Caller can set the buffer size.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/

int CMsgBufVSE::nSetBufferSize(int& nSize)
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
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVSE::AdvanceToNextMsg()
{
    HRESULT Result = CALL_SUCCESS;

    if (m_nMsgCount == 0)
    {
        Result = EMPTY_APP_BUFFER;
    }
    else
    {
        nAdvanceReadIndex();//Helper function to advance the read index
        //to the next msg.
    }

    return Result;
}
/**********************************************************************************
Function Name   :   nAdvanceReadIndex()
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
                    if allocated memory by the caller is not enough.
Functionality   :   Helper function. Current msg is skipped.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVSE::nAdvanceReadIndex(void)
{
    int nResult = CALL_SUCCESS;
    static BYTE abyHeader[HEADER_LEN] = {0};
    nGetCurrMsgHeader(abyHeader); // Get current msg header TYPE, MSG LENGTH
    int nMsgLen = 0;
    memcpy(&nMsgLen, abyHeader + MSGLEN_OFFSET, DATA_LEN_SIZE);

    if ((m_nIndexRead + HEADER_LEN + nMsgLen) < m_nBufferSize)
    {
        m_nIndexRead += (HEADER_LEN + nMsgLen);
    }
    else
    {
        m_nIndexRead = (m_nIndexRead + HEADER_LEN + nMsgLen) - m_nBufferSize;
    }

    m_nMsgSkipped++;
    m_nMsgCount--;
    return nResult;
}

/**********************************************************************************
Function Name   :   nWriteBuffer()
Input(s)        :   TYPE, SIZE, pointer to the message.
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   Helper function. Current msg is written into the
                    circular buffer.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVSE::nWriteBuffer(INT nType, BYTE* pbyMsg, INT nSize)
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
Input(s)        :   TYPE, SIZE, pointer to the byte where header will be stored.
Output          :   CALL_SUCCESS for success. ERR_READ_MEMORY_SHORT
                    if allocated memory by the caller is not enough.
Functionality   :   Helper function. Current msg pointed by read index
                    is read.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
HRESULT CMsgBufVSE::ReadBuffer(INT& nType, BYTE* pbyMsg, INT& nSize)
{
    HRESULT Return = CALL_SUCCESS;
    BYTE abyHeader[HEADER_LEN];
    Return = nGetCurrMsgHeader(abyHeader);
    // It is expected to return CALL_SUCCESS
    ASSERT(Return == CALL_SUCCESS);
    DATA_LEN MsgLen = 0;
    TYPE Type;
    memcpy(&Type, (abyHeader + TYPE_OFFSET), TYPE_SIZE);
    nType = (INT) Type;
    memcpy(&MsgLen, (abyHeader + MSGLEN_OFFSET), DATA_LEN_SIZE);

    if ((int) MsgLen > nSize)
    {
        Return = ERR_READ_MEMORY_SHORT;
        nSize   = MsgLen - nSize;
    }
    else
    {
        if ((m_nIndexRead + HEADER_LEN + MsgLen) <= m_nBufferSize)
        {
            memcpy(pbyMsg, (m_pbyMsgBuffer + m_nIndexRead + HEADER_LEN), MsgLen);
            m_nIndexRead += (HEADER_LEN + MsgLen);
        }
        else if ((m_nIndexRead + HEADER_LEN) <= m_nBufferSize)
        {
            memcpy(pbyMsg, (m_pbyMsgBuffer + m_nIndexRead + HEADER_LEN), (m_nBufferSize - m_nIndexRead - HEADER_LEN));
            memcpy((pbyMsg + (m_nBufferSize - m_nIndexRead - HEADER_LEN)), m_pbyMsgBuffer, (MsgLen - (m_nBufferSize - m_nIndexRead - HEADER_LEN)));
            m_nIndexRead = MsgLen - (m_nBufferSize - m_nIndexRead - HEADER_LEN);
        }
        else
        {
            memcpy(pbyMsg, (m_pbyMsgBuffer + (m_nIndexRead + HEADER_LEN - m_nBufferSize)), MsgLen);
            m_nIndexRead = (m_nIndexRead + HEADER_LEN - m_nBufferSize) +  MsgLen;
        }

        nSize = MsgLen;
    }

    return Return;
}
/**********************************************************************************
Function Name   :   nConstructHeader()
Input(s)        :   TYPE, SIZE, Array of byte where header will be stored.
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   Header is constructed with TYPE & SIZE.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVSE::nConstructHeader(INT nType, INT nSize, BYTE* pbyHeader)
{
    TYPE Type = (TYPE)nType;
    DATA_LEN MsgLen = (DATA_LEN)nSize;
    memcpy(pbyHeader + TYPE_OFFSET, &Type, TYPE_SIZE);
    memcpy(pbyHeader + MSGLEN_OFFSET, &MsgLen, DATA_LEN_SIZE);
    return CALL_SUCCESS;
}

/**********************************************************************************
Function Name   :   nGetCurrMsgHeader()
Input(s)        :   Array of byte where header will be stored.
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   Current msg's header is retrieved.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/
int CMsgBufVSE::nGetCurrMsgHeader(BYTE* pbyHeader)
{
    if ((m_nIndexRead + HEADER_LEN) <= m_nBufferSize)
    {
        memcpy(pbyHeader, (m_pbyMsgBuffer + m_nIndexRead), HEADER_LEN);
    }
    else
    {
        memcpy(pbyHeader, (m_pbyMsgBuffer + m_nIndexRead), m_nBufferSize - m_nIndexRead);
        memcpy(pbyHeader + (m_nBufferSize - m_nIndexRead), m_pbyMsgBuffer, (m_nIndexRead + HEADER_LEN) - m_nBufferSize);
    }

    return CALL_SUCCESS;
}

/**********************************************************************************
Function Name   :   nHandleBufferOverrun()
Input(s)        :   -
Output          :   CALL_SUCCESS for success. CALL_FAILURE for failure.
Functionality   :   If buffer overrun criteria is met, read index
                    is advanced until it solves buffer overrun.
Member of       :   CMsgBufVSE
Friend of       :   -
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
Modifications   :   -
************************************************************************************/

int CMsgBufVSE::nHandleBufferOverrun(INT nSize)
{
    int nResult = CALL_SUCCESS;

    if (m_nIndexRead < m_nIndexWrite)
    {
        if ((m_nIndexRead + m_nBufferSize) <=  (m_nIndexWrite + nSize))
        {
            nResult = WARN_BUFFER_OVERRUN;
            nAdvanceReadIndex();
            nHandleBufferOverrun(nSize);
        }
    }
    else if (m_nIndexRead > m_nIndexWrite)
    {
        if ((m_nIndexRead) <=  (m_nIndexWrite + nSize))
        {
            nResult = WARN_BUFFER_OVERRUN;
            nAdvanceReadIndex();
            nHandleBufferOverrun(nSize);
        }
    }
    else
    {
        if (m_nMsgCount != 0)
        {
            nResult = WARN_BUFFER_OVERRUN;
            nAdvanceReadIndex();
            nHandleBufferOverrun(nSize);
        }
    }

    return nResult;
}
