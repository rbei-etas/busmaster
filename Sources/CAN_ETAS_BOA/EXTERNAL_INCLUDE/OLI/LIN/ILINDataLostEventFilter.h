///////////////////////////////////////////////////////////
//  ILINDataLostEventFilter.h
//  Defines the ILINDataLostEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINDATALOSTEVENTFILTER_INCLUDED__)
#define __OLI_ILINDATALOSTEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINDataLostEventFilter;

/**
 * This function instantiates an object supporting ILINDataLostEventFilter. See \ref BinaryCompatibility for an explanation
 * of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINDataLostEventFilter::Create().
 *
 * \param[in]  minCount
 * \param[out] ppLinDataLostEventFilter     A pointer to an object supporting ILINDataLostEventFilter. The object will
 *                                          already have a reference count of 1 and must be reference-counted by the caller,
 *                                          using the object's methods AddRef() and Release(). This is easily done by wrapping
 *                                          the object pointer in an instance of the AutoPtr class, which will be done
 *                                          automatically if the caller accesses ILINDataLostEventFilter_Create() via the
 *                                          wrapper ILINDataLostEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINDataLostEventFilter_Create( uint32 minCount, ILINDataLostEventFilter** ppLinDataLostEventFilter );

/** A specialized event filter class for LINWakeUp events.
*/

OLI_INTERFACE ILINDataLostEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINDataLostEventFilter() OLI_NOTHROW {};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint32 OLI_CALL GetMinCount() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINDataLostEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    static AutoPtr<ILINDataLostEventFilter> OLI_CALL 
    Create ( uint32 minCount )
    {
        ILINDataLostEventFilter* pLinDataLostEventFilter = NULL;
        CheckAndThrow( ILINDataLostEventFilter_Create( minCount, &pLinDataLostEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINDataLostEventFilter>( pLinDataLostEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINDATALOSTEVENTFILTER_INCLUDED__)
