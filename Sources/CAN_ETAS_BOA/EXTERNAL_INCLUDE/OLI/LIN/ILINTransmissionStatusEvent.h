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
* @brief  ILINTransmissionStatusEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ILINTRANSMISSIONSTATUSEVENT__INCLUDED__)
#define __OLI_ILINTRANSMISSIONSTATUSEVENT__INCLUDED__

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
* @brief  Transmission result of the latest LIN frame.
*
* This event reports everything about the transmitted frame except
* for the payload.
*
* This interface's implementation of @ref IMessage::GetID returns the @ref IMessage::GetID "frame ID"
* of the transmitted frame.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ILINLink, ILINTransmissionStatusEventFilter
*/

OLI_INTERFACE ILINTransmissionStatusEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINTransmissionStatusEvent() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_EVENT_BASE + 4};

    /** @brief  Get the transmission state.
        
                This value will tell the client application whether the
                frame was transmitted correctly. If it wasn't, it will 
                return the kind of failure that was detected.

        @return Transmission state. 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual LINTransmissionState OLI_CALL GetState() const OLI_NOTHROW = 0;

    /** @brief  Get the direction from the perspective of this node.
        
        @return Direction of the frame from the perspective of this node. 
        @exception <none> This function must not throw exceptions.

        @remark The result may be any value from @ref LINFrameDirection.
                In particular, @ref LIN_FRAME_NOP is a valid return value.
        @since  BOA 1.3 
     */
    virtual LINFrameDirection OLI_CALL GetDirection() const OLI_NOTHROW = 0;

    /** @brief  Get the frame's payload size (DLC).
        
        @return The payload size (DLC). 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual uint8 OLI_CALL GetDLC() const OLI_NOTHROW = 0;

    /** @brief  Get the CRC type that was used.
        
        @return The CRC type that was used. 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINTRANSMISSIONSTATUSEVENT__INCLUDED__)
