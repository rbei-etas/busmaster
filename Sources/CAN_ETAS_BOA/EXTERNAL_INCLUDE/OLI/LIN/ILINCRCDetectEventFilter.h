///////////////////////////////////////////////////////////
//  ILINCRCDetectEventFilter.h
//  Defines the ILINCRCDetectEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINCRCDETECTEVENTFILTER_INCLUDED__)
#define __OLI_ILINCRCDETECTEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINCRCDetectEventFilter;

/**
 * This function instantiates an object supporting ILINCRCDetectEventFilter. See \ref BinaryCompatibility for an explanation
 * of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINCRCDetectEventFilter::Create().
 *
 * \param[in]  frameIDMask
 * \param[in]  frameIDValue
 * \param[in]  crcMask                      Any combination of @ref LINCRCType flags.
 * \param[in]  crcValue                     Any combination of @ref LINCRCType flags.
 * \param[out] ppLinCrcDetectEventFilter    A pointer to an object supporting ILINCRCDetectEventFilter. The object will
 *                                          already have a reference count of 1 and must be reference-counted by the caller,
 *                                          using the object's methods AddRef() and Release(). This is easily done by wrapping
 *                                          the object pointer in an instance of the AutoPtr class, which will be done automatically
 *                                          if the caller accesses ILINCRCDetectEventFilter_Create() via the wrapper
 *                                          ILINCRCDetectEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINCRCDetectEventFilter_Create( uint8 frameIDMask, uint8 frameIDValue, uint8 crcMask, uint8 crcValue, ILINCRCDetectEventFilter** ppLinCrcDetectEventFilter );

/** A specialized event filter class for LINCRCDetect events.
*/

OLI_INTERFACE ILINCRCDetectEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINCRCDetectEventFilter() OLI_NOTHROW{};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint8 OLI_CALL GetCRCTypeMask() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetCRCTypeValue() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINCRCDetectEventFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    static AutoPtr<ILINCRCDetectEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint8 crcMask
           , uint8 crcValue )
    {
        ILINCRCDetectEventFilter* pLINCRCDetectEventFilter = NULL;
        CheckAndThrow( ILINCRCDetectEventFilter_Create( frameIDMask, frameIDValue, crcMask, crcValue, &pLINCRCDetectEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINCRCDetectEventFilter>( pLINCRCDetectEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINCRCDETECTEVENTFILTER_INCLUDED__)
