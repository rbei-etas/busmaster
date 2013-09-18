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
* @brief  ILINCRCDetectEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINCRCDETECTEVENTFILTER_INCLUDED__)
#define __OLI_ILINCRCDETECTEVENTFILTER_INCLUDED__

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

class ILINCRCDetectEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINCRCDetectEventFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINCRCDetectEventFilter::Create().
*
* @param[in]  frameIDMask   
*         Any mask for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  frameIDValue  
*         Any value for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  crcMask
*         Any combination of @ref LINCRCType flags to be
*         applied to @ref ILINCRCDetectEvent::GetCRCType.
*         The filter will report it in 
*         @ref ILINCRCDetectEventFilter::GetCRCTypeMask.
* @param[in]  crcValue
*         Any combination of @ref LINCRCType flags to be
*         applied to @ref ILINCRCDetectEvent::GetCRCType.
*         The filter will report it in 
*         @ref ILINCRCDetectEventFilter::GetCRCTypeValue.
* @param[out] ppLinCrcDetectEventFilter   
*         A pointer to an object supporting @ref ILINCRCDetectEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINCRCDetectEventFilter_Create() 
*         via the wrapper @ref ILINCRCDetectEventFilter::Create().
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
*         ILINCRCDetectEventFilter
*/
OLL_API IError* OLI_CALL ILINCRCDetectEventFilter_Create( 
    uint8 frameIDMask, 
    uint8 frameIDValue, 
    uint8 crcMask, 
    uint8 crcValue, 
    ILINCRCDetectEventFilter** ppLinCrcDetectEventFilter);


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINCRCDetectEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref ILINCRCDetectEvent::GetCRCType "CRC type".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the frame ID mask and
* frame ID value supplied to ILINCRCDetectEventFilter::Create.
*
* The implementation is expected to match @ref ILINCRCDetectEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINCRCDetectEvent
*/

OLI_INTERFACE ILINCRCDetectEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINCRCDetectEventFilter() OLI_NOTHROW{};

public:

    /** @brief  Returns the mask for the condition on the event's
                @ref ILINCRCDetectEvent::GetCRCType "CRC type".

        @return @ref filterConcepts "filter mask" for the event's 
                @ref ILINCRCDetectEvent::GetCRCType "CRC type".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINCRCDetectEvent
     */
    virtual uint8 OLI_CALL GetCRCTypeMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's
                @ref ILINCRCDetectEvent::GetCRCType "CRC type".

        @return @ref filterConcepts "filter value" for the event's
                @ref ILINCRCDetectEvent::GetCRCType "CRC type".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINCRCDetectEvent
     */
    virtual uint8 OLI_CALL GetCRCTypeValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINCRCDetectEventFilter instance.

                The instance returned here will only match 
                @ref ILINCRCDetectEvent messages. All other
                types will not pass the filter.

        @param[in]  frameIDMask   
                Any mask for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  frameIDValue  
                Any value for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  crcMask
                Any combination of @ref LINCRCType flags to be
                applied to @ref ILINCRCDetectEvent::GetCRCType.
                The filter will report it in @ref GetCRCTypeMask.
        @param[in]  crcValue
                Any combination of @ref LINCRCType flags to be
                applied to @ref ILINCRCDetectEvent::GetCRCType.
                The filter will report it in @ref GetCRCTypeValue.
        @return New @ref ILINCRCDetectEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINCRCDetectEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINCRCDetectEvent
     */
    static AutoPtr<ILINCRCDetectEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint8 crcMask
           , uint8 crcValue )
    {
        ILINCRCDetectEventFilter* pLINCRCDetectEventFilter = NULL;
        CheckAndThrow( 
            ILINCRCDetectEventFilter_Create( frameIDMask, 
                                             frameIDValue, 
                                             crcMask, 
                                             crcValue, 
                                             &pLINCRCDetectEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ILINCRCDetectEventFilter>( pLINCRCDetectEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINCRCDETECTEVENTFILTER_INCLUDED__)
