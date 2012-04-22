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
 * \file      MsgBufVSE.h
 * \brief     Defines concrete class for circular queue in VSE.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines concrete class for circular queue in VSE.
 */

#pragma once

#include "BaseMsgBufAll.h"

/////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************
Class Name      :   CMsgBufVSE
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
************************************************************************************/
class CMsgBufVSE : public CBaseMsgBufVSE
{
private:
    BYTE* m_pbyMsgBuffer;
    CRITICAL_SECTION m_CritSectionForGB;
    int m_nBufferSize, m_nIndexRead, m_nIndexWrite, m_nMsgCount, m_nMsgSkipped;
    HANDLE m_hNotifyingEvent;
    /* Helper function to advance the read index to next msg*/
    int nAdvanceReadIndex(void);
    /* Helper function to read current msg from the circular buffer*/
    HRESULT ReadBuffer(INT& nType, BYTE* pbyMsg, INT& nSize);
    /* Helper function to write msg into the circular buffer*/
    int nWriteBuffer(INT nType, BYTE* pbyMsg, INT nSize);
    /* Helper function construct header with TYPE and DATA LENGTH*/
    int nConstructHeader(INT nType, INT nSize, BYTE* pbyHeader);
    /* Helper function to get current message header */
    int nGetCurrMsgHeader(BYTE* pbyHeader);
    /* Helper function to handle buffer overrun*/
    int nHandleBufferOverrun(INT nSize);
public:
    CMsgBufVSE();
    ~CMsgBufVSE();
    /* Reads current message from the buffer. 
    Caller has to allocate memory for the out parameter */
    HRESULT ReadFromBuffer(INT& nType, BYTE* psMsg, INT& nSize);
    /* Writes message into the buffer. Caller needs to allocate memory for the
    out parameter */
    int WriteIntoBuffer(INT nType, BYTE* ps_Msg, INT nSize);
    // To get the buffer length
    int GetBufferLength(void) const;
    /* Clears the buffer and resets read & write indices */
    void vClearMessageBuffer(void);
    /* Caller can set the buffer size */
    int nSetBufferSize(int& nSize);
    /* Current message will be skipped and advances to next msg */
    HRESULT AdvanceToNextMsg(void);

    /* Gets the message count */
    int GetMsgCount(void) const;
    /* Gets the notifying event handler */
    HANDLE hGetNotifyingEvent(void) const;
    /* Gets no of skipped msgs because of buffer overrun */
    int GetSkippedMsgCount(void) const;
};
