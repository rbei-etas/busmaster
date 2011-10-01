///////////////////////////////////////////////////////////
//  ILINWakeUpEventFilter.h
//  Defines the ILINWakeUpEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINWAKEUPEVENTFILTER_INCLUDED__)
#define __OLI_ILINWAKEUPEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINWakeUpEventFilter;

/**
 * This function instantiates an object supporting ILINWakeUpEventFilter. See \ref BinaryCompatibility for an explanation
 * of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINWakeUpEventFilter::Create().
 *
 * \param[in]  frameIDMask
 * \param[in]  frameIDValue
 * \param[in]  flagsMask
 * \param[in]  flagsValue
 * \param[out] ppLinWakeUpEventFilter   A pointer to an object supporting ILINWakeUpEventFilter. The object will already
 *                                      have a reference count of 1 and must be reference-counted by the caller, using the
 *                                      object's methods AddRef() and Release(). This is easily done by wrapping the object
 *                                      pointer in an instance of the AutoPtr class, which will be done automatically if
 *                                      the caller accesses ILINWakeUpEventFilter_Create() via the wrapper ILINWakeUpEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINWakeUpEventFilter_Create( uint8 frameIDMask, uint8 frameIDValue, uint16 flagsMask, uint16 flagsValue, ILINWakeUpEventFilter** ppLinWakeUpEventFilter );

/** A specialized event filter class for LINWakeUp events.
*/

OLI_INTERFACE ILINWakeUpEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINWakeUpEventFilter() OLI_NOTHROW {};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint16 OLI_CALL GetFlagsMask() const OLI_NOTHROW = 0;
	virtual uint16 OLI_CALL GetFlagsValue() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINWakeUpEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ILINWakeUpEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint16 flagsMask
           , uint16 flagsValue )
    {
        ILINWakeUpEventFilter* pLinWakeUpEventFilter = NULL;
        CheckAndThrow( ILINWakeUpEventFilter_Create( frameIDMask, frameIDValue, flagsMask, flagsValue, &pLinWakeUpEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINWakeUpEventFilter>( pLinWakeUpEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINWAKEUPEVENTFILTER_INCLUDED__)
