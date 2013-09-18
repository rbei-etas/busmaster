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
* @brief  ICANFrameFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANFRAMEFILTER_INCLUDED__)
#define __OLI_ICANFRAMEFILTER_INCLUDED__

/// @todo update doxygen comments

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IFrameFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class ICANFrameFilter;

/**
* @ingroup GROUP_OLI_CAN_FILTERS
* @brief  This function instantiates an object supporting 
*         @ref ICANFrameFilter. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ICANFrameFilter::Create().
*
* @param[in]  idMask   
*         Any mask for the CAN frame ID.
*         The filter will report it in @ref IFilter::GetIDMask.
* @param[in]  idValue  
*         Any value for the CAN frame ID.
*         The filter will report it in @ref IFilter::GetIDValue.
* @param[in]  isExtendedID             
*         If this is @c true, the filter will accept only frames 
*         with extended IDs; if this is @c false, the filter will accept 
*         only frames with standard IDs. The filter will return this
*         value in @ref ICANFrameFilter::IsExtendedID.
* @param[in]  isRemoteFrame            
*         If this is @c true, the filter will accept only remote frames; 
*         if this is @c false, the filter will accept only non-remote frames.
*         The filter will return this value in @ref 
*         ICANFrameFilter::IsRemoteFrame.
* @param[in]  filterByExtendedID       
*         Indicates whether the value of @a isExtendedID should be 
*         considered when applying the filter. The filter will return this
*         value in @ref ICANFrameFilter::FilterByExtendedID.
* @param[in]  filterByRemoteFrame      
*         Indicates whether the value of @a isRemoteFrame should be 
*         considered when applying the filter. The filter will return this
*         value in @ref ICANFrameFilter::FilterByRemoteFrame.
* @param[in]  payloadConditions                
*         Conditions on the payload of the frame (see @ref IFrameFilter
*         for details). The conditions will be accessible through
*         @ref IFrameFilter::GetCondition. May be NULL only if
*         @ref payloadConditionsCount is 0.
* @param[in]  payloadConditionsCount               
*         Number of conditions on the payload, i.e. number of valid
*         elements in @ref payloadConditions. May be 0.
*         The filter will report it in @ref IFrameFilter::GetCount.
* @param[out] ppCanFrameFilter   
*         A pointer to an object supporting @ref ICANFrameFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ICANFrameFilter_Create() 
*         via the wrapper @ref ICANFrameFilter::Create().
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
*         ICANFrameFilter, IFrameFilter
*/
OLL_API IError* OLI_CALL ICANFrameFilter_Create(
    uint32                  idMask,
    uint32                  idValue,
    bool                    isExtendedID,
    bool                    isRemoteFrame,
    bool                    filterByExtendedID,
    bool                    filterByRemoteFrame,
    const PayloadCondition* payloadConditions,
    uint32                  payloadConditionsCount,
    ICANFrameFilter**       ppCanFrameFilter );


/** @ingroup GROUP_OLI_CAN_FILTERS
* @brief  A specialized frame filter class for CAN frames.
*
* In addition to the base class conditions, this interface checks 
* for the frame's @ref ICANTxFrame::IsExtendedID "CAN ID type" and 
* @ref ICANTxFrame::IsRemoteFrame "remote frame property".
*
* These additional tests on boolean properties are slightly different 
* from the standard @ref filterConcepts "mask and value" filtering in 
* so far as the value part gets completely ignored when the mask part 
* is 0 (@c false).
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue return the frame ID mask and
* frame ID value supplied to ICANFrameFilter::Create.
*
* The implementation is expected to match @ref ICANTxFrame 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ICANTxFrame
*/

OLI_INTERFACE ICANFrameFilter 
    : public IFrameFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ICANFrameFilter() OLI_NOTHROW {};

