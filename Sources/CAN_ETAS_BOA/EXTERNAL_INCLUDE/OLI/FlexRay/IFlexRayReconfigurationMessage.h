///////////////////////////////////////////////////////////
//  IFlexRayReconfigurationMessage.h
//  Definition of the IFlexRayReconfigurationMessage interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYRECONFIGMESSAGE_INCLUDED__)
#define __OLI_IFLEXRAYRECONFIGMESSAGE_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/ITxMessage.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_FLEXRAY_MESSAGES

/// @{

class IFlexRayReconfigurationMessage;

/**
 * This function instantiates an object supporting IFlexRayReconfigurationMessage. See \ref BinaryCompatibility for an
 * explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRayReconfigurationMessage::Create().
 *
 * \param[in]  txBufferIndex
 * \param[in]  channel
 * \param[in]  slot
 * \param[in]  cyclePeriod
 * \param[in]  cycleOffset
 * \param[in]  payloadLength
 * \param[in]  payloadPreambleIndicator
 * \param[out] ppFlexRayReconfigMsg         A pointer to an object supporting IFlexRayReconfigurationMessage. The object is owned
 *                                          by the OLL, not by the caller. The OLL assumes that the caller will eventually pass
 *                                          the object pointer back to the OLL for transmission. If this is not done, the object's
 *                                          memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRayReconfigurationMessage_Create(
    uint32          txBufferIndex,
    FlexRayChannel  channel,
    uint32          slot,
    uint32          cyclePeriod,
    uint32          cycleOffset,
    uint32          payloadLength,
    bool            payloadPreambleIndicator,
    IFlexRayReconfigurationMessage** ppFlexRayReconfigMsg );

/** OLI reprentation of the OCI_FlexRayReconfigurationMessage OCI type.
* \ref GetID is not meaningful for this message and will return the 
* buffer index.
*/

OLI_INTERFACE IFlexRayReconfigurationMessage : public ITxMessage
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~IFlexRayReconfigurationMessage() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = FLEXRAY_TYPE_RECONFIGURATION_MESSAGE};

    /// \name FlexRay-specific interface
	/// @{

    virtual uint32 OLI_CALL GetTxBufferIndex() const OLI_NOTHROW = 0;
    virtual FlexRayChannel OLI_CALL GetChannel() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetSlot() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetCyclePeriod() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetCycleOffset() const OLI_NOTHROW = 0;
    virtual uint32 OLI_CALL GetPayloadLength() const OLI_NOTHROW = 0;
    virtual bool OLI_CALL GetPayloadPreambleIndicator() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps IFlexRayReconfigurationMessage_Create(): see \ref BinaryCompatibility and
     * \ref ErrorReporting for an explanation of why it is needed.
     */
    static IFlexRayReconfigurationMessage* OLI_CALL Create 
        ( uint32 txBufferIndex
        , FlexRayChannel channel
        , uint32 slot
        , uint32 cyclePeriod
        , uint32 cycleOffset
        , uint32 payloadLength
        , bool payloadPreambleIndicator )
    {
        IFlexRayReconfigurationMessage* pFlexRayReconfigMsg = NULL;
        CheckAndThrow( IFlexRayReconfigurationMessage_Create( txBufferIndex, channel, slot, cyclePeriod, cycleOffset, payloadLength, payloadPreambleIndicator, &pFlexRayReconfigMsg ) );
        return pFlexRayReconfigMsg;
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYRECONFIGMESSAGE_INCLUDED__)
