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
* @brief  ILINWakeUpEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINWAKEUPEVENT__INCLUDED__)
#define __OLI_ILINWAKEUPEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  This LIN node woke up.
*
* This event is received when a LIN node wakes up (comes out of the 
* sleep state) or receives a wake up frame.
*
* The only public extension is the specific @ref TYPE "type identifier".
*
* This interface's implementation of @ref IMessage::GetID always returns 0.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ILINLink, ILINWakeUpEventFilter
*/
OLI_INTERFACE ILINWakeUpEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINWakeUpEvent() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_EVENT_BASE + 2};
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINWAKEUPEVENT__INCLUDED__)
