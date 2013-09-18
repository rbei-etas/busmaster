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
* @brief  IFlexRayFrameFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IFLEXRAYFRAMEFILTER_INCLUDED__)
#define __OLI_IFLEXRAYFRAMEFILTER_INCLUDED__

// include used interface

#include "FlexRayBase.h"

#include "../Common/AutoPtr.h"
#include "../Common/IFrameFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class IFlexRayFrameFilter;

/**
* @ingroup GROUP_OLI_FLEXRAY_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref IFlexRayFrameFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref IFlexRayFrameFilter::Create().
*
* @param[in]  channelMask   
*         Any combination of @ref FlexRayChannel flags to be
*         applied to @ref IFlexRayRxFrame::GetChannel.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetChannelMask.
* @param[in]  channelValue  
*         Any combination of @ref FlexRayChannel flags to be
*         applied to @ref IFlexRayRxFrame::GetChannel.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetChannelValue.
* @param[in]  slotMask
*         Any mask for the slot in which the frame was received.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetSlotMask.
* @param[in]  slotValue
*         Any value for the slot in which the frame was received.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetSlotValue.
* @param[in]  cycleNumberMask
*         Any mask for the cycle in which the frame was received.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetCycleNumberMask.
* @param[in]  cycleNumberValue
*         Any value for the cycle in which the frame was received.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetCycleNumberValue.
* @param[in]  payloadLengthMask
*         Any mask for the length of the frame's payload.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetPayloadLengthMask.
* @param[in]  payloadLengthValue
*         Any mask for the length of the frame's payload.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetPayloadLengthValue.
* @param[in]  frameTypeMask   
*         Any combination of @ref FlexRayFrameType flags to be
*         applied to @ref IFlexRayRxFrame::GetFrameType.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetFrameTypeMask.
* @param[in]  frameTypeValue  
*         Any combination of @ref FlexRayFrameType flags to be
*         applied to @ref IFlexRayRxFrame::GetFrameType.
*         The filter will report it in 
*         @ref IFlexRayFrameFilter::GetFrameTypeValue.
* @param[in]  payloadConditions                
*         Conditions on the payload of the frame (see @ref IFrameFilter
*         for details). The conditions will be accessible through
*         @ref IFrameFilter::GetCondition. May be NULL only if
*         @ref payloadConditionsCount is 0.
* @param[in]  payloadConditionsCount               
*         Number of conditions on the payload, i.e. number of valid
*         elements in @ref payloadConditions. May be 0.
*         The filter will report it in @ref IFrameFilter::GetCount.
* @param[out] ppFlexRayFrameFilter   
*         A pointer to an object supporting @ref IFlexRayFrameFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref IFlexRayFrameFilter_Create() 
*         via the wrapper @ref IFlexRayFrameFilter::Create().
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
*         IFlexRayFrameFilter, IFrameFilter
*/
OLL_API IError* OLI_CALL IFlexRayFrameFilter_Create(
    uint32                  channelMask,
    uint32                  channelValue,
    uint32                  slotMask,
    uint32                  slotValue,
    uint32                  cycleNumberMask,
    uint32                  cycleNumberValue,
    uint32                  payloadLengthMask,
    uint32                  payloadLengthValue,
    uint32                  frameTypeMask,
    uint32                  frameTypeValue,
    const PayloadCondition* payloadConditions,
    uint32                  payloadConditionsCount,
    IFlexRayFrameFilter**   ppFlexRayFrameFilter );


