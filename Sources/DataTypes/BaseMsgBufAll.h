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
 * \file      BaseMsgBufAll.h
 * \brief     Defines base class for circular queue, both FSE and VSE.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines base class for circular queue, both FSE and VSE.
 */

#pragma once

/* This is the interface class of a circular queue where each entry is of fixed 
size. Implemented as a template class so as to cater to any data type. Here 
SMSGBUFFER is the data type in operation. This is expected to be a structure 
having contiguous memory layout. FSE stands for 'fixed sized entry'. */
template <typename SMSGBUFFER>
class CBaseMsgBufFSE
{
public:
    CBaseMsgBufFSE();   // Standard constructor
	~CBaseMsgBufFSE();  // Destructor

    // To read an entry from the circular queue
	virtual HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer) = 0;

    // To write an entry into the circular queue
	virtual HRESULT WriteIntoBuffer(SMSGBUFFER* psMsgBuffer) = 0;

    // To get the present queue length (number of entries)
    virtual int GetMsgCount(void) const = 0;

    // To clear the queue. This doesn't remove the buffer, only clears it.
    virtual void vClearMessageBuffer(void) = 0;

    // To get handle of the notifying event that will be signalled when there
    // is at least one entry in the queue.
    virtual HANDLE hGetNotifyingEvent(void) const = 0;

    // To read an entry from the circular queue	
	virtual HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, __int64 nSlotId)=  0;
    virtual HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, int nIndex)=  0;

    // To write an entry into the circular queue	
	virtual HRESULT WriteIntoBuffer(const SMSGBUFFER* psMsgBuffer, 
								__int64 nSlotId, int& nIndex)= 0;

    // To set the current queue length    
	virtual int nSetBufferMsgSize(int nMsgDataSize)= 0;
};

template <typename SMSGBUFFER> CBaseMsgBufFSE<SMSGBUFFER>::CBaseMsgBufFSE()
{
    ;
}

template <typename SMSGBUFFER> CBaseMsgBufFSE<SMSGBUFFER>::~CBaseMsgBufFSE()
{
    ;
}


/* This is the interface class of a circular queue where each entry is of 
variable size. VSE stands for 'variable sized entry'. Therefore, function 
prototypes are a bit different. VSE makes it possible for the queue to 
accommodate data types of various types. Expectedly, the class treats each 
entry as a continuous byte stream. */
class CBaseMsgBufVSE
{
public:
	CBaseMsgBufVSE() // Standard constructor
	{
	};

	~CBaseMsgBufVSE() // Destructor
	{
	};

    // To read an entry from the circular queue
	virtual HRESULT ReadFromBuffer(INT& nType, BYTE* psMsg, INT& nSize) = 0;

    // To write an entry into the circular queue
	virtual int WriteIntoBuffer(INT nType, BYTE* ps_Msg, INT nSize)= 0;

    // To get the present queue length (number of entries)
    virtual int GetBufferLength(void) const = 0;

    // To clear the queue. This doesn't remove the buffer, only clears it.
    virtual void vClearMessageBuffer(void) = 0;

    // To get handle of the notifying event that will be signalled when there
    // is at least one entry in the queue.
    virtual HANDLE hGetNotifyingEvent(void) const = 0;

    // To set the current queue length
    virtual int nSetBufferSize(int& nSize) = 0;
};
/* This is the interface class of a circular queue where each entry is of 
variable size. VVSE stands for 'variant variable sized entry'. It is a special type of VSE
where user can read n'th element from the buffer. */
class CBaseMsgBufVVSE
{
public:
	CBaseMsgBufVVSE() // Standard constructor
	{
	};

	~CBaseMsgBufVVSE() // Destructor
	{
	};

    // To write an entry into the circular queue
	virtual int WriteIntoBuffer(INT nType, BYTE* ps_Msg, INT nSize)= 0;

    // To get the present queue length (number of entries)
    virtual int GetMsgCount(void) const = 0;

    // To clear the queue. This doesn't remove the buffer, only clears it.
    virtual void vClearMessageBuffer(void) = 0;

    // To get handle of the notifying event that will be signalled when there
    // is at least one entry in the queue.
    virtual HANDLE hGetNotifyingEvent(void) const = 0;

    // To set the current queue length
    virtual int nSetBufferSize(int& nSize) = 0;
	//To read the nEntry'th element.
	virtual HRESULT ReadEntry(int& nType, BYTE* pbyMsg, int& nSize, int nEntry, BOOL bSetNextIndexStartPos) = 0;
};

//template <typename SMSGBUFFER>
//class CBaseMsgBufVFSE
//{
//public:
//	CBaseMsgBufVFSE() // Standard constructor
//	{
//	};
//
//	~CBaseMsgBufVFSE() // Destructor
//	{
//	};
//
//    // To read an entry from the circular queue	
//	virtual HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, __int64 nSlotId)=  0;
//    virtual HRESULT ReadFromBuffer(SMSGBUFFER* psMsgBuffer, int nIndex)=  0;
//
//    // To write an entry into the circular queue	
//	virtual HRESULT WriteIntoBuffer(const SMSGBUFFER* psMsgBuffer, 
//								__int64 nSlotId, int& nIndex)= 0;
//	                
//    // To get the present queue length (number of entries)
//    virtual int GetMsgCount(void) const = 0;
//
//    // To clear the queue. This doesn't remove the buffer, only clears it.
//    virtual void vClearMessageBuffer(void)= 0;
//
//    // To get handle of the notifying event that will be signalled when there
//    // is at least one entry in the queue.
//    virtual HANDLE hGetNotifyingEvent(void) const = 0;
//
//    // To set the current queue length    
//	virtual int nSetBufferMsgSize(int nMsgDataSize)= 0;
//};
