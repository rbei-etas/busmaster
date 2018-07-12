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
 * \file      MsgBufVVSE.h
 * \brief     Defines a variant of circular queue class in VSE.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines a variant of circular queue class in VSE.
 */

#pragma once

#include "BaseMsgBufAll.h"

/////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************
Class Name      :   CMsgBufVVSE
Authors         :   Pradeep Kadoor
Date Created    :   22/06/2009
************************************************************************************/
class CMsgBufVVSE : public CBaseMsgBufVVSE
{
private:
    BYTE* m_pbyMsgBuffer;
    CRITICAL_SECTION m_CritSectionForGB;
    int m_nBufferSize, m_nIndexWrite, m_nMsgCount;//, m_nMsgSkipped;

    // Two special variables to read the Msg at the index
    int m_nStartIndex, m_nReadIndexTmp;

    // holds the msg number for which m_nReadIndexTmp is pointing to.
    int m_nTmpMsgCount;

    HANDLE m_hNotifyingEvent;

    /* Helper function to advance the nIndex to next msg*/
    int nAdvanceReadIndex(int& nIndex);
    /* Helper function to read msg from the circular buffer from the index nIndex*/
    HRESULT ReadBuffer(INT& nType, BYTE* pbyMsg, INT& nSize, INT& nIndex);
    /* Helper function to write msg into the circular buffer*/
    int nWriteBuffer(INT nType, BYTE* pbyMsg, INT nSize);
    /* Helper function construct header with TYPE and DATA LENGTH*/
    int nConstructHeader(INT nType, INT nSize, BYTE* pbyHeader);
    /* Helper function to get header of 'nIndex'th message*/
    int nGetCurrMsgHeader(int nIndex, BYTE* pbyHeader);
    /* Move the temporary read index if required whenever overrun happens*/
    void vHandleTempReadIndex();
    /* Handle start index whenever buffer overrun happens*/
    int nHandleStartIndex(int nSize);
public:
    CMsgBufVVSE();
    ~CMsgBufVVSE();

    /* Writes message into the buffer. Caller needs to allocate memory for the
    out parameter */
    int WriteIntoBuffer(INT nType, BYTE* ps_Msg, INT nSize);
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
    //int GetSkippedMsgCount(void) const;
    /* Sets the start pos to read the msg*/
    HRESULT SetStartPos(int nEntry);
    /* Reads the nEntryth Msg from the start position,
       user will have an option set the next entry as start pos*/
    HRESULT ReadEntry(int& nType, BYTE* pbyMsg, int& nSize, int nEntry, BOOL bSetNextIndexStartPos);
};
