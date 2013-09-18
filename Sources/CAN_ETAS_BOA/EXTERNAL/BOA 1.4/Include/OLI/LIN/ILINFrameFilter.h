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
* @brief  ILINFrameFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINFRAMEFILTER_INCLUDED__)
#define __OLI_ILINFRAMEFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IFrameFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "LINBase.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class ILINFrameFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ILINFrameFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINFrameFilter::Create().
*
* @param[in]  frameIDMask   
*         Any mask for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  frameIDValue  
*         Any value for the LIN frame ID.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  crcTypeMask
*         Any combination of @ref LINCRCType flags to be
*         applied to @ref ILINTxFrame::GetCRCType.
*         The filter will report it in 
*         @ref ILINFrameFilter::GetCRCTypeMask.
* @param[in]  crcTypeValue
*         Any combination of @ref LINCRCType flags to be
*         applied to @ref ILINTxFrame::GetCRCType.
*         The filter will report it in 
*         @ref ILINFrameFilter::GetCRCTypeValue.
* @param[in]  dlcMask
*         Any mask for the LIN frame DLC.
*         The filter will report it in 
*         @ref ILINFrameFilter::GetDLCMask.
* @param[in]  dlcValue
*         Any value for the LIN frame DLC.
*         The filter will report it in 
*         @ref ILINFrameFilter::GetDLCValue.
* @param[in]  payloadConditions                
*         Conditions on the payload of the frame (see @ref IFrameFilter
*         for details). The conditions will be accessible through
*         @ref IFrameFilter::GetCondition. May be NULL only if
*         @ref payloadConditionsCount is 0.
* @param[in]  payloadConditionsCount               
*         Number of conditions on the payload, i.e. number of valid
*         elements in @ref payloadConditions. May be 0.
*         The filter will report it in @ref IFrameFilter::GetCount.
* @param[out] ppLinFrameFilter   
*         A pointer to an object supporting @ref ILINFrameFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref IILINFrameFilter_Create() 
*         via the wrapper @ref ILINFrameFilter::Create().
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
*         ILINFrameFilter, IFrameFilter
*/
OLL_API IError* OLI_CALL ILINFrameFilter_Create(
    uint32                  frameIDMask,
    uint32                  frameIDValue,
    uint8                   crcTypeMask,
    uint8                   crcTypeValue,
    uint8                   dlcMask,
    uint8                   dlcValue,
    const PayloadCondition* payloadConditions,
    uint32                  payloadConditionsCount,
    ILINFrameFilter**       ppLinFrameFilter );


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  A specialized frame filter class for LIN frames.
*
* In addition to the base class conditions, this interface checks 
* for the frame's @ref ITxFrame::GetSize "payload length" and 
* @ref ILINTxFrame::GetCRCType "CRC type".
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the frame ID mask and
* frame ID value supplied to ILINFrameFilter::Create.
*
* The implementation is expected to match @ref ILINTxFrame 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINTxFrame
*/

OLI_INTERFACE ILINFrameFilter 
    : public IFrameFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINFrameFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the mask for the condition on the frame's
                @ref ITxFrame::GetSize "DLC value".

        @return @ref filterConcepts "filter mask" for the frame's 
                @ref ITxFrame::GetSize "DLC value".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINTxFrame
     */
    virtual uint8 OLI_CALL GetDLCMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the frame's
                @ref ITxFrame::GetSize "DLC value".

        @return @ref filterConcepts "filter value" for the frame's
                @ref ITxFrame::GetSize "DLC value".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINTxFrame
     */
	virtual uint8 OLI_CALL GetDLCValue() const OLI_NOTHROW = 0;

    /** @brief  Returns the mask for the condition on the frame's
                @ref ILINTxFrame::GetCRCType "CRC type".

        @return @ref filterConcepts "filter mask" for the frame's 
                @ref ILINTxFrame::GetCRCType "CRC type".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINTxFrame
     */
    virtual uint8 OLI_CALL GetCRCTypeMask() const OLI_NOTHROW = 0;

    /** @brief  Returns the value for the condition on the frame's
                @ref ILINTxFrame::GetCRCType "CRC type".

        @return @ref filterConcepts "filter value" for the frame's
                @ref ILINTxFrame::GetCRCType "CRC type".
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINTxFrame
     */
    virtual uint8 OLI_CALL GetCRCTypeValue() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINFrameFilter instance.

                The instance returned here will only match 
                @ref ILINTxFrame messages. All other
                types will not pass the filter.

        @param[in]  frameIDMask   
                Any mask for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  frameIDValue  
                Any value for the LIN frame ID.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  crcTypeMask
                Any combination of @ref LINCRCType flags to be
                applied to @ref ILINTxFrame::GetCRCType.
                The filter will report it in @ref GetCRCTypeMask.
        @param[in]  crcTypeValue
                Any combination of @ref LINCRCType flags to be
                applied to @ref ILINTxFrame::GetCRCType.
                The filter will report it in @ref GetCRCTypeValue.
        @param[in]  dlcMask
                Any mask for the LIN frame DLC.
                The filter will report it in @ref GetDLCMask.
        @param[in]  dlcValue
                Any value for the LIN frame DLC.
                The filter will report it in @ref GetDLCValue.
        @param[in]  payloadConditions                
                Conditions on the payload of the frame (see @ref IFrameFilter
                for details). The conditions will be accessible through
                @ref IFrameFilter::GetCondition. May be NULL only if
                @ref payloadConditionsCount is 0.
        @param[in]  payloadConditionsCount               
                Number of conditions on the payload, i.e. number of valid
                elements in @ref payloadConditions. May be 0.
                The filter will report it in @ref IFrameFilter::GetCount.
        @return New @ref ILINFrameFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINFrameFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ILINTxFrame, IFrameFilter
     */
    static AutoPtr<ILINFrameFilter> OLI_CALL 
    Create ( uint32 frameIDMask
           , uint32 frameIDValue
           , uint8 crcTypeMask
           , uint8 crcTypeValue
           , uint8 dlcMask
           , uint8 dlcValue
           , const PayloadCondition *payloadConditions
           , uint32 payloadConditionsCount )
    {
        ILINFrameFilter* pLinFrameFilter = NULL;
        CheckAndThrow( ILINFrameFilter_Create( frameIDMask, 
                                               frameIDValue, 
                                               crcTypeMask, 
                                               crcTypeValue, 
                                               dlcMask, 
                                               dlcValue, 
                                               payloadConditions, 
                                               payloadConditionsCount, 
                                               &pLinFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.

        return AutoPtr<ILINFrameFilter>( pLinFrameFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINFRAMEFILTER_INCLUDED__)
