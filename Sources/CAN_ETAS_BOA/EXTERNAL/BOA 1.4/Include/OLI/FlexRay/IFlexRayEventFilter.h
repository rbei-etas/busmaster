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
* @brief  IFlexRayEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IFLEXRAYEVENTFILTER_INCLUDED__)
#define __OLI_IFLEXRAYEVENTFILTER_INCLUDED__

// include used interface

#include "FlexRayBase.h"

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class IFlexRayEventFilter;

/**
* @ingroup GROUP_OLI_FLEXRAY_FILTERS
* @brief This function instantiates an object supporting 
*        @ref IFlexRayEventFilter. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref IFlexRayEventFilter::Create().
*
* @param[in]  eventCodeMask   
*         Any combination of @ref FlexRayEventCode flags.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  eventCodeValue  
*         Any combination of @ref FlexRayEventCode flags.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  protocolOperationControlStatusMask
*         Any combination of @ref 
*         FlexRayControllerProtocolOperationControlStatus flags to be
*         applied to @ref IFlexRayEvent::GetProtocolOperationControlStatus.
*         The filter will report it in 
*         @ref IFlexRayEventFilter::GetProtocolOperationControlStatusMask.
* @param[in]  protocolOperationControlStatusValue
*         Any combination of @ref 
*         FlexRayControllerProtocolOperationControlStatus flags to be
*         applied to @ref IFlexRayEvent::GetProtocolOperationControlStatus.
*         The filter will report it in 
*         @ref IFlexRayEventFilter::GetProtocolOperationControlStatusValue.
* @param[out] ppFlexRayEventFilter   
*         A pointer to an object supporting @ref IFlexRayEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref IFlexRayEventFilter_Create() 
*         via the wrapper @ref IFlexRayEventFilter::Create().
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
*         IFlexRayEventFilter
*/
OLL_API IError* OLI_CALL IFlexRayEventFilter_Create( 
    uint32 eventCodeMask, 
    uint32 eventCodeValue, 
    uint32 protocolOperationControlStatusMask, 
    uint32 protocolOperationControlStatusValue, 
    IFlexRayEventFilter** ppFlexRayEventFilter);


/** @ingroup GROUP_OLI_FLEXRAY_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref IFlexRayEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref IFlexRayEvent::GetProtocolOperationControlStatus 
* "protocol operation control status".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the event code mask and
* event code value supplied to IFlexRayEventFilter::Create.
*
* The implementation is expected to match @ref IFlexRayEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", IFlexRayEvent
*/

OLI_INTERFACE IFlexRayEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFlexRayEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the event
                @ref IFlexRayEvent::GetProtocolOperationControlStatus 
                "protocol operation control status".

        @return @ref filterConcepts "filter mask" for the event 
                @ref IFlexRayEvent::GetProtocolOperationControlStatus 
                "protocol operation control status".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayEvent
     */
    virtual uint32 OLI_CALL 
        GetProtocolOperationControlStatusMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event
                @ref IFlexRayEvent::GetProtocolOperationControlStatus 
                "protocol operation control status".

        @return @ref filterConcepts "filter value" for the event 
                @ref IFlexRayEvent::GetProtocolOperationControlStatus 
                "protocol operation control status".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayEvent
     */
    virtual uint32 OLI_CALL 
        GetProtocolOperationControlStatusValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IFlexRayEventFilter instance.

                The instance returned here will only match 
                @ref IFlexRayEvent messages. All other
                types will not pass the filter.

        @param[in]  eventCodeMask   
                Any combination of @ref FlexRayEventCode flags.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  eventCodeValue  
                Any combination of @ref FlexRayEventCode flags.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  protocolOperationControlStatusMask
                Any combination of @ref 
                FlexRayControllerProtocolOperationControlStatus flags to be
                applied to @ref IFlexRayEvent::GetProtocolOperationControlStatus.
                The filter will report it in 
                @ref GetProtocolOperationControlStatusMask.
        @param[in]  protocolOperationControlStatusValue
                Any combination of @ref 
                FlexRayControllerProtocolOperationControlStatus flags to be
                applied to @ref IFlexRayEvent::GetProtocolOperationControlStatus.
                The filter will report it in 
                @ref GetProtocolOperationControlStatusValue.
        @return New @ref IFlexRayEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref IFlexRayEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IFlexRayEvent
     */
    static AutoPtr<IFlexRayEventFilter> OLI_CALL 
    Create ( uint32 eventCodeMask
           , uint32 eventCodeValue
           , uint32 protocolOperationControlStatusMask
           , uint32 protocolOperationControlStatusValue )
    {
        IFlexRayEventFilter* pFlexRayEventFilter = NULL;
        CheckAndThrow( 
            IFlexRayEventFilter_Create( eventCodeMask, 
                                        eventCodeValue, 
                                        protocolOperationControlStatusMask, 
                                        protocolOperationControlStatusValue, 
                                        &pFlexRayEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<IFlexRayEventFilter>( pFlexRayEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYEVENTFILTER_INCLUDED__)
