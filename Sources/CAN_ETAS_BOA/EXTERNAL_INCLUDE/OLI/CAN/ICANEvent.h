///////////////////////////////////////////////////////////
//  ICANEvent.h
//  Defines the ICANEvent interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANEVENT__INCLUDED__)
#define __OLI_ICANEVENT__INCLUDED__

/// @todo update doxygen comments

// include used interface and constants

#include "CANBase.h"
#include "../Common/IEvent.h"
#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_MESSAGES

/// @{

/** The only event type available on CAN.
*/

OLI_INTERFACE ICANEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ICANEvent() OLI_NOTHROW {};

public:

    /// \ref GetID can return a combination of these


    /// returned by @ref GetType

    enum {TYPE = CAN_TYPE_EVENT_BASE + 1};
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANEVENT__INCLUDED__)