/** @ingroup GROUP_OLI_FLEXRAY_FILTERS
* @brief  A specialized frame filter class for FlexRay frames.
*
* In addition to the base class conditions, this interface checks 
* for the @ref IFlexRayRxFrame::GetChannel "channel",
* @ref IFlexRayRxFrame::GetSlot "direction" and
* @ref IFlexRayRxFrame::GetCycleNumber "cycle offset" of the
* LPDU-ID in which the frame was received. It allows also to
* filter for the @ref IFlexRayRxFrame::GetFrameType "frame type"
* and @ref IFlexRayRxFrame::GetPayloadLength "payload length"
* (in 16-bit words).
*
* The implementation is expected to match @ref IFlexRayRxFrame 
* instances only.
*
* This interface's implementation of IFilter::GetIDMask returns a value created by combining the
* @ref GetChannelMask "channel mask", @ref GetSlotMask "slot mask" and @ref GetCycleNumberMask "cycle offset mask"
* specified in IFlexRayFrameFilter::Create():
* @code
*                +------+------+----------+--------------+
* bit position   |  18  |  17  | 16 .. 6  |    5 .. 0    | 
*                +------+------+----------+--------------+
* content        | ch B | ch A |   slot   | cycle offset |
*                +------+------+----------+--------------+
* @endcode
* This interface's implementation of IFilter::GetIDValue returns a similar combination of
* @ref GetChannelValue "channel value", @ref GetSlotValue "slot value" and @ref GetCycleNumberValue "cycle offset value".
*
* It is important that these two encoding schemes match the similar scheme used by the IFlexRayTxFrame implementation
* for encoding the value returned by IMessage::GetID.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", IFlexRayTxFrame
*/
OLI_INTERFACE IFlexRayFrameFilter 
    : public IFrameFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFlexRayFrameFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the
                @ref IFlexRayRxFrame::GetChannel "channel".

        @return @ref filterConcepts "filter mask" for the 
                @ref IFlexRayRxFrame::GetChannel "channel" on
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetChannelMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the 
                @ref IFlexRayRxFrame::GetChannel "channel".

        @return @ref filterConcepts "filter value" for the 
                @ref IFlexRayRxFrame::GetChannel "channel" on
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetChannelValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the 
                @ref IFlexRayRxFrame::GetSlot "slot".

        @return @ref filterConcepts "filter mask" for the 
                @ref IFlexRayRxFrame::GetSlot "slot" in
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetSlotMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the 
                @ref IFlexRayRxFrame::GetSlot "slot".

        @return @ref filterConcepts "filter value" for the 
                @ref IFlexRayRxFrame::GetSlot "slot" in
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetSlotValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the 
                @ref IFlexRayRxFrame::GetCycleNumber "cycle number".

        @return @ref filterConcepts "filter mask" for the 
                @ref IFlexRayRxFrame::GetCycleNumber "cycle number" in
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetCycleNumberMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the 
                @ref IFlexRayRxFrame::GetCycleNumber "cycle number".

        @return @ref filterConcepts "filter value" for the 
                @ref IFlexRayRxFrame::GetCycleNumber "cycle number" in
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetCycleNumberValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the 
                @ref IFlexRayRxFrame::GetPayloadLength "payload length".

        @return @ref filterConcepts "filter mask" for the 
                @ref IFlexRayRxFrame::GetPayloadLength "payload length" in
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetPayloadLengthMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the 
                @ref IFlexRayRxFrame::GetPayloadLength "payload length".

        @return @ref filterConcepts "filter value" for the 
                @ref IFlexRayRxFrame::GetPayloadLength "payload length" in
                which the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetPayloadLengthValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the 
                @ref IFlexRayRxFrame::GetFrameType "frame type".

        @return @ref filterConcepts "filter mask" for the 
                @ref IFlexRayRxFrame::GetFrameType "type" of
                the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetFrameTypeMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the 
                @ref IFlexRayRxFrame::GetFrameType "frame type".

        @return @ref filterConcepts "filter value" for the 
                @ref IFlexRayRxFrame::GetFrameType "type" of
                the frame was received.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayRxFrame
     */
    virtual uint32 OLI_CALL GetFrameTypeValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IFlexRayFrameFilter instance.

                The instance returned here will only match 
                @ref IFlexRayRxFrame messages. All other
                types will not pass the filter.

        @param[in]  channelMask   
                Any combination of @ref FlexRayChannel flags to be
                applied to @ref IFlexRayRxFrame::GetChannel.
                The filter will report it in @ref GetChannelMask.
        @param[in]  channelValue  
                Any combination of @ref FlexRayChannel flags to be
                applied to @ref IFlexRayRxFrame::GetChannel.
                The filter will report it in @ref GetChannelValue.
        @param[in]  slotMask
                Any mask for the slot in which the frame was received.
                The filter will report it in @ref GetSlotMask.
        @param[in]  slotValue
                Any value for the slot in which the frame was received.
                The filter will report it in @ref GetSlotValue.
        @param[in]  cycleNumberMask
                Any mask for the cycle in which the frame was received.
                The filter will report it in @ref GetCycleNumberMask.
        @param[in]  cycleNumberValue
                Any value for the cycle in which the frame was received.
                The filter will report it in @ref GetCycleNumberValue.
        @param[in]  payloadLengthMask
                Any mask for the length of the frame's payload.
                The filter will report it in @ref GetPayloadLengthMask.
        @param[in]  payloadLengthValue
                Any mask for the length of the frame's payload.
                The filter will report it in @ref GetPayloadLengthValue.
        @param[in]  frameTypeMask   
                Any combination of @ref FlexRayFrameType flags to be
                applied to @ref IFlexRayRxFrame::GetFrameType.
                The filter will report it in @ref GetFrameTypeMask.
        @param[in]  frameTypeValue  
                Any combination of @ref FlexRayFrameType flags to be
                applied to @ref IFlexRayRxFrame::GetFrameType.
                The filter will report it in @ref GetFrameTypeValue.
        @param[in]  payloadConditions                
                Conditions on the payload of the frame (see @ref IFrameFilter
                for details). The conditions will be accessible through
                @ref IFrameFilter::GetCondition. May be NULL only if
                @ref payloadConditionsCount is 0.
        @param[in]  payloadConditionsCount               
                Number of conditions on the payload, i.e. number of valid
                elements in @ref payloadConditions. May be 0.
                The filter will report it in @ref IFrameFilter::GetCount.
        @return New @ref IFlexRayFrameFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref IFlexRayFrameFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                IFlexRayFrameFilter, IFrameFilter
     */
    static AutoPtr<IFlexRayFrameFilter> OLI_CALL 
    Create ( uint32                     channelMask
           , uint32                     channelValue
           , uint32                     slotMask
           , uint32                     slotValue
           , uint32                     cycleNumberMask
           , uint32                     cycleNumberValue
           , uint32                     payloadLengthMask
           , uint32                     payloadLengthValue
           , uint32                     frameTypeMask
           , uint32                     frameTypeValue
           , const PayloadCondition*    payloadConditions
           , uint32                     payloadConditionsCount )
    {
        IFlexRayFrameFilter* pFlexRayFrameFilter = NULL;
        CheckAndThrow( IFlexRayFrameFilter_Create( channelMask, 
                                                    channelValue, 
                                                    slotMask, 
                                                    slotValue, 
                                                    cycleNumberMask, 
                                                    cycleNumberValue, 
                                                    payloadLengthMask, 
                                                    payloadLengthValue, 
                                                    frameTypeMask, 
                                                    frameTypeValue, 
                                                    payloadConditions, 
                                                    payloadConditionsCount, 
                                                    &pFlexRayFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.

        return AutoPtr<IFlexRayFrameFilter>( pFlexRayFrameFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_FLEXRAYFRAMEFILTER_INCLUDED__)
