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
* @brief  ILINErrSyncEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINERRSYNCEVENTFILTER_INCLUDED__)
#define __OLI_ILINERRSYNCEVENTFILTER_INCLUDED__

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

class ILINErrSyncEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINErrSyncEventFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINErrSyncEventFilter::Create().
*
* @param[in]  baudrateMask
*         Any mask for the baudrate (DLC) to be
*         applied to @ref ILINErrSyncEvent::GetBaudrate.
*         The filter will report it in 
*         @ref ILINErrSyncEventFilter::GetBaudrateMask.
* @param[in]  baudrateValue
*         Any value for the baudrate to be
*         applied to @ref ILINErrSyncEvent::GetBaudrate.
*         The filter will report it in 
*         @ref ILINErrSyncEventFilter::GetBaudrateValue.
* @param[out] ppLinErrSyncEventFilter   
*         A pointer to an object supporting @ref ILINErrSyncEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINErrSyncEventFilter_Create() 
*         via the wrapper @ref ILINErrSyncEventFilter::Create().
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
*         ILINErrSyncEventFilter
*/
OLL_API IError* OLI_CALL ILINErrSyncEventFilter_Create( 
    uint32 baudrateMask, 
    uint32 baudrateValue, 
    ILINErrSyncEventFilter** ppLinErrSyncEventFilter);


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINErrSyncEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref ILINErrSyncEvent::GetBaudrate "baudrate".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue always return 0.
*
* The implementation is expected to match @ref ILINErrSyncEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINErrSyncEvent
*/

OLI_INTERFACE ILINErrSyncEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINErrSyncEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the event's
                @ref ILINErrSyncEvent::GetBaudrate "baudrate".

        @return @ref filterConcepts "filter mask" for the event's 
                @ref ILINErrSyncEvent::GetBaudrate "baudrate".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINErrSyncEvent
     */
    virtual uint32 OLI_CALL GetBaudrateMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's
                @ref ILINErrSyncEvent::GetBaudrate "baudrate".

        @return @ref filterConcepts "filter value" for the event's
                @ref ILINErrSyncEvent::GetBaudrate "baudrate".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINErrSyncEvent
     */
	virtual uint32 OLI_CALL GetBaudrateValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINErrSyncEventFilter instance.

                The instance returned here will only match 
                @ref ILINErrSyncEvent messages. All other
                types will not pass the filter.

        @param[in]  baudrateMask
                Any mask for the baudrate (DLC) to be
                applied to @ref ILINErrSyncEvent::GetBaudrate.
                The filter will report it in @ref GetBaudrateMask.
        @param[in]  baudrateValue
                Any value for the baudrate to be
                applied to @ref ILINErrSyncEvent::GetBaudrate.
                The filter will report it in @ref GetBaudrateValue.
        @return New @ref ILINErrSyncEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINErrSyncEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINErrSyncEvent
     */
    static AutoPtr<ILINErrSyncEventFilter> OLI_CALL 
    Create ( uint32 baudrateMask
           , uint32 baudrateValue )
    {
        ILINErrSyncEventFilter* pLinErrSyncEventFilter = NULL;
        CheckAndThrow( 
            ILINErrSyncEventFilter_Create( baudrateMask, 
                                           baudrateValue, 
                                           &pLinErrSyncEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ILINErrSyncEventFilter>( pLinErrSyncEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINERRSYNCEVENTFILTER_INCLUDED__)
