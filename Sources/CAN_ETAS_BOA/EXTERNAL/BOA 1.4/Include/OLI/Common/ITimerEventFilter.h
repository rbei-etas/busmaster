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
* @brief  ITimerEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ITIMEREVENTFILTER_INCLUDED__)
#define __OLI_ITIMEREVENTFILTER_INCLUDED__

// include used interface

#include "AutoPtr.h"
#include "IEventFilter.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

class ITimerEventFilter;

/**
* @ingroup GROUP_OLI_COMMON_FILTERS
* @brief This function instantiates an object supporting 
*        @ref ITimerEventFilter. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref ITimerEventFilter::Create().
*
* @param[in]  eventCodeMask   
*         Any combination of @ref TimerEventCode flags.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  eventCodeValue  
*         Any combination of @ref TimerEventCode flags.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[out] ppTimerEventFilter   
*         A pointer to an object supporting @ref ITimerEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ITimerEventFilter_Create() 
*         via the wrapper @ref ITimerEventFilter::Create().
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
*         ITimerEventFilter
*/
OLL_API IError* OLI_CALL ITimerEventFilter_Create( 
    uint32 eventCodeMask, 
    uint32 eventCodeValue, 
    ITimerEventFilter** ppTimerEventFilter );

/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ITimerEvent -specific members.
*
* This interface adds no public methods except for a static
* @ref Create method. The implementation, however, is expected 
* to match @ref ITimerEvent instances only.
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the event code mask and
* event code value supplied to ITimerEventFilter::Create.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ITimerEvent
*/

OLI_INTERFACE ITimerEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ITimerEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Create an @ref ITimerEventFilter instance.

                The instance returned here will only match @ref ITimerEvent 
                messages. All other types will not pass the filter.

        @param[in]  eventCodeMask   
                Any combination of @ref TimerEventCode flags.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  eventCodeValue  
                Any combination of @ref TimerEventCode flags.
                The filter will report it in @ref IFilter::GetIDValue.
        @return New @ref ITimerEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ITimerEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ITimerEventFilter
     */
    static AutoPtr<ITimerEventFilter> OLI_CALL 
    Create( uint32 eventCodeMask, uint32 eventCodeValue )
    {
        ITimerEventFilter* pTimerEventFilter = NULL;
        CheckAndThrow( ITimerEventFilter_Create( eventCodeMask, 
                                                 eventCodeValue, 
                                                 &pTimerEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ITimerEventFilter>( pTimerEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ITIMEREVENTFILER_INCLUDED__)
