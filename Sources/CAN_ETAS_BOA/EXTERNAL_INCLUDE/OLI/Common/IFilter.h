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
* @brief  IFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IFILTER_INCLUDED__)
#define __OLI_IFILTER_INCLUDED__

#include "IRefCountable.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @page filterConcepts Filter Concepts
*
* The OLI uses a white filter approach. Per default, no data
* will pass and at least one matching filter must be found
* to make the respective message pass.
*
* All standard filter implementations provided with OLI
* are stateless: Each filter is usually a combination of 
* conditions on the message data wich must all be met,
* e.g. one condition on the frame ID and one on the
* frame content.
*
* Every condition follows the same basic pattern:
* @code
* message.VALUE & filter.MASK == filter.VALUE
* @endcode
*
* To let all frames pass through a filter set all conditions to 
* @code
* filter.MASK = 0
* filter.VALUE = 0 
* @endcode
*
* To filter for an exact match use 
* @code
* MASK = 0xffffffff
* VALUE = required value.
* @endcode
*
* To filter a subset of flags in a bitfield, i.e. to
* block all if any @em other flag is set, use 
* @code
* MASK = ~(PASSED_FLAG_A|PASSED_FLAG_B|PASSED_FLAG_C) 
* VALUE = 0
* @endcode
*/

// interface forward declarations

class IRxMessage;
class IQuickFilterSupport;

/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Base interface for all @ref IRxMessage "message" filters. 
*
* This base interface is implemented by any object which is able to filter instances of @ref IRxMessage, usually
* in order to decide whether such an instance should be  added to a @ref IRxQueue "receive queue".
* An object implementing @ref IFilter encapsulates a criterion which it applies to the  @ref IMessage::GetID "ID"
* of any instance of @ref IRxMessage. The criterion takes the form of a (mask, value) pair, as described in
* @ref filterConcepts "Filter concepts". During the method @ref IFilter::Matches(), the implementation must apply
* the criterion to the supplied instance of @ref IRxMessage.
*
* The documentation for IMessage discusses how, in some cases, an object which implements a subclass of IMessage
* may not have a property which can easily be interpreted as an "ID". As a result, the various subclasses of
* IMessage implement @ref IMessage::GetID in various ways. This also affects the corresponding subclasses of
* IFilter. For example, if IQueueEvent implements IMessage::GetID by returning an event code, users of
* IQueueEventFilter must supply an event code mask and value when they construct an instance of IQueueEventFilter,
* and IQueueEventFilter's implementation of IFilter::GetIDMask and IFilter::GetIDValue will return that event code
* mask and value. Please refer to the documentation of the various subclasses of IFilter for information about how
* each implements IFilter::GetIDMask and IFilter::GetIDValue.
*
* Clearly it may be necessary to construct a filter which tests more than the @ref IMessage::GetID "ID" of an
* instance of @ref IRxMessage. An interface derived from @ref IFilter can add other filter criteria specific to
* a particular subclass of @ref IRxMessage. Then, during the method @ref IFilter::Matches(), the derived filter
* can use type-specific criteria (plus run-time typing), as well as the "ID" criterion described above, to filter
* the supplied instance of @ref IRxMessage.
*
* Although the OLI ships with a number of pre-defined implementations of interfaces derived from IFilter, client
* applications are free to implement IFilter themselves, and perhaps to define their own derived filter interfaces.
* Nevertheless, IFilter is the only interface which a client @em must implement to make their filters work within
* the OLI.
*
* Messages will only be added to the receive queue, if at least
* one of the filters @ref IRxQueue::AddFilter "added" to that
* queue @ref Matches "matches". Because the order in which filters
* get probed is undefined, there is no guarantee that the
* @ref Matches method of any particular filter instance gets
* called.
*
* Any kind of stateless or stateful filter implementation is allowed.
* For maximum efficiency, filters should also support 
* @ref IQuickFilterSupport "quick filtering". Return a pointer to
* that interface in you implementation of @ref GetQuickFilterSupport.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", 
*         IRxQueue, IRxMessage, IQuickFilterSupport
*/

OLI_INTERFACE IFilter : public IRefCountable
{
protected:

    /** @brief  Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the @ref IQuickFilterSupport interface, if
                quick filtering is supported by this filter instance.

        Client applications implementing @ref IFilter are encouraged
        to implement @ref IQuickFilterSupport as well and not to
        return NULL in this method.
        
        @return Pointer to the @ref IQuickFilterSupport implementation
                or NULL, if not supported.
        @exception <none> This function must not throw exceptions.

        @remark The life-time of the reference returned must be
                no less than the life-time of this filter instance. 
        @remark This method is necessary because OLI does not support 
                multiple inheritance. Client applications may use multiple
                inheritance internally to implement that second interface.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    IQuickFilterSupport
     */
    virtual const IQuickFilterSupport* OLI_CALL GetQuickFilterSupport() const OLI_NOTHROW = 0;

    /** @brief  Returns whether the given @a message matches the
                filter condition.

        @return @a true, if the @a message satisfies all conditions
                of this filter instance.
        @exception <none> This function must not throw exceptions.

        @remark There is no guarantee that this function will be
                called at all. Other filters or the quick filtering 
                mechanism may already accepted the respective message.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IQuickFilterSupport
     */
	virtual bool OLI_CALL Matches (const IRxMessage* message) const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the message
                @ref IMessage::GetID "ID".

        @return @ref filterConcepts "filter mask" for the message 
                @ref IMessage::GetID "ID".
        @exception <none> This function must not throw exceptions.

        @remark Must be invariant for this instance if
                @ref IQuickFilterSupport "quick filtering" is
                supported.
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                GetQuickFilterSupport, IRxMessage
     */
	virtual uint32 OLI_CALL GetIDMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the message
                @ref IMessage::GetID "ID".

        @return @ref filterConcepts "filter value" for the message 
                @ref IMessage::GetID "ID".
        @exception <none> This function must not throw exceptions.

        @remark Must be invariant for this instance if
                @ref IQuickFilterSupport "quick filtering" is
                supported.
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                GetQuickFilterSupport, IRxMessage
     */
    virtual uint32 OLI_CALL GetIDValue() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IFILTER_INCLUDED__)
