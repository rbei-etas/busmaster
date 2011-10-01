///////////////////////////////////////////////////////////
//  ICANErrorFrameFilter.h
//  Defines the ICANErrorFrameFilter interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANERRORFRAMEFILTER_INCLUDED__)
#define __OLI_ICANERRORFRAMEFILTER_INCLUDED__

/// @todo update doxygen comments

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_FILTERS
/// @{

class ICANErrorFrameFilter;

/**
 * This function instantiates an object supporting ICANErrorFrameFilter. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ICANErrorFrameFilter::Create().
 *
 * \param[in]  frameIDMask              Any combination of \ref CANErrorFrame flags.
 * \param[in]  frameIDValue             Any combination of \ref CANErrorFrame flags.
 * \param[out] ppCanErrorFrameFilter    A pointer to an object supporting ICANErrorFrameFilter; the function AddRefs this
 *                                      object before handing it out. The object must be reference-counted by the caller, using
 *                                      the object's methods AddRef() and Release(). This is easily done by wrapping the object
 *                                      pointer in an instance of the AutoPtr class, which will be done automatically if the caller
 *                                      accesses ICANErrorFrameFilter_Create() via the wrapper ICANErrorFrameFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ICANErrorFrameFilter_Create( uint32 frameIDMask, uint32 frameIDValue, ICANErrorFrameFilter** ppCanErrorFrameFilter );

/** A specialized event filter class for CAN error frames.
*/

OLI_INTERFACE ICANErrorFrameFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ICANErrorFrameFilter() OLI_NOTHROW {};

public:

    /**
     * This is a helper method which wraps ICANErrorFrameFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    static AutoPtr<ICANErrorFrameFilter> OLI_CALL 
    Create( uint32 frameIDMask, uint32 frameIDValue)
    {
        ICANErrorFrameFilter* pCanErrorFrameFilter = NULL;
        CheckAndThrow( ICANErrorFrameFilter_Create( frameIDMask, frameIDValue, &pCanErrorFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ICANErrorFrameFilter>( pCanErrorFrameFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANERRORFRAMEFILTER_INCLUDED__)
