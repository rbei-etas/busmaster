///////////////////////////////////////////////////////////
//  IFlexRayEventFilter.h
//  Defines the IFlexRayEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYEVENTFILTER_INCLUDED__)
#define __OLI_IFLEXRAYEVENTFILTER_INCLUDED__

// include used interface

#include "FlexRayBase.h"

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_FLEXRAY_FILTERS

/// @{

class IFlexRayEventFilter;

/**
 * This function instantiates an object supporting IFlexRayEventFilter. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRayEventFilter::Create().
 *
 * \param[in]  idMask                               Any combination of \ref FlexRayEventCode flags.
 * \param[in]  idValue                              Any combination of \ref FlexRayEventCode flags.
 * \param[in]  protocolOperationControlStatusMask   Any combination of \ref FlexRayControllerProtocolOperationControlStatus flags.
 * \param[in]  protocolOperationControlStatusValue  Any combination of \ref FlexRayControllerProtocolOperationControlStatus flags.
 * \param[out] ppFlexRayEventFilter                 A pointer to an object supporting IFlexRayEventFilter, which has already been
 *                                                  AddRef-ed. The object must be reference-counted by the caller, using the object's
 *                                                  methods AddRef() and Release(). This is easily done by wrapping the object
 *                                                  pointer in an instance of the AutoPtr class, which will be done automatically
 *                                                  if the caller accesses IFlexRayEventFilter_Create() via the wrapper
 *                                                  IFlexRayEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRayEventFilter_Create( uint32 idMask, uint32 idValue, uint32 protocolOperationControlStatusMask, uint32 protocolOperationControlStatusValue, IFlexRayEventFilter** ppFlexRayEventFilter );

/** A specialized event filter class for FlexRay events.
*/

OLI_INTERFACE IFlexRayEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~IFlexRayEventFilter() OLI_NOTHROW {};

public:

    /// \name FlexRay-specific interface
	/// @{
    virtual uint32 OLI_CALL GetProtocolOperationControlStatusMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetProtocolOperationControlStatusValue() const OLI_NOTHROW = 0;
    /// @}

    /**
     * This is a helper method which wraps IFlexRayEventFilter_Create(): : see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    static AutoPtr<IFlexRayEventFilter> OLI_CALL 
    Create ( uint32 idMask
           , uint32 idValue
           , uint32 protocolOperationControlStatusMask
           , uint32 protocolOperationControlStatusValue )
    {
        IFlexRayEventFilter* pFlexRayEventFilter = NULL;
        CheckAndThrow( IFlexRayEventFilter_Create( idMask, idValue, protocolOperationControlStatusMask, protocolOperationControlStatusValue, &pFlexRayEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<IFlexRayEventFilter>( pFlexRayEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYEVENTFILTER_INCLUDED__)
