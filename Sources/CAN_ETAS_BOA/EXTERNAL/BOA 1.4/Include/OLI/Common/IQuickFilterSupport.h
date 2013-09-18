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
* @brief  IQuickFilterSupport definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IQUICKFILTERSUPPORT_INCLUDED__)
#define __OLI_IQUICKFILTERSUPPORT_INCLUDED__

#include "OLIDefs.h"
#include "OLITypes.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Auxilliary interface for efficient message filtering.
*
* The information provided through this interface can be
* used by the OLL-internal filter mechanisms to pre-filter
* most imcoming data. If the client application has a high
* filter ratio, i.e. any received message gets added to only
* very few of many active queues, the pre-filtering will 
* drastically improve performance. 
*
* To allow that, this interface describes static filter properties. 
* The most basic information a message carries is its 
* @ref IMessage::GetType "type" and @ref IMessage::GetID "ID".
* Since @ref IFilter already tests for the ID, we add only
* the @ref GetTypeMask "type filter" here. Their combination
* will already allow for a highly selective pre-filtering.
*
* To make this work, all information returned by this interface
* plus the @ref IFilter::GetIDMask and @ref IFilter::GetIDValue
* methods must return values that will not change over the
* filter's life-time.
*
* User-defined filter classes will generally need to apply
* additional conditions. In that case, @ref RequiresPostFiltering
* must return @a true. Otherwise, messages passing the pre-filter
* will be put into the queue with no further filtering - bypassing 
* the filter object entierly.
*
* All filter classes shipped with OLI support this interface.
* User-defined filters should do this interface as well.
*
* @remark All public methods are thread-safe.
* @remark An object implementing this interface must remain valid 
*         as long as the @ref IFilter instance returning it through
*         @ref IFilter::GetQuickFilterSupport remains valid.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", IFilter, IRxMessage
*/

OLI_INTERFACE IQuickFilterSupport
{
protected:

    /** @brief  Destructor.

        This destructor has been hidden since this interface is
        considered a part of respective @ref IFilter instance.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IFilter
     */
    virtual ~IQuickFilterSupport() OLI_NOTHROW {};

public:

    /** @brief  Returns whether additional conditions must be checked.

                In most cases, filtering for @ref IMessage::GetType "type"
                and @ref IMessage::GetID "ID" will not be the only
                conditions defined in the respective filter instance.
                Only if indicated by this method, will the 
                @ref IFilter::Matches method be called and perform
                arbitrary tests.

        @return @a true, if the respective filter needs to do more tests.
        @exception <none> This function must not throw exceptions.

        @remark Must be invariant for this instance.
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts"
     */
    virtual bool OLI_CALL RequiresPostFiltering() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the message
                @ref IMessage::GetType "type".

        @return @ref filterConcepts "filter mask" for the message 
                @ref IMessage::GetType "type".
        @exception <none> This function must not throw exceptions.

        @remark Must be invariant for this instance.
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IRxMessage
     */
	virtual uint32 OLI_CALL GetTypeMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the message
                @ref IMessage::GetType "type".

        @return @ref filterConcepts "filter value" for the message 
                @ref IMessage::GetType "type".
        @exception <none> This function must not throw exceptions.

        @remark Must be invariant for this instance.
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IRxMessage
     */
    virtual uint32 OLI_CALL GetTypeValue() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IQUICKFILTERSUPPORT_INCLUDED__)
