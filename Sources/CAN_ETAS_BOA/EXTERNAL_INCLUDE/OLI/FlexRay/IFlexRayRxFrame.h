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
* @brief  IFlexRayRxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IFLEXRAYRXFRAME_INCLUDED__)
#define __OLI_IFLEXRAYRXFRAME_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/IRxFrame.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  Interface describing a received FlexRay frame.
*
* In addition to the base class data, this interface provides information 
* on the @ref GetChannel "channel", @ref GetSlot "slot" and @ref 
* GetCycleNumber "cycle number (offset)" of the LPDU-ID in which the frame was 
* received. It also reports the @ref GetFrameType "frame type", a @ref
* GetHeaderCRC "CRC value" for the header as well as the @ref 
* GetPayloadLength "payload length" in 16-bit words (@ref IRxFrame::GetSize
* will return twice the value).
*
* This interface's implementation of @ref IMessage::GetID returns a value created by combining the
* @ref GetChannel "channel number", @ref GetSlot "slot" and @ref GetCycleNumber "cycle number (offset)"
* of the LPDU-ID in which the frame was  received:
* @code
*                +------+------+----------+--------------+
* bit position   |  18  |  17  | 16 .. 6  |    5 .. 0    | 
*                +------+------+----------+--------------+
* content        | ch B | ch A |   slot   | cycle offset |
*                +------+------+----------+--------------+
* @endcode
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them. Consequently, the same applies to the 
*         payload as well.
* @since  BOA 1.3
* @see    IRxQueue, IFlexRayLink
*/
OLI_INTERFACE IFlexRayRxFrame : public IRxFrame
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFlexRayRxFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = FLEXRAY_TYPE_FRAME_BASE + 1};

    /** @brief  Get the type of the received frame.
        
        @return Type of the received frame. 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual FlexRayFrameType OLI_CALL GetFrameType() const OLI_NOTHROW = 0;

    /** @brief  Channel(s) on which the frame was received.
        
        @return Channel(s) on which the frame was received.
                Maybe @ref FLEXRAY_CHANNEL_BOTH depending on the
                FlexRay setup in place.
        @exception <none> This function must not throw exceptions.

        @remark This information will also be mapped into the bits
                17 and 18 of the @ref IMessage::GetID "combined frame ID".
        @since  BOA 1.3 
        @see    @ref IMessage::GetID
     */
    virtual FlexRayChannel OLI_CALL GetChannel() const OLI_NOTHROW = 0;

    /** @brief  Get slot in which the frame was received.
        
        @return Slot in which the frame was received. 
        @exception <none> This function must not throw exceptions.

        @remark This information will also be mapped into the bits
                6 through 16 of the @ref IMessage::GetID "combined frame ID".
        @since  BOA 1.3 
        @see    @ref IMessage::GetID
     */
    virtual uint16 OLI_CALL GetSlot() const OLI_NOTHROW = 0;

    /** @brief  Get the cycle number (offset) in which the frame 
                was received.
        
        @return Cycle number (offset) in which the frame 
                was received. 
        @exception <none> This function must not throw exceptions.

        @remark This information will also be mapped into the bits
                0 through 5 of the @ref IMessage::GetID "combined frame ID".
        @since  BOA 1.3 
        @see    @ref IMessage::GetID
     */
    virtual uint8 OLI_CALL GetCycleNumber() const OLI_NOTHROW = 0;
        
    /** @brief  Get the CRC checksum for the header.
        
        @return CRC for the header.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual uint16 OLI_CALL GetHeaderCRC() const OLI_NOTHROW = 0;
    
    /** @brief  Length of the payload in 16 bit words.
        
        @return Length of the payload in 16 bit words.
        @exception <none> This function must not throw exceptions.

        @remark This value must not be larger than @ref 
                IMessage::GetSize() / 2. Because the payload length
                is a construct taken from the FlexRay bus while the
                receive buffer size is a driver internal construct,
                there is no guarantee that both are equivalent.
        @since  BOA 1.3 
     */
    virtual uint8 OLI_CALL GetPayloadLength() const OLI_NOTHROW = 0;
};

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYRXFRAME_INCLUDED__)
