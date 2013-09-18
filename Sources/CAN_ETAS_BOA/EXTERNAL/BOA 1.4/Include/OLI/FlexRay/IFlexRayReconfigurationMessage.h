/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4412 $
*/

/** 
* @file
* @brief  IFlexRayReconfigurationMessage definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYRECONFIGMESSAGE_INCLUDED__)
#define __OLI_IFLEXRAYRECONFIGMESSAGE_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/ITxMessage.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declarations

class IFlexRayReconfigurationMessage;

/**
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  This function instantiates an object supporting 
*         @ref IFlexRayReconfigurationMessage. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref IFlexRayReconfigurationMessage::Create().
*
* @param[in]  txBufferIndex
*         Index of the controller-level send buffer to reconfigure.
*         Use @ref IFlexRayLink::GetTxBufferIndex to find a specific buffer.
* @param[in]  channel
*         Channel to assign the buffer to.
* @param[in]  slot
*         Slot to assign the buffer to.
*         It should be part of the dynamic segment.
* @param[in]  cyclePeriod
*         The buffer shall be serviced every @a cyclePeriod bus cycles.
* @param[in]  cycleOffset
*         Cycle number within the @a cyclePeriod to which the send buffer 
*         shall be assigned.
* @param[in]  payloadLength
*         Maximum length (in 16 bit words) of a frame that can be sent
*         through this buffer. In the static segment, this value should
*         match the respective slot size. Also, in certain cases, a 
*         different payload length may not be supported at all by the 
*         underlying driver implementation.
* @param[in]  payloadPreambleIndicator
*         Determines the polarity of payload preamble indicator bit.
* @param[out] ppFlexRayReconfigMsg     
*         A pointer to an object supporting @ref 
*         IFlexRayReconfigurationMessage. The object 
*         is owned by the caller, i.e. the client application is expected
*         to eventually call the @ref ITxMessage::Destroy method.
*         This can be ensured by wrapping the object pointer in an instance 
*         of the @ref AutoDestroyPtr class.
*
* @return A pointer to an interface based on @ref IError, describing 
*         the error which occurred during this function. @c NULL if 
*         no error occurred. See @ref ErrorReporting "error reporting"
*         for more information on how errors are reported.
*
* @exception <none> This function must not throw exceptions.
*
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         IFlexRayReconfigurationMessage
*/
OLL_API IError* OLI_CALL IFlexRayReconfigurationMessage_Create(
    uint32          txBufferIndex,
    FlexRayChannel  channel,
    uint32          slot,
    uint32          cyclePeriod,
    uint32          cycleOffset,
    uint32          payloadLength,
    bool            payloadPreambleIndicator,
    IFlexRayReconfigurationMessage** ppFlexRayReconfigMsg);

