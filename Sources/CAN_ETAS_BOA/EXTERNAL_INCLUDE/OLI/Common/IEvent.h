/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/** 
* @file
* @brief  IEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IEVENT_INCLUDED__)
#define __OLI_IEVENT_INCLUDED__

// include base interface

#include "IRxMessage.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief  Base interface for all events.
*
* Since events cannot be sent but only received, i.e. observed,
* all events are implicitly @ref IRxMessage instances.
*
* This interface has been introduced to structure the interface
* inheritance tree. It does not expose additional concepts.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them. Consequently, the same applies to the 
*         payload as well.
* @since  BOA 1.3
* @see    IRxQueue, IRxMessage
*/

OLI_INTERFACE IEvent : public IRxMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IEvent() OLI_NOTHROW {};
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IEVENT_INCLUDED__)
