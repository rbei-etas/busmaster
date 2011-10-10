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
* @brief  ILINWakeUpEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINWAKEUPEVENTFILTER_INCLUDED__)
#define __OLI_ILINWAKEUPEVENTFILTER_INCLUDED__

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

class ILINWakeUpEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINWakeUpEventFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINWakeUpEventFilter::Create().
*
* @param[out] ppLinWakeUpEventFilter   
*         A pointer to an object supporting @ref ILINWakeUpEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINWakeUpEventFilter_Create() 
*         via the wrapper @ref ILINWakeUpEventFilter::Create().
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
*         ILINWakeUpEventFilter
*/
OLL_API IError* OLI_CALL ILINWakeUpEventFilter_Create( 
    ILINWakeUpEventFilter** ppLinWakeUpEventFilter);


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINWakeUpEvent -specific members.
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue always return 0.
*
* The implementation is expected to match @ref ILINWakeUpEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINWakeUpEvent
*/

OLI_INTERFACE ILINWakeUpEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINWakeUpEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Create an @ref ILINWakeUpEventFilter instance.

                The instance returned here will only match 
                @ref ILINWakeUpEvent messages. All other
                types will not pass the filter.

        @return New @ref ILINWakeUpEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINWakeUpEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINWakeUpEvent
     */
    static AutoPtr<ILINWakeUpEventFilter> OLI_CALL Create()
    {
        ILINWakeUpEventFilter* pLinWakeUpEventFilter = NULL;
        CheckAndThrow( 
            ILINWakeUpEventFilter_Create( &pLinWakeUpEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ILINWakeUpEventFilter>( pLinWakeUpEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINWAKEUPEVENTFILTER_INCLUDED__)
