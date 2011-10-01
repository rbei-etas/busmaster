///////////////////////////////////////////////////////////
//  ICANFrameFilter.h
//  Defines the ICANFrameFilter interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANFRAMEFILTER_INCLUDED__)
#define __OLI_ICANFRAMEFILTER_INCLUDED__

/// @todo update doxygen comments

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IFrameFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_FILTERS
/// @{

class ICANFrameFilter;

/**
 * This function instantiates an object supporting ICANFrameFilter. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ICANFrameFilter::Create().
 *
 * \param[in]  idMask                   A frame will pass the filter if (frame ID) & \a idMask == \a idValue.
 * \param[in]  idValue                  A frame will pass the filter if (frame ID) & \a idMask == \a idValue.
 * \param[in]  isExtendedID             If this is true, the filter will accept only frames with extended IDs; if this is false, the filter will accept only frames with standard IDs.
 * \param[in]  isRemoteFrame            If this is true, the filter will accept only remote frames; if this is false, the filter will accept only non-remote frames. 
 * \param[in]  filterByExtendedID       Indicates whether the value of \a isExtendedID should be considered when applying the filter.
 * \param[in]  filterByRemoteFrame      Indicates whether the value of \a isRemoteFrame should be considered when applying the filter.
 * \param[in]  payloadConditions        An array of PayloadCondition filters to be applied to the payload.
 * \param[in]  payloadConditionsCount   The number of elements at \a payloadConditions.
 * \param[out] ppCanFrameFilter         A pointer to an object supporting ICANFrameFilter, which has already been AddRef-ed. The
 *                                      object must be reference-counted by the caller, using the object's methods AddRef() and
 *                                      Release(). This is easily done by wrapping the object pointer in an instance of the AutoPtr
 *                                      class, which will be done automatically if the caller accesses ICANFrameFilter_Create() via
 *                                      the wrapper ICANFrameFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
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

/** A specialized frame filter class for CAN frames.
*/

OLI_INTERFACE ICANFrameFilter 
    : public IFrameFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ICANFrameFilter() OLI_NOTHROW {};

public:

    /// \name CAN-specific interface
    /// @{
    virtual bool OLI_CALL IsExtendedID() const OLI_NOTHROW = 0;
    virtual bool OLI_CALL IsRemoteFrame() const OLI_NOTHROW = 0;
    virtual bool OLI_CALL FilterByExtendedID() const OLI_NOTHROW = 0;
    virtual bool OLI_CALL FilterByRemoteFrame() const OLI_NOTHROW = 0;
    /// @}

    /**
     * This is a helper method which wraps ICANFrameFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
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
        CheckAndThrow( ICANFrameFilter_Create( idMask, idValue, isExtendedID, isRemoteFrame, filterByExtendedID, filterByRemoteFrame, payloadConditions, payloadConditionsCount, &pCanFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ICANFrameFilter>( pCanFrameFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANFRAMEFILTER_INCLUDED__)
