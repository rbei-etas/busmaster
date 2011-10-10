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
* @brief  ICANErrorFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANERRORFRAME__INCLUDED__)
#define __OLI_ICANERRORFRAME__INCLUDED__

// include used interface and constants

#include "CANBase.h"
#include "../Common/IEvent.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief Interface for CAN error frames.
*
* The only public extension is the specific @ref TYPE "type identifier".
*
* This interface's implementation of @ref IMessage::GetID returns a @ref CANErrorFrame event code.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ICANLink, ICANErrorFrameFilter
*/

OLI_INTERFACE ICANErrorFrame : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ICANErrorFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface. 
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = CAN_TYPE_EVENT_BASE + 2};
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANERRORFRAME__INCLUDED__)
