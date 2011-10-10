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
* @brief  ILINSleepEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINSLEEPEVENT__INCLUDED__)
#define __OLI_ILINSLEEPEVENT__INCLUDED__

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
* @brief  Notification that the sleep mode state of the LIN bus has changed.
*
* There can be different reasons to receive this message:
* @li After creating a controller instance, when a HW port is activated.
* @li After sending a wake-up signal.
* @li After receiving a LIN message the LIN master goes back into sleep state.
*
* Extends the base interface by adding a method to get the 
* @ref GetMode "new sleep mode".
*
* This interface's implementation of @ref IMessage::GetID always returns 0.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ILINLink, ILINSleepEventFilter
*/
OLI_INTERFACE ILINSleepEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINSleepEvent() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_EVENT_BASE + 3};

    /** @brief  Get the new sleep.
        
        @return New sleep mode. 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual LINSleepMode OLI_CALL GetMode() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINSLEEPEVENT__INCLUDED__)
