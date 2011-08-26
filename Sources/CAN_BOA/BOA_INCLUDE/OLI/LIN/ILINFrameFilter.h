///////////////////////////////////////////////////////////
//  ILINFrameFilter.h
//  Defines the ILINFrameFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINFRAMEFILTER_INCLUDED__)
#define __OLI_ILINFRAMEFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IFrameFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "LINBase.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINFrameFilter;

/**
 * This function instantiates an object supporting ILINFrameFilter. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINFrameFilter::Create().
 *
 * \param[in]  idMask
 * \param[in]  idValue
 * \param[in]  crcTypeMask
 * \param[in]  crcTypeValue
 * \param[in]  dlcMask
 * \param[in]  dlcValue
 * \param[in]  payloadConditions
 * \param[in]  payloadConditionsCount
 * \param[out] ppLinFrameFilter         A pointer to an object supporting ILINFrameFilter. The object will already have a
 *                                      reference count of 1 and must be reference-counted by the caller, using the object's
 *                                      methods AddRef() and Release(). This is easily done by wrapping the object pointer
 *                                      in an instance of the AutoPtr class, which will be done automatically if the caller
 *                                      accesses ILINFrameFilter_Create() via the wrapper ILINFrameFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINFrameFilter_Create(
    uint32                  idMask,
    uint32                  idValue,
    uint8                   crcTypeMask,
    uint8                   crcTypeValue,
    uint8                   dlcMask,
    uint8                   dlcValue,
    const PayloadCondition* payloadConditions,
    uint32                  payloadConditionsCount,
    ILINFrameFilter**       ppLinFrameFilter );

/** A specialized frame filter class for LIN frames.
*/

OLI_INTERFACE ILINFrameFilter 
    : public IFrameFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINFrameFilter() OLI_NOTHROW {};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint8 OLI_CALL GetCRCTypeMask() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetCRCTypeValue() const OLI_NOTHROW = 0;

    virtual uint8 OLI_CALL GetDLCMask() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetDLCValue() const OLI_NOTHROW = 0;

	/// @}

    /** construct an instance of this interfaces' default implementation
    @param idMask
    @param idValue
    @param crcTypeMask              Any combination of @ref LINCRCType flags.
    @param crcTypeValue             Any combination of @ref LINCRCType flags.
    @param dlcMask
    @param dlcValue
    @param *payloadConditions
    @param payloadConditionsCount
  */
    /**
     * This is a helper method which wraps ILINFrameFilter_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ILINFrameFilter> OLI_CALL 
    Create ( uint32 idMask
           , uint32 idValue
           , uint8 crcTypeMask
           , uint8 crcTypeValue
           , uint8 dlcMask
           , uint8 dlcValue
           , const PayloadCondition *payloadConditions
           , uint32 payloadConditionsCount )
    {
        ILINFrameFilter* pLinFrameFilter = NULL;
        CheckAndThrow( ILINFrameFilter_Create( idMask, idValue, crcTypeMask, crcTypeValue, dlcMask, dlcValue, payloadConditions, payloadConditionsCount, &pLinFrameFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINFrameFilter>( pLinFrameFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINFRAMEFILTER_INCLUDED__)
