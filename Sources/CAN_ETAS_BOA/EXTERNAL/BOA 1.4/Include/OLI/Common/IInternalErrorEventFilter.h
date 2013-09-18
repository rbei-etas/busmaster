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
* @brief  IInternalErrorEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IINTERNALERROREVENTFILER_INCLUDED__)
#define __OLI_IINTERNALERROREVENTFILER_INCLUDED__

// include used interface

#include "AutoPtr.h"
#include "IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class IInternalErrorEventFilter;

/**
* @ingroup GROUP_OLI_COMMON_FILTERS
* @brief This function instantiates an object supporting 
*        @ref IInternalErrorEventFilter. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref IInternalErrorEventFilter::Create().
*
* @param[in]  eventCodeMask   
*         Any combination of @ref InternalErrorEventCode flags.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  eventCodeValue  
*         Any combination of @ref InternalErrorEventCode flags.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  errorCodeMask
*         Any mask for @ref IInternalErrorEvent::GetErrorCode.
*         The filter will report it in 
*         @ref IInternalErrorEventFilter::GetErrorCodeMask.
* @param[in]  errorCodeValue
*         Any value for @ref IInternalErrorEvent::GetErrorCode.
*         The filter will report it in 
*         @ref IInternalErrorEventFilter::GetErrorCodeValue.
* @param[out] ppInternalErrorEventFilter   
*         A pointer to an object supporting @ref IInternalErrorEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref IInternalErrorEventFilter_Create() 
*         via the wrapper @ref IInternalErrorEventFilter::Create().
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
*         IInternalErrorEventFilter
*/
OLL_API IError* OLI_CALL IInternalErrorEventFilter_Create( 
    uint32 eventCodeMask, 
    uint32 eventCodeValue, 
    uint32 errorCodeMask, 
    uint32 errorCodeValue, 
    IInternalErrorEventFilter** ppInternalErrorEventFilter);


/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref IInternalErrorEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref IInternalErrorEvent::GetErrorCode "error code".
* The implementation is expected to match @ref IInternalErrorEvent 
* instances only.
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the event code mask and
* event code value supplied to IInternalErrorEventFilter::Create.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", IInternalErrorEvent
*/

OLI_INTERFACE IInternalErrorEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IInternalErrorEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the event
                @ref IInternalErrorEvent::GetErrorCode "error code".

        @return @ref filterConcepts "filter mask" for the event 
                @ref IInternalErrorEvent::GetErrorCode "error code".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IInternalErrorEvent
     */
    virtual uint32 OLI_CALL GetErrorCodeMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event
                @ref IInternalErrorEvent::GetErrorCode "error code".

        @return @ref filterConcepts "filter value" for the event 
                @ref IInternalErrorEvent::GetErrorCode "error code".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IInternalErrorEvent
     */
    virtual uint32 OLI_CALL GetErrorCodeValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IInternalErrorEventFilter instance.

                The instance returned here will only match 
                @ref IInternalErrorEvent messages. All other
                types will not pass the filter.

        @param[in]  eventCodeMask   
                Any combination of @ref InternalErrorEventCode flags.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  eventCodeValue  
                Any combination of @ref InternalErrorEventCode flags.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  errorCodeMask
                Any mask for @ref IInternalErrorEvent::GetErrorCode.
                The filter will report it in @ref GetErrorCodeMask.
        @param[in]  errorCodeValue
                Any value for @ref IInternalErrorEvent::GetErrorCode.
                The filter will report it in @ref GetErrorCodeValue.
        @return New @ref IInternalErrorEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref IInternalErrorEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", IInternalErrorEvent
     */
    static AutoPtr<IInternalErrorEventFilter> OLI_CALL Create(
        uint32 eventCodeMask,
        uint32 eventCodeValue,
        uint32 errorCodeMask,
        uint32 errorCodeValue )
    {
        IInternalErrorEventFilter* pInternalErrorEventFilter = NULL;
        CheckAndThrow( 
            IInternalErrorEventFilter_Create( eventCodeMask, 
                                              eventCodeValue, 
                                              errorCodeMask, 
                                              errorCodeValue, 
                                              &pInternalErrorEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<IInternalErrorEventFilter>( pInternalErrorEventFilter, 
                                                   false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IINTERNALERROREVENTFILER_INCLUDED__)
