///////////////////////////////////////////////////////////
//  ILINErrSyncEventFilter.h
//  Defines the ILINErrSyncEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINERRSYNCEVENTFILTER_INCLUDED__)
#define __OLI_ILINERRSYNCEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINErrSyncEventFilter;

/**
 * This function instantiates an object supporting ILINErrSyncEventFilter. See \ref BinaryCompatibility for an explanation
 * of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINErrSyncEventFilter::Create().
 *
 * \param[in]  baudrateMask
 * \param[in]  baudrateValue
 * \param[out] ppLinErrSyncEventFilter  A pointer to an object supporting ILINErrSyncEventFilter. The object will already
 *                                      have a reference count of 1 and must be reference-counted by the caller, using the
 *                                      object's methods AddRef() and Release(). This is easily done by wrapping the object
 *                                      pointer in an instance of the AutoPtr class, which will be done automatically if
 *                                      the caller accesses ILINErrSyncEventFilter_Create() via the wrapper
 *                                      ILINErrSyncEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINErrSyncEventFilter_Create( uint32 baudrateMask, uint32 baudrateValue, ILINErrSyncEventFilter** ppLinErrSyncEventFilter );

/** A specialized event filter class for LINErrSync events.
*/

OLI_INTERFACE ILINErrSyncEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINErrSyncEventFilter() OLI_NOTHROW {};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint32 OLI_CALL GetBaudrateMask() const OLI_NOTHROW = 0;
	virtual uint32 OLI_CALL GetBaudrateValue() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINErrSyncEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    static AutoPtr<ILINErrSyncEventFilter> OLI_CALL 
    Create ( uint32 baudrateMask
           , uint32 baudrateValue )
    {
        ILINErrSyncEventFilter* pLinErrSyncEventFilter = NULL;
        CheckAndThrow( ILINErrSyncEventFilter_Create( baudrateMask, baudrateValue, &pLinErrSyncEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINErrSyncEventFilter>( pLinErrSyncEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINERRSYNCEVENTFILTER_INCLUDED__)
