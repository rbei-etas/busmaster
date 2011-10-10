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
* @brief  ICANErrorFrameFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANERRORFRAMEFILTER_INCLUDED__)
#define __OLI_ICANERRORFRAMEFILTER_INCLUDED__

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

class ICANErrorFrameFilter;

/**
* @ingroup GROUP_OLI_CAN_FILTERS
* @brief This function instantiates an object supporting 
*        @ref ICANErrorFrameFilter. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref ICANErrorFrameFilter::Create().
*
* @param[in]  eventCodeMask   
*         Any combination of @ref CANErrorFrame flags.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  eventCodeValue  
*         Any combination of @ref CANErrorFrame flags.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[out] ppCanErrorFrameFilter   
*         A pointer to an object supporting @ref ICANErrorFrameFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ICANErrorFrameFilter_Create() 
*         via the wrapper @ref ICANErrorFrameFilter::Create().
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
*         ICANErrorFrameFilter
*/
OLL_API IError* OLI_CALL ICANErrorFrameFilter_Create( 
    uint32 eventCodeMask, 
    uint32 eventCodeValue, 
    ICANErrorFrameFilter** ppCanErrorFrameFilter);


/** @ingroup GROUP_OLI_CAN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ICANErrorFrame -specific members.
*
* The implementation is expected to match @ref ICANErrorFrame 
* instances only.
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the event code mask
* and event code value supplied to ICANErrorFrameFilter::Create.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ICANErrorFrame
*/

OLI_INTERFACE ICANErrorFrameFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ICANErrorFrameFilter() OLI_NOTHROW {};

public:

    /** @brief  Create an @ref ICANErrorFrameFilter instance.

                The instance returned here will only match 
                @ref ICANErrorFrame messages. All other
                types will not pass the filter.

        @param[in]  eventCodeMask   
                Any combination of @ref CANErrorFrame flags.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  eventCodeValue  
                Any combination of @ref CANErrorFrame flags.
                The filter will report it in @ref IFilter::GetIDValue.
        @return New @ref ICANErrorFrameFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ICANErrorFrameFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ICANErrorFrame
     */
    static AutoPtr<ICANErrorFrameFilter> OLI_CALL Create( 
        uint32 eventCodeMask, 
        uint32 eventCodeValue)
    {
        ICANErrorFrameFilter* pCanErrorFrameFilter = NULL;
        CheckAndThrow( ICANErrorFrameFilter_Create( eventCodeMask, 
                                                    eventCodeValue, 
                                                    &pCanErrorFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ICANErrorFrameFilter>( pCanErrorFrameFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANERRORFRAMEFILTER_INCLUDED__)
