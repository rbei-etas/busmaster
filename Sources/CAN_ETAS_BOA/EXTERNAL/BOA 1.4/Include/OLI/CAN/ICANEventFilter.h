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
* @brief  ICANEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANEVENTFILTER_INCLUDED__)
#define __OLI_ICANEVENTFILTER_INCLUDED__

// include used interface

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

class ICANEventFilter;

/**
* @ingroup GROUP_OLI_CAN_FILTERS
* @brief This function instantiates an object supporting 
*        @ref ICANEventFilter. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref ICANEventFilter::Create().
*
* @param[in]  eventCodeMask   
*         Any combination of @ref CANBusState flags.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  eventCodeValue  
*         Any combination of @ref CANBusState flags.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[out] ppCanEventFilter   
*         A pointer to an object supporting @ref ICANEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ICANEventFilter_Create() 
*         via the wrapper @ref ICANEventFilter::Create().
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
*         ICANEventFilter
*/
OLL_API IError* OLI_CALL ICANEventFilter_Create( 
    uint32 eventCodeMask, 
    uint32 eventCodeValue, 
    ICANEventFilter** ppCanEventFilter);


/** @ingroup GROUP_OLI_CAN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ICANEvent -specific members.
*
* The implementation is expected to match @ref ICANEvent 
* instances only.
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the event code mask
* and event code value supplied to ICANEventFilter::Create.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ICANEvent
*/

OLI_INTERFACE ICANEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ICANEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Create an @ref ICANEventFilter instance.

                The instance returned here will only match 
                @ref ICANEvent messages. All other
                types will not pass the filter.

        @param[in]  eventCodeMask   
                Any combination of @ref CANBusState flags.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  eventCodeValue  
                Any combination of @ref CANBusState flags.
                The filter will report it in @ref IFilter::GetIDValue.
        @return New @ref ICANEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ICANEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ICANEvent
     */
    static AutoPtr<ICANEventFilter> OLI_CALL 
    Create( uint32 eventCodeMask, uint32 eventCodeValue )
    {
        ICANEventFilter* pCanEventFilter = NULL;
        CheckAndThrow( ICANEventFilter_Create( eventCodeMask, 
                                               eventCodeValue, 
                                               &pCanEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ICANEventFilter>( pCanEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANEVENTFILTER_INCLUDED__)
