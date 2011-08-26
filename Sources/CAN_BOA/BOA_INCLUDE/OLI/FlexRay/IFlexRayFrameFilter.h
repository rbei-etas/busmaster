///////////////////////////////////////////////////////////
//  IFlexRayFrameFilter.h
//  Defines the IFlexRayFrameFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYFRAMEFILTER_INCLUDED__)
#define __OLI_IFLEXRAYFRAMEFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IFrameFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"
#include "FlexRayBase.h"

/// \addtogroup GROUP_OLI_FLEXRAY_FILTERS

/// @{

class IFlexRayFrameFilter;

/**
 * This function instantiates an object supporting IFlexRayFrameFilter. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRayFrameFilter::Create().
 *
 * \param[in]  channelMask              Any combination of \ref FlexRayChannel flags.
 * \param[in]  channelValue             Any combination of \ref FlexRayChannel flags.
 * \param[in]  slotMask
 * \param[in]  slotValue
 * \param[in]  cycleOffsetMask
 * \param[in]  cycleOffsetValue
 * \param[in]  payloadLengthMask
 * \param[in]  payloadLengthValue
 * \param[in]  frameTypeMask            Any combination of \ref FlexRayFrameType flags 
 * \param[in]  frameTypeValue           Any combination of \ref FlexRayFrameType flags 
 * \param[in]  payloadConditions
 * \param[in]  payloadConditionsCount
 * \param[out] ppFlexRayFrameFilter     A pointer to an object supporting IFlexRayFrameFilter, which has been AddRef-ed already.
 *                                      The object must be reference-counted by the caller, using the object's methods AddRef()
 *                                      and Release(). This is easily done by wrapping the object pointer in an instance of the
 *                                      AutoPtr class, which will be done automatically if the caller accesses
 *                                      IFlexRayFrameFilter_Create() via the wrapper IFlexRayFrameFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRayFrameFilter_Create(
    uint32                  channelMask,
    uint32                  channelValue,
    uint32                  slotMask,
    uint32                  slotValue,
    uint32                  cycleOffsetMask,
    uint32                  cycleOffsetValue,
    uint32                  payloadLengthMask,
    uint32                  payloadLengthValue,
    uint32                  frameTypeMask,
    uint32                  frameTypeValue,
    const PayloadCondition* payloadConditions,
    uint32                  payloadConditionsCount,
    IFlexRayFrameFilter**   ppFlexRayFrameFilter );

/** A specialized frame filter class for FlexRay frames.
* Channel, slot and cycle will be combined into a frameID
*/

OLI_INTERFACE IFlexRayFrameFilter 
    : public IFrameFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~IFlexRayFrameFilter() OLI_NOTHROW {};

public:

    /// \name FlexRay-specific interface
	/// @{
    virtual uint32 OLI_CALL GetChannelMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetChannelValue() const OLI_NOTHROW = 0;

    virtual uint32 OLI_CALL GetSlotMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetSlotValue() const OLI_NOTHROW = 0;

    virtual uint32 OLI_CALL GetCycleOffsetMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetCycleOffsetValue() const OLI_NOTHROW = 0;

    virtual uint32 OLI_CALL GetPayloadLengthMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetPayloadLengthValue() const OLI_NOTHROW = 0;

    virtual uint32 OLI_CALL GetFrameTypeMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetFrameTypeValue() const OLI_NOTHROW = 0;
    /// @}

    /**
     * This is a helper method which wraps IFlexRayFrameFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    static AutoPtr<IFlexRayFrameFilter> OLI_CALL 
    Create ( uint32                     channelMask
           , uint32                     channelValue
           , uint32                     slotMask
           , uint32                     slotValue
           , uint32                     cycleOffsetMask
           , uint32                     cycleOffsetValue
           , uint32                     payloadLengthMask
           , uint32                     payloadLengthValue
           , uint32                     frameTypeMask
           , uint32                     frameTypeValue
           , const PayloadCondition*    payloadConditions
           , uint32                     payloadConditionsCount )
    {
        IFlexRayFrameFilter* pFlexRayFrameFilter = NULL;
        CheckAndThrow( IFlexRayFrameFilter_Create( channelMask, channelValue, slotMask, slotValue, cycleOffsetMask, cycleOffsetValue, payloadLengthMask, payloadLengthValue, frameTypeMask, frameTypeValue, payloadConditions, payloadConditionsCount, &pFlexRayFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<IFlexRayFrameFilter>( pFlexRayFrameFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_FLEXRAYFRAMEFILTER_INCLUDED__)
