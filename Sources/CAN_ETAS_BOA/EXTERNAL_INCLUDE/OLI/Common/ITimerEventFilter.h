///////////////////////////////////////////////////////////
//  ITimerEventFilter.h
//  Defines the ITimerEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ITIMEREVENTFILTER_INCLUDED__)
#define __OLI_ITIMEREVENTFILTER_INCLUDED__

// include used interface

#include "AutoPtr.h"
#include "IEventFilter.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "BeginNamespace.h"

/// \addtogroup GROUP_OLI_COMMON_FILTERS

/// @{

class ITimerEventFilter;

/**
 * This function instantiates an object supporting ITimerEventFilter. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ITimerEventFilter::Create().
 *
 * \param[in]  eventCodeMask        Any combination of \ref TimerEventCode flags.
 * \param[in]  eventCodeValue       Any combination of \ref TimerEventCode flags.
 * \param[out] ppTimerEventFilter   A pointer to an object supporting ITimerEventFilter, which has already been AddRef-ed.
 *                                  The object must be reference-counted by the caller, using the object's methods AddRef()
 *                                  and Release(). This is easily done by wrapping the object pointer in an instance of the
 *                                  AutoPtr class, which will be done automatically if the caller accesses
 *                                  ITimerEventFilter_Create() via the wrapper ITimerEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ITimerEventFilter_Create( uint32 eventCodeMask, uint32 eventCodeValue, ITimerEventFilter** ppTimerEventFilter );

/** A specialized event filter interface for @ref ITimerEvent objects.
*/

OLI_INTERFACE ITimerEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ITimerEventFilter() OLI_NOTHROW {};

public:

    /**
     * This is a helper method which wraps ITimerEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ITimerEventFilter> OLI_CALL 
    Create( uint32 eventCodeMask, uint32 eventCodeValue )
    {
        ITimerEventFilter* pTimerEventFilter = NULL;
        CheckAndThrow( ITimerEventFilter_Create( eventCodeMask, eventCodeValue, &pTimerEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ITimerEventFilter>( pTimerEventFilter, false );
    }
};

/// @}

#include "EndNamespace.h"

#endif // !defined(__OLI_ITIMEREVENTFILER_INCLUDED__)
