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
#pragma warning( disable : 4505 )

#include "include/Error.h"
#include "BaseMsgBufAll.h"


const int SIZE_APP_BUFFER       = 20000;


/* This is the concrete template class of a circular queue where each entry is 
of fixed size. Implemented as a template class so as to cater to any data type. 
Here SMSGBUFFER is the data type in operation. */
template <typename SMSGBUFFER>
class CMsgBufFSE : public CBaseMsgBufFSE<SMSGBUFFER>
{
protected:
    SMSGBUFFER m_asMsgBuffer[SIZE_APP_BUFFER]; // The data buffer
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

    CMsgBufFSE();
    ~CMsgBufFSE();

    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer);
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, __int64 nSlotId);
    HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, int nIndex);
    HRESULT WriteIntoBuffer(SMSGBUFFER* psMsgBuffer);
    HRESULT WriteIntoBuffer(const SMSGBUFFER* psMsgBuffer, __int64 nSlotId, int& nIndex);

    int GetMsgCount(void) const;
    int nSetBufferMsgSize(int nMsgDataSize);
    void vClearMessageBuffer(void);
    HANDLE hGetNotifyingEvent(void) const;
};

/******************************************************************************
  Function Name    :  CMsgBufFSE
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER>
CMsgBufFSE<SMSGBUFFER>::CMsgBufFSE()
{
    m_nMsgSize = sizeof(SMSGBUFFER);
    vClearMessageBuffer();
    InitializeCriticalSection(&m_CritSectionForGB);
    m_hNotifyingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);    
}

/******************************************************************************
  Function Name    :  ~CMsgBufFSE
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER>
CMsgBufFSE<SMSGBUFFER>::~CMsgBufFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = NULL;
    DeleteCriticalSection(&m_CritSectionForGB);
}

/******************************************************************************
  Function Name    :  vClearMessageBuffer
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears the message buffer and resets the indices.
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER> void CMsgBufFSE<SMSGBUFFER>::
                                                      vClearMessageBuffer(void)
{
    memset((BYTE *) m_asMsgBuffer, 0, SIZE_APP_BUFFER * m_nMsgSize);
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;    
}

/******************************************************************************
  Function Name    :  ReadFromBuffer
  Input(s)         :  psMsg - The target message entry. An [out] parameter.
  Output           :  EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
  Functionality    :  Reads a message entry and advances the read index. On 
                      successful reading operation the present entry is 
                      invalidated to make room for a new entry.
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER> HRESULT CMsgBufFSE<SMSGBUFFER>::ReadFromBuffer(
                                                            SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexRead > SIZE_APP_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    // Check entry indexed by m_nIndexRead. If this particular entry 
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else
    {
        // Copy the current entry and advance the read index by one.
        memcpy(psMsg, &(m_asMsgBuffer[m_nIndexRead++]), m_nMsgSize);
        if (m_nIndexRead == SIZE_APP_BUFFER)
        {
            m_nIndexRead = 0;
        }
        --m_nMsgCount; // Total number of to-be-read entries decremented by 1.
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}

/******************************************************************************
  Function Name    :  WriteIntoBuffer
  Input(s)         :  psMsg - The source message entry. An [in] parameter.
  Output           :  ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
  Functionality    :  Writes a message entry and advances the write index.
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER> HRESULT CMsgBufFSE<SMSGBUFFER>::WriteIntoBuffer(
                                                             SMSGBUFFER* psMsg)
{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != NULL);
    ASSERT(!(m_nIndexWrite > SIZE_APP_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_BUFFER) // Check for buffer overflow
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        // Write the source entry and advance the write index by one.
        memcpy (&(m_asMsgBuffer[m_nIndexWrite++]), psMsg, m_nMsgSize);
        if (m_nIndexWrite == SIZE_APP_BUFFER)
        {
            m_nIndexWrite = 0;
        }
        ++m_nMsgCount; // Total number of to-be-read entries incremented by 1.
        SetEvent(m_hNotifyingEvent); // Notify addition of an entry.
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}

/******************************************************************************
  Function Name    :  GetBufferLength
  Input(s)         :  void
  Output           :  Number of message entries (int)
  Functionality    :  Returns the number of unread entries in the queue.
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER> int CMsgBufFSE<SMSGBUFFER>::
                                                    GetMsgCount(void) const
{
    return m_nMsgCount;
}

/******************************************************************************
  Function Name    :  hGetNotifyingEvent
  Input(s)         :  void
  Output           :  The notifying event handle (HANDLE)
  Functionality    :  Returns handle of the event that gets signalled when 
                      a message entry is added.
  Member of        :  CMsgBufFSE
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
template <typename SMSGBUFFER> HANDLE CMsgBufFSE<SMSGBUFFER>::
                                                 hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

// To read an entry from the circular queue	
template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* /*psMsgBuffer*/, __int64 /*nSlotId*/)
{
    return ERR_NOT_SUPPORTED;
}

// To read an entry from the circular queue	
template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* /*psMsgBuffer*/, int /*nIndex*/)
{
    return ERR_NOT_SUPPORTED;
}

// To write an entry into the circular queue
template <typename SMSGBUFFER>
HRESULT CMsgBufFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* /*psMsgBuffer*/, 
							__int64 /*nSlotId*/, int& /*nIndex*/)
{
    return ERR_NOT_SUPPORTED;
}

// To set the current queue length
template <typename SMSGBUFFER>
int CMsgBufFSE<SMSGBUFFER>::nSetBufferMsgSize(int /*nMsgDataSize*/)
{
    return ERR_NOT_SUPPORTED;
}