/** 
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  Interface for transmit buffer reconfiguration pseudo-messages.
*
* Certain application protocols, e.g. XCP on FlexRay, require LPDU-IDs
* to be dynamically allocated amongst the partipating nodes. Furthermore,
* typical FlexRay controller firmware supports only a limited number of
* transmit buffers. Thus, these buffers need to be reused for different
* LPDU-IDs.
*
* Sending an instance of this pseudo-message to a transmit queue will
* reconfigure the specified transmit buffer to apply to a different
* LPDU-ID.
*
* Its proccessing will be synchronized with all other messages in the
* queue: The buffer will be reconfigured after all previous messages
* got processed / sent to the bus and for all following messages, the
* new transmit buffer configuration will apply.
*
* @ref IMessage::GetID returns the same value as @ref GetTxBufferIndex.
*
* @remark All public methods except @ref ITxMessage::Destroy are thread-safe.
* @remark The OLI @ref ITxQueue "transmit queues" will never assume
*         ownership over the message instances added to them.
*         Instead, the client is responsible of destroying the
*         message instances after usage.
* @remark Pseudo-messages will not be sent to the bus but consumed by
*         the driver stack itself.
* @remark A queue will only be able to modify transmit buffers assigned to 
*         that queue. Pseudo-messages pertaining to other buffers will be
*         ignored.
* @remark Please note that reconfiguring transmit buffers may cause following
*         messages in the same queue to get "stuck", i.e. never be processed
*         if no suitable (e.g. sufficiently large) active transmit buffer 
*         remains. The only way to resolve that situation would be either to 
*         call the @ref ITxQueue::Clear method or to close the queue entirely.
* @remark Changing the transmit buffer configuration may be unsafe in the
*         static segment.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, IFlexRayLink, IFlexRaySwitchMessage
*
* @todo   verify that unsuitable reconf messages get ignored.
* @todo   how are conflicts handled?
* @todo   Can we reconfigure buffers that are active?
*/
OLI_INTERFACE IFlexRayReconfigurationMessage : public ITxMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFlexRayReconfigurationMessage() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface. 
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = FLEXRAY_TYPE_FRAME_BASE + 3};

    /** @brief  Transmit buffer to reconfigure.
        
        @return Index of the transmit buffer to reconfigure.
        @exception <none> This function must not throw exceptions.

        @remark The transmit buffer should be assigned to the 
                queue to which this message gets added. Otherwise,
                this message will be ignored.
        @todo   verify remark!
        @since  BOA 1.3 
        @see    @ref IFlexRayLink::GetTxBufferIndex
     */
    virtual uint32 OLI_CALL GetTxBufferIndex() const OLI_NOTHROW = 0;

    /** @brief  Channel part of the new LPDU-ID.
        
        @return Channel(s) to which this buffer shall be assigned to.
                @ref FLEXRAY_CHANNEL_BOTH is valid as well.
        @todo   verify return!
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual FlexRayChannel OLI_CALL GetChannel() const OLI_NOTHROW = 0;

    /** @brief  Slot part of the new LPDU-ID.
        
        @return Slot in which this transmit buffer shall be used.
        @exception <none> This function must not throw exceptions.

        @remark The slot should be part of the dynamic segment.
        @since  BOA 1.3 
     */
    virtual uint32 OLI_CALL GetSlot() const OLI_NOTHROW = 0;

    /** @brief  Transmission cycle period.
        
                Determines, in conjunction with @ref GetCycleOffset,
                the cycle number part of the new LPDU-ID.

        @return Number of bus cycles + 1 between two transmission
                attempts. Must be a power of 2 and not exceed 64.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetCycleOffset
     */
    virtual uint32 OLI_CALL GetCyclePeriod() const OLI_NOTHROW = 0;

    /** @brief  Transmission cycle offset.
        
                Determines, in conjunction with @ref GetCyclePeriod,
                the cycle number part of the new LPDU-ID.

        @return Transmission cycle offset relative to the @ref 
                GetCyclePeriod "cycle period". Must be a smaller than 
                the cycle period.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetCyclePeriod
     */
    virtual uint32 OLI_CALL GetCycleOffset() const OLI_NOTHROW = 0;

    /** @brief  Payload length in 16-bit words.
        
                Determines the maximum playload length of any
                frame that can be sent in this LPU-ID.

        @return Maximum payload length in 16-bit words.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IFlexRayTxFrame
        @todo   any limits to resizing?
     */
    virtual uint32 OLI_CALL GetPayloadLength() const OLI_NOTHROW = 0;

    /** @brief  Whether the playload preamble will be transmitted.

        @return The payload preamble indicator bit.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @todo   applicability in both segments?
     */
    virtual bool OLI_CALL GetPayloadPreambleIndicator() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IFlexRayReconfigurationMessage instance.

        @param[in]  txBufferIndex
                Index of the controller-level send buffer to reconfigure.
                Use @ref IFlexRayLink::GetTxBufferIndex to find a specific
                buffer.
        @param[in]  channel
                Channel to assign the buffer to.
        @param[in]  slot
                Slot to assign the buffer to.
        @param[in]  cyclePeriod
                The buffer shall be serviced every @a cyclePeriod bus cycles.
        @param[in]  cycleOffset
                Cycle number within the @a cyclePeriod to which the transmit
                buffer shall be assigned. It should be part of the dynamic 
                segment.
        @param[in]  payloadLength
                Maximum length (in 16 bit words) of a frame that can be sent
                through this buffer. In the static segment, this value should
                match the respective slot size. Also, in certain cases, a 
                different payload length may not be supported at all by the 
                underlying driver implementation.
        @param[in]  payloadPreambleIndicator
                Determines the polarity of payload preamble indicator bit.

        @return New @ref IFlexRayReconfigurationMessage instance. The object 
                is owned by the caller, i.e. the client application is 
                expected to eventually call the @ref ITxMessage::Destroy 
                method. This can be ensured by wrapping the object pointer 
                in an instance of the @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref IFlexRayReconfigurationMessage_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, IFlexRayReconfigurationMessage_Create
     */
    static IFlexRayReconfigurationMessage* OLI_CALL Create 
        ( uint32 txBufferIndex
        , FlexRayChannel channel
        , uint32 slot
        , uint32 cyclePeriod
        , uint32 cycleOffset
        , uint32 payloadLength
        , bool payloadPreambleIndicator )
    {
        IFlexRayReconfigurationMessage* pFlexRayReconfigMsg = NULL;
        CheckAndThrow( 
            IFlexRayReconfigurationMessage_Create( txBufferIndex, 
                                                   channel, 
                                                   slot, 
                                                   cyclePeriod, 
                                                   cycleOffset, 
                                                   payloadLength, 
                                                   payloadPreambleIndicator, 
                                                   &pFlexRayReconfigMsg ) );
        return pFlexRayReconfigMsg;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYRECONFIGMESSAGE_INCLUDED__)
