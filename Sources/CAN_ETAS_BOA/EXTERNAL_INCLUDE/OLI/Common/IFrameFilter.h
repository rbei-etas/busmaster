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
* @brief  IFrameFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IFRAMEFILTER_INCLUDED__)
#define __OLI_IFRAMEFILTER_INCLUDED__

// include base interface

#include "IFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Structure describing a single condition on a frame's payload.
*
* The message payload is interpreted as a little endian byte stream
* and the condition being applied is 
*
* @code
* @a value == @a mask & *(uint32 *)(frame.payload + @a offset).
* @endcode
*
* Data access beyond the @ref IRxFrame::GetSize "size" of the payload
* yields @c 0, i.e. will only match if the respective condition @c value
* is @c 0, too. @ref IFrameFilter "Frame filters" support an arbitrary 
* number of potentially overlapping and contradictory payload filter 
* conditions.
*
* @remark This is a POD.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", IFrameFilter
*/

struct PayloadCondition
{
    /** @brief  4-byte mask to be applied to the payload data stream
                starting at the specified @c offset.
        @since  BOA 1.3
        @see    @ref filterConcepts "Filter concepts", offset
     */
	uint32 mask;

    /** @brief  Offset within the payload data stream where the 
                condition shall be applied.
        @since  BOA 1.3
        @see    @ref filterConcepts "Filter concepts"
     */
	uint32 offset;

    /** @brief  4-byte value that the payload content must match
                after applying the @c mask.
        @since  BOA 1.3
        @see    @ref filterConcepts "Filter concepts", mask
     */
    uint32 value;
};

/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Base interface for all filters on @ref IRxFrame "received frames".
*
* This interface adds the abiltiy to apply an arbitrary number
* of @ref PayloadCondition conditions on a frame's 
* @ref IRxFrame::GetData "payload".
*
* @remark All public methods are thread-safe.
* @remark Payload filtering is relatively slow. You should specify a
*         frame IDs filter consition as well to benefit from fast
*         @ref IQuickFilterSupport "pre-filtering".
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    IRxFrame, PayloadCondition, IQuickFilterSupport
*/

OLI_INTERFACE IFrameFilter : public IFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFrameFilter() OLI_NOTHROW {};

    /** @brief  Provides access to the individual playload conditions.
        @anchor GetCondition_private
     
        @param[in] index
                Selects the payload condition. Valid range is 
                from 0 .. @ref GetCount() - 1.
        @param[out] ppCondition  
                The specified condition.

        @return A pointer to an interface based on @ref IError, describing 
                the error which occurred during this function. @c NULL if 
                no error occurred. See @ref ErrorReporting "error reporting"
                for more information on how errors are reported.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    @ref BinaryCompatibility "binary compatibility", 
                @ref ErrorReporting "error reporting",
                @ref GetCondition_public "GetCondition",
                GetCount
     */
    virtual IError* OLI_CALL GetCondition( uint32 index, const PayloadCondition** ppCondition ) const OLI_NOTHROW = 0;

public:

    /** @brief  Retun the number of payload conditions.
        
        @return Number of @ref PayloadCondition "payload conditions"
                applied by this filter instance, i.e. the valid range 
                for @a index in @ref GetCondition_public "GetCondition". 
                May be 0.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetCondition
     */
    virtual uint32 OLI_CALL GetCount() const OLI_NOTHROW = 0;

    /** @brief  Provides access to the individual playload conditions.
        @anchor GetCondition_public
     
        @param[in] index
                Selects the payload condition. Valid range is 
                from 0 .. @ref GetCount() - 1.

        @return Reference to the selected payload condition.
        @exception COutOfRangeError
                The specified @c index is >= @ref GetCount.

        @remark This is a helper method which wraps the internal
                @ref GetCondition_private "GetCondition()" method: 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    GetCount
     */
    const PayloadCondition& OLI_CALL GetCondition (uint32 index) const
    {
        const PayloadCondition* pCondition = NULL;
        CheckAndThrow( GetCondition( index, &pCondition ) );
        return *pCondition;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IFRAMEFILTER_INCLUDED__)
