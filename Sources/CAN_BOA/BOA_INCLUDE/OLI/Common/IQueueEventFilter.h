///////////////////////////////////////////////////////////
//  IQueueEventFiler.h
//  Defines the IQueueEventFiler interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IQUEUEEVENTFILER_INCLUDED__)
#define __OLI_IQUEUEEVENTFILER_INCLUDED__

// include used interface

#include "AutoPtr.h"
#include "IEventFilter.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "BeginNamespace.h"

/// \addtogroup GROUP_OLI_COMMON_FILTERS

/// @{

class IQueueEventFilter;

/**
 * This function instantiates an object supporting IQueueEventFilter. See \ref BinaryCompatibility for an explanation of why
 * it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IQueueEventFilter::Create().
 *
 * \param[in]  eventCodeMask
 * \param[in]  eventCodeValue
 * \param[out] ppQueueEventFilter   A pointer to an object supporting IQueueEventFilter, which has already been AddRef-ed. The
 *                                  object must be reference-counted by the caller, using the object's methods AddRef() and
 *                                  Release(). This is easily done by wrapping the object pointer in an instance of the AutoPtr
 *                                  class, which will be done automatically if the caller accesses IQueueEventFilter_Create()
 *                                  via the wrapper IQueueEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IQueueEventFilter_Create( uint32 eventCodeMask, uint32 eventCodeValue, IQueueEventFilter** ppQueueEventFilter );

/** A specialized event filter class for @ref IQueueEvent objects.
*/

OLI_INTERFACE IQueueEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~IQueueEventFilter() OLI_NOTHROW {};

public:

    /**
     * This is a helper method which wraps IQueueEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<IQueueEventFilter> OLI_CALL 
    Create( uint32 eventCodeMask, uint32 eventCodeValue )
    {
        IQueueEventFilter* pQueueEventFilter = NULL;
        CheckAndThrow( IQueueEventFilter_Create( eventCodeMask, eventCodeValue, &pQueueEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<IQueueEventFilter>( pQueueEventFilter, false );
    }
};

/// @}

#include "EndNamespace.h"

#endif // !defined(__OLI_IQUEUEEVENTFILER_INCLUDED__)
