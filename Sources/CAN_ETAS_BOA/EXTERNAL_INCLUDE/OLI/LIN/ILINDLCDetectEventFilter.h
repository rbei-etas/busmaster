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
* @brief  ILINDLCDetectEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINDLCDETECTEVENTFILTER_INCLUDED__)
#define __OLI_ILINDLCDETECTEVENTFILTER_INCLUDED__

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

class ILINDLCDetectEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINDLCDetectEventFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINDLCDetectEventFilter::Create().
*
* @param[in]  frameIDMask   
*         Any mask for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  frameIDValue  
*         Any value for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  dlcMask
*         Any mask for the frame length (DLC) to be
*         applied to @ref ILINDLCDetectEvent::GetDLC.
*         The filter will report it in 
*         @ref ILINDLCDetectEventFilter::GetDLCMask.
* @param[in]  dlcValue
*         Any value for the frame length (DLC) to be
*         applied to @ref ILINDLCDetectEvent::GetDLC.
*         The filter will report it in 
*         @ref ILINDLCDetectEventFilter::GetDLCValue.
* @param[out] ppLinDLCDetectEventFilter   
*         A pointer to an object supporting @ref ILINDLCDetectEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINDLCDetectEventFilter_Create() 
*         via the wrapper @ref ILINDLCDetectEventFilter::Create().
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
*         ILINDLCDetectEventFilter
*/
OLL_API IError* OLI_CALL ILINDLCDetectEventFilter_Create( 
    uint8 frameIDMask, 
    uint8 frameIDValue, 
    uint8 dlcMask, 
    uint8 dlcValue, 
    ILINDLCDetectEventFilter** ppLinDlcDetectEventFilter);


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINDLCDetectEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref ILINDLCDetectEvent::GetDLC "deteced DLC".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the frame ID mask and
* frame ID value supplied to ILINDLCDetectEventFilter::Create.
*
* The implementation is expected to match @ref ILINDLCDetectEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINDLCDetectEvent
*/

OLI_INTERFACE ILINDLCDetectEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINDLCDetectEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the event's
                @ref ILINDLCDetectEvent::GetDLC "DLC value".

        @return @ref filterConcepts "filter mask" for the event's 
                @ref ILINDLCDetectEvent::GetDLC "DLC value".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINDLCDetectEvent
     */
    virtual uint8 OLI_CALL GetDLCMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's
                @ref ILINDLCDetectEvent::GetDLC "DLC value".

        @return @ref filterConcepts "filter value" for the event's
                @ref ILINDLCDetectEvent::GetDLC "DLC value".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINDLCDetectEvent
     */
	virtual uint8 OLI_CALL GetDLCValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINDLCDetectEventFilter instance.

                The instance returned here will only match 
                @ref ILINDLCDetectEvent messages. All other
                types will not pass the filter.

        @param[in]  frameIDMask   
                Any mask for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  frameIDValue  
                Any value for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  dlcMask
                Any mask for the frame length (DLC) to be
                applied to @ref ILINDLCDetectEvent::GetDLC.
                The filter will report it in @ref GetDLCMask.
        @param[in]  dlcValue
                Any value for the frame length (DLC) to be
                applied to @ref ILINDLCDetectEvent::GetDLC.
                The filter will report it in @ref GetDLCValue.
        @return New @ref ILINDLCDetectEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINDLCDetectEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINDLCDetectEvent
     */
    static AutoPtr<ILINDLCDetectEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint8 dlcMask
           , uint8 dlcValue )
    {
        ILINDLCDetectEventFilter* pLinDlcDetectEventFilter = NULL;
        CheckAndThrow( 
            ILINDLCDetectEventFilter_Create( frameIDMask, 
                                             frameIDValue, 
                                             dlcMask, 
                                             dlcValue, 
                                             &pLinDlcDetectEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ILINDLCDetectEventFilter>( pLinDlcDetectEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINDLCDETECTEVENTFILTER_INCLUDED__)
