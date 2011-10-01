///////////////////////////////////////////////////////////
//  ILINSleepEventFilter.h
//  Defines the ILINSleepEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINSLEEPEVENTFILTER_INCLUDED__)
#define __OLI_ILINSLEEPEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINSleepEventFilter;

/**
 * This function instantiates an object supporting ILINSleepEventFilter. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINSleepEventFilter::Create().
 *
 * \param[in]  frameIDMask
 * \param[in]  frameIDValue
 * \param[in]  sleepMask
 * \param[in]  sleepValue
 * \param[out] ppLinSleepEventFilter    A pointer to an object supporting ILINSleepEventFilter. The object will already have
 *                                      a reference count of 1 and must be reference-counted by the caller, using the object's
 *                                      methods AddRef() and Release(). This is easily done by wrapping the object pointer in
 *                                      an instance of the AutoPtr class, which will be done automatically if the caller
 *                                      accesses ILINSleepEventFilter_Create() via the wrapper ILINSleepEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINSleepEventFilter_Create( uint8 frameIDMask, uint8 frameIDValue, uint8 sleepMask, uint8 sleepValue, ILINSleepEventFilter** ppLinSleepEventFilter );

/** A specialized event filter class for LINSleep events.
*/

OLI_INTERFACE ILINSleepEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINSleepEventFilter() OLI_NOTHROW {};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint8 OLI_CALL GetSleepMask() const OLI_NOTHROW = 0;
	virtual uint8 OLI_CALL GetSleepValue() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINSleepEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ILINSleepEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint8 sleepMask
           , uint8 sleepValue )
    {
        ILINSleepEventFilter* pLinSleepEventFilter = NULL;
        CheckAndThrow( ILINSleepEventFilter_Create( frameIDMask, frameIDValue, sleepMask, sleepValue, &pLinSleepEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINSleepEventFilter>( pLinSleepEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINSLEEPEVENTFILTER_INCLUDED__)
