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
* @brief  ILINSleepEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINSLEEPEVENTFILTER_INCLUDED__)
#define __OLI_ILINSLEEPEVENTFILTER_INCLUDED__

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

class ILINSleepEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINSleepEventFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINSleepEventFilter::Create().
*
* @param[in]  sleepMask
*         Any combination of @ref LINSleepMode flags to be
*         applied to @ref ILINSleepEvent::GetMode.
*         The filter will report it in 
*         @ref ILINSleepEventFilter::GetSleepMask.
* @param[in]  sleepValue
*         Any combination of @ref LINSleepMode flags to be
*         applied to @ref ILINSleepEvent::GetMode.
*         The filter will report it in 
*         @ref ILINSleepEventFilter::GetSleepValue.
* @param[out] ppLinSleepEventFilter   
*         A pointer to an object supporting @ref ILINSleepEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINSleepEventFilter_Create() 
*         via the wrapper @ref ILINSleepEventFilter::Create().
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
*         ILINSleepEventFilter
*/
OLL_API IError* OLI_CALL ILINSleepEventFilter_Create( 
    uint8 sleepMask, 
    uint8 sleepValue, 
    ILINSleepEventFilter** ppLinSleepEventFilter);


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINSleepEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref ILINSleepEvent::GetMode "new sleep mode".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue always return 0.
*
* The implementation is expected to match @ref ILINSleepEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINSleepEvent
*/

OLI_INTERFACE ILINSleepEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINSleepEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the event's
                @ref ILINSleepEvent::GetMode "sleep mode".

        @return @ref filterConcepts "filter mask" for the event's 
                @ref ILINSleepEvent::GetMode "sleep mode".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINSleepEvent
     */
    virtual uint8 OLI_CALL GetSleepMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's
                @ref ILINSleepEvent::GetMode "sleep mode".

        @return @ref filterConcepts "filter value" for the event's
                @ref ILINSleepEvent::GetMode "sleep mode".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINSleepEvent
     */
	virtual uint8 OLI_CALL GetSleepValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINCRCDetectEventFilter instance.

                The instance returned here will only match 
                @ref ILINSleepEvent messages. All other
                types will not pass the filter.

        @param[in]  sleepMask
                Any combination of @ref LINSleepMode flags to be
                applied to @ref ILINSleepEvent::GetMode.
                The filter will report it in @ref GetSleepMask.
        @param[in]  sleepValue
                Any combination of @ref LINSleepMode flags to be
                applied to @ref ILINSleepEvent::GetMode.
                The filter will report it in @ref GetSleepValue.
        @return New @ref ILINSleepEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINSleepEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINSleepEvent
     */
    static AutoPtr<ILINSleepEventFilter> OLI_CALL 
    Create ( uint8 sleepMask
           , uint8 sleepValue )
    {
        ILINSleepEventFilter* pLinSleepEventFilter = NULL;
        CheckAndThrow( 
            ILINSleepEventFilter_Create( sleepMask, 
                                         sleepValue, 
                                         &pLinSleepEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ILINSleepEventFilter>( pLinSleepEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINSLEEPEVENTFILTER_INCLUDED__)
