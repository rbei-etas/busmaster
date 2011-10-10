/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4794 $
*/

/** 
* @file
* @brief  ITimerEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ITIMEREVENT__INCLUDED__)
#define __OLI_ITIMEREVENT__INCLUDED__

// include used interface and constants

#include "CommonBase.h"
#include "IEvent.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief  Interface for timer events.
*
* Represents a timer event such as loss or re-establishment of
* synchronization. Since these are received as in-band messages,
* the validity or interpretation of the time stamps of all following 
* messages may change while all previous messages will not be affected.
*
* This interface's implementation of @ref IMessage::GetID returns the @ref TimerEventCode "timer event code".
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, TimerEventCode
*/

OLI_INTERFACE ITimerEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ITimerEvent() OLI_NOTHROW {};

public:

    enum {TYPE = COMMON_TYPE_EVENT_BASE + 2 /**< returned by @ref GetType */};

};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ITIMEREVENT__INCLUDED__)