public:

    /** @brief  Whether to accept extended or standard CAN IDs only.

                If this is @c true, the filter will accept only frames 
                with extended (29 bits) CAN IDs; if this is @c false, 
                the filter will accept only frames with standard (11 bit)
                CAN IDs.

                To accept both @ref ICANTxFrame::IsExtendedID "types" 
                of IDs, @ref FilterByExtendedID must be @c false.

        @return The value @ref ICANRxFrame::IsExtendedID must return to
                pass this filter.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ICANTxFrame::IsExtendedID, FilterByExtendedID
     */
    virtual bool OLI_CALL IsExtendedID() const OLI_NOTHROW = 0;

    /** @brief  Whether to accept remote or normal frames only.

                If this is @c true, the filter will accept only frames 
                with extended (29 bits) CAN IDs; if this is @c false, 
                the filter will accept only frames with standard (11 bit)
                CAN IDs.

                To accept both @ref ICANTxFrame::IsRemoteFrame "types" 
                of frames, @ref FilterByRemoteFrame must be @c false.

        @return The value @ref ICANRxFrame::IsRemoteFrame must return to
                pass this filter.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ICANTxFrame::IsRemoteFrame, FilterByExtendedID
     */
    virtual bool OLI_CALL IsRemoteFrame() const OLI_NOTHROW = 0;

    /** @brief  Will frames be tested for their @ref 
                ICANTxFrame::IsExtendedID value?

        @return @a false, if the @ref IsExtendedID will be ignored.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ICANTxFrame::IsExtendedID, IsExtendedID
     */
    virtual bool OLI_CALL FilterByExtendedID() const OLI_NOTHROW = 0;

    /** @brief  Will frames be tested for their @ref 
                ICANTxFrame::IsRemoteFrame value?

        @return @a false, if the @ref IsRemoteFrame will be ignored.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ICANTxFrame::IsRemoteFrame, IsRemoteFrame
     */
    virtual bool OLI_CALL FilterByRemoteFrame() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ICANFrameFilter instance.

                The instance returned here will only match 
                @ref ICANTxFrame messages. All other
                types will not pass the filter.

        @param[in]  idMask   
                Any mask for the CAN frame ID.
                The filter will report it in @ref IFilter::GetIDMask.
        @param[in]  idValue  
                Any value for the CAN frame ID.
                The filter will report it in @ref IFilter::GetIDValue.
        @param[in]  isExtendedID             
                If this is @c true, the filter will accept only frames 
                with extended IDs; if this is @c false, the filter will 
                accept only frames with standard IDs. The filter will 
                return this value in @ref IsExtendedID.
        @param[in]  isRemoteFrame            
                If this is @c true, the filter will accept only remote 
                frames; if this is @c false, the filter will accept only 
                non-remote frames. The filter will return this value in 
                @ref IsRemoteFrame.
        @param[in]  filterByExtendedID       
                Indicates whether the value of @a isExtendedID should be 
                considered when applying the filter. The filter will return 
                this value in @ref FilterByExtendedID.
        @param[in]  filterByRemoteFrame      
                Indicates whether the value of @a isRemoteFrame should be 
                considered when applying the filter. The filter will return 
                this value in @ref FilterByRemoteFrame.
        @param[in]  payloadConditions                
                Conditions on the payload of the frame (see @ref IFrameFilter
                for details). The conditions will be accessible through
                @ref IFrameFilter::GetCondition. May be NULL only if
                @ref payloadConditionsCount is 0.
        @param[in]  payloadConditionsCount               
                Number of conditions on the payload, i.e. number of valid
                elements in @ref payloadConditions. May be 0.
                The filter will report it in @ref IFrameFilter::GetCount.
        @return New @ref ICANFrameFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ICANFrameFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", 
                ICANTxFrame, IFrameFilter
     */
    static AutoPtr<ICANFrameFilter> OLI_CALL 
    Create ( uint32                     idMask
           , uint32                     idValue
           , bool                       isExtendedID
           , bool                       isRemoteFrame
           , bool                       filterByExtendedID
           , bool                       filterByRemoteFrame
           , const PayloadCondition*    payloadConditions
           , uint32                     payloadConditionsCount)
    {
        ICANFrameFilter* pCanFrameFilter = NULL;
        CheckAndThrow( ICANFrameFilter_Create( idMask, 
                                               idValue, 
                                               isExtendedID, 
                                               isRemoteFrame, 
                                               filterByExtendedID, 
                                               filterByRemoteFrame, 
                                               payloadConditions, 
                                               payloadConditionsCount, 
                                               &pCanFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.

        return AutoPtr<ICANFrameFilter>( pCanFrameFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANFRAMEFILTER_INCLUDED__)
