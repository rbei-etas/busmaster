///////////////////////////////////////////////////////////
//  ILINTransmissionStatusEventFilter.h
//  Defines the ILINTransmissionStatusEventFilter interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINTRANSMISSIONSTATUSEVENTFILTER_INCLUDED__)
#define __OLI_ILINTRANSMISSIONSTATUSEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_FILTERS

/// @{

class ILINTransmissionStatusEventFilter;

/**
 * This function instantiates an object supporting ILINTransmissionStatusEventFilter. See \ref BinaryCompatibility for
 * an explanation of why it is needed.
 *
 * It is possible to filter based on all information in the \ref ILINTransmissionStatusEvent. If a part of the message
 * should not used for filtering use 0x00 for the mask and value of that part. For using bitmasks and values for
 * filtering \sa filterConcepts.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINTransmissionStatusEventFilter::Create().
 *
 * \param[in]  frameIDMask
 * \param[in]  frameIDValue
 * \param[in]  directionMask            Any combination of \ref LINFrameDirection flags.
 * \param[in]  directionValue           Any combination of \ref LINFrameDirection flags.
 * \param[in]  dlcMask
 * \param[in]  dlcValue
 * \param[in]  crcTypeMask              Any combination of \ref LINCRCType flags.
 * \param[in]  crcTypeValue             Any combination of \ref LINCRCType flags.
 * \param[in]  stateMask                Any combination of \ref LINTransmissionState flags.
 * \param[in]  stateValue               Any combination of \ref LINTransmissionState flags.
 * \param[out] ppLinTxStatusEventFilter A pointer to an object supporting ILINTransmissionStatusEventFilter. The object
 *                                      will already have a reference count of 1 and must be reference-counted by the
 *                                      caller, using the object's methods AddRef() and Release(). This is easily done
 *                                      by wrapping the object pointer in an instance of the AutoPtr class, which will
 *                                      be done automatically if the caller accesses
 *                                      ILINTransmissionStatusEventFilter_Create() via the wrapper
 *                                      ILINTransmissionStatusEventFilter::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINTransmissionStatusEventFilter_Create(
    uint8 frameIDMask,
    uint8 frameIDValue,
    uint8 directionMask,
    uint8 directionValue,
    uint8 dlcMask,
    uint8 dlcValue,
    uint8 crcTypeMask,
    uint8 crcTypeValue,
    uint32 stateMask,
    uint32 stateValue,
    ILINTransmissionStatusEventFilter** ppLinTxStatusEventFilter );

/** A specialized event filter class for LINTransmissionStatus events.
*/

OLI_INTERFACE ILINTransmissionStatusEventFilter 
    : public IEventFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~ILINTransmissionStatusEventFilter() OLI_NOTHROW {};

public:

    /// \name LIN-specific interface
	/// @{

    virtual uint8 OLI_CALL GetDirectionMask() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetDirectionValue() const OLI_NOTHROW = 0;

    virtual uint8 OLI_CALL GetDLCMask() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetDLCValue() const OLI_NOTHROW = 0;

    virtual uint8 OLI_CALL GetCRCTypeMask() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetCRCTypeValue() const OLI_NOTHROW = 0;

    virtual uint32 OLI_CALL GetStateMask() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetStateValue() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINTransmissionStatusEventFilter_Create(): see \ref BinaryCompatibility and
     * \ref ErrorReporting for an explanation of why it is needed.
     */
    static AutoPtr<ILINTransmissionStatusEventFilter> OLI_CALL 
    Create ( uint8 frameIDMask
           , uint8 frameIDValue
           , uint8 directionMask
           , uint8 directionValue
           , uint8 dlcMask
           , uint8 dlcValue
           , uint8 crcTypeMask
           , uint8 crcTypeValue
           , uint32 stateMask
           , uint32 stateValue )
    {
        ILINTransmissionStatusEventFilter* pLinTxStatusEventFilter = NULL;
        CheckAndThrow( ILINTransmissionStatusEventFilter_Create( frameIDMask, frameIDValue, directionMask, directionValue, dlcMask, dlcValue, crcTypeMask, crcTypeValue, stateMask, stateValue, &pLinTxStatusEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINTransmissionStatusEventFilter>( pLinTxStatusEventFilter, false );
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINTRANSMISSIONSTATUSEVENTFILTER_INCLUDED__)
