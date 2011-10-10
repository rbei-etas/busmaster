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
* @brief  ILINTransmissionStatusEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINTRANSMISSIONSTATUSEVENTFILTER_INCLUDED__)
#define __OLI_ILINTRANSMISSIONSTATUSEVENTFILTER_INCLUDED__

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

class ILINTransmissionStatusEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINTransmissionStatusEventFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINTransmissionStatusEventFilter::Create().
*
* @param[in]  frameIDMask   
*         Any mask for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  frameIDValue  
*         Any value for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  directionMask
*         Any combination of @ref LINFrameDirection flags to be
*         applied to @ref ILINTransmissionStatusEvent::GetDirection.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetDirectionMask.
* @param[in]  directionValue           
*         Any combination of @ref LINFrameDirection flags to be
*         applied to @ref ILINTransmissionStatusEvent::GetDirection.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetDirectionValue.
* @param[in]  dlcMask
*         Any mask for the LIN frame DLC.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetDLCMask.
* @param[in]  dlcValue
*         Any value for the LIN frame DLC.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetDLCValue.
* @param[in]  crcTypeMask
*         Any combination of @ref LINCRCType flags to be
*         applied to @ref ILINTransmissionStatusEvent::GetCRCType.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetCRCTypeMask.
* @param[in]  crcTypeValue
*         Any combination of @ref LINCRCType flags to be
*         applied to @ref ILINTransmissionStatusEvent::GetCRCType.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetCRCTypeValue.
* @param[in]  stateMask                
*         Any combination of @ref LINTransmissionState flags to be
*         applied to @ref ILINTransmissionStatusEvent::GetState.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetStateMask.
* @param[in]  stateValue               
*         Any combination of @ref LINTransmissionState flags to be
*         applied to @ref ILINTransmissionStatusEvent::GetState.
*         The filter will report it in 
*         @ref ILINTransmissionStatusEventFilter::GetStateValue.
* @param[out] ppLinTxStatusEventFilter   
*         A pointer to an object supporting 
*         @ref ILINTransmissionStatusEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINTransmissionStatusEventFilter_Create() 
*         via the wrapper @ref ILINTransmissionStatusEventFilter::Create().
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
*         ILINTransmissionStatusEventFilter
*/
OLL_API IError* OLI_CALL ILINTransmissionStatusEventFilter_Create(
    uint8 frameIDMask,
    uint8 frameIDValue,
    uint8 directionMask,
    uint8 directionValue,
    uint8 dlcMask,
    uint8 dlcValue,
    uint8 crcTypeMask,
    uint8 crcTypeValue,
    uint32 stateMask,
    uint32 stateValue,
    ILINTransmissionStatusEventFilter** ppLinTxStatusEventFilter);


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINTransmissionStatusEvent -specific members.
*
* In addition to the base class conditions, this interface checks for the 
* frame's @ref ILINTransmissionStatusEvent::GetState "transmission state",
* @ref ILINTransmissionStatusEvent::GetDirection "direction", 
* @ref ILINTransmissionStatusEvent::GetDLC "payload length" and 
* @ref ILINTransmissionStatusEvent::GetCRCType "CRC type".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the frame ID mask and
* frame ID value supplied to ILINTransmissionStatusEventFilter::Create.
*
* The implementation is expected to match @ref ILINTransmissionStatusEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINTransmissionStatusEvent
*/

