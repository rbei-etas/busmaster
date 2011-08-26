///////////////////////////////////////////////////////////
//  ICANEventFilter.h
//  Defines the ICANEventFilter interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANEVENTFILTER_INCLUDED__)
#define __OLI_ICANEVENTFILTER_INCLUDED__

/// @todo update doxygen comments

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_FILTERS
/// @{

class ICANEventFilter;

/**
 * This function instantiates an object supporting ICANEventFilter. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ICANEventFilter::Create().
 *
 * \param[in]  eventCodeMask        Any combination of \ref CANBusState flags.
 * \param[in]  eventCodeValue       Any combination of \ref CANBusState flags.
 * \param[out] ppCanEventFilter     A pointer to an object supporting ICANEventFilter, which has been AddRef-ed already. The object
 *                                  must be reference-counted by the caller, using the object's methods AddRef() and Release(). This
 *                                  is easily done by wrapping the object pointer in an instance of the AutoPtr class, which will be
 *                                  done automatically if the caller accesses ICANEventFilter_Create() via the wrapper
 *                                  ICANEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ICANEventFilter_Create( uint32 eventCodeMask, uint32 eventCodeValue, ICANEventFilter** ppCanEventFilter );

/** A specialized event filter class for CAN error frames.
*/

OLI_INTERFACE ICANEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ICANEventFilter() OLI_NOTHROW {};

public:

    /** construct an instance of this interfaces' default implementation
      @param eventCodeMask   Any combination of @ref CANBusState flags.
      @param eventCodeValue  Any combination of @ref CANBusState flags.
    */

    /**
     * This is a helper method which wraps ICANEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ICANEventFilter> OLI_CALL 
    Create( uint32 eventCodeMask, uint32 eventCodeValue )
    {
        ICANEventFilter* pCanEventFilter = NULL;
        CheckAndThrow( ICANEventFilter_Create( eventCodeMask, eventCodeValue, &pCanEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ICANEventFilter>( pCanEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANEVENTFILTER_INCLUDED__)
