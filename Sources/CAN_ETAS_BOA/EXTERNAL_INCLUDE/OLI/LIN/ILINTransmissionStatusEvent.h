///////////////////////////////////////////////////////////
//  ILINTransmissionStatusEvent.h
//  Defines the ILINTransmissionStatusEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINTRANSMISSIONSTATUSEVENT__INCLUDED__)
#define __OLI_ILINTRANSMISSIONSTATUSEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINTransmissionStatusEventMessage event.
*/

OLI_INTERFACE ILINTransmissionStatusEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINTransmissionStatusEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 4};

    /// \name LIN-specific interface
	/// @{

    /// deliver the transmission state of the event
    virtual LINTransmissionState OLI_CALL GetState() const OLI_NOTHROW = 0;

    /// deliver the direction of the event. Valid values are. 
    virtual LINFrameDirection OLI_CALL GetDirection() const OLI_NOTHROW = 0;

    // deliver the DLC of the event. 
    virtual uint8 OLI_CALL GetDLC() const OLI_NOTHROW = 0;

    /// deliver the CRC type of the event. 
    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;


	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINTRANSMISSIONSTATUSEVENT__INCLUDED__)
