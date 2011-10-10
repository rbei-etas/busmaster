/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4831 $
*/

/** 
* @file
* @brief  ICANRxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANRXFRAME_INCLUDED__)
#define __OLI_ICANRXFRAME_INCLUDED__

// include used interface and constants

#include "CANBase.h"
#include "../Common/IRxFrame.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  A received CAN bus data frame.
*
* CAN frames only add various two status flags (@ref 
* IsRemoteFrame "remote frames" and @ref IsSelfReception "self-reception")
* as well as the @ref IsExtendedID "extended CAN ID" indicator.
*
* This interface's implementation of @ref IMessage::GetID returns the message ID of the CAN frame, in whichever
* encoding is used by the underlying OCD.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them. Consequently, the same applies to the 
*         payload as well.
* @since  BOA 1.3
* @see    IRxQueue, ICANLink
*/

OLI_INTERFACE ICANRxFrame : public IRxFrame
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ICANRxFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = CAN_TYPE_FRAME_BASE + 1};

    /** @brief  Returns whether the @ref IMessage::GetID "frame ID" is 
                an extended ID.

        @return @c true, if the @ref IMessage::GetID "frame ID" is and
                extended CAN ID (29 bits). Otherwise, it is an 11 bit
                standard CAN ID.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IMessage::GetID
     */
    virtual bool OLI_CALL IsExtendedID() const OLI_NOTHROW = 0;

    /** @brief  This is a CAN remote frame.

                If set, some CAN node required a frame with the requested
                @ref IMessage::GetID to be sent.

        @return @c true, if is a remote frame.
        @exception <none> This function must not throw exceptions.

        @remark Don't confuse this with the concept of @ref 
                IsSelfReception "self-reception".
        @remark Not all drivers will support remote frames.
        @since  BOA 1.3 
        @see    IMessage::GetID, ILink::GetCapabilities
     */
    virtual bool OLI_CALL IsRemoteFrame() const OLI_NOTHROW = 0;

    /** @brief  Indicates whether this is a self-reception frame.

                If set, the frame was sent on this node. Since most
                CAN controller hardware can't receive the frames they 
                transmit themselves, those frames must be "looped back"
                in the driver layer above. That may distort timestamps
                and reception order.

        @return @c true, if is a remote frame.
        @exception <none> This function must not throw exceptions.

        @remark Self-reception must be activated in the controller / link
                configuration.
        @remark Not all drivers will allow for self-reception.
        @since  BOA 1.3 
        @see    IMessage::GetID, ILink::GetCapabilities, 
                ILink::GetConfiguration
     */
    virtual bool OLI_CALL IsSelfReception() const OLI_NOTHROW = 0;

    /** @brief  Returns the data length code (DLC) of the received CAN frame.

                Usually, the DLC indicates the number of data bytes in the frame, (i.e. the number of valid bytes
                returned by @ref IRxFrame::GetData) and is therefore <= 8 and equal to the value returned by
                @ref IRxFrame::GetSize.
                
                However, the CAN specification implicitly permits DLC values which are > 8 and <= 15, though in such a
                case the frame will still contain only 8 data bytes. Consequently, some CAN controllers are able to
                receive a frame with DLC > 8 (though such a frame still only contains 8 data bytes); the OLI API will
                process such a frame as follows:
                    - @ref ICANRxFrame::GetDLC will return the frame's DLC (> 8);
                    - @ref IRxFrame::GetData will return the frame's payload (= 8 bytes).
                    - @ref IRxFrame::GetSize will return the number of bytes in the frame's payload (= 8);

        @return The data length code (DLC) of the received CAN frame.
        @exception <none> This function must not throw exceptions.

        @see    IRxFrame::GetData, IRxFrame::GetSize
     */
    virtual uint8 OLI_CALL GetDLC() const OLI_NOTHROW = 0;

};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANRXFRAME_INCLUDED__)
