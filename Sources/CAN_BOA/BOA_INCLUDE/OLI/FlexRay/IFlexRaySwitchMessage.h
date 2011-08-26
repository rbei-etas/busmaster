///////////////////////////////////////////////////////////
//  IFlexRaySwitchMessage.h
//  Definition of the IFlexRaySwitchMessage interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYSWITCHMESSAGE_INCLUDED__)
#define __OLI_IFLEXRAYSWITCHMESSAGE_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/ITxMessage.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_FLEXRAY_MESSAGES

/// @{

class IFlexRaySwitchMessage;

/**
 * This function instantiates an object supporting IFlexRaySwitchMessage. See \ref BinaryCompatibility for an explanation of
 * why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRaySwitchMessage::Create().
 *
 * \param[in]  txBufferIndex
 * \param[in]  enable
 * \param[out] ppFlexRaySwitchMsg   A pointer to an object supporting IFlexRaySwitchMessage. The object is owned by the OLL, not
 *                                  by the caller. The OLL assumes that the caller will eventually pass the object pointer back
 *                                  to the OLL for transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRaySwitchMessage_Create( uint32 txBufferIndex, bool enable, IFlexRaySwitchMessage** ppFlexRaySwitchMsg );

/** OLI reprentation of the OCI_FlexRaySwitchMessage OCI type.
* \ref GetID is not meaningful for this message and will return the 
* buffer index.
*/

OLI_INTERFACE IFlexRaySwitchMessage : public ITxMessage
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~IFlexRaySwitchMessage() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = FLEXRAY_TYPE_SWITCH_MESSAGE};

    /// \name FlexRay-specific interface
	/// @{

    virtual uint32 OLI_CALL GetTxBufferIndex() const OLI_NOTHROW = 0;
    virtual bool OLI_CALL GetEnable() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps IFlexRaySwitchMessage_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static IFlexRaySwitchMessage* OLI_CALL Create( uint32 txBufferIndex, bool enable )
    {
        IFlexRaySwitchMessage* pFlexRaySwitchMsg = NULL;
        CheckAndThrow( IFlexRaySwitchMessage_Create( txBufferIndex, enable, &pFlexRaySwitchMsg ) );
        return pFlexRaySwitchMsg;
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYSWITCHMESSAGE_INCLUDED__)