OLI_INTERFACE ILINTransmissionStatusEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINTransmissionStatusEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the event's @ref 
                ILINTransmissionStatusEvent::GetDirection "frame direction".

        @return @ref filterConcepts "filter mask" for the event's @ref 
                ILINTransmissionStatusEvent::GetDirection "frame direction".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint8 OLI_CALL GetDirectionMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's @ref 
                ILINTransmissionStatusEvent::GetDirection "frame direction".

        @return @ref filterConcepts "filter value" for the event's @ref 
                ILINTransmissionStatusEvent::GetDirection "frame direction".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint8 OLI_CALL GetDirectionValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the event's
                @ref ILINTransmissionStatusEvent::GetDLC "DLC value".

        @return @ref filterConcepts "filter mask" for the event's 
                @ref ILINTransmissionStatusEvent::GetDLC "DLC value".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint8 OLI_CALL GetDLCMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's
                @ref ILINTransmissionStatusEvent::GetDLC "DLC value".

        @return @ref filterConcepts "filter value" for the event's
                @ref ILINTransmissionStatusEvent::GetDLC "DLC value".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
	virtual uint8 OLI_CALL GetDLCValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the event's
                @ref ILINTransmissionStatusEvent::GetCRCType "CRC type".

        @return @ref filterConcepts "filter mask" for the event's 
                @ref ILINTransmissionStatusEvent::GetCRCType "CRC type".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint8 OLI_CALL GetCRCTypeMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's
                @ref ILINTransmissionStatusEvent::GetCRCType "CRC type".

        @return @ref filterConcepts "filter value" for the event's
                @ref ILINTransmissionStatusEvent::GetCRCType "CRC type".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint8 OLI_CALL GetCRCTypeValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the event's @ref 
                ILINTransmissionStatusEvent::GetState "transmission state".

        @return @ref filterConcepts "filter mask" for the event's @ref 
                ILINTransmissionStatusEvent::GetState "transmission state".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint32 OLI_CALL GetStateMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the event's @ref 
                ILINTransmissionStatusEvent::GetState "transmission state".

        @return @ref filterConcepts "filter value" for the event's @ref 
                ILINTransmissionStatusEvent::GetState "transmission state".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    virtual uint32 OLI_CALL GetStateValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINTransmissionStatusEventFilter instance.

                The instance returned here will only match 
                @ref ILINTransmissionStatusEvent messages. All other
                types will not pass the filter.

        @param[in]  frameIDMask   
                Any mask for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  frameIDValue  
                Any value for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  directionMask
                Any combination of @ref LINFrameDirection flags to be
                applied to @ref ILINTransmissionStatusEvent::GetDirection.
                The filter will report it in @ref GetDirectionMask.
        @param[in]  directionValue           
                Any combination of @ref LINFrameDirection flags to be
                applied to @ref ILINTransmissionStatusEvent::GetDirection.
                The filter will report it in @ref GetDirectionValue.
        @param[in]  dlcMask
                Any mask for the LIN frame DLC.
                The filter will report it in @ref GetDLCMask.
        @param[in]  dlcValue
                Any value for the LIN frame DLC.
                The filter will report it in @ref GetDLCValue.
        @param[in]  crcTypeMask
                Any combination of @ref LINCRCType flags to be
                applied to @ref ILINTransmissionStatusEvent::GetCRCType.
                The filter will report it in @ref GetCRCTypeMask.
        @param[in]  crcTypeValue
                Any combination of @ref LINCRCType flags to be
                applied to @ref ILINTransmissionStatusEvent::GetCRCType.
                The filter will report it in @ref GetCRCTypeValue.
        @param[in]  stateMask                
                Any combination of @ref LINTransmissionState flags to be
                applied to @ref ILINTransmissionStatusEvent::GetState.
                The filter will report it in @ref GetStateMask.
        @param[in]  stateValue               
                Any combination of @ref LINTransmissionState flags to be
                applied to @ref ILINTransmissionStatusEvent::GetState.
                The filter will report it in @ref GetStateValue.

        @return New @ref ILINTransmissionStatusEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINTransmissionStatusEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTransmissionStatusEvent
     */
    static AutoPtr<ILINTransmissionStatusEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint8 directionMask
           , uint8 directionValue
           , uint8 dlcMask
           , uint8 dlcValue
           , uint8 crcTypeMask
           , uint8 crcTypeValue
           , uint32 stateMask
           , uint32 stateValue )
    {
        ILINTransmissionStatusEventFilter* pLinTxStatusEventFilter = NULL;
        CheckAndThrow( 
            ILINTransmissionStatusEventFilter_Create( 
                frameIDMask, 
                frameIDValue, 
                directionMask, 
                directionValue, 
                dlcMask, 
                dlcValue, 
                crcTypeMask, 
                crcTypeValue, 
                stateMask, 
                stateValue, 
                &pLinTxStatusEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.

        return AutoPtr<ILINTransmissionStatusEventFilter>( 
            pLinTxStatusEventFilter, 
            false);
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINTRANSMISSIONSTATUSEVENTFILTER_INCLUDED__)
