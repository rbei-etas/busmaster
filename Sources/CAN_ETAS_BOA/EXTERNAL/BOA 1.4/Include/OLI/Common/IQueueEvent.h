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
* @brief  IQueueEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IQUEUEEVENT__INCLUDED__)
#define __OLI_IQUEUEEVENT__INCLUDED__

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
* @brief Interface for receive queue events.
*
* Represents a queue overflow event. If this event is received,
* at least one message has been lost somewhere in the driver stack.
* In other words, the client application did not process the steam
* of incoming messages fast enough.
*
* This interface's implementation of @ref IMessage::GetID returns the @ref QueueEventCode "queue event code".
* Currently, only @ref QUEUE_OVERFLOW gets detected.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, QueueEventCode
*/

OLI_INTERFACE IQueueEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IQueueEvent() OLI_NOTHROW {};

public:

    enum {TYPE = COMMON_TYPE_EVENT_BASE + 3 /**< returned by @ref GetType */};
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IQUEUEEVENT__INCLUDED__)
